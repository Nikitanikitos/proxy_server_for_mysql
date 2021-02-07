//
// Created by nikita on 2/7/21.
//

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	queue_mutex = new pthread_mutex_t;
	pthread_mutex_init(queue_mutex, nullptr);

	read_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(read_stage_mutex, nullptr);

	generate_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(generate_stage_mutex, nullptr);

	send_stage_mutex = new pthread_mutex_t;
	pthread_mutex_init(send_stage_mutex, nullptr);
}

ThreadPool::~ThreadPool() {
	pthread_mutex_destroy(queue_mutex);
	delete queue_mutex;

	pthread_mutex_destroy(read_stage_mutex);
	delete read_stage_mutex;

	pthread_mutex_destroy(generate_stage_mutex);
	delete generate_stage_mutex;

	pthread_mutex_destroy(send_stage_mutex);
	delete send_stage_mutex;
}

Client*		ThreadPool::popTask() {
	Client*		client = tasks_queue.front();

	tasks_queue.pop();
	return (client);
}

void		ThreadPool::pushTask(Client* client) {
	pthread_mutex_lock(queue_mutex);
	client->setProcessed(true);
	tasks_queue.push(client);
	pthread_mutex_unlock(queue_mutex);
}
