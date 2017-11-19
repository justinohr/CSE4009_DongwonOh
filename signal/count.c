#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int fd = -1;
struct sigaction act;
int c[1];
pid_t pid[3];
int n[1];
int count;
int f_open(char * f){
   if((fd = open(f, O_RDWR)) > 0){
      if(pread(fd, c, 4, 0) != 4){
         fprintf(stderr, "read error\n");
         return 0;
      }
       c[0] = 0;
      if(pwrite(fd, c, 4, 0) != 4){
         fprintf(stderr, "pwrite error\n");
         return 0;
      }
   }
   else if((fd = open(f, O_RDWR | O_CREAT | O_EXCL |O_TRUNC, 0644)) > 0){
      c[0] = 0;
      if(pwrite(fd, c, 4, 0) != 4){
         fprintf(stderr, "pwrite error\n");
         return 0;
      }
   }
   return fd;
}
void signal_h(void*arg){
  pread(fd,pid,12,8);
  pread(fd,c,4,0);
  pread(fd,n,4,4);
   c[0]++;
   if(pwrite(fd, c, 4, 0) != 4){
      fprintf(stderr, "write error\n");
      return NULL;
   }
   if(c[0]==n[0])
	killpg(getppid(),SIGUSR2);
   if(getpid()==pid[0])
      kill(pid[1], SIGUSR1);
   else if(getpid()==pid[1])
      kill(pid[2], SIGUSR1);
   else
      kill(pid[0], SIGUSR1);

   sigaction(SIGUSR1, &act, NULL);
   return;
}
int main(int argc, char * argv[]){
   act.sa_handler = signal_h;
   sigemptyset(&act.sa_mask);

   if(argc == 1){
      fprintf(stderr,"input error\n");
      return 0;
   } 
   if(argc == 2){
      fprintf(stderr,"input error\n");
      return 0;
   }

   if((fd=f_open(argv[2])) == 0){
      fprintf(stderr, "open error\n");
      return 0;
   }
   if(atoi(argv[1])<=0){
     fprintf(stderr,"number error\n");
     return 0;
   }
   n[0] = atoi(argv[1]);
   if(pwrite(fd, n, 4, 4) != 4){
      fprintf(stderr, "pwrite error\n");
      return 0;
   }

   if(sigaction(SIGUSR1, &act, NULL) < 0)
         fprintf(stderr, "sig error\n");

   if( (pid[0]=fork()) < 0 ){
      fprintf(stderr, "fork error\n");
      exit(1);
   }
   else if(pid[0] == 0){ 
      while(1)
         pause();
   }
   if( (pid[1]=fork()) < 0){
      fprintf(stderr, "fork error\n");
      exit(1);
   }
   else if(pid[1] == 0){

      while(1)
         pause();
   }
   if( (pid[2]=fork()) < 0 ){
      fprintf(stderr, "fork error\n");
      exit(1);
   }
   else if(pid[2]==0){
      while(1)
         pause();
   }
   sleep(1);

   if(pwrite(fd, pid, 12, 8) != 12){
   fprintf(stderr, "pwrite error\n");
      return 0;
   }
   kill(pid[0],SIGUSR1);
   pause();
   return 0;
}
