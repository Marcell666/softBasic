#include <stdio.h>
#include "conv_utf.h"


int utf8_16(FILE *arq_entrada, FILE *arq_saida){
	unsigned int uniC;
	unsigned int u16;
	unsigned char c;
	while(fread(&c,1,1,arq_entrada)>0){
		uniC = leMaisDeUmByte(arq_entrada,c, contaUns(c));
		if(uniC==-1){ fprintf(stderr, "Erro na leitura! Arquivo corrompido");
			return -1;
		}
		if(uniC<=0xFFFF){
			fwrite(&uniC, 2, 1, arq_saida);
		}
		else if(uniC<=0x10FFFF){
			uniC-=0x10000;
			u16=(uniC&0xFC00)<<16;
			u16+=uniC&0x3FF;
			u16+=0xD800<<16;
			u16+=0xDC00;
			fwrite(&u16, 4, 1, arq_saida);
		}
	}
	return 0;
}


//https://github.com/Marcell666/softBasic/invitations

int leMaisDeUmByte(FILE *f, unsigned char c, int n){//n= {2,3,4}
	int ret;
	int i;
	ret=(c&(0xFF>>n));
	if(n>2) ret=ret<<(6*(n-1));
	for(i=n;i>=2;i--){
		if(fread(&c,1,1,f)<1) return -1;//mudei aqui
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
	int a=0;
	unsigned short s=0;
	int ret=0;
	
	leShort(&s, arq_entrada);
	if(s!=0xFEFF){
		if(s==0xFFFE)
			fprintf(stderr, "Erro! Arquivo Little-Endian!");
		else
			fprintf(stderr, "Erro! BOM inválido/ausente!");
		return -1;
	}
	while((a=leShort(&s, arq_entrada))>0){
		if(s>=0xD800){	//tem dois code units
			printf("1:%x ", s);
			s-=0xD800;
			ret = s;
			ret = ret<<10;
			if(leShort(&s, arq_entrada)==-1){ 
				fprintf(stderr, "Erro de leitura! Arquivo corrompido!");
				return -1;
			}
			printf("2:%x ", s);
			s-= 0xDC00;
			ret+= s;
			ret+= 0x10000;
		}
		else		//so tem um code unit
			ret = s;
		//ESCREVER -> transformar ret (unicode) em utf_8

		printf("%x ", ret);
	}
	if(a==-1){ 
		fprintf(stderr, "Erro de leitura! Arquivo corrompido!");
		return -1;
	}
	return ret;
}

int leShort(unsigned short *s, FILE *f){
	int ret=0;
	unsigned char c=0;
	ret = fread(s, 1, 1, f);
	if(ret<1) return ret;
	*s=(*s<<8);
	ret = fread(&c, 1, 1, f);
	if(ret<1) return -1; 
	*s+=c;
	return ret;
}
