#include "hybrid_lock.h"
#include <errno.h>
#include <time.h>


long timedifference(struct timeval *s, struct timeval *c){
	return (c->tv_sec - s->tv_sec) * 1000000 + c->tv_usec - s->tv_usec;
}

int hybrid_lock_init(struct hybrid_lock *lock){
	if(!lock) return EINVAL;
	pthread_mutex_init(&lock->mutex, NULL);
	return 0;
}
int hybrid_lock_destroy(struct hybrid_lock *lock){
	if (!lock) return EINVAL;
	pthread_mutex_destroy(&lock->mutex);
	return 0;
}
int hybrid_lock_lock(struct hybrid_lock *lock){
	struct timeval start, current;
	long t;
	int times;
	int i;
	gettimeofday(&start, NULL);
	if(!pthread_mutex_trylock(&lock->mutex)) return 0;
	gettimeofday(&current, NULL);
	t = timedifference(&start, &current);
	if(t > 0) times = (int)(1.0/t * 1000000);
	else times = 1500000;
	for(i = 0; i < times; i++)
		if(pthread_mutex_trylock(&lock->mutex) == 0) return 0;
	return pthread_mutex_lock(&lock->mutex);
}
int hybrid_lock_unlock(struct hybrid_lock *lock){
	return pthread_mutex_unlock(&lock->mutex);
}

