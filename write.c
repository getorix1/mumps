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

int     write_code() {


      fprintf (Out, "	if (svPtr->out_file[svPtr->io]==NULL) \n");

      fprintf (Out, "		ErrorMessage(\"Write to input file\", "
               "svPtr->LineNumber); // m1944 \n");

      at_flg=atTest(xd, pd1, &pd1Last, lines, xpx);

      if (at_flg == -1) return ABRTX; // goto abrtx;

      if (at_flg) {

            char tmp[1024];

            strcpy (tmp, "w ");
            atCopy (at_flg, tmp);

            INTERPRET

            xpx = at_flg;

            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional 1920*/\n");
                  POST_flg = 0;
                  }
            return 0; // goto next_cmnd;
            }

      t0px = 0;

      while (1) {

            if (xd[xpx] == '!' || xd[xpx] == '#') { //* new line or page
                  i = 1;
                  if (xd[xpx] != '#')
                        i = 2;
                  xpx++;
                  goto iosub;
                  }

            if (xd[xpx] == '*') {             // "*" output*/
                  xpx++;
                  t2 = 1;
                  if ((ierr = parse_ ()))
                        return PARSE_ERROR; // goto parse_err;
                  linepad (fprintf (Out, "	_i=atoi( CSTR %s);", &pd1[sdlim]), "Convert to integer");




                  linepad (fprintf (Out, "	putchar(_i);"), "Direct output");


                  i = 4;
                  goto iosub1;
                  }

            if (xd[xpx] == '?') { /*tab */
                  t2 = 1;
                  xpx++;
                  if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
                  i = 3;
                  goto iosub;
                  }

            t2 = 1;
            if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
            i = 4;

iosub:
            inout (&pd1[sdlim], i, &pd1[sdlim]);

iosub1:

            if (xd[xpx] == ',') {
                  xpx++;
                  continue;
                  }

            if (xd[xpx] == 0 || xd[xpx] == ' ' || xd[xpx] == '\t') {
                  if (POST_flg) {
                        fprintf (Out, "	} /* post conditional 4092*/\n");
                        POST_flg = 0;
                        }
                  return 0; // goto next_cmnd;
                  }
            }

      }
