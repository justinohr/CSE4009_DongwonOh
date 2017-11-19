#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct mat mat;
struct mat{
	int **A;
	int **B;
	long long int** C;
};
void* mul1(void *arg);
void* mul2(void *arg);
void add(long long int**);
int main(void){
	mat arg;
	FILE *fd1, *fd2;
	time_t start, finish;
	pthread_t tid1, tid2;
	arg.A = (int**)malloc(sizeof(int*) * 4000);
	arg.B = (int**)malloc(sizeof(int*) * 4000);
	int i, j;
	arg.C = (long long int**)malloc(sizeof(long long int*) * 4000);
	for(i = 0; i < 4000; i++)
		arg.A[i] = (int*)malloc(sizeof(int)*4000),
		arg.B[i] = (int*)malloc(sizeof(int)*4000), 
		arg.C[i] = (long long int*)malloc(sizeof(long long int) * 4000);
	fd1 = fopen("sample1.txt", "r");
	fd2 = fopen("sample2.txt", "r");
	for(i = 0; i < 4000; i++) for(j = 0; j < 4000; j++) 
		fscanf(fd1, "%d", &(arg.A[i][j])), fscanf(fd2, "%d", &(arg.B[i][j])); 
	printf("Starting multiplication\n");
	start = time(NULL);
	pthread_create(&tid1, NULL, mul1, (void*)&arg);
	pthread_create(&tid2, NULL, mul2, (void*)&arg);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	add(arg.C);
	finish = time(NULL);
	printf("Elapsed seconds: %ld", finish - start);
	return 0;
}

void* mul1(void *arg){
	int i,j,k;
	mat* argv  = (mat*)arg;
	int **A = argv->A;
	int **B = argv->B;
	long long int **C = argv->C;
	long long int sum;
	for(i = 0; i < 2000; i++){
		for(j = 0; j < 4000; j++){
			sum = 0;
			for(k = 0; k < 4000; k++){
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
			printf("C[%d][%d] done\n",i,j);
		}
	}
}
void* mul2(void* arg){
	int i,j,k;
	mat* argv = (mat*)arg;
	int **A = argv->A;
	int **B = argv->B;
	long long int **C = argv->C;
	long long int sum;
	for(i = 2000; i < 4000; i++){
		for(j = 0; j < 4000; j++){
			sum = 0;
			for(k = 0; k < 4000; k++){
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
			printf("C[%d][%d] done\n",i,j);
		}
	}
}
void add(long long int **C){
	long long int sum = 0;
	int i,j;
	for(i = 0; i < 4000; i++) for(j = 0; j < 4000; j++) sum += C[i][j];
}
