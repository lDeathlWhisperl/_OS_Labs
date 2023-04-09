#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

using std::cout;
using std::flush;
using std::cin;

struct Arg
{
    int flag = 1;
    int f_des[2];
    const size_t  BUFF_SIZE = 32;

    ~Arg()
    {        
        close(f_des[0]);
        close(f_des[1]);
    }
};

void* thr_1(void* args)
{
	Arg *arg = (Arg*) args;
	char buff[arg->BUFF_SIZE];
	
	while(arg->flag)
	{
		cout << "\x1b[42mMessage:\x1b[0m ";
		getlogin_r(buff, arg->BUFF_SIZE);
		cout << buff << '\n';
		
		if(write(arg->f_des[1], &buff, arg->BUFF_SIZE) == -1)
			perror("write");
		
		sleep(1);
	}
	close(arg->f_des[1]);
	pthread_exit((void*)0);
}

void* thr_2(void* args)
{
	Arg *arg = (Arg*) args;
	char buff[arg->BUFF_SIZE];
	
	while(arg->flag)
	{
		if(read(arg->f_des[0], &buff, arg->BUFF_SIZE) == -1)
		{
			perror("read");
			sleep(1);
			continue;
		}		
		cout << "\x1b[44mEcho: " << buff << "\x1b[0m" << ' ' << '\n' << flush;
	}
	close(arg->f_des[1]);
	pthread_exit((void*)0);
}

int main(int argc, char **argv)
{
    system("clear");
    Arg arg;

    switch(argc)
    {
    case 1:
        cout << "\x1b[42mpipe():\x1b[0m\n\n";
        pipe(arg.f_des);        
        break;
    case 2:
        cout << "\x1b[42mpipe2():\x1b[0m\n\n";
        pipe2(arg.f_des, O_NONBLOCK);
        break;
    default:
        cout << "\x1b[42mpipe() & fcntl():\x1b[0m\n\n";
        pipe(arg.f_des);
        fcntl(arg.f_des[0], F_SETFL,  O_NONBLOCK);
        fcntl(arg.f_des[1], F_SETFL,  O_NONBLOCK);
        break;
    }
    
    pthread_t id[2];
    
    pthread_create(id, NULL, thr_1, (void*)&arg);
    pthread_create(id+1, NULL, thr_2, (void*)&arg);
    
    getchar();
    arg.flag = 0;
    
    pthread_join(id[0], NULL);
    pthread_join(id[1], NULL);
    return 0;
}
