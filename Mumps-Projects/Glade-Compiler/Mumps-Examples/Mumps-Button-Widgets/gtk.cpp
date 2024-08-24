
// ==============================================================
//#+ This module generated by Mumps Compiler @VERSION@ Aug 24 2022
//#+  
//#+ Portions of this code are part of the Mumps Compiler
//#+ 
//#+ Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 
//#+ 2010, 2017, 2020, 2022 by Kevin C. O'Kane  
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

#undef _MDH_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <mumpsc/inline.h>
#include <mumpsc/defines.h>
#include <mumpsc/strmax.h>
#include <mumpsc/libmpscpp.h>
#include <mumpsc/sym.h>
#include <mumpsc/builtin.h>
#include <mumpsc/compiler.h>
#define FORPARM STR_MAX  /* Size of For loop parameter */
#define CSTR (char *)
#define USTR unsigned char *
#define CCHR const char *
#include <mumpsc/keyfix.h>
#include <mumpsc/globalOrder.h>
#include <mumpsc/stateVector.h>
int (*__label_lookup)(char *);
char * (*__text_function)(int);
void fcn(MSV *);
void zfcn(unsigned char *, unsigned char *, struct MSV *); /* z functions */
char * sym_(int, unsigned char *, unsigned char *, struct MSV *); /* symbol table */
char * makeLock(char *, char *, char *, char, char *); /* global array lock service */
char * cleanLocks(char *); /* global array lock service */
char * releaseLock(char *, char *, char *); /* global array lock service */
char * eraseLocks(char *); /* global array lock service */
void add(const char *a,const char *b,char *c); /* math function */
void sub(const char *a,const char *b,char *c); /* math function */
void mult(const char *a,const char *b,char *c); /* math function */
void divx(const char *a,const char *b,char *c); /* math function */
void divi(const char *a,const char *b,char *c); /* math function */
void _fnumber(unsigned char *,unsigned char *,
       unsigned char *,unsigned char *, MSV *); /* math function */
int DBSem = 0; /* deadlock prevention */
void SyncClock() {
    DBSem = 1;
    }
unsigned int set_alarm (unsigned int seconds) {
    struct itimerval old, xnew;
    xnew.it_interval.tv_usec = 0;
    xnew.it_interval.tv_sec = 0 ; // interval between alarms
    xnew.it_value.tv_usec = 0;
    xnew.it_value.tv_sec = (long int) 0;
    if (setitimer (ITIMER_REAL, &xnew, &old) < 0)
        return 0;
    else
        return old.it_value.tv_sec;
    }
#include <mumpsc/global.h>  /* Global array definitions */
  /* Interface to Mglobal(): */
  /* g = command */
  /* key = key sought or stored */
  /* bd = value returned or stored */
  /* svPtr = address of state vector */
#ifndef SUB
#include <mumpsc/fcns.h>  /* System support functions */
#include <mumpsc/sysfunc.h>  /* Many common global variables */
#endif
 // !/usr/bin/mumps
 //  Jan 9, 2021
 #include "gtk1.h"
//================================================================================= 
//  Zmain 
//================================================================================= 

int main(int argc, char *argv[]) {

//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+ Intermediate temporary variables used during execution of string expressions
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

char *_ftmp = new char[STR_MAX]; /* intermediate temp */
/*** Temporary variables */
jmp_buf for0J;
jmp_buf Efor0J;
int tpx0;
unsigned char *tmp0 = new unsigned char[STR_MAX];
unsigned char *tmp1 = new unsigned char[STR_MAX];
unsigned char *_dummy = new unsigned char[STR_MAX]; /* temporary variable */
unsigned char *atmp =new unsigned char[STR_MAX]; /* temporary variable */
unsigned char *xgtmp = new unsigned char [STR_MAX]; /* temporary variable */
unsigned char *gtmpL = new unsigned char[STR_MAX]; /* temporary variable */
unsigned char *tmp = new unsigned char[STR_MAX]; /* temporary variable */
unsigned char *gtmp = new unsigned char [2*STR_MAX]; /* temporary variable */
int _label_lookup(char *); /* resolution of labels */
char * _text_function(int);
static unsigned char _ReturnVal[STR_MAX]; /* function returns */
int _ArgCount=0; /* Nbr of args to extrinsic */
long _i,_jj,_j,_k,_new;
int _SymPush=0,f=0;
int g=0;
jmp_buf _doStack[128]; /* <do> return stack mechanism */
short   _doStackN[128]={0}; /* <do> return stack mechanism */
static int     _doStx=0; /* <do> return stack mechanism */
char _Refs[10][32];
int _Refsx=0;

if (svPtr == NULL) svPtr = AllocSV();

_SymPush=0; // zap pushed tables 

svPtr->in_file[5]=stdin; // console default/
svPtr->out_file[5]=stdout; // console default

strcpy(svPtr->UDAT,"data.dat");
strcpy(svPtr->UKEY,"key.dat");

if ( 0) { // no alarm if slice is zero
	signal(SIGALRM, SyncClock);
	set_alarm(1);
	}

__label_lookup = _label_lookup;
__text_function = _text_function;

$SymPut("%",argv[0]); 

if (argc > 0) { 
	for (int i = 0; i < argc; i++) { 
		char t[STR_MAX]; 
		sprintf(t,"%%%d", i); 
		$SymPut(t, argv[i]); 
		} 
	} 

try { 
 #include "gtk2.h"
//================================================================================= 
svPtr->LineNumber=11; //  do ^gtk4.h 
//================================================================================= 
	_ArgCount=0;  //section D1000 
	svPtr->Rargsx=0;
	{
	 _ArgCount=0;
	_jj=Interpret("x \"do ^gtk4.h\"",svPtr);
	svPtr->ERROR=_jj;
	if (!svPtr->NOERR && _jj<0) ErrorMessage("Indirection error",svPtr->LineNumber);	} 
  gtk_main();
//================================================================================= 
svPtr->LineNumber=15; //  write "Goodbye!",! 
//================================================================================= 
	if (svPtr->out_file[svPtr->io]==NULL) 
		ErrorMessage("Write to input file", svPtr->LineNumber); // W1001 
	svPtr->hor[svPtr->io] += fprintf(svPtr->out_file[svPtr->io], "%s", 
		"Goodbye!");
	fprintf(svPtr->out_file[svPtr->io], "\n"); 
	svPtr->hor[svPtr->io]=0; svPtr->ver[svPtr->io]++; 
//================================================================================= 
svPtr->LineNumber=16; //  zexit 
//================================================================================= 

//=================================================================================
_epilogue: tmp0[0]=0; tmp1[0]=0;  /*  Function Epilogue */
	Mltoa(getpid(),atmp);
	if (svPtr->LockDone) cleanLocks((char *) atmp);
	Mglobal(CLOSE,tmp0,tmp1,svPtr);  /* Close Globals */

} // end of try block 

catch(...) { 
     if (GlobalExceptionCode == INPUT_ERROR) cout << "Input Error" << endl;
     else if (GlobalExceptionCode == INPUT_LENGTH) cout << "Input Length Error" << endl;
     else if (GlobalExceptionCode == SYMTAB_ERROR) cout << "Symbol Table Error" << endl;
     else if (GlobalExceptionCode == GLOBAL_NOT_FOUND) cout << "Global Not Found Error" << endl;
     else if (GlobalExceptionCode == INTERPRETER_ERROR) cout << "Interprerer Error" << endl;
     else if (GlobalExceptionCode == INVALID_CONVERSION) cout << "Invalid Conversion Error" << endl;
     else if (GlobalExceptionCode == DATA_RANGE_ERROR) cout << "Data Range Error" << endl;
     else if (GlobalExceptionCode == NUMERIC_RANGE) cout << "Numeric Range Error" << endl;
     else if (GlobalExceptionCode == UNSPECIFIED) cout << "Unspecified Error" << endl;
	cout << "Uncaught error in or near line " 
	<< svPtr->LineNumber << endl;
	Mglobal(CLOSE,tmp0,tmp1,svPtr);  /* Close Globals */
	} 

return EXIT_SUCCESS; }  /* Terminate */
//=================================================================================


int _label_lookup(char *label) { return -1; }
char * _text_function(int disp) { return NULL; }

 #include "gtk3.h"
