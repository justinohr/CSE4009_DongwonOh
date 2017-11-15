#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo);
static void sig_int(int signo);
unsigned int sleep2(unsigned int seconds);

int main(void)
{
	unsigned int unslept;
	struct sigaction act_new, act_old;
	sigset_t set;
	act_new.sa_handler = sig_int;
	sigaddset(&set, SIGALRM);
	act_new.sa_mask = set;
	sigaction(SIGINT, &act_new, &act_old);
	unslept = sleep2(5);

	printf("sleep2 returned: %u\n", unslept);
	return 0;
}

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
}

static void sig_int(int signo)
{
	int i,j;
	volatile int k;

	printf("\nsig_int starting\n");

	for (i = 0; i<30000; i++)
		for (j = 0; j<80000; j++)
			k += i*j;
	
	printf("sig_int finished\n");
}

unsigned int sleep2(unsigned int seconds)
{
	struct sigaction act_new, act_old;
	act_new.sa_handler = sig_alrm;
	sigemptyset(&act_new.sa_mask);	
	sigaction(SIGALRM, &act_new, &act_old);
	
	if (setjmp(env_alrm) == 0){
		alarm(seconds);
		pause();
	}
	return alarm(0);
}
