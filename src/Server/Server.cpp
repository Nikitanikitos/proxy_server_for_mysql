//
// Created by nikita on 2/6/21.
//

#include "Server.hpp"

Server::Server(const std::string& sql_host, const std::string& sql_port, const char* logfile_name) : server_socket(initSocket()) {
	if ((logfile_fd = open(logfile_name, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1)
		throw Exception("Errno: " + std::to_string(errno) + " (" + strerror(errno) + "). " + "Can't open logfile");
	initSqlAddr(sql_host, sql_port);
}

Server::~Server() {
	close(getServerSocket());
	close(logfile_fd);
	for (auto & client : clients)
		delete client;
}

int		Server::initSocket() {
	struct				sockaddr_in	addr;
	int					server_sock;
	const int			opt = 1;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(3305);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		throw Exception("Errno: " + std::to_string(errno) + " (" + strerror(errno) + "). " + "Bind returned -1");

	fcntl(server_sock, F_SETFL, O_NONBLOCK);
	listen(server_sock, 100);
	return (server_sock);
}

void	Server::initSqlAddr(const std::string& sql_host, const std::string& sql_port) {
	memset(&sql_addr, 0, sizeof(sql_addr));
	sql_addr.sin_family = PF_INET;
	sql_addr.sin_port = inet_addr(sql_host.c_str());
	sql_addr.sin_addr.s_addr = inet_addr(sql_host.c_str());
}

void	Server::initSets() {
	max_fd = server_socket;
	FD_ZERO(&readfd_set);
	FD_ZERO(&writefd_set);

	FD_SET(getServerSocket(), &readfd_set);
}

void	Server::addNewClient() {
	int		client_socket;

	if (FD_ISSET(getServerSocket(), &readfd_set)) {
		client_socket = accept(getServerSocket(), 0, 0);
		clients.push_back(new Client(client_socket, &sql_addr));
	}
}

void	Server::handleConnect() {
	for (auto& client : clients) {
		if (FD_ISSET(client->getSocket(), &readfd_set) && client->getStage() == read_client_request)
			readClientRequest(client);
		if (FD_ISSET(client->getSocket(), &writefd_set) && client->getStage() == send_response_to_client)
			sendResponseToClient(client);
		if (FD_ISSET(client->getSqlSocket(), &readfd_set) && client->getStage() == read_sql_server_response)
			readSqlServerResponse(client);
		if (FD_ISSET(client->getSqlSocket(), &writefd_set) && client->getStage() == send_request_to_sql_server)
			sendRequestToSqlServer(client);
	}
}

void	Server::runServer() {
	while (true) {
		initSets();
		addClientSocketInSet();

		if (select(max_fd + 1, &readfd_set, &writefd_set, 0, 0) == -1)
			throw Exception("Errno: " + std::to_string(errno) + " (" + strerror(errno) + "). " + "Select returned -1");

		addNewClient();
		handleConnect();
	}
}

void	Server::addClientSocketInSet() {
	for (auto& client : clients) {
		if (client->getStage() == read_client_request) {
			FD_SET(client->getSocket(), &readfd_set);
			max_fd = (client->getSocket() > max_fd) ? client->getSqlSocket() : max_fd;
		}
		else if (client->getStage() == read_sql_server_response) {
			max_fd = (client->getSqlSocket() > max_fd) ? client->getSqlSocket() : max_fd;
			FD_SET(client->getSqlSocket(), &readfd_set);
		}
		else if (client->getStage() == send_response_to_client) {
			max_fd = (client->getSocket() > max_fd) ? client->getSqlSocket() : max_fd;
			FD_SET(client->getSocket(), &writefd_set);
		}
		else if (client->getStage() == send_request_to_sql_server) {
			max_fd = (client->getSqlSocket() > max_fd) ? client->getSqlSocket() : max_fd;
			FD_SET(client->getSqlSocket(), &writefd_set);
		}
	}
}

void	Server::readClientRequest(Client* client) {
	char	buff[1024];
	int		buff_size(1024);
	int		read_bytes;

	if ((read_bytes = recv(client->getSocket(), buff, buff_size, 0)) <= 0)
		client->setStage(close_connection);
	else {
		buff[read_bytes] = 0;
		client->setStage(send_request_to_sql_server);
		client->addDataToBody(buff, read_bytes);
		if (buff[4] == com_query)
			writeLog(buff, read_bytes);
	}
}

void	Server::sendResponseToClient(Client* client) {
	send(client->getSocket(), client->getBody().c_str(), client->getBody().size(), 0);
	client->clearBody();
	client->setStage(read_client_request);
}

void	Server::readSqlServerResponse(Client* client) {
	char	buff[1024];
	int		buff_size(1024);
	int		read_bytes;

	read_bytes = recv(client->getSqlSocket(), buff, buff_size, 0);
	buff[read_bytes] = 0;
	client->setStage(send_response_to_client);
	client->addDataToBody(buff, read_bytes);
}

void	Server::sendRequestToSqlServer(Client* client) {
	send(client->getSqlSocket(), client->getBody().c_str(), client->getBody().size(), 0);
	client->clearBody();
	client->setStage(read_sql_server_response);
}

void	Server::writeLog(const char* data, int read_bytes) const {
	struct tm*	local;
	time_t		t;
	char		time_buff[80];
	size_t		time_buff_length;

	t = time(nullptr);
	local = localtime(&t);
	time_buff_length = strftime(time_buff, 80, "%F %X", local);
	write(logfile_fd, time_buff, time_buff_length);
	write(logfile_fd, data + 5, read_bytes - 5);
	write(logfile_fd, "\n", 1);
}
