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
(ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * These functions blit on the GUI's screen (e.g. the SDL screen)
 */
#include "bin.h"

#if HAVE_LIBSDL
void bin_blit_wh(Display *screen, char *binbuf, uint8 xsz, uint8 ysz)
{
	int i,j;
	for ( j = 0; j < ysz; j++) {
		for ( i = 0; i < xsz; i++) {
			blit(screen,i,j,binbuf[j],binbuf[i]);
		}
	} 
}

void bin_blit(Display *screen, char *binbuf)
{
	int i;
	for ( i = 0; i < sizeof(*binbuf); i++)
		blit(screen,i%SCREEN_WITDH,j%SCREEN_HEIGHT,binbuf[i++],binbuf[i]);
}

#else

void bin_blit_wh(Display *screen, char *binbuf, uint8 xsz, uint8 ysz) 
{
	/* idle loop, do nothing */
}

void bin_blit(Display *screen, char *binbuf)
{
	/* idle loop, do nothing */
}

#endif

char* asm2bin(char *asmstr)
{
	/* FIXME regexp the bin_bkg (background) and bin_spr (sprite) 
	 * e.g. with asm2bin("lda \w+ [0-9]+); */
	char *retbuf = (char *)malloc(sizeof(char)*2);

	/* output la as 2 bytes of (substituted) assembler code 
	 * see non_ignore_substitute */
	if (strcmp(asmstr, "label") == 0) {
		sprintf	(retbuf, "%c%c", 'l','a');
		return retbuf;
	}

	int i;
	for ( ; ++i, ++i <= sizeof(*amstr); ) {

		/* skip whitespace */
		int j;
		for ( j = 0; asmstr[i+j] == ' '; j++)
			;

		i += j;

		char b[3];
		sprintf(b, "%c%c%c", asmstr[i],asmstr[i+1],asmstr[2]);

		int ui = atoi(b);
		switch (ui) {
		/* lda */
		case 108*256+100*16+97:{
			sprintf("%x","0xa0");
			break;
		}
		/* ldx */
		case 108*256+100*16+120:{
			sprintf("%x","0xa2");
			break;
		}
		/* ldy */
		case 108*256+100*16+121:{
			sprintf("%x","0xa0");
			break;
		}
		/* sta */
		case 115*256+116*16+97:{
			sprintf("%x","0xa0");
			break;
		}
		/* stx */
		case 115*256+116*16+120:{
			sprintf("%x","0xa0");
			break;
		}
		/* sty */
		case 115*256+116*16+121:{
			sprintf("%x","0xa0");
			break;
		}
		default:
		fprintf(stdout, "asm2bin : assemblerstring not understood.");
		//exit(99);
		}
	}
	return retbuf;
}

void setup_bin_bkg(char *buf)
{
	int i;

	/* 
	 * search for a label which paints a bkg, then override it with
	 * a blit function so the GUI is sped up
         */
	/* search for 'lda ourbkg, #256' */
	i = 0;
	if (i = non_ignore_recognize(buf, asm2bin("lda"))) {
		/* skip 2 bytes of label (64k offset label jump) */
		if (i = non_ignore_recognize_sugar(buf, asm2bin("label"))) {
			i += 2;
			buf += i;
			if (buf[i]*16+buf[i+1] < 32*32*8*8) {
				/* valid label (in between 0 and 64k) */
			}
			return;
		}
	}
	/* search for 'lda outbkg, X' */
	i = 0;
	if (i = non_ignore_recognize(buf, asm2bin("lda") 
		&& 
		buf+=4)) {
		if (non_ignore_recognize(buf+i, asm2bin("inx")) 
			||
			non_ignore_recognize(buf+i, asm2bin("iny"))) {

		} 
		return;
	}
}	
