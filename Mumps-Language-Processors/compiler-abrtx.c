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

//	May 17, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int     abrtx() {


      if (MAIN_flg == 0 && FCN_flg == 0) {
            printf ("\n*** Warning: No Mumps Zmain or Zfunction\n");
            ExitCode = 0;
            }

      if (MAIN_flg) {
            epi (ExecName, pid);
            MAIN_flg = 0;
            }

      else {
            if (FCN_flg) {
                  EP(1,ExecName, pid);
                  F_flg = 0;
                  FCN_flg = 0;
                  }
            else {
                  }
            }


            {

            int i=0;
            fclose (Out);

            printf ("%d lines of Mumps;\n", lines - 1);
            Out = fopen(output_file, "r");

            output_file[strlen(output_file) - 2] = '\0';
            strcat(output_file,".cpp");
            Final = fopen(output_file, "w");
		if (Final == NULL) {
			printf("Unable to open %s. Check protections.\n",
				output_file);
			abort();
			}

            while (fgets(tmp2, 1024, Out) != NULL) {
                  i++;
                  if (strncmp(tmp2,"INCLUDE",7)==0) {
                        char file[64];
                        FILE *incl;
                        tmp2[strlen(tmp2)-1] = '\0';
                        strcpy(file,&tmp2[8]);
                        incl = fopen(file,"r");
                        if (incl != NULL) {
                              while (fgets(tmp2,1024,incl)!=NULL) {
                                    i++;
                                    fputs(tmp2,Final);
                                    }
                              fclose(incl);
                              remove(file);
                              }
                        continue;
                        }
                  fputs(tmp2,Final);
                  }

            fclose(Out);
            fclose(Final);

            output_file[strlen(output_file)-2] = '\0';
            strcat(output_file,".m");
            remove(output_file);   /* remove intermediate .m file */
            printf ("%d lines of C++ generated\n",i);
            }

      return ExitCode;

      }
