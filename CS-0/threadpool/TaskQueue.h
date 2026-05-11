#pragma once
#include<queue>
#include<pthread.h>
using namespace std;

using callback = void(*)(void* arg);
struct Task 
{
	Task() {
		function = nullptr;
		arg = nullptr;
	}
	Task(callback f, void* arg) {
		this->arg = (void*)(arg);
		this->function = f;
	}

	callback function;
	void* arg;
};
class TaskQueue
{public:
	TaskQueue();
	~TaskQueue();
	void addTask(Task task);
	void addTask(callback f,void* arg);
	Task takeTask();
	int taskNumber();
private:
	pthread_mutex_t m_mutex;
	queue<Task>m_taskQ;
};

