//
// Created by nikita on 2/6/21.
//

#ifndef DATAARMOR_TEST_SERVER_HPP
# define DATAARMOR_TEST_SERVER_HPP


# include <vector>
# include <string>

# include <zconf.h>
# include <cstring>
# include <arpa/inet.h>
# include <fcntl.h>

# include "Client.hpp"
# include "Exception.hpp"

enum {
	com_query = 3
};

class Server {
	int						server_socket;
	sockaddr_in				sql_addr;
	std::vector<Client*>	clients;
	fd_set					readfd_set;
	fd_set					writefd_set;
	int						max_fd;
	int						logfile_fd;

	static int						initSocket();
	void					initSets();
	void initSqlAddr(const std::string& sql_host, const std::string& sql_port);
	void					addNewClient();
	void					addClientSocketInSet();
	void					handleConnect();

	void					readClientRequest(Client* client);
	static void				sendResponseToClient(Client* client);
	static void				readSqlServerResponse(Client* client);
	static void				sendRequestToSqlServer(Client* client);

	void					writeLog(const char* data, int read_bytes) const;
public:
	Server(const std::string& sql_host, const std::string& sql_port = "3306", const char* logfile_name = "default.log");
	~Server();

	void	runServer();

	inline int		getServerSocket() { return (server_socket); }
};

#endif //DATAARMOR_TEST_SERVER_HPP
