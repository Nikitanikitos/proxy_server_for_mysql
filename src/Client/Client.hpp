//
// Created by nikita on 2/6/21.
//

#ifndef DATAARMOR_TEST_CLIENT_HPP
# define DATAARMOR_TEST_CLIENT_HPP

# include <zconf.h>
# include <netdb.h>
# include "bytes.hpp"

# include "Exception.hpp"

enum stages {
	read_client_request,
	read_sql_server_response,
	send_response_to_client,
	send_request_to_sql_server,
	close_connection
};

class Client {
	bytes			body;
	int				client_socket;
	int				sql_socket;
	stages			stage;
	bool			in_processed;

	int				initSqlConnection(const sockaddr_in* sql_addr);

public:
	Client(int client_socket, const sockaddr_in* sql_addr);
	~Client();

	inline void				setStage(enum stages stage_) { stage = stage_; }
	inline void				setProcessed(bool status) { in_processed = status; }

	inline int				getStage() const { return (stage); }
	inline int				getSocket() const { return (client_socket); }
	inline int				getSqlSocket() const { return (sql_socket); }
	inline const bytes&		getBody() const { return (body); }

	inline void				addDataToBody(char* data, size_t size) { body.add(data, size); }

	inline void				clearBody() { body.clear(); }

	inline bool				inTaskQueue() const { return (in_processed);}
};

#endif //DATAARMOR_TEST_CLIENT_HPP
