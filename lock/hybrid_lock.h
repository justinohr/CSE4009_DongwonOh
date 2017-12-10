#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

struct hybrid_lock {
	pthread_mutex_t mutex;
};

int hybrid_lock_init(hybrid_lock *lock);
int hybrid_lock_destroy(hybrid_lock *lock);
int hybrid_lock_lock(hybrid_lock *lock);
int hybrid_lock_unlock(hybrid_lock *lock);
