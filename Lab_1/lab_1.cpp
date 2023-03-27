#include <iostream>
#include <pthread.h>
#include <unistd.h>

using std::cout;

static void* thread_1(void* flag)
{
    cout << "thread 1 start\n";

    while(*(int*)flag)
    {      
        sleep(1);
        cout << 1 << std::flush;
    }

    cout << "\nthread 1 end\n";
    pthread_exit((void*)3);
}

static void* thread_2(void* flag)
{
    cout << "thread 2 start\n";
    
    while(*(int*)flag)
    {      
        sleep(1);
        cout << 2 << std::flush;
    }

    cout << "\nthread 2 end\n";
    pthread_exit((void*)4);
}

int main()
{
    system("clear");

    cout << "\x1b[32mProgramm start\x1b[0m\n\n";
    pthread_t id1, 
              id2;

    int *flag1 = new int, 
        *flag2 = new int;

    *flag1 = 1;
    *flag2 = 1;
    
    pthread_create(&id1, NULL, &thread_1, (void*)flag1);
    pthread_create(&id2, NULL, &thread_2, (void*)flag2);

    getchar();
    
    *flag1 = 0;
    *flag2 = 0;

    int exit_code_1, 
        exit_code_2;

    pthread_join(id1, (void**)&exit_code_1);
    pthread_join(id2, (void**)&exit_code_2);
    
    cout << "\nthread_1 finished with exit code " << exit_code_1 << '\n'
         << "thread_2 finished with exit code " << exit_code_2 << '\n'
         << "\x1b[32m\nProgramm end\x1b[0m\n";

    delete flag1, flag2;
    return 0;
}

