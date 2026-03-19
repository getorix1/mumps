//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+ Mumps Compiler Run-Time Support Functions
//+ Copyright (c) 2001 - 2026 by Kevin C. O'Kane
//+
//+ kc.okane@gmail.com
//+ okane@uni.edu
//+ http://threadsafebooks.com/
//+ http://www.cs.uni.edu/~okane/
//+
//+ This library is free software; you can redistribute it and/or
//+ modify it under the terms of the GNU Lesser General Public
//+ License as published by the Free Software Foundation; either
//+ version 2.1 of the License, or (at your option) any later version.
//+
//+ This library is distributed in the hope that it will be useful,
//+ but WITHOUT ANY WARRANTY; without even the implied warranty of
//+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//+ Lesser General Public License for more details.
//+
//+ You should have received a copy of the GNU Lesser General Public
//+ License along with this library; if not, write to the Free Software
//+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//+
// #++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//	astyle --style=banner --indent=tab interp-sql-btree.cpp.in

//	Feb 13, 2026

/*************************************************************************
   interp.cpp - Mumps Runtime Library Mumps interpretation routines - can
   be used for indirection operations in the compiled code.
**************************************************************************/

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>


#include <mumpsc/defines.h>
#include <mumpsc/btree.h>
#include <sys/types.h>
#include <locale.h>
#include <monetary.h>
#include <semaphore.h>

#include <unistd.h>

#include <time.h>
#include <mumpsc/arith.h>
#include <mumpsc/globalOrder.h>

#define SQLITE



#define _INTERP_

#include <mumpsc/global.h>
#include <mumpsc/fcns.h>
#include <mumpsc/sysparms.h>
#include <mumpsc/keyfix.h>

#define GOSUBMAX 50
#define SWAP 30
#define FORMAX 30
#define EXECLEVS 40
#define LBLS 80
#define CSTR (char *)
#define USTR (unsigned char *)

#define OPEN_PAREN 206
#define COMMA 207
#define CLOSE_PAREN 209

void   lpad(unsigned char *, unsigned char *, short);
void   substr(unsigned char *, unsigned char *, int, int);
void   sigint(int);
void   LocalName(unsigned char *, unsigned char *, unsigned char *, MSV *);
void   GlobalQuery(unsigned char *, unsigned char *, char, MSV *);
void   Qsub(unsigned char *, unsigned char *, unsigned char *, MSV*);
void   Qlength(unsigned char *, unsigned char *, MSV*);
void   ErrorMessage(const char *, long);
void   zfcn(unsigned char *, unsigned char *, MSV *);
void   fcn(struct MSV*);

int    xindex(const unsigned char *, const unsigned char *, short);
int    TimeOut(int, unsigned char *, char *, int);
int    DirectRead (int, unsigned char *);
int    TimeOut_getstr1(int, unsigned char *, char *, int);
int    _SymFree(int, MSV *);
int    sql(int, struct MSV *, char *DataSourceName, char *Command, char *, const char *);

char*  makeLock(char *, char *, char *, char, char *); /* global array lock service */
char*  cleanLocks(char *); /* global array lock service */
static void patrn1(short *jpx, short *ernbr, struct MSV *);
void patrn(unsigned char *ad, struct MSV *svPtr);

 extern short ierr1;

void match(short pts[3][20], short lx, short ct[], unsigned char *ad, 
	short *rslt, unsigned char *bd) {

        unsigned char tmp[2],tmp1[1024],tmp2[1024];
        short ptr,i,j;
        ptr=(-1);
        *rslt=0;
        tmp[1]=0;
        for (i=1; i<=lx; i++)
        {       if (ct[i]==0) continue;
                switch (bd[pts[1][i]])
        {

case 'C':
case 'c':       for(j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        if (ad[ptr]<32) continue;
                        if (ad[ptr]==127) continue;
                        return;
                                }
                break;

case 'N':
case 'n':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        if (ad[ptr]>='0'&&ad[ptr]<='9') continue;
                        return;
                        } /* for j=...*/
                break;

case 'P':
case 'p':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        tmp[0]=ad[ptr];
                        if
                        (xindex( (const unsigned char *) " !\"#$%&''()*+,-./:;<=>?@[\\]^_`{|}~",tmp,1)>0)
                                 continue;
                        return;
                                } /* for j=...*/
                break;

case 'A':
case 'a':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        if (ad[ptr]>=65&&ad[ptr]<=90) continue;
                        if (ad[ptr]>=97&&ad[ptr]<=122) continue;
                        return;
                        } /* for j...*/
                break;

case 'L':
case 'l':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        if (ad[ptr]>=97&&ad[ptr]<=122) continue;
                        return;
                        } /*for j=...*/
                break;

case 'U':
case 'u':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        if (ad[ptr]>=65&&ad[ptr]<=90) continue;
                        return;
                        } /* for j=...*/
                break;

case 'E':
case 'e':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen( (char *) ad)) return;
                        if (ad[ptr]>127) return;
                        }
                break;

case '"':       for (j=1; j<=ct[i]; j++)
                {       
                        if (pts[2][i]==0) continue;
                        if (ptr+pts[2][i]-1>strlen( (char *) ad)) return;
                        ptr++;
                        substr(&ad[ptr],tmp1,1,pts[2][i]-1);
                        substr(&bd[1],tmp2,pts[1][i]+1,pts[2][i]-1);
                        if (strcmp( (char *) tmp2, (char *) tmp1)!=0) return;
                        ptr=ptr+strlen( (char *) tmp1)-1;
                        }
                break;

default:        return;         }
                                } /* for i=... */
        if (ptr+1!=strlen( (char *) ad)) return;
        *rslt=1;
        return;
                        } /* match() */


/*===========================================================================*
 *                                  strmove                                  *
 *===========================================================================*/

#include <mumpsc/inline.h>

void cannon(char a[])

{       int i,j,k;
        char tmp[25];

        i=0; j=0;
        while(a[i]=='+'||a[i]=='-') {
                if (a[i]=='-') if (j==0) j=1; else j=0; i++; }
        if (i>0) {
                if (j==0) strcpy(a,&a[i]);
                        else { a[0]='-';
                                if (i>1) strcpy(&a[1],&a[i]); } }
        j= -1;
        while(a[i]=='.'||(a[i]>='0'&&a[i]<='9')) {
                if (a[i]=='.') j=i; i++; }
        if (i==0) { a[0]='0'; a[1]=0; return; }
        if (a[i]!='E'&&a[i]!='e') a[i]=0;
        if (a[i]==0) {
                if (j>=0) {
                                for (i--; a[i]=='0'; i--) a[i]=0;
                                if (a[i]=='.') a[i]=0;
                                if (i==0) { a[0]='0'; a[1]=0; } }
                if (a[0]=='-') j=1; else j=0;
                while (a[j]=='0') strcpy(&a[j],&a[j+1]);
                if (a[j]==0) { a[0]='0'; a[1]=0; }
                if (a[j]=='.'&&a[j+1]==0) a[j]='0';
                return; }
        if (a[i]!='e'&&a[i]!='E') return;
        a[i]=0; i++;
        if (a[i]=='-') { i++; j=1; }
                else { j=0; if (a[i]=='+') i++; }
        for (k=0; (tmp[k]=a[i])!=0&&(
                tmp[k]>='0'&&tmp[k]<='9'); (k++,i++));
        tmp[k]=0; i=atoi(tmp);
        if (i>0) if (j==0) for (j=1; j<=i; j++) mult(a,"10",a);
                else for (j=1; j<=i; j++) mult(a,".1",a);
        return; }


/*===========================================================================*
 *                                   parse                                   *
 *                                   PARSE                                   *
 *===========================================================================*/


int	parse_ (struct MSV * svPtr) {

	/* * #+ 206 = open; 207 = close; 208 = comma; 209 = colon */

	short ernbr, f, spx, adx, jpx, j, i, g;
	const static unsigned char cod209[2] = { 209, 0 };
	unsigned char s1p[40];
	unsigned char bbtyp, tmp2[4], tmp1[25], tmp3[STR_MAX], nxtchr;
	unsigned char *bp = &svPtr->bd[1];
	unsigned char *v1dp, *to, *from;
	int FcnArgToggle=0;

	const static unsigned char code[26] = {

		/*  0 */ 99, 99, 99, 99, 99, 99, 99, 99, 18, 19,
		/* 10 */ 99, 20, 18, 99, 99, 21, 22, 23, 99, 99,
		/* 20 */ 99, 99, 99, 99, 26, 27
		};

	const static unsigned char opcode[256] = {

		/*   0 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  30 */ 99, 99, 99, 25, 99, 14, 99, 99, 24, 12,
		/*  40 */  0, 99,  2,  4, 99,  3, 99,  1, 99, 99,
		/*  50 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  60 */ 11,  8,  9, 17, 28, 99, 99, 99, 99, 99,
		/*  70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  90 */ 99, 15, 13, 16, 99,  7, 99, 99, 99, 99,
		/* 100 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 110 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 120 */ 99, 99, 99, 99, 99, 99, 99, 99
		};

	const static unsigned char ncode[256] = {

		/*   0 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  30 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  40 */ 99, 99, 99, 99, 99, 99, 10, 99, 10, 10,
		/*  50 */ 10, 10, 10, 10, 10, 10, 10, 10, 99, 99,
		/*  60 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  90 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 100 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 110 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 120 */ 99, 99, 99, 99, 99, 99, 99, 99
		};

	const static unsigned char dcode[256] = {

		/*   0 */ 10, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  30 */ 99, 99, 10, 10, 99, 10, 99, 99, 10, 10,
		/*  40 */ 99, 10, 10, 10, 10, 10, 99, 10, 99, 99,
		/*  50 */ 99, 99, 99, 99, 99, 99, 99, 99, 10, 99,
		/*  60 */ 10, 10, 10, 10, 10, 99, 99, 99, 99, 99,
		/*  70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/*  90 */ 99, 10, 10, 10, 10, 10, 99, 99, 99, 99,
		/* 100 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 110 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
		/* 120 */ 99, 99, 99, 99, 99, 99, 99, 99
		};

	const static unsigned char operand[256] = {

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 1, 1, 2, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		2, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 2, 1, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 0, 0, 0, 0, 0
		};

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + initialization + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	svPtr->bd[0]=0;
	v1dp = &svPtr->v1d[1];
	svPtr->xpx--;
	spx = 0;
	svPtr->pd1[svPtr->pd1len + 2] = 0;
	svPtr->pd1[svPtr->pd1len + 3] = 0;
	svPtr->sdlim = svPtr->pd1len + 3;

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + main recursive internal entry point + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

start:
	spx++;
	s1p[spx] = PEMPTY;                            /* stack foundation */

nchar:

	if ((nxtchr = svPtr->xd[++svPtr->xpx]) == 0) goto finish;

	if (operand[nxtchr] == 1) goto scan_operand;

	if (nxtchr == CodedOpen) {
		unsigned char *p1, *p2;
		spx++;

		while (svPtr->pd1[svPtr->sdlim++]);

		p1 = &svPtr->v1d[1];
		p2 = &svPtr->pd1[svPtr->sdlim];

		while ((*p2++ = *p1++));

		*p2 = 0;
		p2--;
		*p2 = CodedOpen;
		s1p[spx] = OPENC;
		goto nchar;
		}

	if (nxtchr == ',') {
		if (s1p[spx - 1] != OPENC) {
			if (svPtr->t2 > 0) goto finish;
			return (16);
			}

		/* concatenate stack tops and add coded comma */

		/* join tops */
		for (i = --svPtr->sdlim; (svPtr->pd1[i] = svPtr->pd1[i + 1]); i++);

		svPtr->pd1[i] = CodedComma;
		svPtr->pd1[i + 1] = 0;                    /* coded comma */
		svPtr->sdlim--;

		while (svPtr->pd1[svPtr->sdlim--]);       /* find new stack top start */

		svPtr->sdlim += 2;
		spx--;                                    /* stack pointer */
		goto nchar;
		}

	if (nxtchr == ')') {

		if (s1p[spx - 1] != POPEN) {
			if (s1p[spx - 1] != OPENC) return (16);

			/*
			 * fcn/array return section
			 */

			svPtr->sdlim--;

			for (i = svPtr->sdlim; (svPtr->pd1[i] = svPtr->pd1[i + 1]); i++);

			/* join
			 * tops */
			svPtr->pd1[i] = CodedClose;
			svPtr->pd1[i + 1] = 0;                /* coded close */
			svPtr->sdlim--;

			while (svPtr->pd1[svPtr->sdlim--]);   /* find new stack top start */

			svPtr->sdlim += 2;
			spx--;                                /* stack pointer */
			goto un_nest;
			}

//--------------------------------
//       precedence close paren
//--------------------------------

		if (s1p[spx] != OPERAND) return (12);

		/* extract value from stack top */

		strmove (bp, &svPtr->pd1[svPtr->sdlim]);

		do
			svPtr->sdlim--;

		while (svPtr->pd1[svPtr->sdlim - 1]);     /* compress stack */

		goto dec_stk;
		}

	if (nxtchr == ':') {
		i = svPtr->sdlim;

		do
			i--;

		while (svPtr->pd1[i - 1]);

		if (svPtr->pd1[i] != '$' || (svPtr->pd1[i + 1] != 's' && svPtr->pd1[i + 1] != 'S'))
			goto finish;

		svPtr->sdlim--;
		strmove (&svPtr->pd1[svPtr->sdlim], &svPtr->pd1[svPtr->sdlim + 1]);

		while (svPtr->pd1[svPtr->sdlim--]);

		svPtr->sdlim += 2;
		strcat ((char *) &svPtr->pd1[svPtr->sdlim], (char *) cod209);
		spx--;
		goto nchar;
		}

	if (nxtchr == ' ' || nxtchr == TAB) goto finish;

	if (nxtchr == '^' && svPtr->t2 == 2 && s1p[spx] == OPERAND) goto finish;

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + check for delimiters in list + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	spx++;

	if (nxtchr == QUOTE) {
		/* insert not based operator code */
		svPtr->xpx++;
		s1p[spx] = opcode[svPtr->xd[svPtr->xpx]];

		if (s1p[spx] != 99) {
			s1p[spx] = code[opcode[svPtr->xd[svPtr->xpx]]];

			// patch for ']]
			if (svPtr->xd[svPtr->xpx+1] == ']') ++svPtr->xpx;

			if (s1p[spx] != 99) goto nchar;
			}

		svPtr->xpx--;
		}

	if (nxtchr == '*' && svPtr->xd[svPtr->xpx + 1] == '*') {
		// exponentiation
		svPtr->xpx++;
		s1p[spx] = EXPONEN;
		goto nchar;
		}

	if (nxtchr == ']' && svPtr->xd[svPtr->xpx + 1] == ']') {
		// collates after - becomes follows
		svPtr->xpx++;
		s1p[spx] = FOLLOWS;
		goto nchar;
		}

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + insert code + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	s1p[spx] = opcode[nxtchr];

	if (s1p[spx] != 99) goto nchar;

	svPtr->ErrChar=nxtchr;
	return (12);

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + scan for operand + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

scan_operand:

	jpx = svPtr->xpx;

	if (s1p[spx] == NOTPATTERN || s1p[spx] == PATTERN) {
		svPtr->bd[1] = 0;
		patrn1 (&jpx, &ernbr, svPtr);

		if (ernbr) return (ernbr);

		goto exec;
		}

	/***************************
	    numeric operand
	***************************/

	if (ncode[svPtr->xd[svPtr->xpx]] == 10) {

		if (svPtr->t0px>0 && svPtr->xd[svPtr->xpx] == '.' &&
		        ( isalpha(svPtr->xd[svPtr->xpx+1]) ||
		          svPtr->xd[svPtr->xpx+1] == '_' ||
		          svPtr->xd[svPtr->xpx+1] == '%') ) {
			svPtr->bd[1]='.';
			svPtr->xpx++;

			for (i = 2; isalnum( (svPtr->bd[i] = svPtr->xd[svPtr->xpx]) ) ; (i++, svPtr->xpx++));

			svPtr->bd[i] = 0;
			svPtr->xpx--;
			goto exec;
			}

		for ( ; svPtr->xd[svPtr->xpx] == '0'; svPtr->xpx++); // remove leading zeros

		if (ncode[svPtr->xd[svPtr->xpx]] != 10 ) svPtr->xpx--; // all zeros

		for (i = 1; (svPtr->bd[i] = svPtr->xd[svPtr->xpx]) && ncode[svPtr->bd[i]] == 10; (i++, svPtr->xpx++));

		if (i == 1 && svPtr->bd[1] == '.') return (9);

		svPtr->bd[i] = 0;
		svPtr->xpx--;
		goto exec;
		}

	/***************************
	    literal operand
	***************************/

	if (svPtr->xd[svPtr->xpx] == '"') {
		j = 0;

		while (1) {
			while (svPtr->xd[++svPtr->xpx] != '"') {
				if ((svPtr->bd[++j] = svPtr->xd[svPtr->xpx]) == 0) return (2);
				}

			if (svPtr->xd[svPtr->xpx + 1] != '"') {
				svPtr->bd[++j] = 0;
				goto exec;
				}

			svPtr->xpx++;
			svPtr->bd[++j] = '"';
			}
		}

	/***************************
	    variable name
	***************************/

	svPtr->v1d[1] = svPtr->xd[svPtr->xpx++];
	j = 2;

	while (1) {

//------------------------------------------------------
//	indirect name and indirect indices case @x@(1,1)
//------------------------------------------------------

		if (svPtr->xd[svPtr->xpx]=='@' && svPtr->xd[svPtr->xpx+1]=='(' && s1p[spx]!=INDIRECT) {
			strcpy((char *)&svPtr->xd[svPtr->xpx],(char *) &svPtr->xd[svPtr->xpx+1]);
			goto idxin;
			}

		if (dcode[svPtr->xd[svPtr->xpx]] == 10) {
			svPtr->v1d[j] = 0;
			svPtr->xpx--;
			goto var1;
			}

idxin:
		svPtr->v1d[j++] = svPtr->xd[svPtr->xpx];

		if (svPtr->xd[svPtr->xpx] == '(') {
			svPtr->v1d[--j] = 0;
			svPtr->xd[svPtr->xpx--] = CodedOpen;
			goto start;                           /* recurse */
			}

		svPtr->xpx++;
		}

un_nest:                                      /* copy answer from stack */

	while (svPtr->pd1[--svPtr->sdlim]);

	strmove (v1dp, &svPtr->pd1[svPtr->sdlim + 1]);

	while (svPtr->pd1[--svPtr->sdlim]);

	svPtr->sdlim++;
	spx -= 2;

var1:

	if (svPtr->t0px == 1 && spx == 1) {
		spx--;
		svPtr->t0px = 0;
		return (0);
		}

// $piece interface

	if (svPtr->pd1[svPtr->sdlim] == '$' && svPtr->t0px == 1 &&
	        (svPtr->pd1[svPtr->sdlim + 1] == 'P' || svPtr->pd1[svPtr->sdlim + 1] == 'p'))

		if (svPtr->setname[0] == 0) strmove (svPtr->setname, v1dp);

// $extract interface

	if (svPtr->pd1[svPtr->sdlim] == '$' && svPtr->t0px == 1 &&
	        (svPtr->pd1[svPtr->sdlim + 1] == 'E' || svPtr->pd1[svPtr->sdlim + 1] == 'e'))
		if (svPtr->setname[0] == 0) {
			strmove(svPtr->setname, v1dp);
			}

	if (svPtr->v1d[1] == '^') {                   /* global var */

		g = RETRIEVE;

// NO EVAL GLOBAL ZZ INTERFACE

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'Z' &&
		        toupper (svPtr->pd1[svPtr->sdlim + 2]) == 'Z') {
			for(int i=0; v1dp[i]!=0; i++) if (v1dp[i]>127) v1dp[i]=1;
			strcpy((char*)&svPtr->bd[1],(char *)v1dp);
			goto exec;
			}

		if (svPtr->pd1[svPtr->sdlim] == '$' && s1p[spx] != INDIRECT) {

			FcnArgToggle ++;
			FcnArgToggle %= 2;

//----------------------------------------------------------------------------------------
// some global references must be passed as references to subroutines without evaluation.
// this code hides the global ref in a static array and places the <gbl> token as the
// result of the 'evaluation'
// use with $name, $query $qlength $qsubscript
// $name function requires $na as minimum abbreviation while $next only requires $n
//----------------------------------------------------------------------------------------

			if ( (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'N' &&
			        toupper (svPtr->pd1[svPtr->sdlim + 1]) != 'A' ) ||
			        toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'Q') {
				g = NEXT;
				/* hide global in a static array in BuildGlobal for later extraction */
				BuildGlobal(0, g, v1dp, NULL, svPtr);
				strcpy( (char *) bp,(const char *) "<gbl>");
				goto exec;
				}

//-----------
// $ORDER FCN
//-----------

			if ( toupper(svPtr->pd1[svPtr->sdlim + 1]) == 'O') {

				for (i = svPtr->sdlim; svPtr->pd1[i]; i++);

				i--;

				if (svPtr->pd1[i] == OPEN_PAREN ) {      // first argument
					strcpy( (char *) bp,(const char *) v1dp);

					for (int i=0; bp[i]; i++) if (bp[i]>127) bp[i]=1;

					goto exec;
					}

				else {
					f = Mglobal (g, v1dp, bp, svPtr);
					goto exec;
					}

				}

			if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'B') {
				g = ORDERPREV;
				BuildGlobal(0,g,v1dp,NULL,svPtr);
				strcpy( (char *) bp,(const char *) "<gbl>");
				goto exec;
				}

			}

		f = Mglobal (g, v1dp, bp, svPtr);

		/*******************************
		    fix $order end string
		*******************************/

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'O')
			if (strcmp ((char *) bp, "-1") == 0) svPtr->bd[1] = 0;   /* empty */

		/*
		 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		 * + + + check for $data + + +
		 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		 */

		if (svPtr->pd1[svPtr->sdlim] == '$' && s1p[spx] != INDIRECT &&
		        (svPtr->pd1[svPtr->sdlim + 1] == 'D' || 
			svPtr->pd1[svPtr->sdlim + 1] == 'd')) {

			if (f == 0) bbtyp = '0';
			else bbtyp = '1';

			i = strlen ((char *) v1dp);

//-------------
//	child?
//-------------

#if !(defined(SQLITE))
			if (v1dp[i - 1] != 1) {
				v1dp[i++] = 1;
				v1dp[i++] = 1;
				v1dp[i++] = 0;
				}

			v1dp[i] = 2;
			v1dp[i + 1] = 1;
			v1dp[i + 2] = 0;

			f = Mglobal (NEXT, v1dp, svPtr->bd, svPtr);
	#endif

#if (defined(SQLITE))
			v1dp[i++] = 1;
			v1dp[i++] = 1;
			v1dp[i++] = 0;
			f = Mglobal (NEXT, v1dp, svPtr->bd, svPtr);
	#endif

			svPtr->bd[2] = bbtyp;

			if (f == 1)
				svPtr->bd[1] = '1';
			else
				svPtr->bd[1] = '0';

			if (svPtr->bd[1] == '0') {
				svPtr->bd[1] = svPtr->bd[2];
				svPtr->bd[2] = 0;
				}

			svPtr->bd[3] = 0;
			goto exec;
			}

		svPtr->tpx = f;
		goto exec;
		}

	/*
	 * normal global
	 */

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + built-in variables/fcns + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	if (svPtr->v1d[1] == '$') {

		if (svPtr->v1d[2] == 'd' ||
		        /*  svPtr->v1d[2] == 'n' ||
		            svPtr->v1d[2] == 'o' || svPtr->v1d[2] == 'O' ||
		            svPtr->v1d[2] == 'b' || svPtr->v1d[2] == 'B' || */
		        svPtr->v1d[2] == 'D' /* || svPtr->v1d[2] == 'N' */ ) {

			/* open */
			for (i = 1; svPtr->v1d[i] != CodedOpen; i++);

			j = 0;

			/* close */
			while ((svPtr->bd[++j] = svPtr->v1d[++i]) != CodedClose);

			svPtr->bd[j] = 0;
			goto exec;
			}

		if (svPtr->v1d[2] == 'z' || svPtr->v1d[2] == 'Z') {
			zfcn (svPtr->v1d, &svPtr->bd[1],svPtr);
			}
		else {
			fcn (svPtr);
			}

		if (svPtr->ierr == 0) goto exec;

		return (svPtr->ierr);
		}


	if (svPtr->pd1[svPtr->sdlim] == '$' && s1p[spx] == OPENC &&
	        ( svPtr->pd1[svPtr->sdlim + 1] == 'G' || svPtr->pd1[svPtr->sdlim + 1] == 'g') ) {

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'G') svPtr->symflg = SymName;
		else svPtr->symflg = SymNext;

		sym_(svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr);

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'N' && svPtr->bd[1] == 0)
			strmove (bp, (unsigned char *) "-1");

		if (svPtr->symflg && s1p[spx] != INDIRECT) goto exec;

		return (27);
		}


	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + retrieve look-up + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	if (svPtr->v1d[1] == PREDICATEVAR) {
		strmove (bp, v1dp);
		goto exec;
		}

	if (svPtr->pd1[svPtr->sdlim]=='$' && s1p[spx]==OPENC) {

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'Q' ) { // query qlength qsubscript
			g = NEXT;
			BuildLocal(0,g,v1dp,NULL,svPtr);
			strcpy( (char *) bp,(const char *) "<lcl>");
			goto exec;
			}

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'N' ) {
			g = NEXT;
			BuildLocal(0,g,v1dp,NULL,svPtr);
			strcpy( (char *) bp,(const char *) "<lcl>");
			goto exec;
			}

		if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'O') {

// fff for (int i=0; v1dp[i]; i++) printf("%d ",v1dp[i]);
// fff printf("\n\n");

			FcnArgToggle ++;
			FcnArgToggle %= 2;

			if (FcnArgToggle ==1 ) {


				g = ORDERNEXT;

				BuildLocal(0, g, v1dp, NULL, svPtr);

				strcpy( (char *) bp,(const char *) "<lcl>");
				goto exec;
				}
			}
		}


	svPtr->symflg = SymRetrieve;

// if LHS $extract(), do not resolve variable

//    if (svPtr->pd1[svPtr->sdlim] == '$' && svPtr->t0px == 1 &&
//            (svPtr->pd1[svPtr->sdlim+1] == 'e' || svPtr->pd1[svPtr->sdlim+1] == 'E') ) {
//        svPtr->symflg=1;
//        strcpy((char *)bp,"");
//        }

// normal variable lookup

//    else  if (sym_ (svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr) == NULL)
	if (sym_ (svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr) == NULL)
		svPtr->symflg=0;

	else svPtr->symflg=1;

	if (svPtr->pd1[svPtr->sdlim] != '$') goto sym1;

	// * local variable $DATA

	if (s1p[spx] == OPENC && (svPtr->pd1[svPtr->sdlim + 1] == 'd' || svPtr->pd1[svPtr->sdlim + 1] == 'D')) {
		svPtr->symflg = SymData;
		sym_ (svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr);
		goto exec;
		}


	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + check for variable not found + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

sym1:

	if (svPtr->symflg == 0) {
		int i = 1, j, k = 1;

		while (1) {                                   /* look for a label of the same name */
			for (j = i; svPtr->pd1[j] != TAB && svPtr->pd1[j] != ' '; j++);

			if (strncmp ((char *) &svPtr->pd1[i], (char *) v1dp, j - i) == 0 &&
			        strlen ((char *) v1dp) == j - i) {
				sprintf ((char *) bp, "%d", k);
				goto exec;
				}

			i = i + strlen ((char *) &svPtr->pd1[i]) + 1;

			if (i > svPtr->pd1len) break;

			k++;
			}

		return (17);
		}


	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 * + process value in bb or svPtr->bd + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

exec:

	if (s1p[spx] != PEMPTY && s1p[spx] != POPEN && s1p[spx] != OPENC) {
		goto nxt_expr;
		}

	spx++;

	while (svPtr->pd1[svPtr->sdlim++]);

	strmove (&svPtr->pd1[svPtr->sdlim], bp);
	s1p[spx] = OPERAND;
	goto nchar;

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + process expression - check for operator on stack top + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

nxt_expr:

	if (s1p[spx] == POPEN || s1p[spx] == OPERAND) {
		return (11);
		}

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + check for number under operator + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	if (s1p[spx - 1] != OPERAND) {

		switch (s1p[spx]) {

			default:

				return (1);

			case INDIRECT:

				strcat ((char *) bp, (char *) &svPtr->xd[svPtr->xpx + 1]);
				strmove (&svPtr->xd[1], bp);
				spx--;
				svPtr->xpx = 0;

				if (svPtr->kflg && spx <= 1) {
					svPtr->xpx = 1;
					return (0);
					}

				goto nchar;

			case MINUS:                               /* unary minus */

				strmove (tmp3, (unsigned char *) "-1");
				mult ((char *) bp, (char *) tmp3, (char *) bp);
				break;

			case NOT_NOT:                                  /* not not */

				svPtr->bd[1] = numcomp (bp, (unsigned char *) "0") ? '1' : '0';
				svPtr->bd[2] = 0;
				break;

			case NOT:                                 /* unary not */

				svPtr->bd[1] = numcomp (bp, (unsigned char *) "0") ? '0' : '1';
				svPtr->bd[2] = 0;
				break;

			case PLUS:                                /* unary plus */

				strmove (tmp3, (unsigned char *) "0");
				add ((char *) bp, (char *) tmp3, (char *) bp);

			}                                             /* switch */

		spx--;
		goto nxt_operator;
		}

//-----------------------------
// extract value under operator
//-----------------------------

	for (adx = svPtr->sdlim--; svPtr->pd1[svPtr->sdlim - 1]; svPtr->sdlim--);

//-------------------------------
// branch depending upon operator
//-------------------------------

	switch (s1p[spx]) {

		case OPERAND:
		case PEMPTY:
		case NOT:
		default:

			return (12);

		case DIVIDE:

			divx ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case EXPONEN:

			expx ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case MULTIPLY:

			mult ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case MINUS:

			sub ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case PLUS:

			add ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case CONCAT:

			to = &svPtr->pd1[adx]; 			// start of left operand
			while( *(to++) );			// find end of left operand
			--to;					// retreat fron null
			from = bp;				// start of right operand
			while( *(to++) = *(from++) );		// concatenate
			if ( to - &svPtr->pd1[adx] >= STR_MAX)
				return 35;			// too long
			*to=0;					// null
			to=bp;					// address of target
			from=&svPtr->pd1[adx];			// address of source
			while (*(to++) = *(from++) );		// copy result to bp
			break;

		case EQUALS:

			svPtr->bd[1] = strcmp ((char *) &svPtr->pd1[adx], (char *) bp) ? '0' : '1';
			svPtr->bd[2] = 0;
			break;

		case NOTEQ:
			svPtr->bd[1] = strcmp ((char *) &svPtr->pd1[adx], (char *) bp) ? '1' : '0';
			svPtr->bd[2] = 0;
			break;

		case GREATER:

			svPtr->bd[1] = (numcomp (&svPtr->pd1[adx], bp) > 0) ? '1' : '0';
			svPtr->bd[2] = 0;
			break;

		case LESSTHAN:

			svPtr->bd[1] = (numcomp (&svPtr->pd1[adx], bp) < 0) ? '1' : '0';
			svPtr->bd[2] = 0;
			break;

		case INTDIVIDE:

			divi ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case MODULO:

			modulo ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
			break;

		case CONTAINS:

			if (svPtr->pd1[adx] == 0 && bp == 0) {
				svPtr->bd[1] = '1';
				svPtr->bd[2] = 0;
				break;
				}

			if (xindex ((unsigned char *) &svPtr->pd1[adx], (unsigned char *) bp, (short) 1) == 0)
				svPtr->bd[1] = '0';
			else
				svPtr->bd[1] = '1';

			svPtr->bd[2] = 0;
			break;

		case FOLLOWS:

			if (strcmp ((char *) &svPtr->pd1[adx], (char *) bp) <= 0)
				svPtr->bd[1] = '0';
			else
				svPtr->bd[1] = '1';

			svPtr->bd[2] = 0;
			break;

int rpm(unsigned char [], const unsigned char [], struct MSV *);

case PATTERN:

                        if (rpm (&svPtr->pd1[adx], 
				(const unsigned char *) &svPtr->bd[0],svPtr) == 0)
                               		svPtr->bd[1] = '0';
                        else
					svPtr->bd[1] = '1';
                
                        svPtr->bd[2] = 0;
                        break;
        
case NOTPATTERN:
        
                        if (rpm ( &svPtr->pd1[adx], 
				(const unsigned char *) &svPtr->bd[0],svPtr) == 0)
					svPtr->bd[1] = '1';
                        else
					svPtr->bd[1] = '0';

                        svPtr->bd[2] = 0;
                        break;

		case NOTGREATER:

			if (numcomp (&svPtr->pd1[adx], bp) <= 0)
				svPtr->bd[1] = '1';
			else
				svPtr->bd[1] = '0';

			svPtr->bd[2] = 0;
			break;

		case NOTLESS:

			if (numcomp (&svPtr->pd1[adx], bp) >= 0)
				svPtr->bd[1] = '1';
			else
				svPtr->bd[1] = '0';

			svPtr->bd[2] = 0;
			break;

		case NOTCONTAINS:

			if (xindex ((unsigned char *) &svPtr->pd1[adx], (unsigned char *) bp, (short) 1) == 0)
				svPtr->bd[1] = '1';
			else
				svPtr->bd[1] = '0';

			svPtr->bd[2] = 0;
			break;

		case NOTFOLLOWS:

			if (strcmp ((char *) &svPtr->pd1[adx], (char *) bp) <= 0)
				svPtr->bd[1] = '1';
			else
				svPtr->bd[1] = '0';

			svPtr->bd[2] = 0;
			break;

		case AND:

			svPtr->bd[1] = numcomp (bp, (unsigned char *) "0")
			               && numcomp (&svPtr->pd1[adx], (unsigned char *) "0") ? '1' : '0';
			svPtr->bd[2] = 0;
			break;

		case OR:
			svPtr->bd[1] = numcomp (bp, (unsigned char *) "0")
			               || numcomp (&svPtr->pd1[adx], (unsigned char *) "0") ? '1' : '0';
			svPtr->bd[2] = 0;
			break;

		case NOTAND:

			if (numcomp (bp, (unsigned char *) "0")
			        && numcomp (&svPtr->pd1[adx], (unsigned char *) "0"))
				svPtr->bd[1] = '0';
			else
				svPtr->bd[1] = '1';

			svPtr->bd[2] = 0;
			break;

		case NOTOR:

			if (numcomp (bp, (unsigned char *) "0")
			        || numcomp (&svPtr->pd1[adx], (unsigned char *) "0"))
				svPtr->bd[1] = '0';
			else
				svPtr->bd[1] = '1';

			svPtr->bd[2] = 0;

		}


dec_stk:

	spx -= 2;

nxt_operator:

	if (s1p[spx] != POPEN && s1p[spx] != OPENC && s1p[spx] != PEMPTY)
		goto nxt_expr;

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + push answer + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

	spx++;
	svPtr->sdlim += strlen ((char *) &svPtr->pd1[svPtr->sdlim]) + 1;

	to = &svPtr->pd1[svPtr->sdlim];
	from = bp;
	while ( *(to++) = *(from++) );
	*to = 0;

	s1p[spx] = OPERAND;
	goto nchar;

	/*
	 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
	 * + exit sequence + + +
	 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 */

finish:

	if (s1p[spx - 1] != PEMPTY || s1p[spx] != OPERAND) return (16);
	else return (0);

	}


/*===========================================================================*
 *                                  patrn1                                   *
 *===========================================================================*/

void patrn1(short *jpx, short *ernbr, struct MSV * svPtr)

/* pattern argument [right side) set up */
	{
	const static unsigned char cod210[2] = { 210, 0 };
	unsigned char tmp1[2] = { 0, 0 };

a1285:

	if (svPtr->xd[*jpx] == '"') {
		strcat((char *) &svPtr->bd[0], "\"");

a1283:

		*jpx = (*jpx + 1);

		if (svPtr->xd[*jpx] == 0) {
			*ernbr = 2;
			return;
			}

		if (svPtr->xd[*jpx] == '"') {
			if (svPtr->xd[*jpx + 1] != '"')
				goto a1281;

			strcat((char *) &svPtr->bd[0], (char *) cod210);
			*jpx = (*jpx + 1);
			}

		else {
			tmp1[0] = svPtr->xd[*jpx];
			strcat((char *) &svPtr->bd[0], (char *) tmp1);
			}

		goto a1283;
		}

a1281:

	tmp1[0] = svPtr->xd[*jpx];

	if (svPtr->xd[*jpx] != 0) {
		if (xindex((unsigned char *) " ,&:!''><=[)+-*/#_", (unsigned char *) tmp1, (short) 1)
		        == 0) {
			strcat((char *) &svPtr->bd[0], (char *) tmp1);
			*jpx = (*jpx + 1);
			goto a1285;
			}
		}

	svPtr->xpx = (*jpx - 1);
	*ernbr = 0;
	return;
	}

/*===========================================================================*
 *                                  patrn                                   *
 *===========================================================================*/

void patrn(unsigned char* ad, struct MSV *svPtr) {

        unsigned char indef, tmp[10];
        short iptr, k, j, i, m, pts[3][20], lx, count[20], ct2[20], stk[20], stx;
        short high[20], low[20], lad;

        indef =0 ; lx = 0; lad = strlen((char *) ad);
        for (i = 0; i < 20; i++)
                { high[i] = lad; low[i] = 0; }
        for (i = 0; i <= strlen((char *) &svPtr->bd[0]); i++) {

        lx++;
        if (svPtr->bd[i] <' 0' || svPtr->bd[i] > '9') {
                if (svPtr->bd[i] != '.') i--;
                count[lx] = (-1); indef = 1; i++;
                if (svPtr->bd[i] >= '0' && svPtr->bd[i] <= '9') {
                        j = i; while(svPtr->bd[i] >= '0' && svPtr->bd[i] <= '9') i++;
                        substr(&svPtr->bd[1], tmp, j, i - j);
                        cannon((char *) tmp);
                        high[lx] = atoi((char *) tmp); }
                }

        else {  j = i;
                while(svPtr->bd[i] >= '0' && svPtr->bd[i] <= '9') i++;
                substr(&svPtr->bd[1], tmp, j, i - j);
                cannon((char *)tmp);
                count[lx] = atoi((char *) tmp);
                if (svPtr->bd[i] == '.') {
                        low[lx] = count[lx];
                        count[lx] = -1;
                        indef = 1;
                        i++;
                        if (svPtr->bd[i] >= '0' && svPtr->bd[i] <= '9') {
                                j = i;
                                while(svPtr->bd[i] >= '0' && svPtr->bd[i] <= '9') i++;
                                substr(&svPtr->bd[1], tmp, j, i - j);
                                cannon((char *)tmp);
                                high[lx]=atoi((char *) tmp); }           
                        }
                }

        if (svPtr->bd[i] != '"') {
                tmp[0] = svPtr->bd[i]; tmp[1] = 0;
                if (xindex((const unsigned char *)"cnpalue", tmp, 1) == 0 &&
                    xindex((const unsigned char *)"CNPALUE", tmp, 1) == 0) goto err;
                pts[1][lx] = i; pts[2][lx] = 1; continue; }
        j = i;
        if (svPtr->bd[j+1] == '"') {
                j++; pts[1][lx] = i; pts[2][lx] = 0; i = j; continue; }

a108:   if (svPtr->bd[++j] == 0) goto err;
        if (svPtr->bd[j] != '"') { if (svPtr->bd[j] == 210) svPtr->bd[j] = '"'; goto a108; }
        pts[1][lx] = i; pts[2][lx] = j - i; i = j;
        }

	if (indef != 1) 
	{ match(pts, lx, count, ad, &j, svPtr->bd); ierr1 = j; return; }
        
        stx = 0;
        for ( i = 1; i <= lx; i++) { if (count[i] < 0) stk[++stx] = i; }
        for (i = 1; i <= lx; i++)
                { if (count[i] >= 0) ct2[i] = count[i]; else ct2[i] = low[i]; }
        iptr = stx; goto retry1;

retry:  ct2[stk[1]]++;
retry1: if (ct2[stk[1]] > lad || ct2[stk[1]] > high[stk[1]]) {
                ct2[stk[1]] = low[stk[1]]; m = 2;
                if (stx == 1) {ierr1 = 0; return; }
                ct2[stk[m]]++;
recount:        if (ct2[stk[m]] > lad || ct2[stk[m]] > high[stk[m]]) {
                        if (m >= stx) {ierr1 = 0; return; }
                        ct2[stk[m]] = low[stk[m]]; m++; ct2[stk[m]]++;
                        goto recount; } }
        k = 0; for (j = 1; j <= lx; j++) k = k + ct2[j];
        if (k > lad) goto retry;
        match(pts, lx, ct2, ad, &j, svPtr->bd);
        if ((ierr1 = j) == 1) {return;}
        goto retry;

err:    ierr1 = 99; return;
                        }

/*===========================================================================*
 *                                  getstr2                                  *
 *===========================================================================*/

short getstr2 (short int lun, unsigned char area[], struct MSV * svPtr) {

	short int i, chr;

	if (lun == 5) svPtr->in_file[lun] = stdin;

	for (i = 0; i < STR_MAX; i++) {               // restrict input line length
		chr = fgetc (svPtr->in_file[lun]);

		if (chr == EOF) {
			area[0] = 0;
			return (-1);
			}

		area[i] = chr;

		if (area[i] == LINEFEED) {
			area[i] = 0;
			return (i);
			}

		if (area[i] > 127) area[i] = ' ';

		if (area[i] == '\n') i--;
		else if (area[i] == '\r') i--;
		else if (area[i] == TAB) area[i] = TAB;
		else if (area[i] < 32) area[i] = ' ';
		}

	area[STR_MAX] = 0;                            // restricted length
	return (-1);
	}


/*===========================================================================*
 *                                   readf                                   *
 *===========================================================================*/

void readf_ (unsigned char zd[], short pd1org, struct MSV * svPtr) {

	short int i, j, k, l;
	FILE *infile;

	j = pd1org;
	strcpy(svPtr->CurrentFile,(char *)zd);
	infile = fopen ((const char *) zd, "r");

	if (infile == NULL) {
		zd[0] = 0;
		return;
		}

	l = j;

	for (i = j; i < svPtr->symlen; i++) {
		k = getc (infile);

		if (k > 127) k = 32;

		if (k =='\\') { // \ at the end of line is continuation
			k = getc (infile);

			if (k<1) {
				printf("\n*** Input error in readf_()\n");
				sigint(100);
				}

			if (k == '\n') {
				while (k=getc(infile)) {
					if (k==' ' || k=='\t') continue;

					if (k=='\r') continue;

					if (k<1) break;

					if (k > 0) svPtr->pd1[i] = k;

					break;
					}
				}
			else {
				svPtr->pd1[i++]='\\';
				svPtr->pd1[i] = k;
				continue;
				}
			}


		if (k == CRTN || k == CTRLZ) i--;

		else {
			if (k > 0) svPtr->pd1[i] = k;
			}

		if (k == LINEFEED) {
			svPtr->pd1[i] = 0;
			continue;
			}

		if (k < 1) {
			fclose (infile);
			svPtr->pd1[i] = 0;
			svPtr->pd1[i + 1] = 0;
			svPtr->pd1len = i - 1;
			return;
			}

		}

	if (i>=svPtr->symlen)
		ErrorMessage("Interpreter cache overflow\n*** Increase ibuf in configure",
		             svPtr->LineNumber);
	}


/*===========================================================================*
 *                                   inout                                   *
 *===========================================================================*/

void inout (unsigned char text[], short cmnd, short col, struct MSV * svPtr) {
	short int i, j, dev;

	dev = svPtr->io;

	switch (cmnd) {

		case 1:                                   /* new page */

			fputc ('\014', svPtr->out_file[dev]);
			svPtr->hor[dev] = 1;
			svPtr->ver[dev] = 0;
			return;

		case 2:                                   /* skip line */

			fprintf (svPtr->out_file[dev], "\n");
			svPtr->hor[dev] = 0;
			svPtr->ver[dev]++;
			return;

		case 3:                                   /* tab */

			if (col == svPtr->hor[dev]) return;

			if (col < 1) return;

			if (dev == 5) {
				if (svPtr->hor[5]>col) {
					putchar('\n');
					svPtr->hor[5]=0;
					}

				for (i = svPtr->hor[dev]; i <= col - 1; i++) putchar(' ');
				}

			else {
				if (svPtr->hor[dev]>col) fputc ('\n', svPtr->out_file[dev]);

				for (j = svPtr->hor[dev]; j <= col - 1; j++)
					fputc (' ', svPtr->out_file[dev]);
				}

			svPtr->hor[dev] = col;
			return;

		case 4:                                   /* text print */

			fprintf (svPtr->out_file[dev], "%s", text);

			if (dev == 5) fflush(stdout);

			svPtr->hor[dev] = svPtr->hor[dev] + strlen ( (const char *) text);
			return;
		}
	}


/*===========================================================================*
 *                                  prnterr                                  *
 *===========================================================================*/
void prnterr (int i, struct MSV * svPtr, int pd1cur) {

	int n=1, nx=1, LN=1;

	for (n=1; n<PS; n++) {
		if (! svPtr->pd1[n] ) {
			LN++;
			}

		if ( n > pd1cur ) break;
		}

	if (svPtr->pd1[n]) nx=n;
	else nx=n-1;

	for (; svPtr->pd1[nx]&&nx>0; nx--);

	if (nx>1) {
		nx--;

		for (; svPtr->pd1[nx]&&nx>0; nx--);
		}

	svPtr->LineNumber=LN-1;

	if (svPtr->NOERR) return;

	fprintf (stderr, "\n\n*** in or near line %d:\n%s\n*** E%d ", svPtr->LineNumber, &svPtr->pd1[nx+1], i);

	switch (i) {

		case 1:
			fprintf (stderr, "Multiple adjacent operators");
			break;

		case 2:
			fprintf (stderr, "Unmatched quotes");
			break;

		case 3:
			fprintf (stderr, "Global not found");
			break;

		case 4:
			fprintf (stderr, "Missing comma");
			break;

		case 5:
			fprintf (stderr, "Argument not permitted");
			break;

		case 6:
			fprintf (stderr, "Bad character after post-conditional");
			break;

		case 7:
			fprintf (stderr, "Invalid quote");
			break;

		case 8:
			fprintf (stderr, "label not found:");
			break;

		case 9:
			fprintf (stderr, "Too many/few fcn arguments");
			break;

		case 10:
			fprintf (stderr, "Invalid number");
			break;

		case 11:
			fprintf (stderr, "Missing operator");
			break;

		case 12:
			fprintf (stderr, "Unrecognized operator: %c",svPtr->ErrChar);
			break;

		case 13:
			fprintf (stderr, "Keyword");
			break;

		case 14:
			fprintf (stderr, "Argument list");
			break;

		case 15:
			fprintf (stderr, "Divide by zero");
			break;

		case 16:
			fprintf (stderr, "Invalid expression");
			break;

		case 17:
			fprintf (stderr, "Variable not found");
			break;

		case 18:
			fprintf (stderr, "Invalid reference");
			break;

		case 19:
			fprintf (stderr, "Logical table space overflow");
			break;

		case 20:
			fprintf (stderr, "I/O error");
			break;

		case 23:
			fprintf (stderr, "Symbol table full");
			break;

		case 24:
			fprintf (stderr, "Function argument error");
			break;

		case 25:
			fprintf (stderr, "Global not permitted");
			break;

		case 26:
			fprintf (stderr, "File error");
			break;

		case 27:
			fprintf (stderr, "$N error");
			break;

		case 29:
			fprintf (stderr, "<break> at line:");
			break;

		case 30:
			fprintf (stderr, "Function not found");
			break;

		case 31:
			fprintf (stderr, "Program space exceeded");
			break;

		case 32:
			fprintf (stderr, "Stack overflow");
			break;

		case 33:
			fprintf (stderr, "SQL error");
			break;

		case 34:
			fprintf (stderr, "Arg count mis-match");
			break;

		case 35:
			fprintf(stderr, "String length too long");
			break;

		case 36:
			fprintf(stderr, "Use unit not open");
			break;

		case 37:
			fprintf(stderr, "Unrecognized statement");
			break;

		default:
			fprintf (stderr, "Unknown error");
		}

	fprintf(stderr, "\n\n");
	return;
	}


int BuildGlobal(int code, int g, unsigned char * str, unsigned char * rslt, struct MSV * svPtr) {

	static unsigned char tmp[TSTACK][STR_MAX];
	static int stk=0;

	if (code<0) {
		stk=0;
		return 1;
		}

	if (code==0) {

		if (stk>=TSTACK) {
			printf("*** Build Stack Overflow %d\n\n", svPtr->LineNumber);
			sigint(100);
			}

		strcpy( (char *) tmp[stk++],(const char *) str);
		return 1;
		}

	if (code==2) {
		if (stk==0) rslt[0]=0;
		else strcpy( (char *) rslt,(const char *) tmp[--stk]);

		return 1;
		}

// code 1

	stk--;

	if (g == NEXT) GlobalOrder(tmp[stk],rslt, (unsigned char *) "1", svPtr); //GlobalNext(tmp,rslt,svPtr);
	else if (g == ORDERNEXT) GlobalOrder(tmp[stk],rslt, (unsigned char *) "1",svPtr);
	else if (g == ORDERPREV) GlobalOrder(tmp[stk],rslt, (unsigned char *) "-1",svPtr);
	else return 0;

	return 1;
	}
