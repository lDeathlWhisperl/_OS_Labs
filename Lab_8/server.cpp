#include "cls_server/Server.h"

int main()
{
	std::cout << "\033]2;SERVER\007";
	system("clear");
	
	Server server;
	
	pthread_t thr[3];
	server.checkConnection(thr);
	
	getchar();
	system("clear");
	
	server.flag_off();

	pthread_join(thr[0], NULL);
	pthread_join(thr[1], NULL);
	pthread_join(thr[2], NULL);
	return 0;
}
