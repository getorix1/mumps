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

//	June 15, 2024

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>

#include <mumpsc/memsize.h>
#include <mumpsc/compiler-macros.h>
#include <mumpsc/compiler-globals.h>

int   main (int argc, char *argv[]) {

      int return_code;
      symlen = PS;
      TextStart=NULL;
      strcpy(FunctionName,"");
      pid = getpid();

      printf("The Mumps Compiler\n");

      if (argc == 2 && strcmp(argv[1],"-v") == 0) return 99;

      if (argc == 1) {
            printf ("*** \a Name of program to compile must be given\n\n");
            ExitCode = 99;
            return 99;
            }

      else {

            if (argc == 3) {
                  if (strcmp(argv[1],"-SUB") == 0) {
                        SubFlag = 1;
                        }
                  else {
                        printf ("*** Unrecognized option: %s\n\n", argv[1]);
                        ExitCode = 99;
                        return 99;
                        }
                  }

            for (i = strlen(argv[argc-1]); i >= 0 && argv[argc-1][i] != '.'; i--);

            if (i < 0 ) strcpy(tmp2,argv[1]);

            else {
                  argv[argc-1][i]=0;
                  strcpy(tmp2,argv[argc-1]);
                  argv[argc-1][i]='.';
                  }

            strcat(tmp2,".m");

            Out = fopen (tmp2, "w");

            strcpy (output_file, tmp2);

            if ((in = fopen(argv[argc-1], "r")) == NULL) {
                  printf ("*** \a File %s not found.\n\n", argv[argc-1]);
                  ExitCode = 99;
                  return 99;
                  }

            printf ("Translating %s to C++:\n", argv[argc-1]);
            }

      copyright_notice(Out);

//----------------------------------------------------------------
// Check for leading #define symbols to be included first
//----------------------------------------------------------------

            {
            int i;
            char str[512];
            while (1) {
                  i = getc(in);
                  if (i != '+') {
                        ungetc(i, in);
                        break;
                        }
                  if ( fgets(str, 512, in) == NULL ) {
                        printf("Unexpected end of file\n");
                        ExitCode = 99;
                        return abrtx(); // goto abrtx;
                        }
                  fprintf(Out,"%s\n",str);
                  }
            }

//-------------------------------------------------------------
// Write Include, Defines and Global Defs to Output File
//-------------------------------------------------------------

      Sysout ("#undef _MDH_\n");
      Sysout ("#include <stdio.h>\n");
      Sysout ("#include <string.h>\n");
      Sysout ("#include <stdlib.h>\n");
      Sysout ("#include <setjmp.h>\n");
      Sysout ("#include <unistd.h>\n");
      Sysout ("#include <signal.h>\n");
      Sysout ("#include <sys/time.h>\n");
      Sysout ("#include <mumpsc/inline.h>\n");
      Sysout ("#include <mumpsc/defines.h>\n");
      Sysout ("#include <mumpsc/strmax.h>\n");
      Sysout ("#include <mumpsc/libmpscpp.h>\n");
      Sysout ("#include <mumpsc/sym.h>\n");
      Sysout ("#include <mumpsc/builtin.h>\n");
      Sysout ("#include <mumpsc/compiler.h>\n");
      Sysout ("#define FORPARM STR_MAX  /* Size of For loop parameter */\n");
      Sysout ("#define CSTR (char *)\n");
      Sysout ("#define UNS unsigned char *\n");
      Sysout ("#define uns unsigned char *\n");
      Sysout ("#define CCHR const char *\n");
      Sysout ("#include <mumpsc/keyfix.h>\n");
      Sysout ("#include <mumpsc/globalOrder.h>\n");

      if (SubFlag) Sysout ("#define SUB\n");

      Sysout ("#include <mumpsc/stateVector.h>\n");
      Sysout ("int (*__label_lookup)(char *);\n");
      Sysout ("const char * (*__text_function)(int);\n");

      Sysout ("void fcn(MSV *);\n");
      fprintf (Out, "void zfcn(unsigned char *, unsigned char *, struct MSV *); /* z functions */\n");

      fprintf (Out, "char * sym_(int, unsigned char *, unsigned char *, struct MSV *); /* symbol table */\n");

      fprintf (Out, "char * makeLock(char *, char *, char *, char, char *); /* global array lock service */\n");
      fprintf (Out, "char * cleanLocks(char *); /* global array lock service */\n");
      fprintf (Out, "char * releaseLock(char *, char *, char *); /* global array lock service */\n");
      fprintf (Out, "char * eraseLocks(char *); /* global array lock service */\n");
      fprintf (Out, "void add(const char *a,const char *b,char *c); /* math function */\n");
      fprintf (Out, "void sub(const char *a,const char *b,char *c); /* math function */\n");
      fprintf (Out, "void mult(const char *a,const char *b,char *c); /* math function */\n");
      fprintf (Out, "void divx(const char *a,const char *b,char *c); /* math function */\n");
      fprintf (Out, "void divi(const char *a,const char *b,char *c); /* math function */\n");
      fprintf (Out, "void _fnumber(unsigned char *,unsigned char *,\n");
      fprintf (Out, "       unsigned char *,unsigned char *, MSV *); /* math function */\n");
      fprintf (Out, "int DBSem = 0; /* deadlock prevention */\n");


//-----------------
//	functions
//-----------------

      Sysout ("\nstatic inline int floatstring(const char * p) { \n"
              " int f=0;  \n"
              " for (; *p!=0; p++) {  \n"
              "	 if (    isdigit(*p) || \n"
              "	 	*p == '-' ||  \n"
              "		*p == '+' ) continue;  \n"
              " 	if (    *p=='.' ||  \n"
              "		tolower(*p)=='e' ) { \n"
              "		f=1;  \n"
              "		continue;  \n"
              "		} \n"
              "	return 0; \n"
              "	}  \n"
              " return f;  \n"
              " } \n ");


      Sysout ("\ninline int numcomp2(unsigned char *aa) { \n"
              " if (floatstring((char *) aa)) return (atof( (char *) aa) != 0.0);  \n"
              " return atoi((char *) aa);  \n"
              " }\n");

      Sysout ("\ninline char * strxcat(char * a, char *b)  \n"
              "{ while (*a) a++; while( (*(a) = *(b)) ) { a++; b++; } return a; }\n");

      Sysout ("\ninline char * strcatz(char * out, char *in)  \n"
              "{ while (*out) out++; while (*(out) = *(in)) { out++; in++; } *(out++) = '\x01';  \n"
              "*out = 0; return (char *) out; } \n\n");

      fprintf (Out, "void SyncClock() {\n");
      fprintf (Out, "    DBSem = 1;\n");
      fprintf (Out, "    }\n\n");

      fprintf (Out, "unsigned int set_alarm (unsigned int seconds) {\n");
      fprintf (Out, "    struct itimerval old, xnew;\n");
      fprintf (Out, "    xnew.it_interval.tv_usec = 0;\n");
      fprintf (Out, "    xnew.it_interval.tv_sec = 0 ; // interval between alarms\n");
      fprintf (Out, "    xnew.it_value.tv_usec = 0;\n");
      fprintf (Out, "    xnew.it_value.tv_sec = (long int) 0;\n");
      fprintf (Out, "    if (setitimer (ITIMER_REAL, &xnew, &old) < 0)\n");
      fprintf (Out, "        return 0;\n");
      fprintf (Out, "    else\n");
      fprintf (Out, "        return old.it_value.tv_sec;\n");
      fprintf (Out, "    }\n\n");

      linepad (Sysout ("#include <mumpsc/global.h>"),
               "Global array definitions");

      linepad (0, "Interface to Mglobal():");
      linepad (0, "g = command");
      linepad (0, "key = key sought or stored");
      linepad (0, "bd = value returned or stored");
      linepad (0, "svPtr = address of state vector");

      Sysout ("#ifndef SUB\n");
      linepad (fprintf (Out, "#include <mumpsc/fcns.h>"),
               "System support functions");
      linepad (fprintf (Out, "#include <mumpsc/sysfunc.h>"),
               "Many common global variables");
      Sysout ("#endif\n");

//---------------------
// Compiler Start
//---------------------

      pd1cur      = 1;
      symlen      = PS;
      t0px        = 0;
      t2          = 0;
      cmdx        = 0;
      setpiece    = 0;
      forxmax     = 0;
      forx        = 0;
      forxFLG     = 0;
      forLine     = 0;
      pd1Last     = 0;
      ExecLevel   = 0;
      SymIncrFlg  = 0;
      ArgMax      = 0;
      IF_flg_max  = 0;
      cvarx       = 0;

      for (i=0; i<FBMAX; i++) ForBlocks[i] = -1;


//#+*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+*++++++++++ Next Line ++++++++++++++++++++++++++++++++++++++++++++++
//#+*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

next_line:

      EmitLabel = 0;
      iiitmp=0; /* new temp pool for each line */
      forLine = 0;
      CatchFlg=0;

      strcpy(&pd1[1],""); // array content begins at 1, not 0

      if ( fgets(&pd1[1],512,in) == NULL ) { // input line
            pd1cur = 9999;
            pd1len = 1;
            strcpy(&pd1[1],"");
            }

      else {
            pd1cur = 1; // start
            pd1len = 2048;
            }

      if ( pd1[strlen(&pd1[1])] < 32 ) pd1[strlen(&pd1[1])] = 0;  // <cr>
      if ( pd1[strlen(&pd1[1])] < 32 ) pd1[strlen(&pd1[1])] = 0;  // <lf>

      for (i = 1; pd1[i] != 0 && i < 512; i++)
            if ( pd1[i] == '	' ) pd1[i] = ' '; // convert tabs to blanks

      lines++;

      while ( IF_flg ) { // closing braces
            fprintf ( Out, "	}\n" );
            IF_flg--;
            }

      if ( pd1cur > pd1len ) {
            if ( ExecLevel > 0 ) {
                  for (; ExecLevel > 0; ExecLevel--) {
                        fprintf (Out, "	} while (0);   // dotted un-indent %d\n", ExecX - 1);
                        fprintf (Out, "	longjmp(Efor%dJ,99);    // return \n", ExecStack[--ExecX]);

                        fprintf (Out, "\nDo_Block_Bypass%d: ; //A541 \n",
                                 ExecStack[ExecX]);
                        }

                  ExecLevel = 0;
                  }

            goto abrt_exit;
            }

      if (pd1[pd1cur] == 0) {
            pd1cur++;
            goto next_line;
            }

      strcpy (cur_line, &pd1[pd1cur]); // copy next program line

//---------------------------------
//	inline C code  - emit as is
//---------------------------------

      if (pd1[pd1cur] == '%' || pd1[pd1cur] == '+') {

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Inline\n");
	    fprintf(Out, "//--------------\n\n");

            fprintf (Out, "%s\n", &pd1[pd1cur + 1]);
            while (pd1[pd1cur]) pd1cur++;
            pd1cur++;
            goto next_line;

            }

      if (pd1[pd1cur] == '#' || (pd1[pd1cur] == '/' && pd1[pd1cur+1] == '/' ) ) { //* comment
            if ( pd1[pd1cur] == '/' ) pd1cur++;
            fprintf (Out, "\n// %s\n", &pd1[pd1cur + 1]);
            pd1cur++;
            while (pd1[pd1cur]) pd1cur++;
            pd1cur++;

            if (!MAIN_flg && 0) {

                  Sysout ( "\nint main(int argc, char *argv[]) {\n\n");

                  cvarx=0;
                  fcninit(output_file, pid);

                  fprintf(Out,"__label_lookup=_label_lookup; \n");
                  fprintf(Out,"__text_function=_text_function; \n\n");

                  Sysout("#include <mumpsc/cgi.h>\n");

                  Sysout("	$SymPut(\"%%\", argv[0]); // program name\n");
                  Sysout("	if (argc > 0) { // any parameters? \n");
                  Sysout("		for (int i = 0; i < argc; i++) { \n");
                  Sysout("			char t[STR_MAX]; \n");
                  Sysout("			sprintf(t, \"%%%%%%d\", i); \n");
                  Sysout("			$SymPut(t, argv[i]); \n");
                  Sysout("			} \n");
                  Sysout("		} \n");

                  fprintf(Out, "\n	try { \n");

                  Sysout_skip;
                  MAIN_flg = 1;
                  }
            goto next_line;
            }

      i = pd1cur;
      while (pd1[pd1cur] && pd1[pd1cur] != TAB) pd1cur++;

      if (pd1[pd1cur] !=0 )
            while (pd1[pd1cur] != 0 && pd1[pd1cur] == TAB) pd1cur++;

//**************
// multiple TABs
//**************

      pd1cur = i;

      if (pd1[pd1cur] != TAB) {
            for (; pd1[pd1cur] != TAB && pd1[pd1cur] != 0; pd1cur++)
                  if (pd1[pd1cur]=='(')  while (pd1[pd1cur]!=')' && pd1[pd1cur]!=0) pd1cur++;

            pd1[pd1cur] = 0;


                  {
                  int flg = 1;
                  int ii, j, k, kk = 0, n = 0;
                  int thistime=0;

                  if (pd1[i] == '^' && pd1[i+1] == '^') {    //* function start
                        i = i+2;
                        F_flg = 1;
                        if (MAIN_flg) EP(0,ExecName, pid);  //* entry point router
                        if (FCN_flg)  EP(1,ExecName, pid);  //* entry point router
                        FCN_flg = 1;          //* am in subfunction
                        thistime=1;
                        }

                  else if (pd1[i] == '^' && pd1[i+1] != '^') {    //* function start
                        i = i+1;
                        F_flg = 1;
                        if (MAIN_flg) EP(0,ExecName, pid);  //* entry point router
                        if (FCN_flg)  EP(1,ExecName, pid);  //* entry point router
                        FCN_flg = 1;          //* am in subfunction
                        thistime=1;
                        }

                  ii = i;
                  for (; pd1[i] != 0; i++) {

//--------------------------------------------------
//	open paren in label - must be function ep
//--------------------------------------------------

                        if (pd1[i] == '(') {

                              if (flg) {

                                    Sysout  ("\n//========================================="
                                             "========================================\n");

                                    fprintf (Out, "//	Entry Point %s + + + + + + + \n", &pd1[ii]);

                                    Sysout  ("\n//========================================="
                                             "========================================\n");

                                    flg = 0;            //* arguments found
                                    }

                              pd1[i] = 0;

                              if (!F_flg && !MAIN_flg) goto Missing_Zmain;

                              if (F_flg && thistime) {            //* this is a C subfunction
                                    int ff = 0;
                                    cvarx = 0;
                                    strcpy(FunctionName, &pd1[ii]);

                                    fprintf (Out, "char * %s(struct MSV * svPtr, char * offset", &pd1[ii]);

                                    if (pd1[i+1] != ')') fprintf (Out, ",");

                                    for (j = i + 1; pd1[j] != 0; j++) {       /* separately compiled extrinsic */

                                          if (pd1[j]=='+' ) { /* non mumpsc parameter */
                                                j++;
                                                while (1) {
                                                      fprintf (Out,"%c",pd1[j]);
                                                      j++;
                                                      if (pd1[j]==',' || pd1[j]==')') break;
                                                      }

                                                if (pd1[j]==')') {
                                                      ff=0;
                                                      goto fe1;
                                                      }

                                                fprintf (Out,",");
                                                continue;
                                                }

                                          if (isalpha(pd1[j])) ff=1;

                                          if ( (pd1[j] == ',' || pd1[j] == ')') && ff ) {
                                                fprintf (Out, "const char * _Arg%d%c", kk, pd1[j]);
                                                kk++;
                                                }
fe1:
                                          if (pd1[j] == ')') {
                                                if (!ff) fprintf(Out,") ");
                                                fprintf (Out, "{\n\n");
                                                fcninit (output_file, pid);
                                                break;
                                                }
                                          }

                                    /* ArgMax=kk+1; */

                                    fprintf (Out, "__label_lookup=%s_label_lookup; \n", FunctionName);
                                    fprintf (Out, "__text_function=%s_text_function; \n\n", FunctionName);

                                    fprintf (Out, "try { // try001 \n ");

                                    Sysout ("svPtr->_SymPush++; \n");
                                    Sysout ("_SymPush++; \n");
                                    Sysout ("svPtr->start[svPtr->_Sym]=NULL; \n");
                                    SymIncrFlg = 1;
                                    fprintf (Out, "if (svPtr->_Sym > SYM_MAX) ");
                                    fprintf (Out, "ErrorMessage(\"Too many function calls\",svPtr->LineNumber); \n");
                                    fprintf (Out, "_doStackN[_doStx-1] = 1; // flag to restore namespace\n");
                                    Sysout ("	if (svPtr->Rargsx) { // EP1005\n");
                                    Sysout ("		for (_i = 0; _i < svPtr->Rargsx; _i++) "
                                            "strcpy(_Refs[_i], svPtr->Rargs[_i]); \n");
                                    Sysout ("		_Refsx = svPtr->Rargsx; \n");
                                    Sysout ("		} \n");
                                    }

                              else {
                                    if (!isalnum(pd1[ii])) goto lab_err;
                                    if (isdigit(pd1[ii])) fprintf(Out, "_" );
                                    fprintf (Out, "%s: \n", &pd1[ii]); // source code Label
                                    Sysout ("svPtr->start[svPtr->_Sym] = NULL; \n");
                                    SymIncrFlg = 1;
                                    Sysout ("_SymPush++; \n");
                                    fprintf (Out, "if (svPtr->_Sym > SYM_MAX) ");
                                    fprintf (Out, "ErrorMessage(\"Too many function calls\",svPtr->LineNumber); \n");
                                    fprintf (Out, "_doStackN[_doStx-1]=1; // flag to restore namespace \n");
                                    }

                              pd1[i] = '(';
                              for (j = i + 1; pd1[j] != 0; j++) {

                                    if (pd1[j] == '+') {
                                          while(1) {
                                                if (pd1[j] == ',' || pd1[j] == ')') break;
                                                j++;
                                                }

                                          if (pd1[j] == ')') {
                                                j++;
                                                break;
                                                }

                                          i=j;
                                          continue;
                                          }
                                    if (pd1[j] == ',' || pd1[j] == ')') {
                                          k = pd1[j];
                                          pd1[j] = 0;
                                          if (strlen(&pd1[i+1]) == 0) break;

                                          if (F_flg) {

                                                fprintf (Out, /* C function */
                                                         "	if (_Arg%d!=NULL) \n		"
                                                         "sym_(100, (UNS) \"%s\", (UNS) _Arg%d, svPtr);"
                                                         " // Store Arguments EP1001\n",
                                                         n, &pd1[i + 1], n);

                                                fprintf (Out,
                                                         "	strcpy (svPtr->RefParm[%d], \"%s\" ); "
                                                         "// EP1002\n", n, &pd1[i + 1]);
                                                }

                                          else {

                                                fprintf (Out,
                                                         "	if (__Arg%d!=NULL) sym_(100, (UNS) \"%s\", (UNS) "
                                                         "__Arg%d,svPtr); // Store Arguments EP1003 \n",
                                                         n, &pd1[i + 1], n);

                                                fprintf (Out,
                                                         "	strcpy (svPtr->RefParm[%d], \"%s\" ); // EP1004 \n",
                                                         n, &pd1[i + 1]);
                                                }

                                          n++;
                                          if (n > ArgMax) ArgMax = n;
                                          pd1[j] = k;
                                          if (k == ')') break;
                                          i = j;
                                          }
                                    }
                              if (F_flg && thistime) {
                                    fprintf(Out,"	goto _eps;  // entry point label router \n");
                                    fprintf(Out,"_fcn: \n");
                                    }
                              }
                        }

                  if (!flg && !F_flg)
                        fprintf (Out,
                                 "	if (_ArgCount < %d) ErrorMessage(\"Bad argument count\", "
                                 "svPtr->LineNumber); \n", n);

                  if (flg) {
                        int i;
                        for (i = ii; pd1[i] != 0; i++)

                              if (pd1[i] != ' ') { // label

                                    EmitLabel = 1;

                                    if (!isalnum(pd1[ii])) goto lab_err;

                                    if (LabelsStart == NULL) { // linked list setup
                                          LabelsStart = malloc(sizeof(struct txt));
                                          if (LabelsStart == NULL) goto mem_err;
                                          LabelsStart->next = NULL;
                                          LabelsStart->lnbr = lines;
                                          LabelsStart->name = malloc(strlen(&pd1[ii]) + 1);
                                          if (LabelsStart->name == NULL) goto mem_err;
                                          if (isdigit(pd1[ii])) strcpy(LabelsStart->name, "_");
                                          else strcpy(LabelsStart->name, "");
                                          strcat(LabelsStart->name, &pd1[ii]);
                                          }

                                    else { // existing linked list
                                          p1 = malloc(sizeof(struct txt));
                                          if (p1 == NULL) goto mem_err;
                                          p1->next = LabelsStart;
                                          LabelsStart = p1;
                                          p1->lnbr = lines;
                                          p1->name = malloc(strlen(&pd1[ii]) + 1);
                                          if (p1->name == NULL) goto mem_err;
                                          if (isdigit(pd1[ii])) strcpy(LabelsStart->name, "_");
                                          else strcpy(LabelsStart->name, "");
                                          strcat(LabelsStart->name, &pd1[ii]);
                                          }

                                    break;
                                    }
                        }

                  pd1[pd1cur] = TAB; /* replace */
                  }
            }

      while (pd1[pd1cur] == TAB) pd1cur++;

//------------------------------------------------------------------------
//  Doty Indents
//------------------------------------------------------------------------

      if (pd1[pd1cur] == '.') {

            j = 0;

            for (i = pd1cur; pd1[i] == '.'||pd1[i]==' '; i++)
                  if (pd1[i]=='.') j++;  // how many levels?

            if (j>10) {
                  printf("*** Too many block indents\n");
                  ernbr=99;
                  goto abrt1;
                  }

            if (j > ExecLevel + 1) {  // skipped a level
                  printf ("*** Code level skip\n");
                  ernbr = 99;
                  goto abrt1;
                  }

//-------------------------------------
//	ExecLevel set to j in function
//-------------------------------------

            if (ernbr = doty(j, &ExecLevel, &ExecName, DoRet, DoRetX, &pd1Last,
                             pd1cur, ExecStack, &ExecX, ExecSym))
                  goto abrt1;

            pd1cur = i;

            }

      else if (ExecLevel > 0) {
            for (; ExecLevel > 0; ExecLevel--) {

                  fprintf (Out, "	} while(0); // dotted un-indent %d \n", ExecX - 1);

                  fprintf (Out, "	longjmp(Efor%dJ,99); // return \n", ExecStack[--ExecX]);

                  fprintf (Out, "\nDo_Block_Bypass%d: ; //B848 \n\n", ExecStack[ExecX]);
                  }

            ExecLevel = 0;
            }

      if (ExecLevel == 0) forxFLG = 0;

//------------------------------------------------------------------------

      xpx = j = 1;

//************************
// copy source line to xd
//************************

      pd1Last = pd1cur;

      while ((xd[j++] = pd1[pd1cur++]) != 0);

      Sysout ("\n//============================================="
              "==================================== \n");

      if ((MAIN_flg || FCN_flg) && ! TryFlg)
            fprintf(Out, "svPtr->LineNumber=%d; // %s \n", lines, cur_line);

      else fprintf(Out,"// %s \n", cur_line);

      Sysout ("//============================================="
              "==================================== \n");

//------------------------
//	write the label
//------------------------

      if (EmitLabel) {
            if (ExecLevel > 0)  goto label_in_block;
            fprintf(Out,"\n%s: \n",LabelsStart->name);
            EmitLabel = 0;
            }

      xd[j] = 0;

next_cmnd:  // continue line entry point

      if (IF_flg > IF_flg_max) IF_flg_max = IF_flg;

      while (1) {

            if (xpx >= 1024 || xd[xpx] == 0 ) { // the order of these tests is important

                  if (IF_flg) {

                        for (i = IF_flg; i > 0; i--) {

                              fprintf (Out, "	} ");

                              if (ForBlocks[i] >= 0) {

                                                fprintf (Out, "while (0); longjmp(for%dJ, _i); "
                                                                 "// FOR loop %d close \n",
                                                                 ForBlocks[i], i);

                                    if (Blocks[i] == FOR) fprintf (Out,"\nFor%dExit: ; // exit for loop \n\n",
                                                                         ForBlocks[i]);

                                    ForBlocks[i] = -1;
                                    }

                              else fprintf(Out,"\n");
                              }

                        IF_flg = 0;
                        }

                  if (TryFlg) {
                        fprintf(Out,"	} \n");
                        TryFlg=1;
                        }

                  if (CatchFlg) {
                        fprintf(Out,"	} \n");
                        CatchFlg=0;
                        }

                  goto next_line;
                  }

            if (xd[xpx] != ' ' && xd[xpx] != '\t')
                  break;

            xpx++;
            }

//#++++++++++++++++++++++++
//#++++ decode command word
//#++++++++++++++++++++++++

      p2px = xpx;
      t2 = t0px = 0;
      JOB_flg = 0;
      POST_flg = 0;

      if (strncmp(&xd[xpx], "//",2) == 0) {
            xpx=9999;
            goto next_cmnd; // c++ style comment found
            }

      icmnd = tolower (xd[xpx]);

            {

            /*........................ check command word ...............................*/

            char word[25];
            static char *ctable[] =  {    "r", "read",
                                          "w", "write","while",
                                          "i", "if",
                                          "o", "open",
                                          "s", "set", "sql", "sql", "shell",
                                          "j", "job",
                                          "g", "goto",
                                          "m", "merge",
                                          "n", "new",
                                          "q", "quit",
                                          "x", "xecute",
                                          "f", "for",
                                          "h", "halt", "hang", "html",
                                          "e", "else", "export",
                                          "d", "do", "declare", "database",
                                          "k", "kill",
                                          "c", "close", "continue", "catch",
                                          "u", "use",
                                          "l", "lock",
                                          "t", "try",
                                          ";",
                                          "zascii", "zmain", "znumber", "zd", "zexit", "zfunction",
                                          "b", "break",
                                          ""
                                     };

            int wx = 1, flg = 1;
            MergeFlg = 0;
            word[0] = icmnd;

            while (isalpha (xd[++xpx])) word[wx++] = tolower(xd[xpx]);  // scan thru rest of word

            word[wx]='\0';

            for (i = 0; strlen(ctable[i]) != 0; i++)
                  if (strcmp(word,ctable[i]) == 0) {
                        flg = 0;
                        break;
                        }

            if (icmnd == ';' ) goto pstrtn;

            if (flg) {
                  printf ("*** Warning, possible command spelling error: %s ",word);
                  printf ("in or near line %d\n", lines);
                  printf ("*** %s\n", &pd1[pd1Last]);
                  }

            /*........................ check command word ...............................*/
            }


      if (xd[xpx] == ':') { // post conditionals
            xpx++;

            at_flg=atTest(xd, pd1, &pd1Last, lines, xpx);

            if (at_flg==99) goto abrt_exit;

            if (at_flg) {

                  char tmp[1024];
                  int i, j;

                  strcpy (tmp, "i ");
                  atCopy (at_flg, tmp);

                  INTERPRET

                  xpx = at_flg;
                  fprintf(Out, "	if (svPtr->tpx) { // m853 postconditional \n");
                  POST_flg = 1;
                  }

            else {
                  if ((ierr = parse_ ())) goto parse_err;
                  fprintf(Out, "	if (atoi( CSTR %s)) { // m859 postconditional\n", 
			Extract2(&pd1[sdlim]));
                  POST_flg = 1;
                  }
            }


//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+
//#+ branch to correct command handler
//#+
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

pstrtn:

      if (xd[xpx]) xpx++;

//    if (!MAIN_flg && !( icmnd == ZCMDS && tolower(xd[p2px+1]) == 'm') ) { // zmain
      if (0) {

            Sysout ( "\nint main(int argc, char *argv[]) { // begin main routine\n\n");

            cvarx=0;
            fcninit(output_file, pid);

            fprintf(Out,"__label_lookup=_label_lookup;\n");
            fprintf(Out,"__text_function=_text_function;\n");

            fprintf(Out,"\n	try { \n");

            Sysout_skip;
            MAIN_flg = 1;

            Sysout("#include <mumpsc/cgi.h>\n");

            Sysout("	strncpy((char *)svPtr->program, argv[0], 512); \n");
            Sysout("	$SymPut(\"%0\",argv[0]); \n");

            Sysout("if (argc>1) { \n");
            Sysout("	char t[STR_MAX]; \n");
            Sysout("	for (int i=1; i<argc; i++) { \n");
            Sysout("		sprintf(t,\"%%%%%%d\",i); \n");
            Sysout("		$SymPut(t,argv[i]); \n");
            Sysout("		} \n");
            Sysout("	} \n");

            }
      switch (icmnd) {

            case CLSQ:

                  Sysout ( "	} /* user close */\n");
                  goto next_cmnd;

            case OPSQ:

                  goto next_cmnd;

            case TRY:

                  fprintf (Out, "try { ");
                  TryFlg=1;
                  if (xd[xpx]==' '||xd[xpx]==0) goto next_cmnd;
                  for (i=xpx; xd[i]!=' '&&xd[i]!=0; i++);
                  j=xd[i];
                  xd[i]=0;
                  if (strcmp(&xd[xpx],"NoMessages")==0) {
                        fprintf(Out,"svPtr->NOERR=1; ");
                        }
                  else goto try_unknown;
                  xd[i]=j;
                  xpx=i;
                  goto next_cmnd;

ctch:
                  fprintf(Out,"catch(");
                  CatchFlg=1;
                  TryFlg=0;
                  if (xd[xpx]==' '||xd[xpx]==0) goto catch_error;
                  for (i=xpx; xd[i]!=' '&&xd[i]!=0; i++);
                  j=xd[i];
                  xd[i]=0;
                  flg=1;
                  for (k=0; strlen(except[k])!=0; k++)
                        if (strcmp(&xd[xpx],except[k])==0) {
                              flg=0;
                              break;
                              }
                  if (flg) goto catch_unknown;
                  fprintf(Out,"%s ) {\n",&xd[xpx]);
                  xd[i]=j;
                  xpx=i;
                  goto next_cmnd;

            case CMMNT:

                  p2px++;
                  if (xd[p2px]==';') {
                        if (TextStart==NULL) {
                              TextStart=malloc(sizeof(struct txt));
                              if (TextStart==NULL) goto mem_err;
                              TextStart->name=malloc(strlen(&xd[p2px])+2);
                              if (TextStart->name==NULL) goto mem_err;
                              strcpy(TextStart->name,&xd[p2px+1]);
                              TextStart->lnbr=lines;
                              TextStart->next=NULL;
                              goto next_line;
                              }
                        p1=malloc(sizeof(struct txt));
                        if (p1==NULL) goto mem_err;
                        i=strlen(&xd[p2px])+2;
                        p1->name=malloc(i);
                        if (p1->name==NULL) goto mem_err;
                        strcpy(p1->name,&xd[p2px+1]);
                        p1->next=TextStart;
                        p1->lnbr=lines;
                        TextStart=p1;
                        goto next_line;
                        }

                  goto next_line;


            case FCN:

                  printf ("*** Not supported\n");
                  printf ("*** %s\n", cur_line);
                  ExitCode=99;
                  return abrtx(); // goto abrtx;

            case NEW:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       New\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = new_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error

            case BREAK:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Break\n");
	    fprintf(Out, "//--------------\n\n");

                  if (forLine) goto nobrk_err;

                  fprintf (Out, "     svPtr->tpx=tpx%d; // break code: restore $test\n"
			   "  longjmp(Efor%dJ,100); \n",
                           ExecStack[ExecX-1], ExecStack[ExecX - 1], forx);

                  if (POST_flg) {
                        fprintf (Out, " } /* post conditional after break */\n");
                        POST_flg = 0;
                        }

                  goto next_cmnd;


            case LOCK: /* lock group */

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Lock\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = lock_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx();
                  if (return_code == BAD_LOCK) goto bad_lock;
                  goto command; // command error

            case READ:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Read\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = read_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error

            case OPEN:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Open\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = mumps_open();
                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  goto command; // default

            case ZCMDS:

                  return_code = zcommands_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  if (return_code == NEXT_LINE) goto next_line;
                  goto command; // command error


case IF:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       If\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = if_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error

            case WRITE:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Write\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = write_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error

            case JOB:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Job\n");
	    fprintf(Out, "//--------------\n\n");

                  linepad (fprintf (Out, "	if (fork()!=0) {"), "I am child process if zero");
                  JOB_flg = 1;
                  goto do_entry;

job_return:

                  fprintf (Out, "	}");
                  goto next_line;

            case MERGE:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Merge\n");
	    fprintf(Out, "//--------------\n\n");

                  MergeFlg=1;

            case SET: // also: sql shell

                  if (toupper(xd[p2px+1])=='H') {  /* SHELL command */

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Shell\n");
	fprintf(Out, "//--------------\n\n");

                        char tmp[4096];
                        strcpy((char *)tmp,&xd[p2px]);
                        INTERPRET(tmp);
                        goto next_line;
                        }

                  if (toupper(xd[p2px+1])=='Q') { //* SQL
                        char tmp[4096];
                        strcpy((char *)tmp,&xd[p2px]);
                        INTERPRET(tmp);
                        goto next_line;
                        }



	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Set\n");
	fprintf(Out, "//--------------\n\n");

                  retz = SET;

                  return_code = set_code(SET); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error


cntnue:

case QUIT:

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Quit\n");
	fprintf(Out, "//--------------\n\n");


                  if (POST_flg) {
			if (ForBlocks[IF_flg] <= 0) fprintf (Out, "	break; } // post conditional quit m2386 \n");
			else fprintf(Out, "	longjmp(for%dJ,100); /* m1146 quit*/ } // m1146 quit\n\n",
				ForBlocks[IF_flg]);
                        POST_flg = 0;
                        goto next_cmnd;
                        }

                  if (ExecLevel == 0 && forxFLG == 0) {

                        Sysout ("	if (svPtr->Rargsx) { // call-by-reference? \n");
                        Sysout ("		int i; \n");
                        Sysout ("		unsigned char *tmp = new unsigned char[STR_MAX]; \n");
                        Sysout ("		for (i = 0; i < svPtr->Rargsx; i++) { \n");
                        Sysout ("		sym_(1, (UNS) svPtr->RefParm[i], (UNS) tmp,svPtr); \n");
                        Sysout ("		sym_(SYMSTORE, (UNS) svPtr->Rargs[i], (UNS) tmp, svPtr); \n");
                        Sysout ("		} \n");
                        Sysout ("		} \n");

                        fprintf (Out, "	strmove((UNS) _ReturnVal, (UNS) \"\"); \n");

                        if (xd[xpx] != ' ' && xd[xpx] != 0) {

                              t2 = 1;
                              if ((ierr = parse_ ())) goto parse_err;

                              fprintf (Out, "	strmove((UNS) _ReturnVal, (UNS) %s); \n",
                                       Extract2(&pd1[sdlim]));
                              }

                        if (F_flg) {


                              fprintf (Out, "	return (char *) _ReturnVal; \n");
                              }

                        else {

                              fprintf (Out, "	if (_doStackN[_doStx]) {  _doStackN[_doStx] = 0; } // Q1001 \n");

                              fprintf (Out, "	if (_doStx > 0) { longjmp(_doStack[_doStx],99); } \n");
                              fprintf (Out, "	else ErrorMessage(\"Too Many Quits\", svPtr->LineNumber); \n");
                              }
                        }

                  else {

                        if (IF_flg == 0 || ForBlocks[IF_flg] < 0) fprintf (Out, "	break; \n");
                        else  fprintf (Out, "	goto For%dExit; \n", ForBlocks[IF_flg]);

                        if (Blocks[IF_flg]==IF) {
                              Blocks[IF_flg] = 0;
                              IF_flg--;
                              fprintf (Out,"	} // if block 2389\n");
                              }

                        }

                  goto next_cmnd;

case XECUTE:

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Xecute\n");
	fprintf(Out, "//--------------\n\n");

                  {
                  char tmp[1024];
                  int i, j;

                  strcpy (tmp, "x ");
                  j=strlen(tmp);
                  for (i=xpx; xd[i]!=0; i++) {
                        if (xd[i]!='\"') {
                              tmp[j++]=xd[i];
                              }
                        else {
                              tmp[j++]='\\';
                              tmp[j++]=xd[i];
                              }
                        }

                  tmp[j]=0;
                  INTERPRET

                  if (POST_flg) {
                        fprintf (Out, "	} // post conditional xecute\n");
                        POST_flg = 0;
                        }
                  }
            xpx=1024;
            goto next_cmnd;

case FOR:

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       For\n");
	fprintf(Out, "//--------------\n\n");

                  return_code = for_code();

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error

case HALT: // and HANG


                  if (toupper(xd[p2px + 1]) != 'T' && xd[xpx] == ' ' ||
                              xd[xpx] == 0) { // halt

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Halt\n");
	fprintf(Out, "//--------------\n\n");

                        if (MAIN_flg) fprintf (Out, "	goto _epilogue; \n");

                        if (FCN_flg) fprintf (Out, "	return \"\"; \n");

                        if (POST_flg) {
                              fprintf (Out, "	} /* post conditional halt*/\n");
                              POST_flg = 0;
                              }

                        goto next_cmnd;
                        }

                  if (toupper(xd[p2px+1]) == 'T') {  /* HTML command */

                        fprintf (Out, " /* HTML */ printf(\"");

                        while (xd[xpx]) {

                              if (xd[xpx] == '&' && xd[xpx + 1] == '~') {
                                    xpx += 2;
                                    fprintf (Out, "\");\n");
                                    t2 = 1;

                                    for (j=xpx; xd[j]!='~'&&xd[j]!=0; j++);

                                    xd[j]=' ';

                                    if ((ierr = parse_ ())) goto parse_err;
                                    fprintf (Out, "	printf(\"%%s\", %s); \n", Extract2(&pd1[sdlim]));
                                    fprintf (Out, "	printf(\"");
                                    xpx++;
                                    continue;
                                    }

                              if (xd[xpx] == '&' && xd[xpx + 1] == '!') {
                                    xpx += 2;
                                    fprintf (Out, "\\n");
                                    continue;
                                    }

                              if (xd[xpx] == '\"') fprintf (Out, "\\\"");
                              else if (xd[xpx] == '\'') fprintf (Out, "\\\'");
                              else if (xd[xpx] == '%') fprintf (Out, "%%%%");
                              else if (xd[xpx] == '\\') fprintf (Out, "\\\\");
                              else fprintf (Out, "%c", xd[xpx]);
                              xpx++;
                              }

                        fprintf (Out, "\");\n");
                        goto next_line;
                        }

                  while (1) { /* Hang */

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Hang\n");
	fprintf(Out, "//--------------\n\n");
                        t2 = 1;
                        if ((ierr = parse_ ())) goto parse_err;
                        fprintf (Out, "	_i = atoi( CSTR %s);\n", Extract2(&pd1[sdlim]));
                        fprintf (Out, "	if (_i > 0) sleep(_i);\n");


                        if (xd[xpx] == ',') {
                              xpx++;
                              continue;
                              }

                        if (POST_flg) {
                              fprintf (Out, "	} /* post conditional hang*/\n");
                              POST_flg = 0;
                              }

                        goto next_cmnd;
                        }

            case ELSE:

                  if (tolower(xd[p2px+1])=='x') {
                        exportflag=1;
                        }

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Else\n");
	fprintf(Out, "//--------------\n\n");

                  if (xd[xpx] == ' ') xpx++;

                  fprintf (Out, "	if (svPtr->tpx == 0) { \n");

                  IF_flg++;
                  goto next_cmnd;

case DO: // declare

                  if (toupper(xd[p2px+1])=='A') {  /* SHELL command */

                        char tmp[4096];
                        int i;

                        for (i=0; xd[p2px]; i++) {
                              if (xd[p2px]=='\"') {
                                    tmp[i++]='\\';
                                    }
                              tmp[i]=xd[p2px++];
                              }
                        tmp[i]=0;
                        INTERPRET;
                        goto next_line;
                        }

                  if (tolower(xd[p2px+1]) == 'e') { /* declare */
                        int i=0;
dcl:
                        if (cvarx>CVARMAX) goto too_many_vars;
                        for (; xd[xpx] != ' ' && xd[xpx] != '\0' && xd[xpx]!=','; xpx++)
                              cvars[cvarx].name[i++] = xd[xpx];
                        cvars[cvarx].name[i]='\0';
                        cvarx++;
                        if (xd[xpx]==',') {
                              i=0;
                              xpx++;
                              goto dcl;
                              }
                        goto next_cmnd;
                        }


do_entry:

	fprintf(Out, "\n//--------------\n");
	fprintf(Out, "//       Do\n");
	fprintf(Out, "//--------------\n\n");

//#++++++++++++++++++
//#+ Argumentless DO
//#++++++++++++++++++

                  if (xd[xpx] == '\0' || xd[xpx] == ' ') {

                        if (JOB_flg) { // is this really a JOB command?
                              printf ("*** JOB requires argument\n");
                              goto arg_list_error;
                              }

                        fprintf (Out, "		tpx%d = svPtr->tpx; \n", ExecName);

                        fprintf (Out, "		if ( (_i = setjmp(Efor%dJ)) == 0) ",ExecName);
                        fprintf (Out, "goto Do_Block_Entry%d; //else continue \n", ExecName);

			if (IF_flg)
				fprintf (Out, "		else if (_i == 100) "
					"goto Do_Block_Bypass%d; // IF_flg m1402\n", ExecName); // must be in IF

                        forx++;
                        DoRet[DoRetX]=ExecName;
                        DoRetX++;

                        if (POST_flg) {
                              fprintf (Out, "	} // post conditional do\n");
                              POST_flg = 0;
                              }

                        goto next_cmnd;
                        }

//#++++++++++++++++++
//#+ Argumented DO
//#++++++++++++++++++

another_do:

                  fprintf (Out, "	_ArgCount=0;  //section D1000 \n");
                  Sysout ("	svPtr->Rargsx=0;\n");

                        {

                        int i=xpx,j=0,k=0;
                        char ttt[2048];
                        memmove(ttt,xd,2048);

                        j=xpx;
                        while(1) { /* scan through label for non-quoted colon */
                              /* a non-quoted colon can't exist in the label field */
                              if (xd[j]=='\0') {
                                    break;
                                    }
                              if (xd[j]==',' && k==k/2*2) {
                                    break;
                                    }
                              if (xd[j]==':' && k==k/2*2) {
                                    break;
                                    }
                              if (xd[j]=='\"') {
                                    if (xd[j+1]=='\"') {
                                          j=j+1;
                                          }
                                    else k++;
                                    }
                              j++;
                              }

                        if (xd[j]==':' && k==k/2*2) {
                              xpx=j+1;
                              t0px=0;
                              t2=1;
                              if ((ierr = parse_ ())) goto parse_err;
                              fprintf(Out, "	if (atoi( CSTR %s)) { // postconditional \n",
                                      Extract2(&pd1[sdlim]));
                              t0px=0;
                              t2=0;
                              xpx=i;
                              memmove(xd,ttt,2048);
                              }

                        else fprintf (Out,"	{\n");
                        }

                  if (xd[xpx]=='@') { /* indirection in DO argument */

			at_flg=atTest(xd, pd1, &pd1Last, lines, xpx);

			if (at_flg == -1) goto abrt_exit;

			if (at_flg) {

				char tmp[1024];

				strcpy (tmp, "d ");
				atCopy (at_flg, tmp);

				INTERPRET

				xpx = at_flg;
				fprintf (Out, "       } /* post conditional 1920*/\n");

				if (POST_flg) {
				fprintf (Out, "       } /* post conditional 1920*/\n");
				POST_flg = 0;
				}
				goto next_cmnd;
			}

                        }

                  else if (xd[xpx]=='$' && isalnum(xd[xpx+1])) {
                        t0px = 0;
                        t2 = 1;
                        if ((ierr = parse_ ())) goto parse_err;
                        fprintf (Out,"	} \n");
                        }

                  else {

                        xd[xpx-2]='$';
                        xd[xpx-1]='$';
                        xpx=xpx-2;
                        t2 = 1;
                        if ((ierr = parse_ ())) goto parse_err;
                        t2 = 0;
                        fprintf (Out,"	} \n ");
                        }

                  if (xd[xpx]==':') { /* bypass post-cond, look for non-quoted, non-parend comma */
                        int j,k=0,m=0;
                        xpx++;
                        j=xpx;
                        while(1) {
                              if (xd[j]=='\0') break;
                              if (xd[j]==',' && m==0 && k==k/2*2) break;
                              if (xd[j]==' ' && m==0 && k==k/2*2) break;
                              if (xd[j]=='(' && k==k/2*2) m++;
                              if (xd[j]==')' && k==k/2*2) m--;
                              if (xd[j]=='\"') {
                                    if (xd[j+1]=='\"')  j=j+1;
                                    else k++;
                                    }
                              j++;
                              }
                        if (xd[j]==',' && m==0 && k==k/2*2) {
                              xpx=j+1;
                              goto another_do;
                              }
                        xpx=j;
                        }

                  if (xd[xpx]==',') {
                        xpx++;
                        goto another_do;
                        }

                  if (POST_flg) { /* end of post-cond */
                        fprintf (Out, "	} /* post conditional 3001*/\n");
                        POST_flg = 0;
                        }

                  if (JOB_flg) goto job_return;

                  goto next_cmnd;

                  while (1) {
                        t2 = 1;
                        if ((ierr = parse_ ())) goto parse_err;
                        if (xd[xpx] != ',') {
                              if (POST_flg) {
                                    fprintf (Out, "	} /* post conditional 3014*/\n");
                                    POST_flg = 0;
                                    }
                              if (JOB_flg) goto job_return;
                              goto next_cmnd;
                              }
                        xpx++;
                        }

case KILL:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Kill\n");
	    fprintf(Out, "//--------------\n\n");

                  return_code = kill_code(); // goto set;  // process item to read

                  if (return_code == 0) goto next_cmnd;
                  if (return_code == PARSE_ERROR) goto parse_err;
                  if (return_code == ARG_LIST_ERROR) goto arg_list_error;
                  if (return_code == NO_GLOBAL_PERMITTED) goto nogblpermitted;
                  if (return_code == MEM_ERROR) goto mem_err;
                  if (return_code == TOO_MANY_LHS) goto too_lhs;
                  if (return_code == KILL_DCL_ERROR) goto kill_dcl_err;
                  if (return_code == ABRTX) return abrtx(); // goto abrtx;
                  goto command; // command error

            case CLOSE:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Close\n");
	    fprintf(Out, "//--------------\n\n");

                  p2px++;
                  if (xd[p2px] <= 90) xd[p2px] += 32;	//*lower case
                  if (xd[p2px] == 'o') goto cntnue; //* continue
                  if (xd[p2px] == 'a') goto ctch; //* catch

                  while (1) {
                        t2 = 1;
                        if ((ierr = parse_ ())) goto parse_err;
                        fprintf (Out, "	_i=atoi( CSTR %s);\n", Extract2(&pd1[sdlim]));
                        fprintf(Out,
                                "	if (svPtr->in_file[_i] == NULL && svPtr->out_file[_i] == NULL) \n");
                        fprintf (Out,
                                 "		ErrorMessage(\"Attempt to close unopened file\", "
                                 "svPtr->LineNumber); \n");

                        fprintf (Out,
                                 "	if (svPtr->in_file[_i]!=NULL) { \n"
                                 "	fclose(svPtr->in_file[_i]); svPtr->in_file[_i]=NULL; }\n");

                        fprintf (Out,
                                 "	else { fclose(svPtr->out_file[_i]); svPtr->out_file[_i]=NULL; }\n");

                        if (xd[xpx] != ',') {
                              if (POST_flg) {
                                    fprintf (Out, "	} /* post conditional 3240*/\n");
                                    POST_flg = 0;
                                    }
                              goto next_cmnd;
                              }
                        xpx++;
                        }

            case USE:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Use\n");
	    fprintf(Out, "//--------------\n\n");

                  t2 = 1;
                  if ((ierr = parse_ ()))
                        goto parse_err;
                  fprintf (Out, "	svPtr->io=atoi( CSTR %s);\n", Extract2(&pd1[sdlim]));

                  if (POST_flg) {
                        fprintf (Out, "	} /* post conditional 3245*/\n");
                        POST_flg = 0;
                        }

                  goto next_cmnd;

            case GOTO:

	    fprintf(Out, "\n//--------------\n");
	    fprintf(Out, "//       Goto\n");
	    fprintf(Out, "//--------------\n\n");

                  if (xd[xpx] == 0) goto lbl_error;

                  if (ExecLevel > 0)  goto goto_in_block;


next_goto:

                  i = 0;

                  while (xd[xpx]) {
                        if (xd[xpx] == ',' || xd[xpx] == ' ' || xd[xpx] == 0 ||
                                    xd[xpx] == '+' || xd[xpx] == ':') break;
                        dosave[i++] = xd[xpx++];
                        }

                  if (xd[xpx] == ':') { // post conditionals
                        xpx++;
                        t0px=0;
                        t2=1;
                        if ((ierr = parse_ ())) goto parse_err;
                        fprintf(Out, "	if (atoi((char *) %s)) /* m1635 postconditional*/", Extract2(&pd1[sdlim]));
                        t0px=0;
                        t2=0;
                        }

                  dosave[i] = 0;

                  for (i=0; dosave[i]!=0 && dosave[i]!='^' && dosave[i]!='+'; i++);

                  if (dosave[i]=='+' || dosave[i]=='(') goto lbl_error;

                  if (dosave[i]==0) { /* simple goto */
                        if (isdigit(dosave[0])) fprintf (Out, "	goto _%s;\n", dosave);
                        else fprintf (Out, "	goto %s;\n", dosave);
                        goto gexit;
                        }

                  if (dosave[0]!='^') {   /* label appears first in a ^ ref */
                        dosave[i]=0;
                        strcpy(tmp2,dosave);
                        strcpy(&dosave[1],&dosave[i+1]); /* copy fcn name backwards */
                        dosave[0]='^';
                        }
                  else strcpy(tmp2,"");

                  fprintf(Out,
                          "	_jj=Interpret(\"x \\\"goto ^%s\\\"\",svPtr);\n",&dosave[1]);

                  Sysout("	goto _epilogue; /* if fcn returns, this is end program */\n");

gexit:

                  if (xd[xpx]==',') {
                        xpx++;
                        goto next_goto;
                        }

                  if (POST_flg) {
                        fprintf (Out, "	} /* post conditional 3431*/\n");
                        POST_flg = 0;
                        }

                  goto next_cmnd;



//----------------
//	errors
//----------------


kill_dcl_err:

                  ernbr=99;
                  printf("\n*** Attempt to Kill declared variable\n");
                  goto abrt1;

parse_err:

                  ernbr = ierr;
                  errmod_(ierr);
                  if (ernbr == 9001) printf("*** Invalid global array reference");
                  if (ernbr == 9002) printf("*** Invalid routine reference");
                  if (ernbr == 9003) printf("*** Wrong number of args to $extract()");
                  if (ernbr == 12) printf("Invalid operator: %c",ErrChar);
                  goto abrt1;

nobrk_err:
                  ernbr=99;
                  printf ("\n*** Break not permitted here");
                  goto abrt1;

indirect_err:

                  ernbr = 72;
                  printf ("\n*** Indirection not permitted here");
                  goto abrt1;

lbl_error:

                  ernbr = 8;
                  printf ("\n*** Label or file not found");
                  goto abrt1;

do_error:

                  ernbr = 8;
                  printf ("\n*** Post-conditional not permitted here");
                  goto abrt1;

lab_err:

                  ernbr=22;
                  printf("\n*** Labels must begin with and alphabetic character");
                  goto abrt1;

bad_lock:

                  ernbr=99;
                  printf("\n*** Lock error");
                  goto abrt1;

quit_err:
                  ernbr=99;
                  printf("\n*** Break not possible");
                  goto abrt1;

Ext_file:

                  ernbr = 8;
                  printf ("\n*** Attempt to branch to external file");
                  goto abrt1;

go_err:

                  ernbr = 99;
                  printf ("\n*** Attempt to branch to function");
                  goto abrt1;

            default:

command:

                  ernbr = 13;
                  printf ("\n*** 13 Command Error");
                  goto abrt1;

arg_list_error:

                  ernbr = 14;
                  printf ("\n*** 14 Argument list error");
                  goto abrt1;

too_many_vars:

                  ernbr=91;
                  printf ("\n*** 91 Too many C variables");
                  goto abrt1;

Missing_Zmain:

                  ernbr=92;
                  printf ("\n*** 92 Missing zmain");
                  goto abrt1;


nogblpermitted:

                  ernbr=92;
                  printf ("\n*** 92 Global not permitted");
                  goto abrt1;

ref_err:

                  ernbr = 18;
                  printf ("\n*** 18 Reference error");
                  goto abrt1;

too_lhs:

                  ernbr=9033;
                  printf ("\n*** 9033 Too many variables in LHS");
                  goto abrt1;

mem_err:

                  ernbr = 19;
                  printf ("\n*** 19 Out of memory");
                  goto abrt1;

catch_error:
                  ernbr=122;
                  printf("\n*** 122 Missing catch argument");
                  goto abrt1;

catch_unknown:
                  ernbr=123;
                  printf("\n*** 123 Unknown exception in catch");
                  goto abrt1;

try_unknown:
                  ernbr=124;
                  printf("\n*** 124 Unknown option in try");
                  goto abrt1;

label_in_block:
                  ernbr = 125;
                  printf("\n*** 125 Label not allowed in block");
                  goto abrt1;

goto_in_block:
                  ernbr = 126;
                  printf("\n*** 126 Goto not allowed in block");
                  goto abrt1;

abrt1:

                  printf (" in or near line %d\n", lines);
                  printf ("***	%s \n\n", &pd1[pd1Last]);
                  ExitCode = ernbr;
                  return 0; // goto next_line;


            }				/* command switch */


abrt_exit:

      return abrtx();

      }



int LevCheck(short ExecName, short DoRetX, short DoRet[]) {
      int i;
      for (i=0; i<DoRetX; i++) if (DoRet[i]==ExecName) return 0;
      return 1;
      }


void atCopy (int at_flg, unsigned char tmp[]) {

      int i, j;

      for (i = xpx, j = 2; xd[i] != 0 && i < at_flg; i++) {

            if (xd[i] != '\"' && xd[i] != '\\') tmp[j++] = xd[i];

            else if (xd[i] == '\"') {
                  tmp[j++] = '\\';
                  tmp[j++] = '\"';
                  }

            else if (xd[i] == '\\') {
                  tmp[j++] = '\\';
                  tmp[j++] = '\\';
                  }
            }
      tmp[j] = 0;
      }


void EP(int flg, short ExecName, long pid) { //* subfucntion entry point router

      if (flg) {
            fprintf(Out, "	goto _epilogue;\n");
            fprintf(Out,"_eps:	if (strlen((const char *) offset)==0) goto _fcn;\n");

            if (LabelsStart!=NULL) {
                  p1=LabelsStart;
                  while (p1!=NULL) {
                        fprintf(Out,"	if (strcmp((const char *) offset,(const char *) \"%s\")==0) goto %s;\n",
                                p1->name,p1->name);
                        p1=p1->next;
                        }
                  }
            fprintf(Out, "	ErrorMessage(\"Error - entry point label not found\", svPtr->LineNumber);\n");
            }

      epi (ExecName, pid);           //* terminate prev function
      MAIN_flg = 0;     //* not in main function
      }

