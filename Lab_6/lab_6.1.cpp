#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

int main()
{
	const char* shm_name = "/shm_name";
    auto sem_read_name   = "/sem_read";
    auto sem_write_name  = "/sem_write";
    
    sem_t *sem_read  = sem_open(sem_read_name,  O_CREAT, 0644, 1);
    sem_t *sem_write = sem_open(sem_write_name, O_CREAT, 0644, 1);

	int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0644);
	const int buff = 32;
	
	ftruncate(fd, buff*sizeof(char));
	
	char *ptr = (char*)mmap(0, buff*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	getlogin_r(ptr, buff);
  	
    fcntl(STDIN_FILENO, F_SETFL,  O_NONBLOCK);
    while(getchar() != '\n')
    {
        sem_wait(sem_write);
    	
    	std::cin >> ptr;
    	std::cout << "\x1b[42mSend:\x1b[0m " << ptr << '\n';
    	
    	sem_post(sem_read);
    	sleep(1);
    }

	munmap(ptr,buff*sizeof(char));
    close(fd);
    sem_unlink(sem_write_name);
    shm_unlink(shm_name);
    sem_destroy(sem_write);
    return 0;
}
