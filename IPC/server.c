#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#define SHMSIZE 1200000
#define SHMKEY 1000
#define MAX(a, b) a >= b? a : b

typedef struct{
	long mtype;
	char mtext[1024];
} MsgType;

int main(void){
	int i, j, k;
	key_t key1 = 1000;
	key_t key2 = 1001;
	key_t key3 = 1002;
	int que_id[4]; /*using index from 1*/
	int pid[4]; /*using index from 1*/
	MsgType msg;
	void *shm_addr;
	int shm_id;
	que_id[1] = msgget(key1, IPC_CREAT|0666);
	que_id[2] = msgget(key2, IPC_CREAT|0666);
	que_id[3] = msgget(key3, IPC_CREAT|0666);
	while((shm_id = shmget(SHMKEY, SHMSIZE, IPC_CREAT|0666)) == -1);
	if((shm_addr = shmat(shm_id, (void*)0, 0)) == (void*) -1){
		printf("shmat fail\n");
		return -1;
	}
	printf("Which processes are in this server?\n");
	for(i = 1; i <= 3; i++){
		printf("Process %d: ", i);
		scanf("%d", &pid[i]);
	}
	int msg_size = sizeof(msg) - sizeof(msg.mtype);
	int nbytes;
	int offset = 0;
	while(1){
		for(i = 1; i <= 3; i++){
			for(j = 1; j <= 4; j++){
				while((nbytes = msgrcv(que_id[i], &msg, msg_size, j <= 3? pid[j] : 1, IPC_NOWAIT)) > 0){
					if(msg.mtype == 1){
						printf("%s\n", msg.mtext);
						sprintf((char*)(shm_addr + offset), "%s\n", msg.mtext);
						offset += strlen(msg.mtext) + 1;
					}
					else{
						for(k = 1; k <= 3; k++){
							if(pid[k] == msg.mtype){
								printf("sent to %d\n", pid[k]);
								msg.mtype = 2;
								msgsnd(que_id[k], &msg, msg_size, IPC_NOWAIT);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
