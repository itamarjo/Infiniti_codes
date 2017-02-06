

#include <stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>




int main(int argc, char *argv[], char **envp)
{

	int status;
	pid_t pid_child = fork();
	while(1)
	{
		if(pid_child != 0)
		{
			wait(&status);
			pid_child = fork();
		}
		if(pid_child == 0)
		{
			execvp(argv[1], argv + 1);
		}
	}
	return(0);
}
