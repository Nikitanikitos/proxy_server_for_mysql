//
// Created by nikita on 2/7/21.
//

#include <Server.hpp>
#include <iostream>

[[noreturn]] void*	worker(void* arg) {
	Server&			server = *(Server*)arg;
	ThreadPool&		thread_pool = server.thread_pool;

	while (true) {
		thread_pool.lockQueueMutex();
		if (!thread_pool.queueIsEmpty()) {
			Client*		client = thread_pool.popTask();
			thread_pool.unlockQueueMutex();
			switch (client->getStage()) {
				case read_client_request:
					server.readClientRequest(client);
					break;
				case send_request_to_sql_server:
					server.sendRequestToSqlServer(client);
					break;
				case read_sql_server_response:
					server.readSqlServerResponse(client);
					break;
				case send_response_to_client:
					server.sendResponseToClient(client);
					break;
			}
			write(server.check_fd_sets[1], "1", 1);
			client->setProcessed(false);
		}
		else
			thread_pool.unlockQueueMutex();
	}
}
