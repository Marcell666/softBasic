/*Bruce Marcellino 1613172 3WB*/
/*Bruno Marinho 1613071 3WB*/

#include <stdio.h>
#include "conv_utf.h"

int contaUns(unsigned char c);
int leShort(unsigned short *s, FILE *f);
int leMaisDeUmByte(FILE *f, unsigned char c, int n);

int utf8_16(FILE *arq_entrada, FILE *arq_saida){
	unsigned int uniC;
	unsigned char c = 0xFE;
	if(fwrite(&c, 1, 1, arq_saida) != 1){
		fprintf(stderr, "Erro de gravação!");
		return -1;
	};
	c = 0xFF;
	if(fwrite(&c, 1, 1, arq_saida) != 1){
		fprintf(stderr, "Erro de gravação!");
		return -1;
	}
	while(fread(&c,1,1,arq_entrada)>0){
		uniC = leMaisDeUmByte(arq_entrada,c, contaUns(c));
		if(uniC==-1){ fprintf(stderr, "Erro na leitura! Arquivo corrompido");
			return -1;
		}
		if(uniC<=0xFFFF){
			c = uniC>>8;
			
			if(fwrite(&c, 1, 1, arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			c = uniC;
			if(fwrite(&c, 1, 1, arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
		}
		else if(uniC<=0x10FFFF){
			uniC-=0x10000;
			
			c = uniC>>18;
			c +=0xD8;
			if(fwrite(&c, 1, 1, arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			c = (uniC>>10);	
			if(fwrite(&c, 1, 1, arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			c = (uniC>>8)&0x03;
			c+=0xDC;
			if(fwrite(&c, 1, 1, arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			c=uniC;
			if(fwrite(&c, 1, 1, arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
		}
	}
	return 0;
}

int leMaisDeUmByte(FILE *f, unsigned char c, int n){//n= {2,3,4}
	int ret;
	int i;

	ret=c;
	if(n==2) ret-=0xC0;
	else if(n==3) ret-=0xE0;
	else if(n==4) ret-=0xF0;
	for(i=n;i>=2;i--){
		ret=ret<<6;
		if(fread(&c,1,1,f)<1) return -1;
		ret+=c&0x3F;
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
		
	
		if (ret <= 0x007F) //primeira faixa
		{
			c1 = ret;
			if(fwrite(&c1,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
		
		}
		else if (ret <= 0x07FF) //segunda faixa
		{

			c1 = 0xC0|(ret>>6); //primeiro byte recebe 5 primeiros bits
			c2 = 0x80|ret;
			c2 = 0xBF&c2;
			// escreve 2 bytes
			if(fwrite(&c1,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			if(fwrite(&c2,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
		}
		else if (ret <= 0xFFFF) //terceira faixa
		{
			c1 = 0xE0|(ret>>12); //primeiro byte recebe 4 primeiros bits
			c2 = 0x3f&(ret>>6);
			c2 = c2|0x80;

			c3 = 0x3F&ret;
			c3 = c3|0x80;
			// escreve 3 bytes
			if(fwrite(&c1,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			if(fwrite(&c2,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			if(fwrite(&c3,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
		}
		else if (ret <= 0x10FFFF)
		{
			c1 = (ret>>18)|0xF0; //primeiro byte recebe 3
			c2 = 0x03f&(ret>>12);
			c2 = c2|0x080;

			c3 = 0x03f&(ret>>6);
			c3 = c3|0x80;

			c4 = 0x03f&ret;
			c4 = c4|0x080;
			// escreve 4 bytes
			if(fwrite(&c1,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			if(fwrite(&c2,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			if(fwrite(&c3,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
			if(fwrite(&c4,1,1,arq_saida) != 1){
				fprintf(stderr, "Erro de gravação!");
				return -1;
			}
		}
		
	}
	if(a==-1){ 
		fprintf(stderr, "Erro de leitura! Arquivo corrompido!");
		return -1;
	}
	return 0;
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
