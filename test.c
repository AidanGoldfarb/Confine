#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void delay(int sec);

int main(int argc, char **argv){
	//delay(4);
	//printf("done\n");
	FILE *f = fopen("output.txt", "w");
	// //printf("I am in test.c\narg1: %s\narg2: %s\n", argv[0],argv[1]);
	char *big = malloc(68108864/8);
	for(int i = 0; i<68108864/8; i++){
		big[i] = 'f';
	}
	fprintf(f,"%s",big);
	fclose(f);
	//printf("big is size: %ld\n", sizeof(big));
	/*buffer overflow: seg fault*/
	// char buf[2];
	// strcpy("LLLLLLLLLLLL", buf);
	/*-end of seg fault code-*/
	return 1;
}
void delay(int sec){
	int mil = 1000000*sec;
	clock_t start = clock();
	while(clock()<start+mil);
}