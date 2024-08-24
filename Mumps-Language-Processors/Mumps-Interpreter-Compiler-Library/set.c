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

//	May 26, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int     set_code(int entry_pt) {

      if (entry_pt == SET_STORE) goto set_store;

set:

            {
            int i;
            for (i=0; i<lhsx; i++) free (lhs[i]);
            }

      lhsf=lhsx=0;

      if (xd[xpx]=='(') {
            lhsf=1;
            xpx++;
            }

      Set_flg = 0;

      at_flg=atTest(xd, pd1, &pd1Last, lines, xpx);

      if (at_flg==99) return ABRTX; // goto abrtx;

      if (at_flg && retz==READ) {

            char tmp[1024];
            int i, j;

            strcpy (tmp, "r ");
            atCopy (at_flg, tmp);

            INTERPRET

            xpx = at_flg;

            if (POST_flg) {
                  fprintf (Out, "	} // post conditional 2097\n");
                  POST_flg = 0;
                  }

            return 0; // goto next_cmnd;
            }

      if (at_flg) {

            char tmp[1024];
            int i, j;

            strcpy (tmp, "s ");
            atCopy (at_flg, tmp);

            INTERPRET

            xpx = at_flg;
            if (POST_flg) {
                  fprintf (Out, "	} /* post conditional 2116*/\n");
                  POST_flg = 0;
                  }
            return 0; // goto next_cmnd;
            }
nxt_lhs:

      t0px = 1;
      t2 = 1;
      setname[0] = 0;

      if ((ierr = parse_ ())) return PARSE_ERROR;

      strcpy (vd, &v1d[1]);

      LocalArray (vd, &Set_flg);    //* check and process local array ref

      if (v1d[1] == '^')
            if (retz==NEW1 || retz==NEW) return NO_GLOBAL_PERMITTED; // goto nogblpermitted;

      GlobalArray(v1d, &GBL_flg, vd);    //* check and process global array

      if (retz == KILL) return 0; //goto kill_return;
      if (retz == NEW) return 0; //goto new_return;
      if (retz == NEW1) return 0; //goto new1_return;
      if (retz == LOCK) return 0; //goto lock_return;

      if (lhsx>24) return TOO_MANY_LHS; // goto too_lhs;
      lhs[lhsx]=malloc(strlen(vd)+1);
      if (lhs[lhsx]==NULL) return MEM_ERROR; // goto mem_err;
      strcpy(lhs[lhsx],vd);
      lhsGF[lhsx]=GBL_flg;
      lhsx++;

      if (xd[xpx+1]==',' || xd[xpx+1]==')') {
            if (xd[xpx+1]!=')') {
                  xpx+=2;
                  goto nxt_lhs;
                  }
            }

      if (lhsf) xpx++;

      if (retz == READ) return 0; // goto set_return;

      xpx += 2;
      t0px = 0;
      t2 = 1;

      if (MergeFlg) {
            t0px=1;
            setname[0]='\0';
            fprintf (Out, "	strcpy((char *) tmp%d, (const char *) atmp); /* hold result */\n",iiitmp);
            MergeTmp=iiitmp++;
            memcheck(iiitmp);
            }

      if ((ierr = parse_ ())) return PARSE_ERROR; /* process RHS */

      if (MergeFlg) {

            strcpy (vd, &v1d[1]);

            LocalArray (vd, &Set_flg);    //* check and process local array ref
            GlobalArray(v1d, &GBL_flg, vd);    //* check and process global array

            if (strcmp(vd, "atmp") == 0) sprintf(lhs[0], "tmp%d", MergeTmp);

            fprintf (Out, "	f=Merge(%s, %s, svPtr); \n", lhs[0], vd);

            fprintf (Out, "	if (f != 0) ErrorMessage(\"Merge failed \", "
                     "svPtr->LineNumber); \n");

            xpx++;
            goto rhs1;
            }

set_store:

      for (i = 0; i < lhsx; i++) {

            GBL_flg = lhsGF[i];

            if (GBL_flg) {

                  if (retz == READ ) {

                        fprintf (Out, "\n");
                        fprintf (Out, "	{ \n");

                        fprintf (Out, "	unsigned char seg[STR_MAX]; \n");

                        fprintf (Out, "	strcpy(seg, %s); \n",
                                 Extract2(&pd1[sdlim] ) );

                        fprintf (Out, "	f = Mglobal(STORE, %s, seg, svPtr); \n",
                                 Extract2(lhs[i] ) );

                        fprintf (Out, "	} \n");
                        fprintf (Out, "\n");

                        }

                  if (retz == SET ) {

                        fprintf (Out, "\n");
                        fprintf (Out, "	{ \n");

                        fprintf (Out, "	f = Mglobal(STORE, %s, %s, svPtr); \n",
                                 Extract2(lhs[i]), Extract2(&pd1[sdlim] ) );

                        fprintf (Out, "	} \n");
                        fprintf (Out, "\n");
                        }

                  if (retz == LOCK ) fprintf (Out, "	f=Mglobal(LOCK, %s, \"\", lhs[i], svPtr); \n");
                  }

            else if (vd[0] == '$' && 
		(tolower (vd[1]) == 'p' || 
		tolower(vd[1]) == 'e')) { // Process LHS $PEICE

                  unsigned char var[256];
                  unsigned char pat[256];
                  unsigned char strt[256];
                  unsigned char end[256];
                  unsigned char rstr[256];

                  for (i = 0; vd[i] < 128; i++);
                  strcpy (var, &vd[i + 1]);
                  for (j = 0; var[j] < 128; j++);
                  var[j] = 0;

                  for (i++; vd[i] < 128; i++);
                  strcpy (pat, &vd[i + 1]);
                  for (j = 0; pat[j] < 128; j++);
                  pat[j] = 0;

                  for (i++; vd[i] < 128 && vd[i] != 0; i++);
                  strcpy (strt, &vd[i + 1]);
                  for (j = 0; strt[j] < 128; j++);
                  strt[j] = 0;

                  for (i++; vd[i] < 128 && vd[i] != 0; i++);

                  if (vd[++i] == 0) strcpy (end, "");

                  else {
                        strcpy (end, &vd[i]);
                        for (j = 0; end[j] < 128; j++) ;
                        end[j] = 0;
                        }

                  strcpy(pat, Extract2(pat)); 
                  strcpy(strt, Extract2(strt)); 
                  strcpy(end, Extract2(end)); 
                  strcpy(rstr, Extract2(&pd1[sdlim]));

//--------------------
//	PIECE LHS
//--------------------

		if (tolower (vd[1]) == 'p' ) {
                  if (strlen(strt) == 0) strcpy(strt,"\"1\"");

 			char tmp[1024];

//---------------------------------------
//	first character is a " character
//	last character is a " character
//---------------------------------------

			strcpy(tmp, Extract1(var));

			if (tmp[0] == '\"') { // variable name not rslt of eval

				strcpy(tmp, &tmp[1]);
				tmp[strlen(tmp) - 1] = 0; // remove final quote; start with index 1

				if (strlen(end) == 0) // 3 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; "
						"sprintf(x, \"s $p(%s,\\\"%%s\\\",\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s, %s);\n",  
						tmp, pat, Extract2(strt), &pd1[sdlim]);

				else  // 4 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; "
						"sprintf(x, \"s $p(%s,\\\"%%s\\\",\\\"%%s\\\",\\\"%%s\\\")=\\\"%%s\\\" \", "
						"%s, %s, %s, %s); \n",  
						tmp, pat, Extract2(strt), end, &pd1[sdlim]);

				fprintf (Out, "       _jj = Interpret( (CCHR) x, svPtr); \n"); 
				fprintf (Out, "       svPtr->ERROR = _jj; \n"); 
				fprintf (Out, "       if (!svPtr->NOERR && _jj < 0) \n"); 
				fprintf (Out, "               ErrorMessage(\"Indirection error\", svPtr->LineNumber); \n");
	
				fprintf(Out, "};\n");
				}

			else { // eval result

				if (strlen(end) == 0) // 3 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; "
						"sprintf(x, \"s $p(%s,\\\"%%s\\\",\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s, %s);\n",  
						tmp, pat, Extract2(strt), &pd1[sdlim]);

				else  // 4 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; "
						"sprintf(x, \"s $p(%s,\\\"%%s\\\",\\\"%%s\\\",\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s, %s, %s); \n",  
						tmp, pat, Extract2(strt), end, &pd1[sdlim]);

				fprintf (Out, "       _jj = Interpret( (CCHR) x, svPtr); \n"); 
				fprintf (Out, "       svPtr->ERROR = _jj; \n"); 
				fprintf (Out, "       if (!svPtr->NOERR && _jj < 0) \n"); 
				fprintf (Out, "               ErrorMessage(\"Indirection error\", svPtr->LineNumber); \n");
	
				fprintf(Out, "};\n");
				}

			}


//-------------------
//	EXTRACT LHS
//-------------------

		else if (tolower (vd[1]) == 'e' ) { // $extract

 			char tmp[1024];

//---------------------------------------
//	first character is a " character
//	last character is a " character
//---------------------------------------

			strcpy(tmp, Extract1(var));

			if (tmp[0] == '\"') { // variable name not rslt of eval
				strcpy(tmp, &tmp[1]);
				tmp[strlen(tmp) - 1] = 0; // remove final quote; start with index 1

				if (strlen(strt) != 0) // 3 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; "
						"sprintf(x, \"s $e(%s,\\\"%%s\\\",\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s, %s); \n",  
						tmp, pat, Extract2(strt), &pd1[sdlim]);

				else  // 2 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; sprintf(x, \"s $e(%s,\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s); \n",  
						tmp, pat, &pd1[sdlim]);

				fprintf (Out, "       _jj = Interpret( (CCHR) x, svPtr); \n"); 
				fprintf (Out, "       svPtr->ERROR = _jj; \n"); 
				fprintf (Out, "       if (!svPtr->NOERR && _jj < 0) \n"); 
				fprintf (Out, "               ErrorMessage(\"Indirection error\", svPtr->LineNumber); \n");

				fprintf(Out, "};\n");
				}

			else { // rslt of exp eval

					if (strlen(strt) != 0) // 3 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; "
						"sprintf(x, \"s $e(%%s,\\\"%%s\\\",\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s, %s, %s); \n",  
						tmp, pat, Extract2(strt), &pd1[sdlim]);

				else  // 2 arg form
					fprintf(Out, "\n"
						"{ char x[STR_MAX]; sprintf(x, \"s $e(%%s,\\\"%%s\\\")=\\\"%%s\\\" \", %s, %s, %s); \n",  
						tmp, pat, &pd1[sdlim]);

				fprintf (Out, "       _jj = Interpret( (CCHR) x, svPtr); \n"); 
				fprintf (Out, "       svPtr->ERROR = _jj; \n"); 
				fprintf (Out, "       if (!svPtr->NOERR && _jj < 0) \n"); 
				fprintf (Out, "               ErrorMessage(\"Indirection error\", svPtr->LineNumber); \n");

				fprintf(Out, "};\n");
				}
			}

                  }

            else {

                  if (cvar(lhs[i])) {
                        fprintf (Out, "	strcpy( (char *) %s, (const char *) %s); // Store result s264\n",
                                 lhs[i], Extract2(&pd1[sdlim]));
                        }

                  else {

                        if (Set_flg)
                              fprintf (Out, "	sym_(SYMSTORE, (UNS) %s, "
                                       "(UNS) %s, svPtr); // Store result s270 \n",
                                       lhs[i], Extract2(&pd1[sdlim]));

                        else fprintf (Out, "	sym_(SYMSTORE, (UNS) \"%s\", "
                                            "(UNS) %s, svPtr); // Store result s274 \n",
                                            lhs[i], Extract2(&pd1[sdlim]));
                        }
                  }
            } // lhs

rhs1:

      if (retz == READ) return 0; //goto cont_read;

      if (xd[xpx] == ',') {
            xpx++;
            goto set;
            }

      if (POST_flg) {
            fprintf (Out, "	} /* post conditional 2313*/\n");
            POST_flg = 0;
            }

      return 0; // goto next_cmnd;
      }
