#include "Client.h"
#include <fcntl.h>

int Client::sock;
int Client::flag = 1;
pthread_mutex_t Client::mute;

Client::Client()
{
	pthread_mutex_init(&mute, NULL);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	socklen_t serv_addr_size;
	
	serv_addr.sin_family 	  = AF_INET;
	serv_addr.sin_port    	  = htons(25565);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr_size		 	  = sizeof(serv_addr);
		
	fcntl(sock, F_SETFL,  O_NONBLOCK);
}

void Client::checkConnection(pthread_t *thr)
{
	int res_sock = 0, i = 0;
	char status[][18] = {"Connecting.\x1b[0m  ",
						 "Connecting..\x1b[0m ",
						 "Connecting...\x1b[0m",
						 "Connected!\x1b[0m  \n"};
	
	while(true)
	{
		res_sock = connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
		
		if(res_sock == -1)
		{
			std::cout << "\033[0;0H\x1b[44m" << status[i++] << std::flush;
			
			sleep(1);
			if(i == 3) i = 0;
			continue;
		}
		
		std::cout << "\033[0;0H\x1b[42m" << status[3] << std::flush;
		
		pthread_create(&thr[0], NULL, &sendRequest,   NULL);
		pthread_create(&thr[1], NULL, &receiveResult, NULL);
		break;
	}
}

void Client::flag_off()
{
	flag = 0;
}

void* Client::sendRequest(void* arg)
{
	int i = 1;
	while(flag)
	{
		pthread_mutex_lock(&mute);
		
		std::string temp = std::to_string(i++);
		const char *msg = temp.c_str();
		
		send(sock, msg, 80, 0);
		
		pthread_mutex_unlock(&mute);
		sleep(1);
	}
	std::cout << "\033[0;0H\x1b[41mDisconnected\x1b[0m\033[" << i+1 << ";0H";
	return 0;
}

void* Client::receiveResult(void* arg)
{
	while(flag)
	{		
		pthread_mutex_lock(&mute);
		
		char temp[80];
		recv(sock, temp, 80, 0);
		std::cout << std::string(temp) << '\n';
		
		
		pthread_mutex_unlock(&mute);
		sleep(1);
	}
	return 0;
}

Client::~Client()
{
	pthread_mutex_destroy(&mute);
	shutdown(sock, SHUT_RDWR);
	close(sock);
}
