//
// Created by nikita on 2/6/21.
//

#ifndef DATAARMOR_TEST_SERVER_HPP
# define DATAARMOR_TEST_SERVER_HPP


# include <list>
# include <string>

# include <zconf.h>
# include <cstring>
# include <arpa/inet.h>
# include <fcntl.h>

# include "ThreadPool.hpp"
# include "Client.hpp"
# include "Exception.hpp"

[[noreturn]] void*	worker(void* arg);

enum {
	com_query = 3
};

class Server {
	friend void*	worker(void* arg);

	int						server_socket;
	sockaddr_in				sql_addr;
	std::list<Client*>		clients;
	fd_set					readfd_set;
	fd_set					writefd_set;
	int						max_fd;
	int						logfile_fd;

	int						number_workers;
	std::vector<pthread_t>	workers;
	ThreadPool				thread_pool;

	int						check_fd_sets[2];

	static int				initSocket();
	void					initSets();
	void					initSqlAddr(const std::string& sql_host, const std::string& sql_port);
	void					addNewClient();
	void					addClientSocketInSet();
	void					handleConnect();

	void					readClientRequest(Client* client);
	void					sendResponseToClient(Client* client);
	void					readSqlServerResponse(Client* client);
	void					sendRequestToSqlServer(Client* client);

	void					writeLog(const char* data, int read_bytes) const;

	void					createWorkers();

public:
	Server(const std::string& sql_host, const std::string& sql_port = "3306", const char* logfile_name = "default.log");
	~Server();

	void	runServer();

	inline int		getServerSocket() { return (server_socket); }
};

#endif //DATAARMOR_TEST_SERVER_HPP
