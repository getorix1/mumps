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

//	June 11, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int doty(int j, short *ExecLevel, short *ExecName, short DoRet[], short DoRetX, short *pd1Last,
         short pd1cur, short ExecStack[], short *ExecX, short ExecSym[]) {

      if (*ExecLevel == j-1) {	// indenting

            if (LevCheck(*ExecName,DoRetX,DoRet)) {

                  printf("\n*** Indentation without preceeding Do\n");
                  *pd1Last=pd1cur;
                  return 99;
                  }

            fprintf (Out, "	goto Do_Block_Bypass%d; // C3551 bypass indented group\n", *ExecName);

	fprintf(Out,"\n//-------------------------------------\n");
            fprintf (Out, "Do_Block_Entry%d:; // Enter Do Block\n", *ExecName);
	fprintf(Out,"//-------------------------------------\n\n");
            ExecStack[(*ExecX)++] = (*ExecName)++;
            fprintf (Out, "do { /* dotted indent entry */\n");
            *ExecLevel = j;
            }

      else if (*ExecLevel > j) { // un-indenting
            for (; *ExecLevel > j; (*ExecLevel)--) {
                  fprintf (Out, "	} while (0); /* dotted un-indent %d */\n", *ExecX - 1);
                  if (ExecSym[*ExecX]) {
                        ExecSym[*ExecX]=0;
                        }

                  fprintf (Out, "	longjmp(Efor%dJ,99); /* return */\n", ExecStack[--(*ExecX)]);
                  fprintf (Out, "\n\tDo_Block_Bypass%d: ; // D3607 \n",
                           ExecStack[*ExecX],ExecStack[*ExecX]);
                  }

            *ExecLevel = j;

            }
      return 0;
      }
