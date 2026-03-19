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

//	June 16, 2024

#include <stdio.h>

#include <mumpsc/memsize.h>

#include <mumpsc/compiler-macros.h>

#include <mumpsc/compiler-globals-ext.h>

int     for_code() {

      for_fcn_flg=0;
      for_fcn_flg2=0;
      forxFLG=1;
      forLine=1;
      if (xd[xpx] == ' ') {			//* argumentless form
            linepad (fprintf (Out, "	while(1) {"), "Argumentless FOR");
            if (xd[xpx + 1] != '{') IF_flg++; // what is this?
            return 0; // goto next_cmnd;
            }

      for_mult = -1;
      SinglesFlg = 0;

for_again:

      t0px = 1;

      if ( xd[xpx] == '$' ) { // while case - no start, no finish
            strcpy(forv,"");
            forx++;
            if (forx > forxmax) forxmax=forx;

            fprintf(Out, "	strcpy(for%d_lim, \"\");\n", forx); // empty string limit

            goto while01;
            }

      if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;

//*   check if FOR variables allocated

      forx++;
      if (forx > forxmax) forxmax=forx;

      fprintf(Out, "	strcpy(for%d_lim, \"\");\n", forx); // empty string limit

      xpx += 2;
      strcpy (forv, &v1d[1]);	// retain loop index variable name

for_again2:

      if ( xd[xpx] == '$' ) { // for i=$xxx(aaa) case
            for_fcn_flg=1;  // a function
            for_fcn_flg2=1;

            if (cvar(forv)) ;

            else fprintf (Out,
                                "	sym_(SYMSTORE, (UNS) \"%s\", (UNS) \"\", svPtr); "
                                "// init $ case 2518 \n",
                                forv);

            fprintf (Out, "\n");
            fprintf (Out, "For_Group%d: // m2522\n\n", forx);
            }

      t2 = 1;
      t0px = 0;
      if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;

      if (for_fcn_flg) {
            goto one_arg;
            }

      fprintf (Out,
               "	strmove((UNS) for%d_init, (UNS) %s); // init value m2534\n",
               forx, Extract2(&pd1[sdlim]));	//* initial index value

      if (xd[xpx] == ',' || xd[xpx] == '\0' || xd[xpx] == ' ') {
            fprintf (Out,
                     "	sym_(SYMSTORE, (UNS) \"%s\",(UNS) for%d_init, svPtr); "
                     "// init list loop variable\n", forv, forx);
            SinglesFlg = 1;
            goto singles;
            }

      if (xd[xpx] != ':') return ARG_LIST_ERROR; // goto arg_list_error;

      for_fcn_flg=0;  // is the increment a function?
      t2 = 1;
      xpx++;

while01:

      if ( xd[xpx]=='$' ) {

            for_fcn_flg=1;  // incr is a function

            if (strlen(forv)) {
                  if (cvar(forv))
                        fprintf (Out, "	strcpy(%s,for%d_init); // init 2559\n",
                                 forv, forx);

                  else fprintf (Out,
                                      "	sym_(SYMSTORE,(UNS) \"%s\", (UNS) for%d_init, svPtr); "
                                      "// init m2564 \n", forv, forx);
                  }
            fprintf (Out, "For_Group%d: // m2566\n",forx);
            }

      t2 = 1;
      t0px = 0;

      if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;

      fprintf (Out, "	strmove( (UNS) for%d_incr, (UNS) %s); // inc/dec m2575\n",
               forx, Extract2(&pd1[sdlim]));	//* increment amount

      if (xd[xpx] != ':') FOR_limit = 0;                                //* no upper limit

      else {
            t2 = 1;
            xpx++;
            if ((ierr = parse_ ())) return PARSE_ERROR; // goto parse_err;
            fprintf (Out, "	strmove((UNS) for%d_lim, (UNS) %s); // limit value\n",
                     forx, Extract2(&pd1[sdlim]));	//* upper limit
            FOR_limit = 1;
            }

one_arg:

      if (!for_fcn_flg) {

//--------------------------------------------
//	extract and setup increment/decrement
//--------------------------------------------

            fprintf (Out, "\n	if (strchr((char *) for%d_incr, '.')) // inc/dec is float\n"
                     "		for%dF = atof(for%d_incr) < 0.0 ? 1 : 0; // incr or decr?\n",
                     forx, forx, forx, forx, forx);

            fprintf (Out, "	else // inc/dec is integer\n"
                     "		for%dF = atol(for%d_incr) < 0 ? 1 : 0; // incr or decr?\n",
                     forx, forx, forx);

//----------------------------------------
//	start loop and init loop variable
//----------------------------------------

            if (cvar(forv)) {
                  fprintf (Out, "\n	for (	strcpy(%s, for%d_init); // init 2613\n",
                           forv, forx);
			}

            else {
			fprintf (Out,
                                 "\n	for ( sym_(SYMSTORE, (UNS) \"%s\", (UNS) for%d_init, svPtr);"
                                 " // init 2618\n",
                                 forv, forx);
		}

//----------------------------
//	if there is a limit
//----------------------------

            fprintf(Out, "\n");

            if (FOR_limit) {
                  if (cvar(forv))

                        fprintf (Out,
                                 "		for%dF ? \n"
                                 " numcomp((UNS) %s, (UNS) for%d_lim) >= 0\n : \n"
                                " numcomp((UNS) %s, (UNS) for%d_lim) <= 0; \n\n",
                                 forx, forv, forx, forv, forx);

                  else

                        fprintf (Out,
                                 "		for%dF ? // loop limit test direction\n"
                                 "			numcomp((UNS) sym_(11, (UNS) \"%s\", \n"
                                 "				 _dummy, svPtr),\n"
                                 "				(UNS) for%d_lim) >= 0 // GE limit value?\n"
                                 "			: // ternary operator\n"
                                 "			numcomp((UNS) sym_(11, (UNS) \"%s\", \n"
                                 "				_dummy,svPtr),\n"
                                 "				(UNS) for%d_lim) <= 0; // LE limit value\n\n",
                                 forx, forv, forx, forv, forx);
                  }

//-----------------
//	no limit
//-----------------

            else {
                  fprintf (Out, "		1;  // limit expression - no limit\n\n");
                  }

//-------------------------------
//	increment or decrement
//-------------------------------

            if (cvar(forv))
                  fprintf (Out,
                           "	add(%s, for%d_incr, %s)\n	)\n\n", 
                           forv, forx, forv);

            else

                  fprintf (Out,

                           "		add( sym_(11, (UNS) \"%s\", _dummy, svPtr), // inc/dec loop var\n"
                           "			for%d_incr, (char *) _dummy),\n"
                           "		sym_(SYMSTORE, (UNS) \"%s\", (UNS) _dummy, svPtr) ) // store rslt\n\n",

                           forv, forx, forv);

singles:

            if (for_mult >= 0) {

		fprintf (Out,
                           "		if ( (_i = setjmp(for%dJ)) == 0) goto For%d; // else continue\n",
                           for_mult, for_mult);

		fprintf(Out, "	else if (_i == 100) goto For%dExit; // f264\n\n", forx);
		}

            else  {
		fprintf (Out,
                    "		if ((_i = setjmp(for%dJ)) == 0) goto For%d; // else continue\n\n",
                    forx, forx);

		fprintf(Out, "	else if (_i == 100) goto For%dExit; // f273\n\n", forx);
		}

            if (xd[xpx] == ',') {

                  xpx++;
                  for_mult = forx;
                  goto for_again2;

                  if (SinglesFlg) {
                        goto for_again2;
                        }

                  else goto for_again;
                  }

            }

      else {  // function increment section

            if ( strlen(forv) && ! for_fcn_flg2 ) {

                  if (cvar(forv)) {
                        fprintf (Out, "	strcpy(%s,for%d_incr); // set loop variable m2711\n",
                                 forv, forx);
                        }

                  else {

                        fprintf (Out, "	sym_(SYMSTORE, (UNS) \"%s\", "
                                 "(UNS) for%d_incr, svPtr);"
                                 " // set loop variable m2719\n",
                                 forv, forx);

                        }

                  fprintf (Out, "	if (strcmp( for%d_incr, for%d_lim) == 0) "
                           "goto For%dExit; // exit if match 2725\n\n",
                           forx,forx,forx);
                  }

            else if ( strlen(forv) && for_fcn_flg2 ) {

//-------------------------------------
//	declared or regular variable?
//-------------------------------------

                  if (cvar(forv)) { // declared
                        fprintf (Out, "	strcpy(%s, CSTR %s);"
                                 " // set loop dcl var $ case\n",
                                 forv, Extract2(&pd1[sdlim]));

                        fprintf (Out,
                                 "	if (strcmp( (char *) %s, \"0\") == 0 || "
                                 "strlen( (const char *) %s) == 0 ) \n"
                                 "		goto For%dExit; // exit if match 2743\n",
                                 Extract2(&pd1[sdlim]), Extract2(&pd1[sdlim]),forx);

                        }

                  else { // regular

                        fprintf (Out,
                                 "	sym_(SYMSTORE, (UNS) \"%s\", "
                                 "(UNS) %s, svPtr); // set loop variable m2752\n",
                                 forv, Extract2(&pd1[sdlim]));

                        fprintf (Out,
                                 "	if (strlen( (const char *) %s) == 0 ) "
                                 "goto For%dExit; // exit if empty 2758\n",
                                 Extract2(&pd1[sdlim]), forx);
                        }
                  }

            else {
                  fprintf (Out,
                           "	if (strcmp( for%d_incr, \"0\")==0 || "
                           "strlen(for%d_incr)==0 ) goto For%dExit;"
                           " // exit if match 2767\n",
                           forx,forx,forx,forx);
                  }

            fprintf (Out, "	if ( setjmp( for%dJ ) == 0 ) goto For%d; \n", forx, forx);


//-------------------------------------------------------------
//	$order optimization hack
//	Purpose: to get the names of the tmp variables
//	used in the initial GlobalOrder() call. This
//	permits using the value returned by GlobalOrder()
//	as the next input to GlobalOrder() without
//	rebuilding the global array reference.
//-------------------------------------------------------------


            fprintf (Out, "	goto For_Group%d; // m2771\n", forx);

            }

      if (xd[xpx] == ' ' ||

                  xd[xpx] == 0 ||
                  xd[xpx] == '\t') {

            if (xd[xpx + 1] != '{') IF_flg++;

            if (for_mult >= 0) {
                  ForBlocks[IF_flg] = for_mult;

                  fprintf (Out, "	goto For%dExit; // done: bypass block \n\n", for_mult);

                  fprintf (Out, "For%d: // for loop entry point A\n\n",  for_mult);

                  ForBlocks[IF_flg]=forx;
                  Blocks[IF_flg]=FOR;
                  }

            else {
                  fprintf (Out, "	goto For%dExit; // bypass block \n\n", forx);

                  fprintf (Out, "For%d: // for loop body entry point B\n\n", forx);

                  ForBlocks[IF_flg]=forx;
                  Blocks[IF_flg]=FOR;
                  }

            fprintf (Out,
                     "	do { // for loop body\n\t\tsvPtr->LineNumber = %d; \n",
                     lines);

            return 0; // goto next_cmnd;
            }

      return ARG_LIST_ERROR; // goto arg_list_error;

      }
