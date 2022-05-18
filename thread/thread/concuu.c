#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREAD      2

pthread_t tid[MAX_THREAD];
int counter;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
/*Dynamically Initializing:
 * int pthread_mutex_init(pthread_mutex_t *mutex,
 * const pthread_mutexattr_t *attr);*/

void *threadFunc(void *argv)
{
	pthread_mutex_lock(&mutex);
	counter += 1;
	printf("Thread %d has started\n", counter);
	sleep(1);
	printf("Thread %d has finished\n", counter);
 
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main(void)
{
	int i = 0;
	int ret = 0;

	for (i = 0; i < MAX_THREAD; i++)
	{
		ret = pthread_create(&(tid[i]), NULL, threadFunc, NULL);
		if (ret != 0)
		{
			printf("Thread [%d] created error\n", i);
		}
	}
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	return 0;
}
