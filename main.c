#include <stdio.h>
#include <stdlib.h>
#include "conv_utf.h"

int main(void){
	/*
	FILE *f;
	unsigned char c;
	f = fopen("asd.txt", "rb");
	
	if(f==NULL){
		printf("erro!");
		exit(1);
	}
	
	while(fscanf(f, "%c", &c)>0) printf("%x ", c);
	fclose(f);
	*/
	printf("\n");
	printf("%d", contaUns((char) 0xE1));

	printf("\n");
	
	return 0;
}
