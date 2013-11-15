%{
/*
Copyright (c) 2013, silverhawk
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "y.tab.h"
#include <stdio.h>

FILE *fp = (FILE *)0;
int i = 0;
int j = 0;
typedef struct label { int offset; char *name; } Label;
Label labels[sizeof(Label)*64*1024];

void yyerror(const char *str)
{
	
}

int yywrap()
{
	return 1;
}

main()
{


	fp = fopen ("./output.o", "w+");

	if (fp == NULL) {
		fprintf(stdout, "Cannot open machine code file : output.o\n");
		exit(99);
	}

	i = 0;
	j = 0;

	yyparse();
}

%}

%token CROSS DOLLAR LEFTPARENS RIGHTPARENS COLON SEMICOLON COMMA NEWLINE NAME IF THEN ELSE DO PLUS MINUS LDA LDX LDY STA STX STY INC INX INY ADC BEQ BNE CPX CPY 
%left '+' '-'
%left '*'

%% 
/*  */
stms: stm stms 		{ printf("STMS"); }	
	| stm				{ printf("STMS3 ");} 
	;
stm:	 ifclause				{ printf("STMSIF "); } 
	| doclause				{ printf("DOSTM "); }
	| exp					{ printf("EXP ");}
	|/* empty */
	;

ifclause:	IF LEFTPARENS stm RIGHTPARENS THEN LEFTPARENS stm RIGHTPARENS	{ printf("if-then clause"); }
	|	IF LEFTPARENS stm RIGHTPARENS THEN LEFTPARENS stm RIGHTPARENS	ELSE LEFTPARENS stm RIGHTPARENS { printf("if-then-else clause"); }
	;


doclause:	DO LEFTPARENS stm RIGHTPARENS stm	{ printf("do clause"); }
	;


exp: 	  exp PLUS exp
	| exp MINUS exp
	| LDA NAME { i+=2; printf("LDA clause"); fprintf(fp, "%x", 0xa0); }
	| LDA CROSS NAME { i+=2; printf("LDA clause"); fprintf(fp, "%x%x", 0xa0,$3); }
	| LDA DOLLAR NAME { i+=2; printf("LDA clause"); fprintf(fp, "%x", 0xa0); }
	| LDA CROSS DOLLAR NAME { i+=2; printf("LDA clause"); fprintf(fp, "%x", 0xa0); }
	| CPX CROSS NAME { i+=2; printf("CPX clause"); if (i%2) fprintf(fp, "%x%x", 0xe0,$3); else fprintf(fp, "%x%x%x",0x00,0xe0,$3); }
	| CPY CROSS NAME { i+=2; printf("CPY clause"); if (i%2) fprintf(fp, "%x%x", 0xc0,$3); else fprintf(fp, "%x%x%x", 0x00,0xc0,$3); }
	| LDX NAME { i+=2; printf("LDX clause"); fprintf(fp, "%x", 0xa2); }
	| LDX CROSS NAME { i+=2; printf("LDX clause"); fprintf(fp, "%x%x", 0xa2,$3); }
	| LDX DOLLAR NAME { i+=2; printf("LDX clause"); fprintf(fp, "%x", 0xa2); }
	| LDX CROSS DOLLAR NAME { i+=2; printf("LDX clause"); fprintf(fp, "%x", 0xa2); }
	| LDY NAME { i+=2; printf("LDY clause"); fprintf(fp, "%x", 0xa0); /* second pair of bytes padded 4x times */ }
	| LDY CROSS NAME { i+=2; printf("LDY clause"); fprintf(fp, "%x%x", 0xa0,$3); }
	| LDY DOLLAR NAME { i+=2; printf("LDY clause"); fprintf(fp, "%x", 0xa0); }
	| LDY CROSS DOLLAR NAME { i+=2; printf("LDY clause"); fprintf(fp, "%x", 0xa0); }
	| STA CROSS NAME { i+=2; printf("STA clause"); fprintf(fp, "%x%x", 0x80,$3); }
	| STX CROSS NAME { i+=2; printf("STX clause"); fprintf(fp, "%x%x", 0x82,$3); }
	| STY CROSS NAME { i+=2; printf("STY clause"); if (i%2) fprintf(fp, "%x%x", 0x80,$3); else fprintf(fp, "%x%x%x",0x00,0x80,$3); }
	| INC DOLLAR NAME { i+=2; printf("INC clause"); fprintf(fp, "%x", 0x00); }
	| ADC DOLLAR NAME { i+=2; printf("ADC clause"); fprintf(fp, "%x%x", 0x60,$3); }
	| INX DOLLAR NAME { i+=2; printf("INX clause"); fprintf(fp, "%x%x", 0xe8,$3); }
	| INY DOLLAR NAME { i+=2; printf("INY clause"); fprintf(fp, "%x%x", 0xc8,$3); }
	| BEQ NAME { /*FIX labeloffset & labelnames */ i+=2; printf("BEQ clause"); fprintf(fp, "%x", 0xf0); }
	| BNE NAME { /*FIX labeloffset & labelnames */ i+=2; printf("BNE clause"); fprintf(fp, "%x", 0xd0); }
	| BEQ NAME COLON { /*FIX labeloffset*/ 
		int k = 0;
		for ( ; strcmp(labels[k++].name, $2) != 0 && k < 64*1024; ) 
			if (k > 64*1024) { fprintf(stdout, "Unknown label\n");
					   exit(99);
			}

		i+=2; 
		printf("BEQ clause"); 
		fprintf(fp, "%x%x", 0xf0,labels[k-1].offset); } 
	| BNE NAME COLON { /*FIX labeloffset*/ 
		int k = 0;
		for ( ; strcmp(labels[k++].name, $2) != 0 && k < 64*1024; ) 
			if (k > 64*1024) { fprintf(stdout, "Unknown label\n");
					   exit(99);
			}


		i+=2; printf("BNE clause"); fprintf(fp, "%x%x", 0xf0,labels[k-1].offset); }
	| NAME COLON { 
		Label *l = (Label*)malloc(sizeof(Label));

		l->offset = i;
		l->name = strdup($1);

		labels[j++] = *l; /* FIX? max 64k */

	} 
	| SEMICOLON stm { ; } 
	;
%%
