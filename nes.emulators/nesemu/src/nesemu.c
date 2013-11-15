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
 * 
 * 
 * This is a runtime system for NES emulation,
 * there are threads for running the NES machine code, for reading keys.
 *
 */

#include<stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "ignores.h"
#include "nesemu.h"

FILE *fp;

void *read_rt_keys(void *a);
void *start_machine_pthread(void *a);
int start_keys_pthread(pthread_t *pids);
int start_threads(pthread_t **pids);

int main(int argc, char *argv[])
{

	if (argc < 2 || strlen((char *)argv[1]) < 1) {
		print_usage(); /* andor open GUI */
	}

	/* strip machine code (rom file) */
	char *retbuf, *buf;
	ignore_init(retbuf,buf,argv[1]);
	ignore_strip(retbuf,buf);

	fp = fopen((char *)argv[1], "r");

	pthread_t *pits = (pthread_t*)0;

	if (start_threads(&pits) < 0) fprintf(stdout, "Threads did not start.\n");

	nesemu_mainloop(&argc, &argv);

	exit(99);
}

int start_threads(pthread_t **pids)
{
	pids = (pthread_t **)calloc(2, sizeof(pthread_t *));
	if (start_machine_pthread((*pids)[0]) < 0) return -1; /* FIX start_machine_fork */
	if (start_keys_pthread((*pids)[1]) < 0) return -1; /* FIX start_keys_fork */

	return 0;
}

void nesemu_mainloop(int *argc, char **argv[])
{
	/* FIX */
}

int start_keys_pthread(pthread_t *pids)
{
	pthread_t pid;
	if (pthread_create(&pid, NULL, read_rt_keys, NULL) && pid != 0) {
		 pids = pid;
		return 0;
	}
	return -1;
}

void *start_machine_pthread(void *a)
{

	if (fp == NULL) print_usage(); /* exit */

	char *c = (char *)malloc(sizeof(char)*4); 
	memset(c, 0, 4);

	for (;;) {
		int i;
		for ( i = 0; i < 4; i++) { 
			*c++ = fgetc(fp); if (*c++ == EOF) goto halt; 
		}; 
		c-=3;
		push(c++,c++,c++,c,NULL);
		c-=3;
		memset(c, 0, 4);
	}
halt:
	fprintf(stdout, "System halted because ROM ended");
	return NULL;
}

/* execute opcode *opc on emulator machine with : */

void push(uint8 *opc, uint8 *opnd11, uint8 *opnd12, uint8 *opnd21, uint8 *opnd22)
{
	switch(*opc) {
	case 0xa0://LDA
	case 0xa2://LDX
	//case 0xa0://LDY + pad
	case 0x80://STA
	case 0x82://STX
	//case 0x80://STY + pad
	case 0x60://ADC
	case 0xf0://BEQ
	case 0xd0://BNE
	case 0xe0://CPX + pad
	case 0xc0://CPY + pad
	case 0x00://DEC
	//case 0x00://INC + pad
	case 0xe8://INX
	case 0xc8://INY
	case 0x4c://JMP
	default:
		break;
	}
}

/*
 * Read the keys of the realtime emulator
 */
void *read_rt_keys(void *a)
{
	/* FIX */
}

void print_usage()
{
	fprintf(stdout, "Usage : nesemu <ROMfile>");
	exit(99);
}

