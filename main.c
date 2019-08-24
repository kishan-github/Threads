#include <stdio.h>
#include <pthread.h>

// Routine to be executed when a thread is created.
void *routine(void *arg)
{
	char *value = NULL;

	value = (char *)arg;
	printf("\nIn routine, Thread id = %lu\n", pthread_self());
	printf("\nArgument received = %s\n", value);
	pthread_exit(NULL);
}

// Main function.
int main()
{
	pthread_t tid;
	int rt = -1;
	char str[10] = "abcd";

	printf("\nIn main, Thread id = %lu\n", pthread_self());
	rt = pthread_create(&tid, NULL, routine, (void *)str);
	if(rt)
	{
		printf("\nThread creation failed\n");
		return 0;
	}
	pthread_join(tid, NULL);
	printf("\nThread finished = %s\n", str);
	pthread_exit(NULL);
	return 0;
}
