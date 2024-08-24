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

int     read_code() {

      int return_code;

      fprintf (Out, "	if (svPtr->in_file[svPtr->io] == NULL) \n");
      fprintf (Out, "		ErrorMessage(\"Read from input file\", "
               "svPtr->LineNumber); \n");

      while (1) {

            dirflg = 0;

            if (xd[xpx] == '!' || xd[xpx] == '#') { // carriage control
                  if (xd[xpx] != '#') i = 2;
                  else i = 1;
                  inout (&pd1[sdlim], i, "");
                  goto cont_read;
                  }

            if (xd[xpx] == '?') { //* tab format
                  t2 = 1;
                  xpx++;
                  if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
                  i = 3;
                  inout (&pd1[sdlim], i, &pd1[sdlim]);
                  goto cont_read;
                  }

            if (xd[xpx] == '"') { // text to be printed first
                  t2 = 1;
                  if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
                  i = 4;
                  inout (&pd1[sdlim], i, "");
                  goto cont_read;
                  }

            if (xd[xpx] == '*') { // direct mode
                  xpx++;
                  dirflg = 1;
                  }

            retz = READ;

            return_code = set_code(SET); // goto set;  // process item to read

            if (return_code == 0) goto set_return;
            if (return_code == PARSE_ERROR) return PARSE_ERROR; // goto parse_err;
            if (return_code == ARG_LIST_ERROR) return ARG_LIST_ERROR; // goto arg_list_error;
            if (return_code == NO_GLOBAL_PERMITTED) return NO_GLOBAL_PERMITTED;
            if (return_code == MEM_ERROR) return MEM_ERROR; // goto mem_err;
            if (return_code == TOO_MANY_LHS) return TOO_MANY_LHS; // goto too_lhs;
            if (return_code == ABRTX) return ABRTX; // goto abrtx;
            return 99;

set_return:

            if (xd[xpx + 1] == ':') { // timeout
                  t2 = 1;
                  xpx += 2;
                  if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
                  fprintf ( Out, " // TIMEOUT REQUESTED FOR %s SECONDS\n", &pd1[sdlim]);
                  //linepad(fprintf(Out,"	file[5]=stdin;"),"// unit ready for input");

                  fprintf(Out,
                          "	if (svPtr->io==5) fflush(stdout);\n");

                  fprintf (Out,
                           "	svPtr->tpx = TimeOut_getstr1(svPtr->io, "
                           "gtmp, (char *) %s, %d); // Read string into gtmp\n",
                           &pd1[sdlim], dirflg);

                  fprintf (Out, "	if (svPtr->tpx >= 0) svPtr->tpx = 1; "
                           "else svPtr->tpx=0; // Set $Test\n");

                  strcpy (&pd1[sdlim], "gtmp");
                  xpx--;
                  goto store;
                  }

            if (dirflg == 1) {
                  fprintf (Out, "	svPtr->tpx=DirectRead(svPtr->io,gtmp); // Read Direct\n");
                  fprintf (Out, "	sprintf((char *)atmp,\"%%d\",(char *) gtmp[0]);\n");
                  strcpy (&pd1[sdlim], "atmp");
                  }

            else {

                  fprintf (Out, "	svPtr->tpx = getstr1(svPtr->in_file[svPtr->io], &gtmp);"
                           " // Read string into gtmp\n");

                  fprintf (Out, "	if (svPtr->tpx >= 0) svPtr->tpx = 1; else svPtr->tpx = 0;"
                           " // Set $Test\n");

                  strcpy (&pd1[sdlim], "gtmp");
                  }

store:
            retz = READ;

            return_code = set_code(SET_STORE); // goto set;  // process item to read

            if (return_code == 0) goto cont_read;
            if (return_code == PARSE_ERROR) return PARSE_ERROR; // goto parse_err;
            if (return_code == ARG_LIST_ERROR) return ARG_LIST_ERROR; // goto arg_list_error;
            if (return_code == NO_GLOBAL_PERMITTED) return NO_GLOBAL_PERMITTED;
            if (return_code == MEM_ERROR) return MEM_ERROR; // goto mem_err;
            if (return_code == TOO_MANY_LHS) return TOO_MANY_LHS; // goto too_lhs;
            if (return_code == ABRTX) return ABRTX; // goto abrtx;

cont_read:

            xpx++;
            dirflg = 0;
            if (xd[xpx] == ',') xpx++;

            if (xd[xpx] == 0 || xd[xpx] == ' ' || xd[xpx] == '\t') {

                  if (POST_flg) {
                        fprintf (Out, "	} /* post conditional cont_read */\n");
                        POST_flg = 0;
                        }

                  return 0; // goto next_cmnd;
                  }
            }

      return 0; // goto next_cmnd;
      }
