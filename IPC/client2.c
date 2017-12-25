#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#define MSGSIZE 1024
#define SHMSIZE 1200000
#define SHMKEY 1000

typedef struct{
	long mtype;
	char mtext[MSGSIZE];
} MsgType;

int main(void){
	key_t key = 1001;
	int cmd;
	int target;
	int nbytes;
	int que_id = msgget(key, IPC_CREAT|0666);
	int shm_id;
	MsgType *new_msg;
	char msg[MSGSIZE];
	MsgType temp;
	void *shm_addr;
	int msg_size = sizeof(temp) - sizeof(temp.mtype);
	int pid = getpid();
	if((shm_id = shmget((key_t)SHMKEY, SHMSIZE, IPC_CREAT|0666)) < 0){
		printf("shmget fail\n");
		return -1;
	}
	if((shm_addr = shmat(shm_id, (void*) 0, 0)) == (void*)-1){
		printf("shmat fail\n");
		return -1;
	}
	printf("Your PID is: %d\n", pid);
	while(1){
		printf("1. Send a new message\n");
		printf("2. Check received message(s)\n");
		printf("3. Check chat log\n");
		printf("4. Exit\n");
		scanf("%d", &cmd);
		getchar(); //flushing new line character buffer
		switch(cmd){
		case 1:
			printf("Send to whom? If you want to send to all, input 1.\n");
			scanf("%d", &target);
			getchar();
			printf("Input the message\n");
			new_msg = (MsgType*)malloc(sizeof(MsgType));
			new_msg->mtype = target;
			sprintf(new_msg->mtext, "[%d] ", pid); 
			gets(msg);
			strcat(new_msg->mtext, msg);
			msgsnd(que_id, new_msg, msg_size, IPC_NOWAIT);
			printf("Message is sent successfully\n");
			break;
		case 2:
			nbytes = msgrcv(que_id, &temp, msg_size, 2, IPC_NOWAIT);
			if(nbytes > 0) printf("%s\n", temp.mtext);
			else{
				if(errno == ENOMSG) printf("No message\n");
			}
			break;
		case 3:
			printf("%s", (char*)shm_addr);
			break;
		default:
			if(shmdt(shm_addr) == 0) printf("Shared memory is detached successfully.\n");
			exit(0);
		}
	}
	return 0;
}
