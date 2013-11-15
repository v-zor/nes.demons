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
/*
 BMP to NES CHR File Converter (translated from bmp2nes.bas)
*/

#include<stdlib.h>
#include<stdio.h>

void print_usage();
int bitmask(int c)
{
	switch (c) {
	case 0:
	return 80;
	break;
	case 1:
	return 40;
	break;
	case 2:
	return 20;
	break;
	case 3:
	return 10;
	break;
	case 4:
	return 8;
	break;
	case 5:
	return 4;
	break;
	case 6:
	return 2;
	break;
	case 7:
	return 1;
	break;
	default:
	fprintf(stdout, "Wrong bitmask value.");
	exit(99);
	break;
	}
}

int main(int argc, char *argv[])
{

	int i;
	char c;

	/* volatile */ int bmpfile[8*8*1000+1];

	if (argc < 3)
		print_usage();

	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL) {
		fprintf(stdout, "Cannot find bmp file.\n\n");
		exit(99);
	}
	FILE *outfp = fopen(argv[2], "w+");

	if (outfp == NULL) {
		fprintf(stdout, "Cannot open output file.\n\n");
		exit(99);
	}


	for ( ; (c = getc(fp)) != EOF ; ) {
		if (c == ' ') continue; else bmpfile[i++] = c;
		if (i > 320*200) {
			fprintf(stdout, "x and y size of bmp too big.");
			exit(99);
		}
	}
	
	bmpfile[8*8*1024] = '\0';

	int xsize = *(bmpfile+19);
	int ysize = *(bmpfile+23);

	if (bmpfile[29] != 8) {
		fprintf(stdout, "not a 256 color bmp.");
		exit(99);
	}

	int *seek = bmpfile+55;
	if (xsize > 320 || ysize > 200) {
		fprintf(stdout, "x and y size of bmp too big.");
		exit(99);
	}

	int index = 0;

	for ( ; index < 3; index++) {
		int b,g,r,a;

		b = *seek++ / 4;
		g = *seek++ / 4;
		r = *seek++ / 4;
		a = *seek++;

		fprintf(outfp, "%x%d", 0x3c8, index);
		fprintf(outfp, "%x%d", 0x3c9, r);
		fprintf(outfp, "%x%d", 0x3c9, g);
		fprintf(outfp, "%x%d", 0x3c9, b);

	}

	seek = bmpfile+1079;	

	int buf[xsize*ysize];

	int yy = ysize -1;
	for ( ; yy >= 0; yy--) {
		int xx = 1;
		for ( ; xx <= xsize; xx++) {
			buf[xx-1+yy*xsize] = bmpfile[xx];/* FIXME bmpfile[xx,1] */
		
		}
	}

	int x,y,b1,b2;

	int xtiles = xsize / 8;
	int ytiles = ysize / 8;
	int Tile;
	for ( ; Tile < xtiles * ytiles; Tile++) {
		int ly;
		for ( ; ly < 8; ly++) {
			int lx;
			for ( ; lx < 8; lx++) {

				int xx = x + lx;
				yy = y + ly;

				int px = xx + yy*xsize; /* FIXME BASIC px = point(xx,yy) */
				if (px > 3)
					px = 0;

				int px2 = ((int)(px & 2)) / 2;
				int px1 = px & 1;
				int b1 = b1 | (px1 * bitmask(lx));
				int b2 = b2 | (px2 * bitmask(lx));
			}

			int a = ascii(b1); /* CHR */
			fprintf(outfp, "%c", a);
			fprintf(outfp, "%c", a);	
			/* outfp += 1 */
			b1 = 0;
			b2 = 0;
		}

		x = x +8;
		if (x > (xtiles * 8) - 1) {
			x = 0;
			y = y + 8;
		}
		/* outfp += 8 */

	}

	fclose(outfp);

	fprintf(stdout, "DONE -- Press any key . . .");
	fgetc(stdin);

	exit(99);
}

void print_usage()
{
	fprintf(stdout, "Usage : bton <inputfile> <outputfile>\n\n");
	exit(99);	
}
