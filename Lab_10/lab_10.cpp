#include <ev.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

using std::cout;
using std::endl;

struct Arg
{
    int f_des[2];
    const size_t  BUFF_SIZE = 32;

    ~Arg()
    {        
        close(f_des[0]);
        close(f_des[1]);
    }
}arg;

static void io_cb(EV_P_ ev_io *w, int revents)
{
	char buff[arg.BUFF_SIZE];
	if(read(arg.f_des[0], &buff, arg.BUFF_SIZE) == -1)
	{
		perror("read");
		sleep(1);
		return;
	}		
	cout << "\x1b[44mEcho: " << buff << "\x1b[0m" << endl;	
}

static void finish_cb(EV_P_ ev_io *w, int revents)
{	
	ev_io_stop(EV_A_  w);
	ev_break(EV_A_  EVBREAK_ALL);
}

static void timer_cb(EV_P_ ev_timer *w, int revents)
{
	char buff[arg.BUFF_SIZE];
	
	getlogin_r(buff, arg.BUFF_SIZE);
	cout << "\x1b[42mMessage:\x1b[0m " << buff << endl;
		
	if(write(arg.f_des[1], &buff, arg.BUFF_SIZE) == -1) perror("write");
}

int main(int argc, char **argv)
{
    system("clear");

    switch(argc)
    {
    case 1:
        cout << "\x1b[42mpipe():\x1b[0m\n\n";
        pipe(arg.f_des);        
        fcntl(arg.f_des[0], F_SETFL,  O_NONBLOCK);
        fcntl(arg.f_des[1], F_SETFL,  O_NONBLOCK);
        break;
        
    default:
        cout << "\x1b[42mpipe2():\x1b[0m\n\n";
        pipe2(arg.f_des, O_NONBLOCK);
        break;
    }
        
	struct ev_loop *loop = ev_default_loop(0);
	
	ev_io io_watcher;
	ev_io finish_watcher;
	ev_timer timer_watcher;
	
	ev_io_init(&io_watcher, io_cb, arg.f_des[0], EV_READ);
	ev_io_init(&finish_watcher, finish_cb, 0, EV_READ);
	ev_timer_init(&timer_watcher, timer_cb, 1, 1);
	
	ev_io_start(loop, &io_watcher);
	ev_io_start(loop, &finish_watcher);
	ev_timer_start(loop, &timer_watcher);
	
	ev_run(loop, 0);    
    return 0;
}
