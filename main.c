#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREAD 3
#define MAX_VALUE 30

int var = 10;
pthread_mutex_t update_mutex;
pthread_cond_t value_cond;

// Routine which will wait for particular condition.
void *wait_cond(void *arg)
{
	pthread_mutex_lock(&update_mutex);
	while(var < MAX_VALUE)
	{
		pthread_cond_wait(&value_cond, &update_mutex);
		printf("\nSignal received with value = %d\n", var);
	}
	pthread_mutex_unlock(&update_mutex);
	pthread_exit(NULL);
}

// Routine to be executed when a thread is created.
void *routine(void *arg)
{
	while(var < MAX_VALUE)
	{
		pthread_mutex_lock(&update_mutex);
		var++;
		if((var%5) == 0)
		{
			pthread_cond_signal(&value_cond);
			printf("\nSignal sent with value = %d, thread id = %lu\n", var, pthread_self());
		}

		printf("\nIn routine, Thread id = %lu, Global = %d\n", pthread_self(), var);
		pthread_mutex_unlock(&update_mutex);
		sleep(1);
	}

	pthread_exit(NULL);
}

// Main function.
int main()
{
	pthread_t tid[MAX_THREAD], wait_tid;
	int rt = -1;
	char str[10] = "abcd";
	int idx = 0;

	printf("\nIn main, Thread id = %lu\n", pthread_self());

	pthread_mutex_init(&update_mutex, NULL);
	pthread_cond_init(&value_cond, NULL);

	rt = pthread_create(&wait_tid, NULL, wait_cond, NULL);
	if(rt)
		printf("\n%s : %d - Thread creation failed\n", __func__, __LINE__);

	for(;idx < MAX_THREAD; idx++)
	{
		rt = pthread_create(&tid[idx], NULL, routine, (void *)str);
		if(rt)
		{
			printf("\nThread creation failed\n");
			return 0;
		}
	}

	pthread_join(wait_tid, NULL);
	for(idx = 0; idx < MAX_THREAD; idx++)
	{
		pthread_join(tid[idx], NULL);
	}

	pthread_mutex_destroy(&update_mutex);
	pthread_cond_destroy(&value_cond);
	pthread_exit(NULL);
	return 0;
}
