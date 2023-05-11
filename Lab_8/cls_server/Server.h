#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <queue>

class Server
{
	static int sock;
	static int conn_sock;
	static int flag;
	static const int MSG_SIZE = 80;
	
	sockaddr_in serv_addr, cli_addr;
				
	static std::queue<std::string> quest, ans;
	
	static pthread_mutex_t q_mute;
public:
	Server();

	void checkConnection(pthread_t*);

	void flag_off();

	~Server();
private:
	static void* getRequest(void*);
	
	static void* processRequest(void*);

	static void* transfer(void*);	
};
