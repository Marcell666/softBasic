#include <stdio.h>
#include <stdlib.h>
#include "conv_utf.h"

int main(void){
	FILE *f, *f2;
	f = fopen("asd.txt", "rb");
	f2 = fopen("asd3.txt", "wb");
	
	if(f==NULL){
		printf("erro!");
		exit(1);
	}

	utf8_16(f, f2);

	fclose(f);
	fclose(f2);
	printf("\n");
	//printf("%d", contaUns((char) 0xE1));

	printf("\n");
	
	return 0;
}
