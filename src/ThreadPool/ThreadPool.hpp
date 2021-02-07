//
// Created by nikita on 2/7/21.
//

#ifndef DATAARMOR_TEST_THREADPOOL_HPP
# define DATAARMOR_TEST_THREADPOOL_HPP

# include <Client.hpp>
# include <queue>

class	ThreadPool {
private:
	std::queue<Client*>		tasks_queue;

	pthread_mutex_t*		queue_mutex;
	pthread_mutex_t*		read_stage_mutex;
	pthread_mutex_t*		generate_stage_mutex;
	pthread_mutex_t*		send_stage_mutex;

public:
	ThreadPool();
	~ThreadPool();

	inline bool				queueIsEmpty() const { return (tasks_queue.empty()); }
	inline bool				inTaskInProcess() const { return (tasks_queue.front()->inTaskQueue()); }

	inline void				lockQueueMutex() const { pthread_mutex_lock(queue_mutex); }
	inline void				unlockQueueMutex() const { pthread_mutex_unlock(queue_mutex); }

	inline void				lockReadStageMutex() const { pthread_mutex_lock(read_stage_mutex); }
	inline void				unlockReadStageMutex() const { pthread_mutex_unlock(read_stage_mutex); }
	inline void				lockGenerateStageMutex() const { pthread_mutex_lock(generate_stage_mutex); }
	inline void				unlockGenerateStageMutex() const { pthread_mutex_unlock(generate_stage_mutex); }
	inline void				lockSendStageMutex() const { pthread_mutex_lock(send_stage_mutex); }
	inline void				unlockSendStageMutex() const { pthread_mutex_unlock(send_stage_mutex); }

	void		 			pushTask(Client* client);
	Client*					popTask();

};


#endif //DATAARMOR_TEST_THREADPOOL_HPP
