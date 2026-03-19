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

/*===========================================================================*
 *                                  _select                                  *
 *===========================================================================*/
void _select(unsigned char * out,
             unsigned char *in1,
             unsigned char *in2,
             unsigned char *in3,
             unsigned char *in4,
             unsigned char *in5,
             unsigned char *in6,
             unsigned char *in7,
             unsigned char *in8,
             unsigned char *in9,
             unsigned char *in10,
             unsigned char *in11,
             unsigned char *in12,
             unsigned char *in13,
             unsigned char *in14,
             unsigned char *in15,
             unsigned char *in16,
             unsigned char *in17,
             unsigned char *in18,
             unsigned char *in19,
             unsigned char *in20,
             struct MSV * svPtr) {

      if (atol((char *) in1) != 0) {
            strmove(out, in2);
            return;
            }

      if (in3 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in3) != 0) {
            strmove(out, in4);
            return;
            }

      if (in5 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in5) != 0) {
            strmove(out, in6);
            return;
            }

      if (in7 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in7) != 0) {
            strmove(out, in8);
            return;
            }

      if (in9 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in9) != 0) {
            strmove(out, in10);
            return;
            }

      if (in11 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in11) != 0) {
            strmove(out, in12);
            return;
            }

      if (in13 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in13) != 0) {
            strmove(out, in14);
            return;
            }

      if (in15 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in15) != 0) {
            strmove(out, in16);
            return;
            }

      if (in17 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in17) != 0) {
            strmove(out, in18);
            return;
            }

      if (in19 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in19) != 0) {
            strmove(out, in20);
            return;
            }

      printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
      sigint(100);
      }
