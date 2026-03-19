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

#include<stdio.h>

int atTest (unsigned char xd[], unsigned char pd1[], short *pd1Last,
            short lines, int xpx) {

      int k, at_flg = 0;

      for (k = xpx; xd[k] != 0 && xd[k] != ' '; k++) {

            if (xd[k] == '@') at_flg = 1;

            if (xd[k] == '"') {

                  while (k++) {

                        if (xd[k] == 0) {
                              printf ("*** Imbalanced quotes\n");
                              printf ("*** In or near line %d\n", lines);
                              printf ("*** %s\n", &pd1[*pd1Last]);
                              return -1;
                              }

                        if (xd[k] == '\"' && xd[k + 1] == '\"') {
                              k++;
                              continue;
                              }

                        if (xd[k] == '\"') break;

                        }
                  }
            }

      if (at_flg==1) return k;
      else return 0;
      }

