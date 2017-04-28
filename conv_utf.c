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
	unsigned int ret=0;
	unsigned char c1, c2, c3, c4;
	
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
			s-=0xD800;
			ret = s;
			ret = ret<<10;
			if(leShort(&s, arq_entrada)==-1){ 
				fprintf(stderr, "Erro de leitura! Arquivo corrompido!");
				return -1;
			}
			s-= 0xDC00;
			ret+= s;
			ret+= 0x10000;
		}
		else		//so tem um code unit
			ret = s;
		//ESCREVER -> transformar ret (unicode) em utf_8
		
	
		if (ret <= 0x007F) //primeira faixa
		{
			c1 = ret;
			printf("c1:%x ", c1);
			fwrite(&c1,1,1,arq_saida);
		
		}
		else if (ret <= 0x07FF) //segunda faixa
		{

			c1 = 0xC0|(ret>>6); //primeiro byte recebe 5 primeiros bits
			c2 = 0x80|ret;
			c2 = 0xBF&c2;
			// escreve 2 bytes
			printf("c1:%x ", c1);
			printf("c2:%x ", c2);
			fwrite(&c1,1,1,arq_saida);
			fwrite(&c2,1,1,arq_saida);
		}
		else if (ret <= 0xFFFF) //terceira faixa
		{
			printf("ret:%x ", ret);
			c1 = 0xE0|(ret>>12); //primeiro byte recebe 4 primeiros bits
			c2 = 0x3f&(ret>>6);
			c2 = c2|0x80;

			c3 = 0x3F&ret;
			c3 = c3|0x80;
			// escreve 3 bytes
			printf("c1:%x ", c1);
			printf("c2:%x ", c2);
			printf("c3:%x ", c3);
			fwrite(&c1,1,1,arq_saida);
			fwrite(&c2,1,1,arq_saida);
			fwrite(&c3,1,1,arq_saida);
		}
		else if (ret <= 0x10FFFF)
		{
			c1 = (ret>>18)|0xF0; //primeiro byte recebe 3 primeiros bits
//			1d11e
//			E6B0B4 é com han devia aparcer no txt
			printf("ret:%x ", ret);
			c2 = 0x03f&(ret>>12);
			c2 = c2|0x080;

			c3 = 0x03f&(ret>>6);
			c3 = c3|0x80;

			c4 = 0x03f&ret;
			c4 = c4|0x080;
			// escreve 4 bytes
			printf("c1:%x ", c1);
			printf("c2:%x ", c2);
			printf("c3:%x ", c3);
			printf("c4:%x ", c4);
			fwrite(&c1,1,1,arq_saida);
			fwrite(&c2,1,1,arq_saida);
			fwrite(&c3,1,1,arq_saida);
			fwrite(&c4,1,1,arq_saida);
		}
		
		//FIM
		
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
