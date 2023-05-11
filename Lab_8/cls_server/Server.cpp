#include "Server.h"

int Server::flag = 1;
int Server::sock;
int Server::conn_sock;

pthread_mutex_t Server::q_mute;
				
std::queue<std::string> Server::quest, Server::ans;
				  		
Server::Server()
{
	pthread_mutex_init(&q_mute, NULL);
		
	sock = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
				
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family 	  = AF_INET;
	serv_addr.sin_port    	  = htons(25565);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
	
	fcntl(sock, F_SETFL,  O_NONBLOCK);
	listen(sock, 0);
}

void Server::checkConnection(pthread_t *thr)
{
	int i = 0;
	socklen_t sock_size = sizeof(cli_addr);
	char status[][27] = {"Waiting connection.\x1b[0m  ",
					 	 "Waiting connection..\x1b[0m ",
					 	 "Waiting connection...\x1b[0m",
					 	 "Connected!\x1b[0m           \n"};

	while(true)
	{
		conn_sock = accept(sock, (sockaddr*)&cli_addr, &sock_size);
	
		if(conn_sock == -1)
		{
			std::cout << "\033[0;0H\x1b[44m" << status[i++] << std::flush;
		
			sleep(1);
			if(i == 3) i = 0;
			continue;
		}
		
		std::cout << "\033[0;0H\x1b[42m" << status[3] << std::flush;
	
		pthread_create(&thr[0], NULL, &getRequest, 	   NULL);
		pthread_create(&thr[1], NULL, &processRequest, NULL);
		pthread_create(&thr[2], NULL, &transfer, 	   NULL);
		break;
	}
}

void Server::flag_off()
{
	flag = 0;
}

void* Server::getRequest(void* arg)
{
	char msg[MSG_SIZE];
	while(flag)
	{
		switch(recv(conn_sock, &msg, MSG_SIZE, 0))
		{
			case -1:
				perror("recv");
				break;
			case 0:
				std::cout << "\033[0;0H\x1b[41mDisconnected\x1b[0m\n";
				shutdown(conn_sock, SHUT_RDWR);
				break;
			default:
				pthread_mutex_lock(&q_mute);
				
				std::cout << "Request: " << msg << '\n';
				quest.push(std::string(msg));
				
				pthread_mutex_unlock(&q_mute);
				break;
		}
		sleep(1);
	}
	return 0;
}

void* Server::processRequest(void* arg)
{
	std::string msg;
	while(flag)
	{
		pthread_mutex_lock(&q_mute);
		
		if(!quest.empty())
		{
			char login[MSG_SIZE];
			getlogin_r(login, MSG_SIZE);
			
			msg = "answer " + quest.front() + ": " + login + ";";
			
			quest.pop();
			ans.push(msg);
		}
		
		pthread_mutex_unlock(&q_mute);
		sleep(1);
	}
	return 0;
}

void* Server::transfer(void* arg)
{
	while(flag)
	{
		if(ans.empty()) continue;
		
		std::string temp = ans.front();
		send(conn_sock, temp.c_str(), MSG_SIZE, 0);
		ans.pop();
		
		sleep(1);
	}
	return 0;
}
	
Server::~Server()
{	
	pthread_mutex_destroy(&q_mute);
	close(sock);
}
