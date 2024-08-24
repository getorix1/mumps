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

//	May 12, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>


void GlobalArray(unsigned char v1d[], short *GBL_flg, unsigned char *vd) {
      /* process LHS style global array refs */

      unsigned char t[1024], h[6];
      int i, j = 0;

      if (v1d[1] == '^') {			// Process LHS global array reference

//-----------------------------------------------------------------------
//	starting as 2 eliminates "^" and that means keyfix does not shift
//        for (i = 1; v1d[i] != 0; i++) {
//-----------------------------------------------------------------------

            for (i = 2; v1d[i] != 0; i++) {
                  if (v1d[i] >= 32 && v1d[i] <= 127)
                        t[j++] = v1d[i];
                  else break;
                  }

            *GBL_flg = 1;
            sprintf(vd,"tmp%d",iiitmp);
            if (v1d[i]=='\0') h[2]=206;
            else h[2]=v1d[i];
            sprintf (h, "%2x", h[2]);
            t[j++] = '\\';
            t[j++] = 'x';
            t[j++] = h[0];
            t[j++] = h[1];
            t[j] = 0;

            fprintf (Out, "	strmove((UNS) tmp%d, (UNS) \"%s\"); // M3701\n",
                     iiitmp, t);

            if (v1d[i]=='\0') {
                  iiitmp++;
                  memcheck(iiitmp);
                  return;
                  }
            i++;

            while (1) {

                  j = 0;

                  while (v1d[i] <= 127 && v1d[i] >= 32) t[j++] = v1d[i++];

                  t[j] = 0;

                  if (strlen(t)) {

                        if (strncmp(t,"#G#",3)==0 || strncmp(t,"#S#",3)==0)

                              fprintf (Out,
                                       "	catx = strxcat((char *) tmp%d, (char *) %s); // a2926\n",
                                       iiitmp, Extract2(t));

                        else
                              fprintf (Out,
                                       "	catx = strxcat((char *) tmp%d, (char *) %s); // b\n",
                                       iiitmp, t);

                        }

                  j = 0;

                  if (v1d[i]==0) v1d[i]=207;

                  sprintf (h, "%2x", v1d[i]);

                  t[j++] = '\\';
                  t[j++] = 'x';
                  t[j++] = h[0];
                  t[j++] = h[1];
                  t[j] = 0;

                  fprintf (Out, "	strcat((char *) catx, (char *) \"%s\"); //a3956\n",
                           t);

                  if (v1d[++i] != 0) continue;
                  else break;
                  }
            }

      else *GBL_flg = 0;
      iiitmp++;
      memcheck(iiitmp);
      return;
      }

