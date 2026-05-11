#include "ThreadPool.h"
#include <iostream>
#include<string.h>
#include<string>
#include<unistd.h>
using namespace std;

ThreadPool::ThreadPool(int min, int max)
{
	do
	{
		taskQ = new TaskQueue;
		if (taskQ == nullptr) {
			cout << "new taskQ error" << endl;
			break;
		}

		threadIds = new pthread_t[max];
		if (threadIds == nullptr) {
			cout << "new threadIds error" << endl;
			break;
		}
		memset(threadIds, 0, sizeof(pthread_t) * max);
		minNum = min;
		maxNum = max;
		busyNum = 0;
		livenum = min;
		exitNum = 0;

		if (pthread_mutex_init(&mutexPool, NULL) != 0
			|| pthread_cond_init(&notEmpty, NULL) != 0) {
			cout << "mutex or cond init error" << endl;
			break;
		}

		shutdown = false;

		pthread_create(&managerId, NULL, manager, this);

		for (int i = 0; i < min; i++) {
			pthread_create(&threadIds[i], NULL, worker, this);
		}
		return;
	} while (0);

	if (threadIds) delete[] threadIds;
	if (taskQ) delete taskQ;
}

void* ThreadPool::worker(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	while (true) {
		pthread_mutex_lock(&pool->mutexPool);
		while (pool->taskQ->taskNumber() == 0 && !pool->shutdown) {
			pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

			if (pool->exitNum > 0) {
				pool->exitNum--;
				if (pool->livenum > pool->minNum) {
					pool->livenum--;
					pthread_mutex_unlock(&pool->mutexPool);
					pool->threadExit();
				}
			} 
		}
		if (pool->shutdown) {
			pthread_mutex_unlock(&pool->mutexPool);
			pool->threadExit();
		}

		Task task = pool->taskQ->takeTask();


		pool->busyNum++;
		pthread_mutex_unlock(&pool->mutexPool);

		cout << "thread " << to_string(pthread_self()) << " start working..." << endl;


		if (task.function != nullptr) {
			task.function(task.arg);
			delete task.arg;
			task.arg = nullptr;
		}
		cout << "thread " << to_string(pthread_self()) << " end  working..." << endl;
		pthread_mutex_lock(&pool->mutexPool);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->mutexPool);
	}
	return NULL;
}

void ThreadPool::threadExit()
{
	pthread_t tid = pthread_self();	
	for (int i = 0; i < maxNum; i++) {
		if (threadIds[i] == tid) {
			threadIds[i] = 0;
			cout << "thread " << to_string(tid) << " exit..." << endl;
			break;
		}
	}
	pthread_exit(NULL);
}

void* ThreadPool::manager(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	while (!pool->shutdown) {

		sleep(5);

		pthread_mutex_lock(&pool->mutexPool);
		int queueSize = pool->taskQ->taskNumber();
		int liveNum = pool->livenum;
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->mutexPool);

		if (queueSize > liveNum-busyNum && liveNum < pool->maxNum) {
			pthread_mutex_lock(&pool->mutexPool);
			int addNum = 0;
			for (int i = 0; i < pool->maxNum && addNum < NUMBER
				&& pool->livenum < pool->maxNum; i++)
			{
				if (pool->threadIds[i] == 0) {
					pthread_create(&pool->threadIds[i], NULL, worker, pool);
					addNum++;
					pool->livenum++;
				}
			}
			pthread_mutex_unlock(&pool->mutexPool);
		}
		if (busyNum * 2 < liveNum && liveNum > pool->minNum) {
			pthread_mutex_lock(&pool->mutexPool);
			pool->exitNum = NUMBER;
			pthread_mutex_unlock(&pool->mutexPool);
			for (int i = 0; i < NUMBER; i++) {
				pthread_cond_signal(&pool->notEmpty);
			}
		}
	}
	return NULL;
}

void ThreadPool::addTask(Task task)
{
	if (shutdown) {
		return;
	}
	taskQ->addTask(task);
	pthread_cond_signal(&notEmpty);
}

int ThreadPool::getBusyNum()
{
	pthread_mutex_lock(&mutexPool);
	int busyNum = this->busyNum;
	pthread_mutex_unlock(&mutexPool);
	return busyNum;
}

int ThreadPool::getAliveNum()
{
	pthread_mutex_lock(&mutexPool);
	int aliveNum = this->livenum;
	pthread_mutex_unlock(&mutexPool);
	return aliveNum;
}

ThreadPool::~ThreadPool()
{
	shutdown = true;
	
	for (int i = 0; i < livenum; i++) {
		pthread_cond_signal(&notEmpty);
	}
	for (int i = 0; i < maxNum; i++) {
		if (threadIds[i] != 0) {
			pthread_join(threadIds[i], NULL);
		}
	}
	pthread_join(managerId, NULL);

	if (threadIds) {
		delete[] threadIds;
		threadIds = nullptr;
	}
	if (taskQ) {
		delete taskQ;
		taskQ = nullptr;
	}
	pthread_mutex_destroy(&mutexPool);
	pthread_cond_destroy(&notEmpty);
}