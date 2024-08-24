// ==============================================================
//#+ This module generated by Mumps Compiler @VERSION@ Apr  9 2023
//#+  
//#+ Portions of this code are part of the Mumps Compiler
//#+ 
//#+ Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 
//#+ 2010, 2017, 2020, 2022, 2023 by Kevin C. O'Kane  
//#+ 
//#+ Kevin C. O'Kane, Ph.D.
//#+ Professor Emeritus
//#+ Computer Science Department
//#+ University of Northern Iowa
//#+ Cedar Falls, IA 50614-0507
//#+ kc.okane@gmail.com
//#+ https://threadsafebooks.com
//#+ https://www.cs.uni.edu/~okane
//#+ -----------------------------------
//#+ 
//#+ Consult individual modules for copyright details
//#+ The runtime libraries are covered by the following license:
//#+  
//#+ This library is free software; you can redistribute it and/or
//#+ modify it under the terms of the GNU Lesser General Public
//#+ License as published by the Free Software Foundation; either
//#+ version 2.1 of the License, or (at your option) any later version.
//#+ 
//#+ This library is distributed in the hope that it will be useful,
//#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//#+ Lesser General Public License for more details.
//#+ 
//#+ You should have received a copy of the GNU Lesser General Public
//#+ License along with this library; if not, write to the Free Software
//#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+ 
//#+ This code is unverified - use at your own risk 
//#+==============================================================

#include <stdio.h>
#include <stdlib.h>

void	make_file(const char *);

	int MAX = 0;

int main(int argc, char *argv[]) {

	char tmp[128];
	int cpus = 0, i = 0;

	if (argc != 3) { // fcn name, MAX and cpus
		printf("** Arument count (%d) error to chop\n", argc);
		abort();
		}

	MAX = atoi(argv[1]);
	cpus = atoi(argv[2]);

	printf("MAX sequence = %d\n", MAX); 
	printf("CPUs = %d\n", cpus); 

	for (i = 0; i < cpus; i++) {
		sprintf(tmp, "db-%d", i);
		make_file(tmp);
		}

	}

void	make_file(const char *fn) {

	int flg = 0, in, count = 1;

	FILE *out;

	out = fopen(fn, "w");

	while( (in = getchar()) != EOF) {
		if (flg && in == '>') { 
			putc('\n', out);
			count++;
			if (count > MAX) break;
			}
		putc(in, out); flg = 1;
		}
	if (in == '>') ungetc(in, stdin);
	fclose(out);

	}
