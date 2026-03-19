/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003 - 2025 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     https://threadsafebooks.com/
#+     https://www.cs.uni.edu/~okane
#+
#+ This program is free software; you can redistribute it and/or modify
#+ it under the terms of the GNU General Public License as published by
#+ the Free Software Foundation; either version 2 of the License, or
#+ (at your option) any later version.
#+
#+ This program is distributed in the hope that it will be useful,
#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+ GNU General Public License for more details.
#+
#+ You should have received a copy of the GNU General Public License
#+ along with this program; if not, write to the Free Software
#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//	Dec 13, 2025

#define SQLITE

// enable debug code
// #define SYMDEBUG

/* sym.c - Mumps Runtime Library
 *
 * Mumps symbol table management.  Variables not handled by global.h
 * and friends should be handled by these routines.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <monetary.h>
#include <locale.h>
#include <math.h>
// #include <wait.h>

#define _INTERP_
#define CSTR (char *)
#define USTR (unsigned char *)

#include <mumpsc/defines.h>
#include <mumpsc/sym.h>
#include <mumpsc/btree.h>
#include <mumpsc/globalOrder.h>
#include <mumpsc/keyfix.h>
#include <mumpsc/inline.h>

#include <unistd.h>

#include <time.h>

#include <sys/types.h>

using namespace std;

#include <mumpsc/fcns.h>

#include <mumpsc/builtin.h>
#include <mumpsc/interp.h>

// floatSize is defines BIGFLOAT if selected in configure


// intLong will be defined is 32 bit wanted - 64 otherwise
#define intLong

int Interpret(const char *, MSV *);
int ScanParse(char *);
int Eval(unsigned char *, unsigned char *, struct MSV *);  // evaluate expression

extern int (*__label_lookup)(char *);
extern char* (*__text_function)(int);  /* fcn */

void fcn(MSV * svPtr); 

//-----------------------------------------------------------------------
//	$fnumber() wrapper for compiler
//-----------------------------------------------------------------------

void _fnumber(unsigned char * tmp0, unsigned char * number, 
	unsigned char * arg1, unsigned char * arg2, MSV * svPtr) {

      char open[]  = {(char)CodedOpen,0};
      char comma[] = {(char)CodedComma,0};
      char close[] = {(char)CodedClose,0};

      strcpy((char *) &svPtr->v1d[1], "$fn");
      strcat((char *) &svPtr->v1d[1], open);

      strcat((char *) &svPtr->v1d[1], (char *) number);
      strcat((char *) &svPtr->v1d[1], comma);

      strcat((char *) &svPtr->v1d[1], (char *) arg1);
      strcat((char *) &svPtr->v1d[1], comma);

      strcat((char *) &svPtr->v1d[1], (char *) arg2);
      strcat((char *) &svPtr->v1d[1], close);

      fcn (svPtr);

      strcpy((char *) tmp0, (char *) &svPtr->bd[1]);

      return;
      }


