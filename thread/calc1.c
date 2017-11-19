#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mul(int**, int**, long long int**);
void add(long long int**);
int main(void){
	int **A, **B;
	long long int **C;
	int i, j;
	FILE *fd1, *fd2;
	time_t start, finish;
	A = (int**)malloc(sizeof(int*) * 4000);
	B = (int**)malloc(sizeof(int*) * 4000);
	C = (long long int**)malloc(sizeof(long long int*) * 4000);
	for(i = 0; i < 4000; i++){
		A[i] = (int*)malloc(sizeof(int) * 4000);
		B[i] = (int*)malloc(sizeof(int) * 4000);
		C[i] = (long long int*)malloc(sizeof(long long int) * 4000);
	}
	printf("Allocation Done\n");
	fd1 = fopen("sample1.txt", "r");
	fd2 = fopen("sample2.txt", "r");
	for(i = 0; i < 4000; i++) for(j = 0; j < 4000; j++)
		fscanf(fd1, "%d", &A[i][j]), fscanf(fd2, "%d", &B[i][j]);
	printf("Starting multiplication\n");
	start = time(NULL);
	mul(A, B, C);
	add(C);
	finish = time(NULL);
	printf("Elapsed seconds: %ld", finish - start);
	return 0;
}

void mul(int** A, int** B, long long int** C){
	int i,j,k;
	long long int sum;
	for(i = 0; i < 4000; i++){
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
