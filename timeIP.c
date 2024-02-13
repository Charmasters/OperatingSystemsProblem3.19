/*
 * Writen by Samuel Armstrong
 * For Operating Systems Concepts
 * With Dr. Baas
 * For Ch 3 Programming (3.19)
 *
 * This program uses Pipes to share memory between a child and parent process
 * and print the time required run a linux command
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/kernel.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char* args[]){
	if(argc < 2) return 0;
	float time = 0;
	struct timeval startTime;
	struct timeval endTime;
	
	int fd[2];
	if(pipe(fd)==-1){
		printf("Pipe failed\n");
		return 1;
	}

	pid_t pid = fork();

	if(pid < 0){
		printf("Fork failed");
		return 1;
	}
		
	if(pid == 0){
		close(fd[0]);
		gettimeofday(&startTime, 0);
		write(fd[1], &startTime.tv_usec, sizeof(startTime.tv_usec)+1);
		write(fd[1], &startTime.tv_sec, sizeof(startTime.tv_sec)+1);
		execvp(args[1], &args[1]);
		return 0;
	}
	close(fd[1]);
	read(fd[0], &startTime.tv_usec, sizeof(startTime.tv_usec)+1);
	read(fd[0], &startTime.tv_sec, sizeof(startTime.tv_sec)+1);
	wait(NULL);
	gettimeofday(&endTime, 0);
	time = (endTime.tv_sec-startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec)/1000000.0;
	printf("Elapsed Time: %f\n", time);

	return 0;
}
