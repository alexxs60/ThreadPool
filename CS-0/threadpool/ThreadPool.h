#pragma once
#include "TaskQueue.h"
class ThreadPool
{
public:
	ThreadPool(int min, int max);
	~ThreadPool();
	void addTask(Task task);
	int getBusyNum();
	int getAliveNum();
private:
	static void* worker(void* arg);
	static void* manager(void* arg);
	void threadExit();
private:
	TaskQueue* taskQ;

	pthread_t managerId;
	pthread_t* threadIds;
	int minNum;
	int maxNum;
	int busyNum;
	int livenum;
	int exitNum;
	pthread_mutex_t mutexPool;
	pthread_cond_t notEmpty;
	static const int NUMBER = 2;

	bool shutdown;
};

