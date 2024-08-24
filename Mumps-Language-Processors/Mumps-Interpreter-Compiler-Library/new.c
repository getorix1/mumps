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

//	May 10, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int     new_code() {

      int return_code;

      exportflag=0;

      if (xd[xpx]==' '||xd[xpx]==0) { /* no argument form */
            Sysout ( "	if (svPtr->_Sym>SYM_MAX) ");
            fprintf (Out, "ErrorMessage(\"Symbol Table Overflow\",svPtr->LineNumber);\n");
            Sysout ( "	svPtr->nstart=NULL;\n");
            Sysout ("	sym_(7,(UNS) \"\", (UNS) \"\", svPtr);\n");
            ExecSym[ExecX] = 1;

            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional in new */\n");
                  POST_flg = 0;
                  }
            xpx++;
            return 0; // goto next_cmnd;
            }

//#++++++++++++++++++++++++++++++++
//#+    Push  all locals except...
//#++++++++++++++++++++++++++++++++

      if (xd[xpx] == '(') {               //* specific

            fprintf (Out, "	{\n	struct nmes *p1,*p2;\n");
            fprintf (Out, "	svPtr->nstart=NULL;\n");
            xpx++;

new2:

            t0px = 1;
            t2 = 1;
            retz = NEW;

            return_code = set_code(SET); // goto set;  // process item to read

            if (return_code == PARSE_ERROR) return PARSE_ERROR; // goto parse_err;
            if (return_code == ARG_LIST_ERROR) return ARG_LIST_ERROR; // goto arg_list_error;
            if (return_code == NO_GLOBAL_PERMITTED) return NO_GLOBAL_PERMITTED;
            if (return_code == MEM_ERROR) return MEM_ERROR; // goto mem_err;
            if (return_code == TOO_MANY_LHS) return TOO_MANY_LHS; // goto too_lhs;
            if (return_code == ABRTX) return ABRTX; // goto abrtx;

new_return:

            if (strcmp (vd, "atmp") == 0) fprintf (Out, "	keyfix(atmp);\n");

            else {
                  char tmp[256];
                  strcpy (tmp, "\"");
                  strcat (tmp, vd);
                  strcat (tmp, "\"");
                  strcpy (vd, tmp);
                  }

            fprintf (Out,
                     "	p1=(struct nmes *) malloc(sizeof (struct nmes));\n"
                     "	if (p1==NULL) ErrorMessage(\"Out of memory\",svPtr->LineNumber);\n"
                     "	p1->name=(char *) malloc(strlen((const char *) atmp+1));\n"
                     "	if (p1->name==NULL) ErrorMessage(\"Out of memory\",svPtr->LineNumber);\n");

            fprintf (Out, "	strmove((UNS) p1->name, (UNS) %s);\n", vd);

            fprintf (Out,
                     "	p1->next=svPtr->nstart;\n"
                     "	svPtr->nstart=p1;\n");
            xpx++;

            if (xd[xpx] != ')' && xd[xpx]) {
                  xpx++;
                  goto new2;
                  }

            fprintf (Out, "	sym_(7,(UNS) \"\",(UNS) \"\",svPtr); // new except (...) \n");
            fprintf (Out, "	}\n");
            ExecSym[ExecX] = 1;
            if (ExecX==0) {
                  Sysout ( "	if (_doStx == 0) ");
                  fprintf(Out,"	_doStackN[_doStx-1]=1;\n");
                  }

            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional new return*/\n");
                  POST_flg = 0;
                  }

            xpx++;
            return 0; // goto next_cmnd;
            }

//#++++++++++++++++++++++++++++++++
//#+    Push locals inclusive
//#++++++++++++++++++++++++++++++++


      ExecSym[ExecX] = 1;

      if (POST_flg) {
            fprintf (Out, "	} /* post conditional new push */\n");
            POST_flg = 0;
            }

exports:

new3:

      t0px = 1;
      t2 = 1;
      retz = NEW1;

      return_code = set_code(SET); // goto set;  // process item to read

      if (return_code == PARSE_ERROR) return PARSE_ERROR; // goto parse_err;
      if (return_code == ARG_LIST_ERROR) return ARG_LIST_ERROR; // goto arg_list_error;
      if (return_code == NO_GLOBAL_PERMITTED) return NO_GLOBAL_PERMITTED;
      if (return_code == MEM_ERROR) return MEM_ERROR; // goto mem_err;
      if (return_code == TOO_MANY_LHS) return TOO_MANY_LHS; // goto too_lhs;
      if (return_code == ABRTX) return ABRTX; // goto abrtx;

new1_return:

      if (strcmp (vd, "atmp") == 0) fprintf (Out, "	keyfix(atmp);\n");

      else {
            char tmp[256];
            strcpy (tmp, "\"");
            strcat (tmp, vd);
            strcat (tmp, "\"");
            strcpy (vd, tmp);
            }

      if (exportflag)
            fprintf (Out, "	sym_(SymMarkCopyFlag, (UNS) %s, (UNS) \"\",svPtr); // mark copy flag\n", vd);
      else
            fprintf (Out, "	sym_(100,(UNS) %s,(UNS) \"\",svPtr); // create new variable at this level\n", vd);
      xpx++;

      if (xd[xpx] != ' ' && xd[xpx]) {
            xpx++;
            goto new3;
            }

      for( ; xd[xpx]!=' ' && xd[xpx]!=0; xpx++);
      return 0; // goto next_cmnd;

      }
