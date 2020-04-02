#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>			//API for POSIX
#include <errno.h>
//#include <pthread.h>			//POSIX Thread
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "protocol.h"

typedef struct
{
	int pid;
	int num;
	int max;
	int status;
	int turns;
	int count;
	bool done;
} data;

int main(int argc, char *argv[])
{
	pid_t pid;
	int num = atoi(argv[1]);

	int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
	if (fd < 0)
	{
		perror("shm_open() in main\n");
		return EXIT_FAILURE;
	}
	ftruncate(fd, SIZE);
	data *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	ptr->status = 1;
	ptr->count = 0;
	ptr->max = 0;
	ptr->done = false;

	pid = fork();

	if (pid == 0)		//child process
	{
		while (ptr->num != 1)
		{
			while (ptr->status == 1)
				;

			if (ptr->done)
				exit(0);

			if (ptr->num % 2 == 0)
				ptr->num /= 2;
			else
				ptr->num = ptr->num * 3 + 1;

			printf("[%d Child]: %d\n", getpid(), ptr->num);

			ptr->count++;

			if (ptr->num > ptr->max)
			{
				ptr->max = ptr->num;
				ptr->turns = ptr->count;
			}

			if (ptr->num == 1)
			{
				ptr->status = 1;
				exit(0);
			}
			ptr->status = 1;
		}
	}
	else			//parent process
	{
		ptr->pid = pid;
		ptr->num = num;
		ptr->status = 0;

		while (ptr->num != 1)
		{
			while (ptr->status == 0)
				;

			if (ptr->num == 1)
			{
				printf("[%d Parent]: %d\n", getpid(), ptr->num);
				break;
			}

			if (ptr->num % 2 == 0)
				ptr->num /= 2;
			else
				ptr->num = ptr->num * 3 + 1;

			printf("[%d Parent]: %d\n", getpid(), ptr->num);

			ptr->count++;

			if (ptr->num > ptr->max)
			{
				ptr->max = ptr->num;
				ptr->turns = ptr->count;
			}

			if (ptr->num == 1)
			{
				ptr->done = true;
				ptr->status = 0;
				break;
			}
			ptr->status = 0;
		}

		int status;
		waitpid(pid, &status, 0);

		printf("Max: %d\n", ptr->max);
		printf("Turn: %d\n", ptr->turns);

		munmap(ptr, SIZE);
		shm_unlink(NAME);
	}

	return 0;
}

