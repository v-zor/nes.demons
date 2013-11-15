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

#include <math.h>
#include "y.tab.h"

extern int yylval;

%}

DIGIT	[0-9]
ID	[0-9]+
ID2	[a-z]*[a-z0-9]*
ID3	[A-Z]*[a-z0-9]*

%%
","			{ printf("COMMA"); return COMMA; }
";"			{ printf("SEMICOLON"); return SEMICOLON; }
":"			{ printf("SEMICOLON"); return SEMICOLON; }
"+"			{printf("PLUS"); return PLUS; }
"-"			{printf("MINUS"); return MINUS; }
"("			{printf("LEFTPARENS"); return LEFTPARENS; }
")"			{printf("RIGHTPARENS"); return RIGHTPARENS; }

"#"			{printf("CROSS"); return CROSS; }
"$"			{printf("DOLLAR"); return DOLLAR; }

if			{ printf ("IF"); return IF; }
then			{ printf ("THEN"); return THEN; }
else			{ printf ("ELSE"); return ELSE; }

do			{ printf ("DO"); return DO; }


lda			{ printf("LDA"); return LDA; }
ldx			{ printf("LDX"); return LDX; }
ldy			{ printf("LDY"); return LDY; }

sta			{ printf("STA"); return STA; }
stx			{ printf("STX"); return STX; }
sty			{ printf("STY"); return STY; }

inc			{ printf("INC"); return INC; }
inx			{ printf("INX"); return INX; }
iny			{ printf("INY"); return INY; }

cpx			{ printf("cpx"); return CPX; }
cpy			{ printf("cpx"); return CPY; }
bne			{ printf("BNE"); return BNE; }
beq			{ printf("BEQ"); return BEQ; }

adc			{ printf("ADC"); return ADC; }

DIGIT  			{ printf ("digit=%s\n", yytext); return NAME; }
ID  			{ printf ("id=%s\n", yytext); return NAME; }
ID2  			{ printf ("id2=%s\n", yytext); return NAME; }
ID3  			{ printf ("id3=%s\n", yytext); return NAME; }

[ \t]+		{ printf (" "); }
[\n]+		{ printf ("\nnewline\n"); }

.		{ printf( " Unrecognised char: %s\n", yytext); }
%%
