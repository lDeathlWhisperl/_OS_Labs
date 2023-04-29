#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>

int main()
{
	const char *queue_name = "/queue";
	const int buf = 32;
	char message[buf];
	
	mqd_t queue = mq_open(queue_name, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, NULL);
	
	fcntl(STDIN_FILENO, F_SETFL,  O_NONBLOCK);
	while(getchar() != '\n')
	{		
		int result = mq_receive(queue, message, 8192, NULL);
		std::cout << "\x1b[44mReceive:\x1b[0m " << message << '\n';
		
		if(result == -1) perror("mq_receive");		
		
		sleep(1);
	}
	
	mq_close(queue);
	mq_unlink(queue_name);
	return 0;
}
