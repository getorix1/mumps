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

//	June 16, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

void epi (short ExecName, long pid) {
      int i;
      fprintf (Out, "\n");
      fprintf (Out,
               "//=================================================================================\n");
      linepad (fprintf (Out, "_epilogue: tmp0[0]=0; tmp1[0]=0;"), " Function Epilogue");
      fprintf (Out, "	Mltoa(getpid(),atmp);\n");
      fprintf (Out, "	if (svPtr->LockDone) cleanLocks((char *) atmp);\n");
      linepad (fprintf (Out, "	Mglobal(CLOSE,tmp0,tmp1,svPtr);"), "Close Globals");



      fprintf( Out, "\n} // end of try block \n\n");
      fprintf( Out, "catch(...) { \n");

#include <mumpsc/errors.h>

      fprintf( Out, "	cout << \"Uncaught error in or near line \" \n"
               "	<< svPtr->LineNumber << endl;\n");
      linepad (fprintf (Out, "	Mglobal(CLOSE,tmp0,tmp1,svPtr);"), "Close Globals");
      fprintf( Out, "	} \n\n");

      if (FCN_flg) fprintf(Out,"return \"\";} /* Terminate */\n");
      else {
            linepad (fprintf (Out, "return EXIT_SUCCESS; }"), "Terminate");
            }

      fprintf (Out,
               "//=================================================================================\n");

      if (LabelsStart!=NULL) {
            int t=0,l=0;

            fprintf (Out, "\n\nint %s_label_lookup(char *label) {\n\n",FunctionName);

            fprintf (Out, "static struct lab {\n"
                     "	const char *text;\n"
                     "	int offset;\n	}\n"
                     "	L[] = {\n");

            p1=LabelsStart;
            fprintf(Out, "		\"%s\", %d",p1->name, p1->lnbr);
            p1=p1->next;
            l++;
            while (p1!=NULL) {
                  l++;
                  fprintf(Out, ",\n		\"%s\", %d",p1->name, p1->lnbr);
                  p1=p1->next;
                  }
            fprintf(Out, "\n	};\n\n");

            fprintf(Out, "int lx = -1, i, Lx=%d;\n\n",l);

            fprintf(Out,"	for (i=0; i<Lx; i++) {\n");
            fprintf(Out,"		if (strcmp((const char *) L[i].text,(const char *) label)==0) {\n");
            fprintf(Out,"			lx=L[i].offset;\n");
            fprintf(Out,"			break;\n");
            fprintf(Out,"			}\n");
            fprintf(Out,"		}\n");
            fprintf(Out,"	return lx;\n");
            fprintf(Out," }\n");
            p1=LabelsStart;   //* clear label table
            while (p1!=NULL) {
                  p3=p1->next;
                  free(p1);
                  p1=p3;
                  }
            LabelsStart=NULL;
            }

      else {
            fprintf (Out, "\n\nint %s_label_lookup(char *label) { return -1; }\n",FunctionName);
            }

      if (TextStart!=NULL) {
            int t=0,l=0;
            fprintf (Out, "\n\nconst char * %s_text_function(int disp) {\n\n",FunctionName);

            fprintf (Out, "static struct txt {\n"
                     "	const char *text;\n"
                     "	int offset;\n	}\n"
                     "	T[] = {\n");

            p1=TextStart;
            fprintf(Out, "		\"%s\", %d",p1->name, p1->lnbr);
            p1=p1->next;
            t++;
            while (p1!=NULL) {
                  t++;
                  fprintf(Out, ",\n		\"%s\", %d",p1->name, p1->lnbr);
                  p1=p1->next;
                  }
            fprintf(Out, "\n		};\n\n");

            fprintf(Out, "int tx,i,Tx=%d;\n\n",t);

            fprintf(Out,"	for (i=0; i<Tx; i++)\n");
            fprintf(Out,"		if (disp==T[i].offset) return T[i].text;\n");
            fprintf(Out,"	return NULL;\n");
            fprintf(Out,"}\n");
            p1=TextStart;
            while (p1!=NULL) {
                  p3=p1->next;
                  free (p1);
                  p1=p3;
                  }
            TextStart=NULL;
            }

      else {
            fprintf (Out, "const char * %s_text_function(int disp) { return NULL; }\n\n",FunctionName);
            }

            {
            int i;
            FILE *hfile;
            char fn[256];
            if (strlen(FunctionName)==0) {
                  char nme[32];
                  sprintf(nme,"Main_.%d", pid);
                  hfile = fopen(nme,"w");
                  }
            else {
                  char nme[128];
                  sprintf(nme,"%s.%d", FunctionName, pid);
                  hfile = fopen(nme,"w");
                  if (hfile==NULL) abort();
                  }

            for (i=1; i<=forxmax; i++)

                  fprintf(hfile,
                          "char for%d_incr[FORPARM], for%d_lim[FORPARM], for%d_init[FORPARM];\n"
                          "int for%dF; "
                          "int for%dT; \n",
//                          "long for%d; "
//                          "double for%dD;\n",
                          i,i,i,i,i,i);

            for (i=0; i<=forx; i++)
                  fprintf (hfile, "jmp_buf for%dJ;\n",i);
            for (i=0; i<ArgMax; i++)
                  fprintf(hfile, "char *__Arg%d = new char[STR_MAX];\n", i);
            for (i=0; i<=ExecName; i++) {
                  fprintf (hfile, "jmp_buf Efor%dJ;\n",i);
                  fprintf(hfile, "int tpx%d;\n",i);
                  }
            for (i=0; i<MaxTmp; i++) fprintf(hfile,"unsigned char *tmp%d = new unsigned char[STR_MAX];\n",i);

            for (i = 0; i < cvarx; i++)
                  fprintf(hfile, "char %s[STR_MAX]; // declared variable\n", cvars[i].name);
            cvarx=0;

            IF_flg_max=forxmax=0;
            ArgMax = 0;
            fclose(hfile);
            }

      strcpy(FunctionName,"");
      IF_flg=forxmax=forx=0;
      for (i=0; i<FBMAX; i++) ForBlocks[i] = -1;
      MaxTmp=2;
      return;
      }
