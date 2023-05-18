#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/utsname.h>
//CAP_SYS_ADMIN;
int main()
{
	system("clear");
	pid_t pid = fork();
	
	pthread_mutex_t mute;
	pthread_mutex_init(&mute, NULL);
		
	utsname uts;
	
	if(pid != 0)
	{
		sleep(1);
		pthread_mutex_lock(&mute);
		uname(&uts);
				
		std::cout << "\x1b[42mParent:\x1b[0m\n"
				  << "system name: "         << uts.sysname    << ";\n"
				  << "\x1b[44mnode name:   " << uts.nodename   << "\x1b[0m;\n"
				  << "\x1b[44mdomain name: " << uts.domainname << "\x1b[0m;\n"
				  << "release:     "         << uts.release    << ";\n"
				  << "version:     "         << uts.version    << ";\n"
				  << "machine:     "         << uts.machine    << ";\n\n";
				  
		pthread_mutex_unlock(&mute);
	}
	else if (pid == -1)
	{
		perror("fork");
		return 1;
	}
	else
	{
		if(unshare(CLONE_NEWUTS) == -1) perror("unshare");
		pthread_mutex_lock(&mute);
		
		const char *newhost   = "h_banana";
		const char *newdomain = "d_banana";
		
		if(sethostname(newhost, sizeof(newhost)) == -1) perror("sethostname");
		if(setdomainname(newdomain, sizeof(newdomain)) == -1) perror("setdomainname");
		
		uname(&uts);
		
		std::cout << "\x1b[42mDaughter:\x1b[0m\n"
				  << "system name: "         << uts.sysname    << ";\n"
				  << "\x1b[44mnode name:   " << uts.nodename   << "\x1b[0m;\n"
				  << "\x1b[44mdomain name: " << uts.domainname << "\x1b[0m;\n"
				  << "release:     "         << uts.release    << ";\n"
				  << "version:     " 		 << uts.version    << ";\n"
				  << "machine:     " 		 << uts.machine    << ";\n\n";
				  
		pthread_mutex_unlock(&mute);
	}
	pthread_mutex_destroy(&mute);
	return 0;
}
