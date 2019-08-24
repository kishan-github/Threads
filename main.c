#include <stdio.h>
#include <pthread.h>

#define MAX_THREAD 10

int var = 10;
pthread_mutex_t update_mutex;

// Routine to be executed when a thread is created.
void *routine(void *arg)
{
	char *value = NULL;
	static int check = 10;

	value = (char *)arg;

	pthread_mutex_lock(&update_mutex);
	var++;
	check = var;
	pthread_mutex_unlock(&update_mutex);

	printf("\nIn routine, Thread id = %lu, Global = %d, Static = %d\n", pthread_self(), var, check);
	printf("Argument received = %s", value);
	pthread_exit(&check);
}

// Main function.
int main()
{
	pthread_t tid[MAX_THREAD];
	int rt = -1;
	char str[10] = "abcd";
	int idx = 0;
	int *thread_ret = NULL;

	printf("\nIn main, Thread id = %lu\n", pthread_self());

	pthread_mutex_init(&update_mutex, NULL);

	for(;idx < MAX_THREAD; idx++)
	{
		rt = pthread_create(&tid[idx], NULL, routine, (void *)str);
		if(rt)
		{
			printf("\nThread creation failed\n");
			return 0;
		}
	}

	for(idx = 0; idx < MAX_THREAD; idx++)
	{
		pthread_join(tid[idx], (void *)&thread_ret);
		printf("\nValue returned = %d by thread = %lu\n", *thread_ret, tid[idx]);
	}
	printf("\nThread finished = %s\n", str);
	pthread_mutex_destroy(&update_mutex);
	pthread_exit(NULL);
	return 0;
}
