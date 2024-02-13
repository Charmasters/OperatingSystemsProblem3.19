/*
 * Writen by Samuel Armstrong
 * For Operating Systems Concepts
 * With Dr. Baas
 * For Ch 3 Programming (3.19)
 *
 * This program uses Shared Memory to share memory between a child and parent process
 * and print the time required run a linux command
 * Note: must be compiled with - $gcc timeSM.c -o timeSM -lrt
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* args[]){
	if(argc < 2) return 0;
	float time = 0;
	struct timeval startTime;
	struct timeval endTime;
	
	pid_t pid = fork();

	if(pid < 0){
		printf("Fork failed");
		return 1;
	}
		
	if(pid == 0){
		gettimeofday(&startTime, 0);
		int size = 2*sizeof(long);
		int fd = shm_open("TimeOfDay", O_CREAT | O_RDWR, 0666);
		ftruncate(fd, size);
		long* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
		*ptr = startTime.tv_sec;
		ptr += sizeof(long);
		*ptr = startTime.tv_usec;

		execvp(args[1], &args[1]);
		return 0;
	}
	wait(NULL);
	int size = 2*sizeof(long);
	int fd = shm_open("TimeOfDay", O_RDONLY, 0666);
	ftruncate(fd, size);
	long* ptr = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0); 
		
	long startSec = *ptr;
	ptr += sizeof(startTime.tv_sec);
	long startUSec = *ptr;

	gettimeofday(&endTime, 0);
	
	time = (endTime.tv_sec-startSec) + (endTime.tv_usec - startUSec)/1000000.0;
	printf("Elapsed Time: %f\n", time);

	return 0;
}
