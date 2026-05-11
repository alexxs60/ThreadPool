#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<vector>
#include "ThreadPool.h"
using namespace std;

struct sockInfo {
	int client_fd;
	struct sockaddr_in client_addr;
};

struct poolInfo {
	ThreadPool* p;
	int fd;
};


void* working(void* arg);
void* accepting(void* arg);

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		std::cerr << "socket error" << std::endl;
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9999);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	int ret = bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret == -1) {
		std::cerr << "bind error" << std::endl;
		return -1;
	}

	ret = listen(fd, 128);
	if (ret == -1) {
		std::cerr << "listen error" << std::endl;
		return -1;
	}

	ThreadPool* pool = new ThreadPool(3, 10);
	poolInfo* info = new poolInfo;
	info->p = pool;
	info->fd = fd;
	pool->addTask(Task((callback)accepting, info));

	pthread_exit(NULL);
	return 0;
}

void* accepting(void* arg){
	poolInfo* info = (poolInfo*)arg;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	while (1) {
		sockInfo* pinfo = new sockInfo;
		pinfo->client_fd= accept(info->fd, (struct sockaddr*)&pinfo->client_addr, &addr_len);
		if (pinfo->client_fd == -1) {
			std::cerr << "accept error" << std::endl;
			break;
		}
		info->p->addTask(Task((callback)working, (void*)pinfo));
	}
	close(info->fd);

}

void* working(void* arg)
{

	sockInfo* pinfo = (sockInfo*)arg;

	char ip[32];
	cout << "client ip: " << inet_ntop(AF_INET, &pinfo->client_addr.sin_addr.s_addr, ip, sizeof(ip))
		<< " port: " << ntohs(pinfo->client_addr.sin_port) << endl;

	while (1) {
		char buf[1024];
		int len = recv(pinfo->client_fd, buf, sizeof(buf), 0);
		if (len > 0) {
			cout << "Client said: " << buf << endl;
			send(pinfo->client_fd, buf, len, 0);
		}
		else if (len == 0) {
			cout << "Client disconnected" << endl;
			break;
		}
		else {
			cerr << "recv error" << endl;
			break;
		}
	}
	close(pinfo->client_fd);

}