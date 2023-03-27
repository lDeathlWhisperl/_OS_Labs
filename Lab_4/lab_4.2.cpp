#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <fstream>

using std::cout;
using std::flush;

int main()
{
    auto sem_name = "/mysem";
    sem_t *sem = new sem_t;
    sem = sem_open(sem_name, O_CREAT);

    std::ofstream fout;
    fcntl(STDIN_FILENO, F_SETFL,  O_NONBLOCK);

    while(true)
    {
        if(sem == nullptr) continue;
        sem_wait(sem);

        fout.open("output.txt", std::ios::app);
        for(int i = 0; i < 10; ++i)
        {
            fout << 2;
            cout << 2;
        }
        fout.close();

        sem_post(sem);
        sem_unlink(sem_name);

        if(getchar() == '\n') break;

        sleep(1);
    }

    sem_unlink(sem_name);
    sem_destroy(sem);
    return 0;
}
