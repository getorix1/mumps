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

//	June 5, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int     kill_code() {

      if (xd[xpx] == ' ' || xd[xpx] == 0 || xd[xpx] == '\t') {  // Kill all locals
            fprintf (Out, "	_SymFree(0,svPtr);\n");
            goto k001;
            }

//#+++++++++++++++++++++++++++++++++++++++
//# Check for indirection in statement
//#+++++++++++++++++++++++++++++++++++++++

      at_flg = atTest(xd, pd1, &pd1Last, lines, xpx);
      if (at_flg == -1) return ABRTX; // goto abrtx;

      if (at_flg) {

            char tmp[1024];
            int i, j;

            strcpy (tmp, "k ");
            atCopy (at_flg, tmp);

            INTERPRET

            xpx = at_flg;
            if (POST_flg) {
                  fprintf (Out, "	} // post conditional 3053\n");
                  POST_flg = 0;
                  }
            return 0; // goto next_cmnd;
            }

k003:

      if (xd[xpx] == '^') goto k004;      //* kill globals

      if (xd[xpx] != '(') {               //* Kill specific

            t2=1;
            if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
            if (cvar(&v1d[1])) return KILL_DCL_ERROR; // goto kill_dcl_err;

            fprintf (Out, "	sym_(2, (UNS) %s, (UNS) \"\",svPtr); "
                     "//...* kill specific\n", Extract1(&pd1[sdlim]));

            if (xd[xpx] == ',' ) {
                  xpx++;
                  goto k003;
                  }

            goto k001;
            }

//#++++++++++++++++++++++++++++++++
//#+    Kill all locals except...
//#++++++++++++++++++++++++++++++++

      fprintf (Out, "	{ \n	struct nmes *p1, *p2; \n");
      fprintf (Out, "	svPtr->nstart = NULL;\n");
      xpx++;

kill2:

      t0px = 1;
      t2 = 1;
      retz = KILL;

      int return_code = set_code(SET); // goto set;  // process item to read

      if (return_code == PARSE_ERROR) return PARSE_ERROR; // goto parse_err;
      if (return_code == ARG_LIST_ERROR) return ARG_LIST_ERROR; // goto arg_list_error;
      if (return_code == NO_GLOBAL_PERMITTED) return NO_GLOBAL_PERMITTED;
      if (return_code == MEM_ERROR) return MEM_ERROR; // goto mem_err;
      if (return_code == TOO_MANY_LHS) return TOO_MANY_LHS; // goto too_lhs;
      if (return_code == ABRTX) return ABRTX; // goto abrtx;

kill_return:

      if (strcmp (vd, "atmp") == 0) fprintf (Out, "	keyfix(atmp); \n");

      else {

            char tmp[256];
            strcpy (tmp, "\"");
            strcat (tmp, vd);
            strcat (tmp, "\"");
            strcpy (vd, tmp);
            }

      fprintf (Out, "	p1 = (struct nmes *) malloc(sizeof (struct nmes));\n");

      fprintf (Out, "	if (p1 == NULL) ErrorMessage(\"Out of memory\", "
               "svPtr->LineNumber);\n");

      fprintf (Out, "	p1->name = (char *) malloc(strlen((const char *) %s) + 1);\n", vd);
      fprintf (Out, "	if (p1->name == NULL) ErrorMessage(\"Out of memory\", svPtr->LineNumber);\n");
      fprintf (Out, "	strmove((UNS) p1->name, (UNS) %s);\n", vd);
      fprintf (Out, "	p1->next = svPtr->nstart;\n");
      fprintf (Out, "	svPtr->nstart = p1;\n");
      xpx++;

      if (xd[xpx] != ')' && xd[xpx]) {
            xpx++;
            goto kill2;
            }

      fprintf (Out, "	sym_(5, (UNS) \"\",(UNS) \"\", svPtr); // kill all except\n");
      fprintf (Out, "	}\n");
      goto k002;

//#++++++++++++++++++++++++++++++++++++++++++++
//# Kill globals
//#++++++++++++++++++++++++++++++++++++++++++++

k004:

      while (1) {

            t0px = 1;
            if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;

            fprintf (Out, "	strmove( (UNS) gtmp, (UNS) \"\");\n");

                  {
                  char t[1024];
                  int i, j = 0;

                  for (i = 1; v1d[i] != 0; i++) {
                        if (v1d[i] >= 32 && v1d[i] <= 127) t[j++] = v1d[i];
                        else break;
                        }

                  t[j++] = '\x01';
                  t[j] = 0;

                  fprintf (Out, "	strcat((char *) gtmp,(char *) \"%s\"); \n", t);

                  if (v1d[i] != 0) {
                        i++;
                        while (1) {
                              j = 0;
                              while (v1d[i] <= 127 && v1d[i] >= 32) t[j++] = v1d[i++];
                              t[j] = 0;
                              fprintf (Out, "	strcatx((char *) gtmp,(char *) %s);\n", Extract2(t));

                              /**************************************
                                                          j = 0;
                                                          t[j++] = '\x01';
                                                          t[j] = 0;
                                                          fprintf (Out, "	strcat((char *) gtmp,(char *) \"%s\");\n", t);
                              *******************************************/

                              if (v1d[++i] != 0) continue;
                              else break;
                              }
                        }
                  }

            fprintf (Out, "	{ \n");
            fprintf (Out, "	unsigned char tmp14[STR_MAX], tmp15[STR_MAX]; \n");
            fprintf (Out, "	keyfix(gtmp);\n");
            fprintf (Out, "	_k = strlen((const char *) gtmp); \n");	/* original length */

            fprintf (Out, "	strmove((UNS) tmp14, (UNS) gtmp); \n");
            fprintf (Out, "	Mglobal(GKILL, gtmp, tmp15, svPtr); \n");	/* kill first */

            fprintf (Out, "	while(1) { \n");
            fprintf (Out, "		_j = Mglobal(XNEXT, gtmp, tmp15, svPtr); \n"); // kill others like it
            fprintf (Out, "		if (_j && strncmp( (CCHR) tmp14, (CCHR) gtmp,_k) == 0) { \n");
            fprintf (Out, "			Mglobal(GKILL, gtmp, tmp15, svPtr); \n");
            fprintf (Out, "			continue; \n");
            fprintf (Out, "			} \n");

            fprintf (Out, "		break; \n");
            fprintf (Out, "		} \n");
            fprintf (Out, "	}\n");

k002:

            if (xd[++xpx] == ',') {
                  xpx++;
                  goto k003;
                  }

k001:

            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional 3208*/\n");
                  POST_flg = 0;
                  }
            return 0; // goto next_cmnd;
            }
      }
