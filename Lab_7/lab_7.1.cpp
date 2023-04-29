#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <cstring>

int main()
{
	const char *queue_name = "/queue";
	const int buf = 32;
	char message[buf];
	
	mqd_t queue = mq_open(queue_name, O_CREAT | O_WRONLY | O_NONBLOCK, 0644, NULL);
	
	getlogin_r(message, buf);
	
	fcntl(STDIN_FILENO, F_SETFL,  O_NONBLOCK);
	while(getchar() != '\n')
	{
		std::cout << "\x1b[42mSend:\x1b[0m " << message << '\n';
		
		int result = mq_send(queue, message, strlen(message)+1, 1);
		if(result == -1) perror("mq_sand");		
		
		sleep(1);
	}
	
	mq_close(queue);
	mq_unlink(queue_name);
	return 0;
}
