#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<string>
#include<string.h>
#include<arpa/inet.h>
using namespace std;

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		std::cerr << "socket error" << std::endl;
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9999);
	inet_pton(AF_INET, "192.168.61.128", &server_addr.sin_addr.s_addr);
	int ret = connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret == -1) {
		std::cerr << "connect error" << std::endl;
		return -1;
	}

	int number = 0;
	while (1) {
		char buf[1024];
		sprintf(buf, "Hello %d", number++);
		send(fd, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));

		int len = recv(fd, buf, sizeof(buf), 0);
		if (len > 0) {
			cout << "Server said: " << string(buf,len) << endl;
		}
		else if (len == 0) {
			cout << "Server disconnected" << endl;
			break;
		}
		else {
			cerr << "recv error" << endl;
			break;
		}
		sleep(1);
	}
	close(fd);

	return 0;

}