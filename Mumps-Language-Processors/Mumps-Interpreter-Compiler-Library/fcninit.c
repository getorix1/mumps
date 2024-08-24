/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2004, 2008, 2015, 2017,
#+     2022, 2024  by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     okane@uni.edu
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

//	May 15, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

void fcninit (char *output_file, long pid) {

      Sysout ( "//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      Sysout ( "//#+ Intermediate temporary variables used during execution of string expressions\n");
      Sysout ( "//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

      Sysout_skip;
      Sysout ( "char * catx; // intermediate concat ptr\n");

      Sysout ( "char *_ftmp = new char[STR_MAX]; /* intermediate temp */\n");
      linepad2("Temporary variables");
      if (strlen(FunctionName) == 0) fprintf(Out,"INCLUDE Main_.%d\n", pid);
      else fprintf(Out,"INCLUDE %s_.%d\n",FunctionName, pid);

      Sysout ( "unsigned char *_dummy = new unsigned char[STR_MAX]; /* temporary variable */\n");
      fprintf (Out, "unsigned char *atmp =new unsigned char[STR_MAX]; /* temporary variable */\n");
      fprintf (Out, "unsigned char *xgtmp = new unsigned char [STR_MAX]; /* temporary variable */\n");
      fprintf (Out, "unsigned char *gtmpL = new unsigned char[STR_MAX]; /* temporary variable */\n");
      fprintf (Out, "unsigned char *tmp = new unsigned char[STR_MAX]; /* temporary variable */\n");
      fprintf (Out, "unsigned char *gtmp = new unsigned char [2*STR_MAX]; /* temporary variable */\n");

      fprintf (Out,"int %s_label_lookup(char *); /* resolution of labels */\n",FunctionName);
      fprintf (Out,"const char * %s_text_function(int);\n",FunctionName);

      fprintf (Out, "static unsigned char _ReturnVal[STR_MAX]; /* function returns */\n");
      fprintf (Out, "int _ArgCount=0; /* Nbr of args to extrinsic */\n");
      fprintf (Out, "long _i,_jj,_j,_k,_new;\n");
      fprintf (Out, "int _SymPush=0,f=0;\n");
      fprintf (Out, "int g=0;\n");
      Sysout ("jmp_buf _doStack[128]; /* <do> return stack mechanism */\n");
      Sysout ("short   _doStackN[128]={0}; /* <do> return stack mechanism */\n");
      Sysout ("static int     _doStx=0; /* <do> return stack mechanism */\n");
      Sysout ("char _Refs[10][32];\n");
      Sysout ("int _Refsx=0;\n");
      fprintf(Out, "\n");

      Sysout ("if (svPtr == NULL) svPtr = AllocSV();\n\n");

      Sysout ("_SymPush=0; // zap pushed tables \n\n");

      fprintf (Out, "svPtr->in_file[5]=stdin; // console default/\n");
      fprintf (Out, "svPtr->out_file[5]=stdout; // console default\n\n");

      fprintf (Out, "strcpy(svPtr->UDAT,\"data.dat\");\n");
      fprintf (Out, "strcpy(svPtr->UKEY,\"key.dat\");\n\n");

      fprintf (Out, "if ( 0) { // no alarm if slice is zero\n");
      fprintf (Out, "	signal(SIGALRM, SyncClock);\n");
      fprintf (Out, "	set_alarm(1);\n");
      fprintf (Out, "	}\n\n");

      return;
      }

