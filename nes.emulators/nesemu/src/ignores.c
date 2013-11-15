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
#include <stdio.h>
#include "ignores.h"

/* allocates a new buffer which has the size of filename's length.
 * buf contains the file */
void ignore_init(char *retbuf, char *buf, char *fn)
{
	FILE *fp = fopen(fn, "r");
	int pos = -1;
	int i;

	if (fp == NULL) { fprintf(stdout,"Could not open ignore file.\n"); exit(99); }

	fseek(fp, 0L, SEEK_END);
	pos = ftell(fp);
	rewind(fp);

	retbuf = (char*)malloc(pos * sizeof(char));
	buf = (char*)malloc(pos * sizeof(char));

	for ( i = 0; (char c = fgetc(fp)) != EOF ; i++)
		buf[i] = c;		

}

int ignore_snoop(char *buf, char *seq)
{
	int len = sizeof(*buf);
	int i = 0;
	for ( ; ignore_recognize(buf++) != 0; i++)
		if (i >= len)
			return -1;

	return i - sizeof(*seq);
}

/* Change machine code by substituting a seq for a subs */
int ignore_substitute(char *buf, char *subs, char *seq)
{
	int len = sizeof(*buf);
	int i,j;
	for ( i = 0; ignore_recognize(buf++) != 0; i++,j++) {
		if (i >= len)
			return -1;
		//j++;
	}
	if (sizeof(*subs) > sizeof(*seq))
		buf = (char *)realloc(buf, sizeof(*buf) + (sizeof(*subs)-sizeof(*seq)));

	memcpy(buf+sizeof(*subs),buf,len-j);
	buf-=j;
	memcpy(buf, *subs, sizeof(*subs);		
	
	return i - sizeof(*seq);
}

void ignore_strip(char *retbuf, char *buf)
{ 	
	ignore_sequence(retbuf, buf, "/*FIXME*/");
}

/* private functions */

static void ignore_sequence(char *retbuf, char *buf, char *seq)
{
	int i,j;

	for ( j = 0; j < sizeof(*buf); ) {

		for ( i = j; i < sizeof(*seq); i++) {

			if (buf[i] == seq[i-j])
				continue;	

		}	
		if (i == sizeof(*seq)) {
			j += i;	
		} else {
			int k,l;
			for ( k = 0, l = 0; k < i-j; k++) {
				retbuf[k] = buf[j+l++];
			} 
			j++;	
		}
	}
}

/* buf must be on a location (e.g. buf+10) where the search for seq begins */
int ignore_recognize(char *buf, char *seq)
{
	int i;
	for ( ; buf[i] == seq[i]; )
		i++;

	if (i == sizeof(*seq))
		return 0;
	
	return -i;
}

