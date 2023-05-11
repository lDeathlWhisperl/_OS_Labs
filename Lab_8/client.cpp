#include "cls_client/Client.h"

int main()
{
	std::cout << "\033]2;CLIENT\007";
	system("clear");
	
	Client client;
	
	pthread_t thr[2];
	client.checkConnection(thr);
	
	getchar();
	client.flag_off();

	pthread_join(thr[0], NULL);
	pthread_join(thr[1], NULL);
	return 0;
}
