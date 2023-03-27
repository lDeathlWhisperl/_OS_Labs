#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using std::cout;
using std::flush;

int main()
{
    system("clear");
    pid_t pid = fork();

    if(pid>0)
    {
        int status, i = 0;
        cout << "\x1b[42mParent process id: " << getppid() << "; Own id: " << getpid() << "; Daughter process id: " << pid << "\x1b[0m\n";
        const char *wait[] = {"Waiting.  ", "Waiting.. ", "Waiting..."};

        while(waitpid(pid, &status, WNOHANG) == 0)
        {
            cout << wait[i] << flush;
            sleep(1);
            cout << "\b\b\b\b\b\b\b\b\b\b" << flush;
            if(++i > 2) i = 0;
        }
        cout << "\nstatus: " << status << '\n';
    }
    else if (pid == 0)        
        execl("lab_3.1", "\x1b[42mDaughter process id: ", std::to_string(getpid()).c_str(), "; Parent process id: ", std::to_string(getppid()).c_str(), "\x1b[0m ", NULL);
    return 0;
}
