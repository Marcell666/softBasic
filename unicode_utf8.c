

ret // código unicode

	char c1, c2, c3, c4;
	
	if (ret <= 0x007F) //primeira faixa
	{
		c1 = ret;
		fwrite(&c1,1,1,arq_saida);
		
	}
	else if (ret <= 0x07FF) //segunda faixa
	{
		c1 = 0xC0|(ret>>6); //primeiro byte recebe 5 primeiros bits
		c2 = 0x20|ret;
		// escreve 2 bytes
		fwrite(&c1,1,1,arq_saida);
		fwrite(&c2,1,1,arq_saida);
	}
	else if (ret <= 0xFFFF) //terceira faixa
	{
		c1 = 0xE0|(ret>>12); //primeiro byte recebe 4 primeiros bits
		c2 = 0x20|(ret>>6);
		c3 = 0x20|ret;
		// escreve 3 bytes
		fwrite(&c1,1,1,arq_saida);
		fwrite(&c2,1,1,arq_saida);
		fwrite(&c3,1,1,arq_saida);
	}
	else if (ret <= 0x10FFFF)
	{
		c1 = 0xF0|(ret>>18); //primeiro byte recebe 3 primeiros bits
		c2 = 0x20|(ret>>12);
		c3 = 0x20|(ret>>6);
		c4 = 0x20|ret;
		// escreve 4 bytes
		fwrite(&c1,1,1,arq_saida);
		fwrite(&c2,1,1,arq_saida);
		fwrite(&c3,1,1,arq_saida);
		fwrite(&c4,1,1,arq_saida);
	}
