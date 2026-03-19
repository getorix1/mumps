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

int     lock_code() {

      int return_code;

            {
            // lock group

            char Lf=' ';
            char to[32];

            if (xd[xpx] == ' ' || xd[xpx] == 0) { /* no argument form */
                  fprintf (Out, "	Mltoa(getpid(),(char *) atmp);\n");
                  fprintf (Out, "	if (svPtr->LockDone) cleanLocks((char *) atmp); "
                           "/* remove locks for this job */\n");
                  fprintf(Out,"	svPtr->LockDone = 0; /* indicates a lock has been "
                          "performed */\n");
                  goto lck1;
                  }

NxtLckArg:

            fprintf(Out,"	svPtr->LockDone = 1; /* indicates a lock has been "
                    "performed */\n");
            LockFlag=1;
            retz = LOCK;
            if (xd[xpx]=='+' || xd[xpx]=='-') Lf=xd[xpx++];
            else Lf=' ';
            if (xd[xpx] != '^') return BAD_LOCK; // goto bad_lock;
//            goto set;

            return_code = set_code(SET); // goto set;  // process item to read

            if (return_code == 0) goto lock_return;
            if (return_code == PARSE_ERROR) return PARSE_ERROR; // goto parse_err;
            if (return_code == ARG_LIST_ERROR) return ARG_LIST_ERROR; // goto arg_list_error;
            if (return_code == NO_GLOBAL_PERMITTED) return NO_GLOBAL_PERMITTED;
            if (return_code == MEM_ERROR) return MEM_ERROR; //goto mem_err;
            if (return_code == TOO_MANY_LHS) return TOO_MANY_LHS; // goto too_lhs;
            if (return_code == ABRTX) return ABRTX; // goto abrtx;


lock_return:

            fprintf(Out, "	keyfix(%s);\n",vd);

            strcpy(to,"NULL");
            if (xd[xpx+1] == ':') { /* timeout */
                  t2 = 1;
                  xpx += 2;
                  if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
                  fprintf ( Out, " // TIMEOUT REQUESTED FOR %s SECONDS\n", &pd1[sdlim]);
                  strcpy(to,&pd1[sdlim]);
                  xpx--;
                  }

            fprintf (Out, "	Mltoa(getpid(),atmp);\n");
            fprintf (Out,"	if (strcmp((const char *) makeLock((char *) %s,(char *) \"1\", "
                     "(char *) atmp,\'%c\',(char *) %s),(const char *) \"1\")==0) "
                     "svPtr->tpx=1; else svPtr->tpx=0;\n",
                     vd,Lf,to);

lck1:
            xpx++;
            dirflg = 0;
            if (xd[xpx] == ',') {
                  xpx++;
                  goto NxtLckArg;
                  }

            if (xd[xpx] == 0 || xd[xpx] == ' ' || xd[xpx] == '\t') {

                  if (POST_flg) {
                        fprintf (Out, "	} /* post conditional in break */\n");
                        POST_flg = 0;
                        }

                  return 0; // goto next_cmnd;
                  }

            return 0; // goto next_cmnd;

            } /* lock group */

      }
