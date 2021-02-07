//
// Created by nikita on 2/6/21.
//

#include "Client.hpp"

Client::Client(int client_socket, const sockaddr_in* sql_addr) : client_socket(client_socket),
																 stage(read_sql_server_response) {
	sql_socket = initSqlConnection(sql_addr);
}

Client::~Client() {
	close(getSqlSocket());
	close(getSocket());
}

int Client::initSqlConnection(const sockaddr_in* sql_addr) {
	int		socket_for_connection;

	socket_for_connection = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(socket_for_connection, (struct sockaddr*)sql_addr, sizeof(*sql_addr)) == -1)
		throw Exception("Errno: " + std::to_string(errno) + " (" + strerror(errno) + "). " + "Connect returned -1");
	return (socket_for_connection);
}
