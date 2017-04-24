#include <stdio.h>
#include "conv_utf.h"


int utf8_16(FILE *arq_entrada, FILE *arq_saida){
	//unsigned char c;
	
	
	//while(fscanf(arq_entrada, "%c", &c)>0){}
	

}

int leMaisDeUmByte(FILE *f, unsigned char c, int n){//n= {2,3,4}
	int ret;
	int i;
	ret+=(c&(0xFF>>n))<<(6*(n-1));
	for(i=n;i>=2;i--){
		fscanf(f, "%c", &c);
		ret+=(c&0x3F)<<(6*(i-2));
	}
	return ret;
}

int contaUns(unsigned char c){
	int cont = 0;
	int i=7;
	while(i>=4){
		if((c>>i)&0x1) cont++;
		else break;
		i--;
	}
	return cont;
}


int utf16_8(FILE *arq_entrada, FILE *arq_saida){
	int a;
	unsigned short s;
	unsigned short next=0;
	int ret;
	
	fscanf(arq_entrada, "%hu", &s);
	if(s!=0xFFFE && s!=0xFEFF)
		fprintf(stderr, "Erro! BOM inválido/ausente!");
		fseek(arq_entrada, -2, SEEK_CUR);
	}

	while(	fscanf(arq_entrada, "%hu", &s)>=0 ||
		(a=fscanf(arq_entrada, "%hu", &next))>=0
		){
		if(a>0 && (next>>10)&1)
			ret=((s&0x3FF)<<10)+(next&0x3FF);
		else
			ret=s&0x3FF;
		//ESCREVER -> transformar ret (unicode) em utf_8
	}

}
















