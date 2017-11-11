#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

static void sig_fn(int);
int main(void){
	struct sigaction act_new;
	struct sigaction act_old;

	act_new.sa_handler = sig_fn;
	sigemptyset(&act_new.sa_mask);

	printf("Signal number: %d\n", SIGINT);
	sigaction(SIGINT, &act_new, &act_old); //register handler
	for(;;) pause();
	return 0;
}

static void sig_fn(int signo){
	if(signo == SIGINT)
		printf("Ctrl-C is pressed. Try Again\n");
	return;
}
