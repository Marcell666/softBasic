#include <stdio.h>

int utf8_16(FILE *arq_entrada, FILE *arq_saida);
int utf16_8(FILE *arq_entrada, FILE *arq_saida);
int contaUns(unsigned char c);
int leShort(unsigned short *s, FILE *f);
int leMaisDeUmByte(FILE *f, unsigned char c, int n);
