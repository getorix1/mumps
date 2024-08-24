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


void LocalArray (unsigned char vd[], short *Set_flg) {

//#+ Process array reference

      int i, k, j;
      char tmp[1024];

      if (v1d[1] == '^' || v1d[1] == '$') return;

      for (i = 0; vd[i] != 0; i++) if (vd[i] > 127) goto has_array;

      return;     //* Not an array reference - no open paren

//#+ Process LHS style local array reference

has_array:

      fprintf (Out, "	{	char tmp[STR_MAX];  // expand lhs array reference\n"
               "		int i;\n");

      for (i = 0; vd[i] != 0; i++)
            if (vd[i] == 206) { //Open
                  vd[i] = 0;
                  fprintf (Out, "		strmove((UNS) tmp, (UNS) \"%s\"); // array name\n", vd);
                  fprintf (Out, "		i=strlen(tmp);\n");
                  fprintf (Out, "		tmp[i]=206; // coded open paren\n");
                  fprintf (Out, "		tmp[i+1]=0;\n");
                  vd[i] = 206;

                  for (j = i + 1; vd[j] != 0; j++)
                        if (vd[j] == 207 || vd[j] == 208) break;

                  k = vd[j];
                  vd[j] = 0;
                  if (strncmp(&vd[i+1],"#G#",3)==0 || strncmp(&vd[i+1],"#S#",3)==0) {
                        fprintf (Out,"	");
                        fprintf (Out, "	strcat((char *) tmp,(char *) %s); // index\n", Extract2(&vd[i + 1]));
                        }
                  else fprintf (Out, "	strcat((char *) tmp,(char *) %s); // index\n", &vd[i + 1]);
                  fprintf (Out, "		i=strlen(tmp);\n");
                  fprintf (Out, "		tmp[i]=%d; // coded comma or close paren\n", k);
                  fprintf (Out, "		tmp[i+1]=0;\n");
                  vd[j] = k;
                  i = j - 1;
                  }

            else if (vd[i] == 207) {			// Close
                  fprintf (Out, "	strmove((UNS) atmp, (UNS) tmp); // preserve expanded array reference\n");
                  *Set_flg = 1;
                  fprintf (Out, "		}\n");
                  strcpy (vd, "atmp");
                  break;
                  }

            else if (vd[i] == 208) {			/* comma */
                  for (j = i + 1; vd[j] != 0; j++)
                        if (vd[j] == 207 || vd[j] == 208) break;
                  k = vd[j];
                  vd[j] = 0;
                  if (strncmp(&vd[i+1],"#G#",3)==0 || strncmp(&vd[i+1],"#S#",3)==0) {
                        fprintf (Out,"	");
                        fprintf (Out, "		strcat((char *) tmp,(char *) %s); // index\n", Extract2(&vd[i + 1]));
                        }
                  else fprintf (Out, "		strcat((char *) tmp,(char *) %s); // index\n", &vd[i + 1]);
                  fprintf (Out, "		i=strlen((const char *) tmp);\n");
                  fprintf (Out, "		tmp[i]=%d; // coded comma or close paren\n", k);
                  fprintf (Out, "		tmp[i+1]=0;\n");
                  vd[j] = k;
                  i = j - 1;
                  }
      }
