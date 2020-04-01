#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
pid_t   pid;
int 	i;
	/* fork another process */
	i=100;
		printf("This is the fork() sample code.\n");
	pid = fork();
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		exit(-1);
	}
	else if (pid == 0) { /* child process */
		srand(time(NULL));
		i = (rand() % 10) +1;
		sleep(i);
		printf("This is the child process %d! I sleep %d seconds.\n",pid,i);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		srand(time(NULL)+pid);
		i = (rand() % 10) +1;
		sleep(i);
		printf ("This is the parent process %d! I sleep %d seconds.\n",pid,i);
		exit(0);
	}
}