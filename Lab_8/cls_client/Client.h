#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mutex>

class Client
{
	static int sock, flag;
	sockaddr_in serv_addr;
	static pthread_mutex_t mute;
public:
	Client();

	void checkConnection(pthread_t*);

	void flag_off();
	
	~Client();

private:
	static void* sendRequest(void*);
	
	static void* receiveResult(void*);
};
