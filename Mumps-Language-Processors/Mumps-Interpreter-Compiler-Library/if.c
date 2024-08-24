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

//	May 5, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int     if_code() {


      if (xd[xpx] == ' ' || xd[xpx]=='\0') { /* no argument form */

            xpx++;
            fprintf (Out, "	if (svPtr->tpx) {\n");
            IF_flg++;
            return 0; // goto next_cmnd;
            }

      at_flg=atTest(xd, pd1, &pd1Last, lines, xpx);

      if (at_flg == -1) return ABRTX; // goto abrtx;

      if (at_flg) {

            char tmp[1024];

            strcpy (tmp, "i ");
            atCopy (at_flg, tmp);

            INTERPRET

            xpx = at_flg;

            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional 1856*/\n");
                  POST_flg = 0;
                  }

            linepad (fprintf (Out, "	if (svPtr->tpx) {"), "Test if expression true");
            goto if_structure;
            }

if_again:

      t2 = 1;
      if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;

      fprintf (Out, "	svPtr->tpx = 0; //Default $Test indicator\n");

//            fprintf (Out, "	if (numcomp((UNS) %s, "
//                     "(UNS) \"0\")) { // Test if true \n",
//                     Extract2(&pd1[sdlim]));

      fprintf (Out, "	if (numcomp2((UNS) %s )) { // Test if true \n",
               Extract2(&pd1[sdlim]));

      fprintf (Out, "	svPtr->tpx = 1; // $test becomes true\n");

if_structure:

      tpx = 1;
      if (xd[xpx + 1] != '{') IF_flg++;

      if (IF_flg > 0) ForBlocks[IF_flg] = ForBlocks[IF_flg-1];

      Blocks[IF_flg] = IF;

      if (xd[xpx] == ',') {
            xpx++;
            goto if_again;
            }

      return 0; // goto next_cmnd;
      }
