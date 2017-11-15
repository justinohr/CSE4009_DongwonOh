#include <stdio.h>

int main(void){
	int i,j,k;
	for (i = 0; i<30000; i++)
		for (j = 0; j<80000; j++)
			k += i*j;
	return 0;
}
