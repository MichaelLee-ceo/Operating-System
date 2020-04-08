#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid < 0)
	{
		perror("Fork()");
		return EXIT_FAILURE;
	}
	else if (pid == 0)
	{
		printf("PID of child process: %d\n", getpid());
		sleep(3);
	}
	else
	{
		int status;
//		waitpid(pid, &status, 0);
		printf("PID of parent process: %d\n", getpid());

	}

	return 0;
}
