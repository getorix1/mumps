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

//	May 6, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int	mumps_open() {

openf:

      t2 = 1;
      if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
      fprintf (Out, "	_i = atoi( CSTR %s);\n", Extract2(&pd1[sdlim]));
      if (xd[xpx] != ':') return ARG_LIST_ERROR; // arg_list_error;
      xpx++;
      t2 = 1;
      if ((ierr = parse_ ())) return PARSE_ERROR; //goto parse_err;

      if (strcmp ("tmp0", &pd1[sdlim]) != 0)
            fprintf (Out, "	strmove((UNS) tmp0, (UNS) %s);\n", Extract2(&pd1[sdlim]));

      fprintf (Out, "	for(_j=0; tmp0[_j]!=0 && tmp0[_j]!=\',\';_j++);\n");

      fprintf (Out, "	if (tmp0[_j]!=\',\') ");
      fprintf (Out, "ErrorMessage(\"File open error\",svPtr->LineNumber);\n");

      fprintf (Out, "	tmp0[_j++]=0;\n");
      fprintf (Out, "	strmove((UNS) gtmp, (UNS) tmp0);\n");

      fprintf (Out,
               "	if (strcmp( (CCHR) &tmp0[_j], (CCHR) \"old\") == 0 || \n"
               "		strcmp( (CCHR) &tmp0[_j], (CCHR) \"OLD\") == 0) _new = 0; \n");

      fprintf (Out,
               "	else if (strcmp( (CCHR) &tmp0[_j], (CCHR) \"new\") == 0 || \n"
               "		strcmp((CCHR) &tmp0[_j], (CCHR) \"NEW\") == 0) _new = 1; \n");

      fprintf (Out,
               "	else if (strcmp( (CCHR) &tmp0[_j], (CCHR) \"append\") == 0 || \n"
               "		strcmp( (CCHR) &tmp0[_j], (const char *) \"APPEND\") == 0) _new=2; \n");

      fprintf (Out, "	else ErrorMessage(\"File open error\",svPtr->LineNumber);\n");

      fprintf (Out, "	if (svPtr->out_file[_i] != NULL || svPtr->in_file[_i] != NULL) \n");
      fprintf (Out, "		ErrorMessage(\"File already open\", svPtr->LineNumber); \n");

      fprintf (Out, "	if (_new==1) { /* output */ \n");

      fprintf (Out, "		svPtr->out_file[_i] = fopen((const char *) gtmp, \"w\"); \n");

      fprintf (Out, "		if(svPtr->out_file[_i] == NULL) svPtr->tpx = 0; \n");
      fprintf (Out, "		else svPtr->tpx = 1; \n");
      fprintf (Out, "		} \n");
      fprintf (Out, "	else if (_new == 2) { /* append */ \n");

      fprintf (Out, "		svPtr->out_file[_i] = fopen((const char *) gtmp, \"a\"); \n");

      fprintf (Out, "		if(svPtr->out_file[_i] == NULL) svPtr->tpx = 0; \n");
      fprintf (Out, "		else svPtr->tpx = 1; \n");
      fprintf (Out, "		} \n");
      fprintf (Out, "	else { /* input */ \n");

      fprintf (Out, "		svPtr->in_file[_i] = fopen((const char *) gtmp, \"r\"); \n");

      fprintf (Out, "		if(svPtr->in_file[_i] == NULL) svPtr->tpx = 0; \n");
      fprintf (Out, "		else svPtr->tpx = 1; \n");
      fprintf (Out, "		}\n");

      if (xd[++xpx] != ',') {

            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional open */\n");
                  POST_flg = 0;
                  }
            return 0;
            }

      xpx++;
      goto openf;

      }
