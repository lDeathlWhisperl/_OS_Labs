#include <iostream>
#include <unistd.h>

using std::cout;
using std::flush;

struct thread_args
{
    int flag;    
    pthread_mutex_t mute;

    thread_args() : flag(0) 
    {
        pthread_mutex_init(&mute, NULL);
    }

    ~thread_args()
    {
        pthread_mutex_destroy(&mute);
    }
};

void* thread_1(void* arg)
{
    cout << "\x1b[42mthread 1 start\x1b[0m\n";
    thread_args *args = (thread_args*) arg;
    timespec time;

    while(args->flag)
    {        
        clock_gettime(CLOCK_REALTIME, &time);
        time.tv_sec += 1;

        if(pthread_mutex_timedlock(&args->mute, &time)) continue;
        
        for(int i = 0; i < 10; ++i)
        {
            cout << 1 << flush;
            sleep(1);
        }

        pthread_mutex_unlock(&args->mute);
        sleep(1);
    }
        
    cout << "\x1b[42m\nthread_1 end\x1b[0m\n";
    pthread_exit((void*)1);
}

void* thread_2(void* arg)
{
    cout << "\x1b[42mthread 2 start\x1b[0m\n";
    thread_args *args = (thread_args*)arg;
    timespec time;

    while(args->flag)
    {        
        clock_gettime(CLOCK_REALTIME, &time);
        time.tv_sec += 1;

        if(pthread_mutex_timedlock(&args->mute, &time)) continue;
        
        for(int i = 0; i < 10; ++i)
        {
            cout << 2 << flush;
            sleep(1);
        }

        pthread_mutex_unlock(&args->mute);
        sleep(1);
    }

    cout << "\x1b[42m\nthread_2 end\x1b[0m\n";
    pthread_exit((void*)2);
}

int main()
{
    system("clear");
    cout << "\x1b[32mProgramm start\x1b[0m\n\n";

    pthread_t thr_1, thr_2;

    int exit_code_1, exit_code_2;

    thread_args arg;
    arg.flag = 1;
    
    pthread_create(&thr_1, NULL, &thread_1, &arg);
    pthread_create(&thr_2, NULL, &thread_2, &arg);

    getchar();

    arg.flag = 0;

    pthread_join(thr_1, (void**)&exit_code_1);
    pthread_join(thr_2, (void**)&exit_code_2);

    cout << "\x1b[42m\nthread_1\x1b[0m finished with exit code " << exit_code_1 << '\n'
         << "\x1b[42mthread_2\x1b[0m finished with exit code "   << exit_code_2 << '\n'
         << "\x1b[32m\nProgramm end\x1b[0m\n";
    
    return 0;
}
