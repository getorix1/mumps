//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                                  
//#+    #     # #     # #     # ######   #####
//#+    ##   ## #     # ##   ## #     # #     #
//#+    # # # # #     # # # # # #     # #
//#+    #  #  # #     # #  #  # ######   #####
//#+    #     # #     # #     # #             #
//#+    #     # #     # #     # #       #     #
//#+    #     #  #####  #     # #        #####
//#+    
//#+    Mumps Interpreter                            
//#+
//#+    Copyright (C) 2022  Kevin C. O'Kane
//#+
//#+    Kevin C. O'Kane, Ph.D.
//#+    Computer Science Department
//#+    University of Northern Iowa
//#+    Cedar Falls, IA 50614-0507
//#+    kc.okane@gmail.com
//#+    https://threadsafebooks.com/
//#+    https://www.cs.uni.edu/~okane
//#+
//#+    This program is free software; you can redistribute it and/or modify
//#+    it under the terms of the GNU General Public License as published by
//#+    the Free Software Foundation; either version 2 of the License, or
//#+    (at your option) any later version.
//#+
//#+    This program is distributed in the hope that it will be useful,
//#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#+    GNU General Public License for more details.
//#+
//#+    You should have received a copy of the GNU General Public License
//#+    along with this program; if not, write to the Free Software
//#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+
//#+                                                              
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#+                                                              
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                            
//#+               ######     #    ######   #####  #######     
//#+               #     #   # #   #     # #     # #          
//#+               #     #  #   #  #     # #       #         
//#+               ######  #     # ######   #####  #####    
//#+               #       ####### #   #         # #       
//#+               #       #     # #    #  #     # #      
//#+               #       #     # #     #  #####  #######  
//#+                                                       
//#+                                                      
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                      
//#+ This code was originally written in Fortran which should explain
//#+ the unusual array usage.  When written, memory was at a premium
//#+ and techniques were used to minimize memory usage.
//#+                                                      
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "memsize.h"

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defines.h"

#define SymStore 0
#define SymRetrieve 1
#define SymDeleteExplicit 2
#define SymNext 3
#define SymDeleteAll 4
#define SymDeleteAllExcept 5
#define SymData 6

//#define RETRIEVE 0
//#define STORE 1
//#define NEXT 2
//#define XNEXT 8
//#define NEXTX 20
//#define KILL1 30
//#define PREVIOUS 9

#define JOB 0
#define SET 1
#define GOTO 2
#define WRITE 3
#define IF 4
#define ZCMDS 6
#define READ 7
#define QUIT 8
#define XECUTE 9
#define TAB 9
#define FOR 10
#define HALT 11
#define ELSE 12
#define DO 13
#define KILL 14
#define CLOSE 15
#define USE 16
#define LOCK 17
#define COMMENT 18
#define BREAK 19
#define BADCMD 21
#define KILL2 20
#define FCN 20
#define LBL 21
#define QUOTE '\''
#define OPEN 0
#define DIVIDE 1
#define MULTIPLY 2
#define MINUS 3
#define PLUS 4
#define OPERAND 5
#define OPENC 6
#define CONCAT 7
#define EQUALS 8
#define GREATER 9
#define EMPTY 10
#define LESSTHAN 11
#define NOT 12
#define INTDIVIDE 13
#define MODULO 14
#define CONTAINS 15
#define FOLLOWS 16
#define PATTERN 17
#define NOTEQ 18
#define NOTGREATER 19
#define NOTLESS 20
#define NOTCONTAINS 21
#define NOTFOLLOWS 22
#define NOTPATTERN 23
#define AND 24
#define OR 25
#define NOTAND 26
#define NOTOR 27
#define INDIRECT 28
#define PREDICATEVAR '~'
#define CodedOpen 206
#define CodedClose 207
#define CodedComma 208
#define CodedColon 209

#include "externs.h"

void  patrn (unsigned char *);
void  patrn1 (short *, short *);
void  zfcn(void);
int   xindex(unsigned char *, unsigned char *, short);
void  logic(FILE *opnfile[]);
void  errmod_(short msgnbr, unsigned char text[],FILE *opnfile[]);
void  fcn();
void  readf_(unsigned char zd[], short pd1org, FILE *opnfile[]);
void  inout(int popn[7][4], unsigned char opnflg[], FILE *opnfile[],
        unsigned char text[], short cmnd, short col);
int   global(short g, unsigned char key[], unsigned char bd[]);
void  add(char *a, char *b, char *c);
void  sub(char *a, char *b, char *c);
void  mult(char *a, char *b, char *c);
void  divx(char *a, char *b, char *c);
int   numcomp(char aa[], char bb[]);
void  cannon(char a[]);
void  sym_(unsigned char a[], unsigned char b[]);


parse_() {

/* 206 = open; 207 = close; 208 = comma; 209 = colon; */

short ernbr,f,spx,adx,jpx,j,i,g;
static unsigned char cod209[2]={209,0};
static unsigned char s1p[40];
unsigned char bbtyp,tmp2[4],tmp1[25],tmp3[1024],nxtchr;
unsigned char * bp=&bd[1];
unsigned char * v1dp=&v1d[1];

static unsigned char code[26]={
/*0*/   99, 99, 99, 99, 99, 99, 99, 99, 18, 19,
/*10*/  99, 20, 18, 99, 99, 21, 22, 23, 99, 99,
/*20*/  99, 99, 99, 99, 26, 27 };
        
static unsigned char opcode[256] = {
/*0*/   99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*10*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*20*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*30*/  99, 99, 99, 25, 99, 14, 99, 99, 24, 12,
/*40*/   0, 99,  2,  4, 99,  3, 99,  1, 99, 99,
/*50*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*60*/  11,  8,  9, 17, 28, 99, 99, 99, 99, 99,
/*70*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*80*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*90*/  99, 15, 13, 16, 99,  7, 99, 99, 99, 99,
/*100*/ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*110*/ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*120*/ 99, 99, 99, 99, 99, 99, 99, 99 } ;

static unsigned char ncode[256] = {
/*0*/   99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*10*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*20*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*30*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*40*/  99, 99, 99, 99, 99, 99, 10, 99, 10, 10,
/*50*/  10, 10, 10, 10, 10, 10, 10, 10, 99, 99,
/*60*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*70*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*80*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*90*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*100*/ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*110*/ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*120*/ 99, 99, 99, 99, 99, 99, 99, 99 } ;

static unsigned char dcode[256] = {
/*0*/   10, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*10*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*20*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*30*/  99, 99, 10, 10, 99, 10, 99, 99, 10, 10,
/*40*/  99, 10, 10, 10, 10, 10, 99, 10, 99, 99,
/*50*/  99, 99, 99, 99, 99, 99, 99, 99, 10, 99,
/*60*/  10, 10, 10, 10, 10, 99, 99, 99, 99, 99,
/*70*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*80*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*90*/  99, 10, 10, 10, 10, 10, 99, 99, 99, 99,
/*100*/ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*110*/ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*120*/ 99, 99, 99, 99, 99, 99, 99, 99 } ;

static unsigned char operand[256] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 2, 1, 0, 1, 1, 2, 0,
0, 0, 0, 0, 0, 0, 1, 0, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
2, 2, 2, 2, 0, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 2, 0, 2, 1, 0, 0, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 0, 0, 0, 0, 0 } ;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       initialization                                          +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

xpx--; 
spx=0; 
pd1[pd1len+2]=0; 
pd1[pd1len+3]=0; 
sdlim=pd1len+3;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       main recursive internal entry point                     +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

start:  

    spx++; 
    s1p[spx]=EMPTY; // stack foundation

nchar:  if ((nxtchr=xd[++xpx])==0) goto finish;

        if (operand[nxtchr]==1) goto scan_operand;

        if (nxtchr==CodedOpen) {
            unsigned char *p1, *p2;
            spx++;
            while(pd1[sdlim++]);
            if (sdlim>=symlen) return(31);
            p1=&v1d[1];
            p2=&pd1[sdlim];
            while (*p2++=*p1++);
            *p2=0;
            p2--;
            *p2=CodedOpen;
            s1p[spx]=OPENC;       
            goto nchar;
            }

        if (nxtchr==',') {
                if (s1p[spx-1]!=OPENC) {
                        if (t2>0) goto finish;
                        return(16); 
                        }

//*      concatenate stack tops and add coded comma 

                for (i=--sdlim; (pd1[i]=pd1[i+1]); i++); //* join tops 
                pd1[i]=CodedComma; 
                pd1[i+1]=0;  //* coded comma 
                sdlim--;
                while (pd1[sdlim--]); //* find new stack top start 
                sdlim+=2;
                spx--;  //* stack pointer 
                goto nchar;
                }

        if (nxtchr==')') {

                if (s1p[spx-1]!=OPEN) {
                        if (s1p[spx-1]!=OPENC)  return(16);

                        //* fcn/array return section 

                        sdlim--;
                        for (i=sdlim; (pd1[i]=pd1[i+1]); i++); /*join tops*/
                        pd1[i]=CodedClose; 
                        pd1[i+1]=0; /* coded close */
                        sdlim--;
                        while (pd1[sdlim--]); /* find new stack top start */
                        sdlim+=2;
                        spx--;  /* stack pointer */
                        goto un_nest; 
                        }

/* precedence close paren */

                if (s1p[spx]!=OPERAND) return(12);

/* extract value from stack top */

                strcpy(bp,&pd1[sdlim]);
                do sdlim--; while (pd1[sdlim-1]); /*compress stack*/
                goto dec_stk;
                }

        if (nxtchr==':') {
                i=sdlim;
                do i--; while(pd1[i-1]);
                if (pd1[i]!='$' || (pd1[i+1]!='s' && pd1[i+1]!='S')) goto finish;
                sdlim--;
                strcpy(&pd1[sdlim],&pd1[sdlim+1]);
                while(pd1[sdlim--]); 
                sdlim+=2;
                strcat(&pd1[sdlim],cod209); 
                spx--; 
                goto nchar;
                }

        if (nxtchr==' ' || nxtchr=='\t') goto finish;

        if (nxtchr=='^' && t2==2 && s1p[spx]==OPERAND) goto finish;

/*****  if (nxtchr==PREDICATEVAR) goto scan_operand;  ~... variable 
        if (nxtchr=='\037') goto scan_operand; lclid variable 
******/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       check for delimiters in list                            +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        spx++;
        if (nxtchr==QUOTE) { /* insert not based operator code */
                xpx++;
                s1p[spx]=opcode[xd[xpx]];
                if (s1p[spx]!=99) {
                  s1p[spx]=code[opcode[xd[xpx]]];
                  if (s1p[spx]!=99) goto nchar;
                  }
                xpx--;
                }

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       insert code                                             +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        s1p[spx]=opcode[nxtchr];
        if (s1p[spx]!=99) goto nchar;
        return(12);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       scan for operand                                        +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

scan_operand:

        jpx=xpx;
        if (s1p[spx]==NOTPATTERN || s1p[spx]==PATTERN) {
                bd[1]=0; 
                patrn1(&jpx,&ernbr);
                if (ernbr) return(ernbr); 
                goto exec; 
                }

/***************************
    numeric operand
***************************/

        if (ncode[xd[xpx]]==10) {

                for(i=1; (bd[i]=xd[xpx])&&ncode[bd[i]]==10; (i++,xpx++));
                if (i==1 && bd[1]=='.') return(9);
                bd[i]=0;
                cannon(bp);
                xpx--;
                goto exec;
                }

/***************************
    literal operand
***************************/

        if (xd[xpx]=='"') {
          j=0;
          while (1) {
              while (xd[++xpx]!='"') {
                              if ((bd[++j]=xd[xpx])==0) return(2);
                              }
                      if (xd[xpx+1]!='"') {
                              bd[++j]=0;
                              goto exec;
                              }
                      xpx++;
                      bd[++j]='"';
                      }
           }

/***************************
    variable name
***************************/

        v1d[1]=xd[xpx++]; 
        j=2;
        while (1) {
                if (dcode[xd[xpx]]==10) {
                                v1d[j]=0;
                                xpx--;
                                goto var1;
                                }

                v1d[j++]=xd[xpx];

                if (xd[xpx]=='(') {
                        v1d[--j]=0;
                        xd[xpx--]=CodedOpen;
                        goto start; /* recurse */
                        }
                xpx++;
                }

un_nest: //* copy answer from stack 

        while (pd1[--sdlim]);
        strcpy(v1dp,&pd1[sdlim+1]);
        while(pd1[--sdlim]);
        sdlim++;
        spx-=2;

var1:   if (t0px==1 && spx==1) { 
          spx--; 
          t0px=0; 
          return(0); 
          }

        if (pd1[sdlim]=='$' && t0px==1 && 
          (pd1[sdlim+1]=='P' || pd1[sdlim+1]=='p'))

		if (setname[0]==0) strcpy(setname,v1dp);

        if (v1d[1]=='^') {      //* global var 

                g=RETRIEVE;

                if (pd1[sdlim]=='$' && s1p[spx]!=INDIRECT) {

                        if (toupper(pd1[sdlim+1])=='N' ||
                               toupper(pd1[sdlim+1])=='O' ) 
					g = ORDERNEXT; // $order

                        if (toupper(pd1[sdlim+1]) == 'B' ) 
					g=ORDERPREV; // $back

                        }

                f=global(g,v1dp,bp);

/*******************************
    adjustment for $next() 
*******************************/

        if (toupper(pd1[sdlim+1])=='N') 
        	if (strcmp(bp,"")==0) 
			strcpy (&bd[1], "-1"); //* empty 

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       check for $data                                         +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

                if(pd1[sdlim]=='$' && s1p[spx]!=INDIRECT &&
                   (pd1[sdlim+1]=='D' || pd1[sdlim+1]=='d')) {
                        if (pd1[sdlim+2]=='x') { /* special $d */
                                if (f==0) bd[1]='0';
                                else bd[1]='1';
                                bd[2]=0;
                                goto exec;
                                }
                        if (f==0) bbtyp='0'; else bbtyp='1';
                        i=strlen(v1dp);

//-----------------------
//	sqlite or native?
//-----------------------

#ifdef NATIVE
                        v1d[i + 1] = 2; 
#else
                        v1d[i + 1] = 1; 
#endif
                        v1d[i + 2] = 0;
#ifdef NATIVE
                        f=global(NEXT, v1dp, bd);
#else
                        f=global(ORDERNEXT, v1dp, bd);
#endif
                        bd[2] = bbtyp;
                        if (f == 1) bd[1] = '1'; else bd[1] = '0';

                        if (bd[1]=='0') {
                                bd[1]=bd[2];
                                bd[2]=0;
                                }

                        bd[3]=0;
                        goto exec;
                        }

                tpx=f; 
        goto exec; 
        } //* normal global

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       built-in variables/fcns                                 +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        if (v1d[1]=='$') {

        if (v1d[2]=='d' || v1d[2]=='n'||
            v1d[2]=='o' || v1d[2]=='O'||
            v1d[2]=='b' || v1d[2]=='B'||
            v1d[2]=='D' || v1d[2]=='N') {
            for (i=1; v1d[i]!=CodedOpen; i++); /* open */
            j=0;
            while((bd[++j]=v1d[++i])!=CodedClose); /* close */
            bd[j]=0;
            goto exec; 
            }

        if (v1d[2]=='z'||v1d[2]=='Z') zfcn(); else fcn();
        if (ierr==0) goto exec;
        return(ierr); 
        }

        if (pd1[sdlim]=='$' && s1p[spx]==OPENC &&
           (pd1[sdlim+1]=='N' || pd1[sdlim+1]=='n' ||
            pd1[sdlim+1]=='B' || pd1[sdlim+1]=='b' ||
            pd1[sdlim+1]=='O' || pd1[sdlim+1]=='o' )) {

                symflg=SymNext; 
        sym_(v1dp,bp);
        
        if (pd1[sdlim+1]=='N' || pd1[sdlim+1]=='n' &&
            bd[1]==0 ) strcpy(bp,"-1");

        if (symflg && s1p[spx]!=INDIRECT) goto exec; 

        return(27); 
        }

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       retrieve look-up                                        +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        if (v1d[1]==PREDICATEVAR) {
                strcpy(bp,v1dp); 
                goto exec; 
                }
        symflg=SymRetrieve;
        sym_(v1dp,bd);
        if (pd1[sdlim]!='$') goto sym1;

/*...........................*/
/*    local variable $DATA */
/*...........................*/

        if (s1p[spx]==OPENC && (pd1[sdlim+1]=='d' || pd1[sdlim+1]=='D')) {
                symflg=SymData;
                sym_(v1dp,bp);
                goto exec; 
                }

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       check for variable not found                            +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

sym1:   if (symflg==0) {

          int i=1,j,k=1;

          while (1) { /* look for a label of the same name */
              for (j=i; pd1[j]!=TAB; j++);
              if (strncmp(&pd1[i],v1dp,j-i)==0 && strlen(v1dp)==j-i) {
                  sprintf(bp,"%d",k);
                  goto exec;
                  }
              i=i+strlen(&pd1[i])+1;
              if (i>pd1len) break;
              k++;
              }
          return(17);
          }

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       process value in bb or bd                               +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

exec:   if (s1p[spx]!=EMPTY&&s1p[spx]!=OPEN&&s1p[spx]!=OPENC) goto nxt_expr;
        spx++;
        while(pd1[sdlim++]);
        if (sdlim>=symlen) return(31);
        strcpy(&pd1[sdlim],bp);
        s1p[spx]=OPERAND; 
        goto nchar;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       process expression - check  for operator on stack top   +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

nxt_expr:

        if (s1p[spx]==OPEN||s1p[spx]==OPERAND) return(11);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       check for number under operator                         +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        if (s1p[spx-1]!=OPERAND) { 

        switch (s1p[spx]) {

default:

    return(1);

case INDIRECT:

    strcat(bp,&xd[xpx+1]);
    strcpy(&xd[1],bp); 
    spx--; 
    xpx=0;
    if (kflg&&spx<=1) { 
    xpx=1; 
    return(0); 
    } 

    goto nchar;

case MINUS: //* unary minus 

    strcpy(tmp3,"-1");
    mult(bp,tmp3,bp); break;

case NOT: //* unary not 

    if (numcomp(bp,"0")==0) bd[1]='1'; else bd[1]='0';
    bd[2]=0; 
    break;

case PLUS: //* unary plus 

    strcpy(tmp3,"0");
    add(bp,tmp3,bp);

    } //* switch 

    spx--; 
    goto nxt_operator; 
    }

//#++++ extract value under operator 

        for ( adx=sdlim--; pd1[sdlim-1]; sdlim--);

//#++++ branch depending upon operator 

        switch (s1p[spx]) {

case OPERAND:
case EMPTY:
case NOT:
default:

        return(12);

case DIVIDE:

        if (numcomp(bp,"0")==0) return(15);
        divx(&pd1[adx],bp,bp); break;

case MULTIPLY:

        mult(&pd1[adx],bp,bp); break;

case MINUS:

        sub(&pd1[adx],bp,bp); break;

case PLUS:

        add(&pd1[adx],bp,bp); break;

case CONCAT:

        /* if (number(&pd1[adx])) cannon(&pd1[adx]);
        if (number(bp)) cannon(bp); */
        strcat(&pd1[adx],bp); strcpy(bp,&pd1[adx]);
        break;

case EQUALS:
case NOTEQ:

        /* if (number(&pd1[adx])) cannon(&pd1[adx]);
        if (number(bp)) cannon(bp); */
        if (s1p[spx]!=NOTEQ) {
                if (strcmp(&pd1[adx],bp)==0) bd[1]='1';
                else bd[1]='0'; 
                bd[2]=0; 
                break; 
                }

        if (strcmp(&pd1[adx],bp)) bd[1]='1';
        else bd[1]='0'; 
        bd[2]=0; 
        break;

case GREATER:

        if (numcomp(&pd1[adx],bp)>0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0; 
        break;

case LESSTHAN:

        if (numcomp(&pd1[adx],bp)<0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0;  
        break;

case INTDIVIDE:

        strcpy(tmp3,bp);
        if (numcomp(tmp3,"0")==0) return(15);
        divx(&pd1[adx],bp,bp);
        for (i=1; bd[i]; i++) if (bd[i]=='.') { bd[i]=0; break; }
        if (bd[1]==0) { bd[1]='0'; bd[2]=0; }
        break;

case MODULO:

        if (numcomp(bp,"0")==0) return(15);

	modulo(&pd1[adx], bp);

	break;

case CONTAINS:

        if (pd1[adx]==0&&bp==0) { bd[1]='1'; bd[2]=0; break; }
        if (xindex(&pd1[adx],bp,1)==0) bd[1]='0'; 
        else bd[1]='1';
        bd[2]=0; break;

case FOLLOWS:

        if (strcmp(&pd1[adx],bp)<=0) bd[1]='0'; 
        else bd[1]='1';
        bd[2]=0; 
        break;

case PATTERN:

        patrn(&pd1[adx]);
        if (ierr1!=1&&ierr1) return(16);
        if (ierr1==0) bd[1]='0'; 
        else bd[1]='1'; 
        bd[2]=0;
        break;

case NOTGREATER:

        if (numcomp(&pd1[adx],bp)<=0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0; break;

case NOTLESS:

        if (numcomp(&pd1[adx],bp)>=0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0; break;

case NOTCONTAINS:

        if (xindex(&pd1[adx],bp,1)==0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0; 
        break;

case NOTFOLLOWS:

        if (strcmp(&pd1[adx],bp)<=0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0; 
        break;

case NOTPATTERN:

        patrn(&pd1[adx]);
        if (ierr1!=1&&ierr1) return(16);
        if (ierr1==0) bd[1]='1'; 
        else bd[1]='0';
        bd[2]=0; 
        break;

case AND:

        if (numcomp(bp,"0")&&numcomp(&pd1[adx],"0")) bd[1]='1'; 
        else bd[1]='0'; 
        bd[2]=0; 
        break;

case OR:

        if (numcomp(bp,"0")||numcomp(&pd1[adx],"0")) bd[1]='1'; 
        else bd[1]='0'; 
        bd[2]=0; 
        break;

case NOTAND:

        if (numcomp(bp,"0")&&numcomp(&pd1[adx],"0")) bd[1]='0'; 
        else bd[1]='1'; 
        bd[2]=0; 
        break;

case NOTOR:

        if (numcomp(bp,"0")||numcomp(&pd1[adx],"0")) bd[1]='0'; 
        else bd[1]='1'; 
        bd[2]=0;

        }

dec_stk:

        spx-=2;

nxt_operator:

        if (s1p[spx]!=OPEN&&s1p[spx]!=OPENC&&s1p[spx]!=EMPTY) goto nxt_expr;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       push answer                                             +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        spx++;
        sdlim+=strlen(&pd1[sdlim])+1;
        if (sdlim>=symlen) return(31);
        strcpy(&pd1[sdlim],bp);
        s1p[spx]=OPERAND; 
        goto nchar;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                               +
+       exit sequence                                           +
+                                                               +
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

finish: if (s1p[spx-1]!=EMPTY||s1p[spx]!=OPERAND) return(16);
        else return(0); 
}

