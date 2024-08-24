//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+ Mumps Compiler Run-Time Support Functions
//+ Copyright (c) 2001 2002 2006 2007 2008 2012 2103 2014 2015, 2016, 2017
//+ by Kevin C. O'Kane
//+
//+ kc.okane@gmail.com
//+ http://threadsafebooks.com/
//+ http://www.cs.uni.edu/~okane/
//+
//+ This library is free software; you can redistribute it and/or
//+ modify it under the terms of the GNU Lesser General Public
//+ License as published by the Free Software Foundation; either
//+ version 2.1 of the License, or (at your option) any later version.
//+
//+ This library is distributed in the hope that it will be useful,
//+ but WITHOUT ANY WARRANTY; without even the implied warranty of
//+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//+ Lesser General Public License for more details.
//+
//+ You should have received a copy of the GNU Lesser General Public
//+ License along with this library; if not, write to the Free Software
//+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//+
// #++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//	June June, 2024

/*************************************************************************
   interp.cpp - Mumps Runtime Library Mumps interpretation routines - can
   be used for indirection operations in the compiled code.
**************************************************************************/

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>


#include <mumpsc/defines.h>
#include <mumpsc/btree.h>
#include <sys/types.h>
#include <locale.h>
#include <monetary.h>
#include <semaphore.h>

#include <unistd.h>

#include <time.h>
#include <mumpsc/arith.h>
#include <mumpsc/globalOrder.h>

#define NATIVE



#define _INTERP_

#include <mumpsc/global.h>
#include <mumpsc/fcns.h>
#include <mumpsc/sysparms.h>
#include <mumpsc/keyfix.h>

#define GOSUBMAX 50
#define SWAP 30
#define FORMAX 30
#define EXECLEVS 40
#define LBLS 80
#define CSTR (char *)
#define USTR (unsigned char *)

#define OPEN_PAREN 206
#define COMMA 207
#define CLOSE_PAREN 209

void   lpad(unsigned char *, unsigned char *, short);
void   substr(unsigned char *, unsigned char *, int, int);
void   sigint(int);
void   LocalName(unsigned char *, unsigned char *, unsigned char *, MSV *);
void   GlobalQuery(unsigned char *, unsigned char *, char, MSV *);
void   Qsub(unsigned char *, unsigned char *, unsigned char *, MSV*);
void   Qlength(unsigned char *, unsigned char *, MSV*);
void   ErrorMessage(const char *, long);
void   zfcn(unsigned char *, unsigned char *, MSV *);
void   fcn(struct MSV*);

int    xindex(const unsigned char *, const unsigned char *, short);
int    TimeOut(int, unsigned char *, char *, int);
int    DirectRead (int, unsigned char *);
int    TimeOut_getstr1(int, unsigned char *, char *, int);
int    _SymFree(int, MSV *);
int    pm(const unsigned char *, const unsigned char *, MSV *);
int    sql(int, struct MSV *, char *DataSourceName, char *Command, char *, const char *);

char*  makeLock(char *, char *, char *, char, char *); /* global array lock service */
char*  cleanLocks(char *); /* global array lock service */


/***********************************************************
 strcmp()-style function, where aa and bb look like numbers.
     -1 if aa	< bb
      0 if aa == bb
      1 if aa	> bb
***********************************************************/

// Put numeric string into canonical format.

class InterpreterException {

      public:
            InterpreterException();
            char *what();
      private:
            char * message;
      };

// symbol table routing interface

char * sym_(int, unsigned char *, unsigned char *, MSV *);

//  Pattern handling function left over from old '?' operator code

static void patrn1(short *jpx, short *ernbr, struct MSV *);

//===========================================================================
//                                 Interpret
//===========================================================================

int Interpret (const char *parm1, struct MSV * svPtr) {

      short getstr2 (short int lun, unsigned char area[],  struct MSV * svPtr);
      void readf_ (unsigned char zd[], short pd1org, struct MSV * svPtr);
      void inout (unsigned char text[], short cmnd, short col, struct MSV * svPtr);

      long int ilong, jlong;
      short pd1hold1, pd1hold2, ibr, pd1cur, i, j, k;
      short l, New;
      static char *swp[SWAP];                       /* swap pointers */
      char XTAB[2]= {'\t', 0};

      /*----------------------------------------------
      |                                               |
      |       gosub stack                             |
      |                                               |
      |       0 - swapfile origin                     |
      |       1 - pd1cur                              |
      |       2 - addr in cmdstack                    |
      |       3 - return point [label]                |
      |       4 - original pd1len                     |
      |       5 - execlevel prior                     |
      |       8 - New Flag                            |
      |                                               |
      ------------------------------------------------*/

      short gosub[GOSUBMAX][9], gsbx, cmdx, forx, cmdxold, pd1Last;
      int NewFlag,SymInc=0;
      unsigned char cmdstack[8192];

      unsigned char lbls[LBLS][20];
      int lbloff[20], lblx, argless;

#ifdef NATIVE_SHARED
      long PulseCount = 0; // cooperative multitasking count
#endif

      unsigned char forv[FORMAX][256],
               forit[FORMAX],
               forlt[FORMAX],
               ftmp[STR_MAX];

      unsigned char forin[FORMAX][STR_MAX],
               forlv[FORMAX][256];

      unsigned short swapextent, shold;
      short ExecLevel = 1;
      short ExecLevNew[EXECLEVS] = {0};
      short BreakFlag = 0;

      unsigned char dosave[STR_MAX];
      unsigned char cod206[2] = { OPEN_PAREN, 0 };  /* open */
      unsigned char cod208 = 208;                   /* comma */
      unsigned char cod207 = 207;                   /* close */
      unsigned char cod209 = 209;                   /* comma */
      unsigned char cod210 = 210;                   /* colon */
      short retz, icmnd, p2px, ernbr, fileflg;
      unsigned char tmp2[STR_MAX], tmp3[STR_MAX], dirflg, vd[STR_MAX];
      char *pd1p;
      int MergeFlag;
      char MergeRHS[STR_MAX];

      static unsigned char *to;
      static unsigned char * from;

      int sigc (), getche (void);

      pd1p= (char *) &svPtr->pd1[1];

      for (i = 0; i < SWAP; i++) swp[i] = NULL;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// init environment: io default io unit (5=terminal).
// pd1 is user partition (PS bytes long).
// gosub is stack for do's, for's and xecutes.
// symlen is bottom of symbol table (symbol table
// grows down from top of user partition).
// PS is size of user partition.
// pd1cur is current location in user partition.
// pd1len is length of user pgm in partition.
// initialize random nbr generator.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      svPtr->io = 5;

      for (pd1cur = 0; pd1cur < PS; pd1cur++) svPtr->pd1[pd1cur] = ' ';

      svPtr->symlen = PS;
      pd1cur = 1;
      svPtr->pd1len = 0;
      srand (time(NULL));
      svPtr->bd[0]=0;
      svPtr->bd[1]=0;

      for (i = 0; i < GOSUBMAX; i++) for (j = 0; j < 9; j++) gosub[i][j] = 0;

      setlocale(LC_ALL,"en_US.UTF-8");

#if defined(SQLITE)

//----------------------------------------------------------
// sql auto connect - done at interp startup - disabled
//----------------------------------------------------------

            {
            unsigned char key[2]= {0}, bd[2]= {0};
            btree(OPEN, key, bd, svPtr );
            }

#endif

// ****************************************************************************
// *******
// ******* set trap for <interrupt>.
// ******* install default program to execute init.mps.
// *******
// ****************************************************************************

      pd1cur = 1;
      svPtr->pd1[svPtr->pd1len + 1] = 0;
      svPtr->ctrlc = 0;

// ****************************************************************************
// *******
// ******* re-enter here after error.
// ******* re-set environment.
// ******* if user error trap set, copy user error cmnd line to pgm area.
// *******
// ****************************************************************************

restart:

      BreakFlag = 0;
      ExecLevel = 1;

      for (i=0; i<EXECLEVS; i++) ExecLevNew[i]=0;

      svPtr->ierr = 0;
      svPtr->io = 5;
      svPtr->t0px = 0;
      svPtr->t2 = 0;

      for (i = 0; i < SWAP; i++) {                  // clear the swap area
            if (swp[i] != NULL) free (swp[i]);
            swp[i] = NULL;
            }

      swapextent = 1;
      lblx = 0;

// initialize execution buffer

      strcpy ( (char *) &svPtr->pd1[2], (const char *) parm1);
      svPtr->pd1[1] = TAB;
      svPtr->pd1len = strlen ( (const char *) &svPtr->pd1[1]) + 1;

      BreakFlag = 0;
      pd1cur = 1;
      cmdx = 0;
      svPtr->setpiece = 0;
      svPtr->LineNumber = 0;
      lblx = 0;
      forx = 0;
      gsbx = 0;
      svPtr->kflg = 0;
      pd1Last = 0;

// ****************************************************************************
// *******
// ******* re-enter here for each new program command line
// ******* extract next command into xd.
// ******* re-enter here for running program.
// ******* quit running program if at end.
// *******
// ****************************************************************************

next_line:

      BreakFlag=0;

      if (pd1cur > svPtr->pd1len) {
            svPtr->xd[svPtr->xpx] = 0;
            goto quit;
            }

      svPtr->PD1 = PS1 - pd1cur;

      svPtr->LineNumber ++;

      /************************
       comments or special code
      *************************/

      if (svPtr->pd1[pd1cur] == '#' ||
                  svPtr->pd1[pd1cur] == '%' ||
                  svPtr->pd1[pd1cur] == '+' ||
                  svPtr->pd1[pd1cur] == ';' ||
                  ( svPtr->pd1[pd1cur] == '/' &&
                    svPtr->pd1[pd1cur+1] == '/') ) {
            pd1cur++;
            while (svPtr->pd1[pd1cur]) pd1cur++;
            pd1cur++;
            goto next_line;
            }

      if (!svPtr->pd1[pd1cur]) {
            pd1cur++;
            goto next_line;
            }

      i = pd1cur; // remember starting place

//	skip label - find 1st blank or TAB

      while ( svPtr->pd1[pd1cur] &&
                  svPtr->pd1[pd1cur] != TAB &&
                  svPtr->pd1[pd1cur] != ' ' ) pd1cur++;

      /************
      skip multiple TABs
      ************/

      while (svPtr->pd1[pd1cur] == TAB || svPtr->pd1[pd1cur] == ' ' ) pd1cur++;

      /*********************
      copy source line to xd
      *********************/

      int dotcount;

      argless = dotcount = 0;

      while (svPtr->pd1[pd1cur+dotcount]=='.') dotcount++;

      if (dotcount + 1 < ExecLevel) goto quit1;

      if (dotcount)

            if (svPtr->pd1[pd1cur] == '.') {

                  if (strncmp ( (const char *) &svPtr->pd1[pd1cur], ". ", 2) == 0) {
                        if (ExecLevel != 2) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 2;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], ".. ", 3) == 0) {
                        if (ExecLevel != 3) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 3;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], "... ", 4) == 0) {
                        if (ExecLevel != 4) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 4;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], ".... ", 5) == 0) {
                        if (ExecLevel != 5) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 5;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], "..... ", 6) == 0) {
                        if (ExecLevel != 6) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 6;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], "...... ", 7) == 0) {
                        if (ExecLevel != 7) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 7;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], "....... ", 8) == 0) {
                        if (ExecLevel != 8) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 8;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], "........ ", 9) == 0) {
                        if (ExecLevel != 9) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 9;
                        }

                  else if (strncmp ( (const char *) &svPtr->pd1[pd1cur], "......... ", 10) == 0) {
                        if (ExecLevel != 10) {
                              while (svPtr->pd1[pd1cur++]);
                              goto next_line;
                              }
                        pd1cur += 10;
                        }
                  }

next_line_xecute:

      pd1Last = pd1cur;
//    svPtr->xpx = j = 1;

      svPtr->xpx = 1;

//    while ( (svPtr->xd[j++] = svPtr->pd1[pd1cur++]) != 0 );
//    while ( svPtr->xd[j++] = svPtr->pd1[pd1cur++] );

      to = &svPtr->xd[1];
      from = &svPtr->pd1[pd1cur];

      while ( *(to++) = *(from++) );
      *to = 0;
      pd1cur = from - svPtr->pd1;

//    svPtr->xd[j] = 0;

// -----------------------------------------------------------------------------
//
// re-enter here for each new command on current line
// check for interrupt (crrlc)
// extract and de-code command word
//
// -----------------------------------------------------------------------------

next_cmnd:       // continue line entry point

      BuildLocal(-1,0,NULL,NULL,NULL); // stack cleaner
      BuildGlobal(-1,0,NULL,NULL,NULL); // stack cleaner

      while (1) {
            if (svPtr->xd[svPtr->xpx] == 0 || svPtr->xpx >= STR_MAX)
                  if (forx > 0 && gosub[gsbx][3] == FOR) {
                        if (svPtr->ctrlc) {
                              svPtr->ctrlc = 0;
                              ernbr = 29;
                              goto err_call;
                              }
                        goto quit1;
                        }
                  else goto next_line;

            if (svPtr->xd[svPtr->xpx] != ' ' && svPtr->xd[svPtr->xpx] != TAB) break;

            svPtr->xpx++;
            }

//--------------------
// decode command word
//--------------------

      p2px = svPtr->xpx;
      svPtr->t2 = svPtr->t0px = 0;

//    if ( (icmnd = tolower (svPtr->xd[svPtr->xpx]) ) == CMMNT) {
//        svPtr->xpx=STR_MAX;
//        goto next_cmnd;
//        }

      icmnd = tolower (svPtr->xd[svPtr->xpx]);

      while (isalpha (svPtr->xd[++svPtr->xpx]));    // Ignore rest of word

      if (svPtr->xd[svPtr->xpx] == ':') {           // Postconditional
            svPtr->xpx++;

            if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

            if (!atoi ( (const char *) &svPtr->pd1[svPtr->sdlim])) {
                  while (svPtr->xd[svPtr->xpx++] != 0) {
                        if (svPtr->xd[svPtr->xpx] == '"') {
                              svPtr->xpx++;

pc1:
                              while (svPtr->xd[svPtr->xpx] != '"' &&
                                          svPtr->xd[svPtr->xpx] != 0) svPtr->xpx++;
                              svPtr->xpx++;

                              if (svPtr->xd[svPtr->xpx] == '"') {
                                    svPtr->xpx++;
                                    goto pc1;
                                    }
                              }

                        if (svPtr->xd[svPtr->xpx] == ' ') {
                              goto next_cmnd;
                              }
                        }

                  svPtr->xpx--;
                  goto next_cmnd;
                  }
            }

// -----------------------------------
//  branch to correct command handler
// -----------------------------------

      if (svPtr->xd[svPtr->xpx]) svPtr->xpx++;  // post conditional return

      switch (icmnd) {         // go to command handler

            case CMMNT:
            case '/':

                  while (svPtr->xd[svPtr->xpx++]);
                  svPtr->xpx=STR_MAX;
                  goto next_cmnd;     // comment command - do nothing

            case FCN:   // direct function execution

                  svPtr->xpx = p2px;
                  svPtr->t0px = 0;
                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  svPtr->xpx++;
                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case BREAK:      // non-standard use - end a FOR loop from a DO group

                  BreakFlag = 1;
                  goto quit;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case FILELOCK:                            // not meaningful - all files are locked.

                  if (svPtr->xd[svPtr->xpx] == ' ' || svPtr->xd[svPtr->xpx] == 0) {
                        goto next_cmnd;
                        }

                  char Lf; // type of lock

                  if (svPtr->xd[svPtr->xpx] == '-' || svPtr->xd[svPtr->xpx] == '+')
                        Lf=svPtr->xd[svPtr->xpx++];
                  else Lf=' ';

                  svPtr->t0px = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  keyfix(&svPtr->v1d[1]);
                  strcpy((char *)tmp2, (char *) &svPtr->v1d[1]); // lock reference
                  strcpy((char *) dosave, "0"); // timeout

                  svPtr->tpx = 1;

                  if (svPtr->xd[svPtr->xpx + 1] == ':') {
                        svPtr->t2 = 1;
                        svPtr->xpx += 2;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        svPtr->xpx++;
                        j = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);
                        strcpy((char *) dosave, (char *) &svPtr->pd1[svPtr->sdlim]); // timeout
                        svPtr->tpx = 0;
                        goto next_cmnd;
                        }

                  Mltoa(getpid(),tmp3);  // process id

                  if (strcmp((const char *) makeLock((char *) tmp2, (char *) "1", (char *) tmp3, Lf, (char *) dosave),
                              (const char *) "1") == 0) svPtr->tpx=1;
                  else svPtr->tpx=0;

                  svPtr->LockDone=1;;

                  svPtr->xpx++;
                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case READ:

read_command:

                  if (svPtr->io != 5 && svPtr->in_file[svPtr->io] == NULL)
                        goto io_error;

#ifdef NATIVE_SHARED
                  if (  0 ) {  // deadlock prevention
                        Mglobal(PULSE,tmp2,tmp3,svPtr);
                        }
#endif

                  dirflg = 0;

                  if (svPtr->xd[svPtr->xpx] == '@') {
                        // indirect
                        svPtr->t0px = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;
                        }

                  if (svPtr->xd[svPtr->xpx] == '!' || svPtr->xd[svPtr->xpx] == '#') {
                        // carriage control
                        if (svPtr->xd[svPtr->xpx] != '#') i = 2;
                        else i = 1;

                        goto start_io;
                        }

                  if (svPtr->xd[svPtr->xpx] == '?') { // tab format
                        svPtr->t2 = 1;
                        svPtr->xpx--;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        j = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);
                        i = 3;
                        goto start_io;
                        }

                  if (svPtr->xd[svPtr->xpx] == '"') { // text to be printed first
                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;
                        i = 4;
start_io:
                        inout (&svPtr->pd1[svPtr->sdlim], i, j, svPtr);
                        goto cont_read;
                        }

                  if (svPtr->xd[svPtr->xpx] == '*') { // direct mode
                        svPtr->xpx++;
                        dirflg = 1;
                        }

                  svPtr->t0px = 1;                      // extract variable name to be read
                  svPtr->setname[0] = 0;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  strcpy ( (char *) &vd[1], (const char *) &svPtr->v1d[1]);

//--------
// timeout
//--------

                  if (svPtr->xd[svPtr->xpx + 1] == ':') {
                        svPtr->t2 = 1;
                        svPtr->xpx += 2;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        j = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);
                        svPtr->xpx--;

                        if (j <= 0) {
                              svPtr->xpx--;
                              svPtr->tpx = 0;
                              goto cont_read;
                              }


                        if (!dirflg) {
                              char timeout[64];
                              sprintf(timeout,"%d",j);
                              svPtr->tpx = TimeOut_getstr1(svPtr->io, &svPtr->pd1[svPtr->sdlim], timeout, 0);
                              }
                        else {
                              unsigned char i;
                              svPtr->tpx=DirectRead(svPtr->io, &i);
                              sprintf((char *) &svPtr->pd1[svPtr->sdlim],"%d",i);
                              }

                        goto store;
                        }

                  svPtr->hor[svPtr->io] = 0;
                  svPtr->ver[svPtr->io]++;
                  i = svPtr->io;
                  svPtr->sdlim = svPtr->pd1len + 2;

                  if (dirflg == 1 && svPtr->io == 5) {

                        unsigned char i;
                        svPtr->tpx=DirectRead(svPtr->io, &i);
                        sprintf((char *) &svPtr->pd1[svPtr->sdlim],"%d",i);

                        }

                  else {
                        svPtr->tpx = getstr2 (i, &svPtr->pd1[svPtr->sdlim], svPtr);

                        if (svPtr->tpx >= 0) svPtr->tpx = 1;
                        else svPtr->tpx = 0;
                        }

store:
                  retz = READ;
                  goto lhsref;

cont_read:

                  svPtr->xpx++;
                  dirflg = 0;

                  if (svPtr->xd[svPtr->xpx] == ',') svPtr->xpx++;

                  if (svPtr->xd[svPtr->xpx] == ' ' ||
                              svPtr->xd[svPtr->xpx] == 0 ||
                              svPtr->xd[svPtr->xpx] == TAB) goto next_cmnd;

                  goto read_command;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case FILEOPEN:

openf:
                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  k = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);

                  if (k <= 0 || k >= 5) goto file_error;

                  if (svPtr->in_file[k] != NULL ||
                              svPtr->out_file[k] != NULL) goto file_error;

                  if (svPtr->xd[svPtr->xpx] != ':') goto arg_list_error;

                  svPtr->xpx++;
                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  for (i = 0; svPtr->pd1[svPtr->sdlim + i] != 0 && svPtr->pd1[svPtr->sdlim + i] != ','; i++)
                        tmp2[i] = svPtr->pd1[svPtr->sdlim + i];

                  if (svPtr->pd1[svPtr->sdlim + i] != ',') {

                        for (i = 0; svPtr->pd1[svPtr->sdlim + i] != 0 && svPtr->pd1[svPtr->sdlim + i] != '/'; i++)
                              tmp2[i] = svPtr->pd1[svPtr->sdlim + i];
                        }

                  tmp2[i++] = 0;

                  if (strcasecmp ( (const char *) &svPtr->pd1[svPtr->sdlim + i], "old") == 0 ) New = 0;

                  else if (strcasecmp ( (const char *) &svPtr->pd1[svPtr->sdlim + i], "new") == 0 ) New = 1;

                  else if (strcasecmp ( (const char *) &svPtr->pd1[svPtr->sdlim + i], "append") == 0) New = 2;

                  else goto arg_list_error;

                  if (New == 1) { /* output new */
                        if ((svPtr->out_file[k] = fopen ( (const char *) tmp2, "w")) == NULL) svPtr->tpx = 0;
                        else svPtr->tpx = 1;
                        goto opn1;
                        }

                  if (New == 2) { /* output append */
                        if ((svPtr->out_file[k] = fopen ( (const char *) tmp2, "a")) == NULL) svPtr->tpx = 0;
                        else svPtr->tpx = 1;
                        goto opn1;
                        }

                  if ((svPtr->in_file[k] = fopen ( (const char *) tmp2, "r")) == NULL) svPtr->tpx = 0;
                  else svPtr->tpx = 1;

opn1:

                  if (svPtr->xd[++svPtr->xpx] != ',') goto next_cmnd;

                  svPtr->xpx++;
                  goto openf;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

case ZCMDS:

                  p2px++;

                  if (svPtr->xd[p2px] <= 90) svPtr->xd[p2px] += 32;                // lower case

		if (svPtr->xd[p2px] == 'm') goto next_cmnd; // zmain

                  if (svPtr->xd[p2px] == 'g') {             // zg
                        svPtr->v1d[0] = 0;
                        Mglobal (INIT, svPtr->v1d, svPtr->bd, svPtr);
                        goto next_cmnd;
                        }

                  if (svPtr->xd[p2px] == 'a') {
                        svPtr->gpad = 0;
                        goto next_cmnd;
                        }

                  if (svPtr->xd[p2px] == 'n') {
                        svPtr->gpad = 1;
                        goto next_cmnd;
                        }

                  if (tolower(svPtr->xd[p2px]) == 'h') {             // zhalt

                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        j = atoi ( (char *) &svPtr->pd1[svPtr->sdlim]);
                        printf("\n");
                        Mglobal(CLOSE,tmp2,tmp3,svPtr);  /* Close Globals */
                        fflush(NULL);
                        _exit(j);
                        }

                  if (svPtr->xd[p2px] == 'e') {             // zexit
                        return 1;

abrt:

#ifdef TERMINATE_ON_ERROR

                        if (svPtr->NOERR) {
                              svPtr->NOERR=-1;
                              if (gsbx <= 0) goto next_cmnd;
                              goto quit1;
                              }

                        printf("\n");
                        Mglobal(CLOSE,tmp2,tmp3,svPtr);  /* Close Globals */
                        fflush(NULL);
                        _exit(ernbr);

#else

                        return 99;

#endif

                        }

                  goto next_line;                           // all others

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case IF:

                  if (svPtr->xd[svPtr->xpx] == ' ') { // no argument form
                        svPtr->xpx++;
                        if (svPtr->tpx) goto next_cmnd;
                        svPtr->xpx = STR_MAX;
                        goto next_cmnd;
                        }

next_if_arg:

                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

//            if (numcomp ( (unsigned char *) &svPtr->pd1[svPtr->sdlim], (unsigned char *) "0") == 0) {

                  if ( ! atoi ( ( char *) &svPtr->pd1[svPtr->sdlim] ) ) {
                        svPtr->tpx = 0;
                        svPtr->xpx = STR_MAX;
                        goto next_cmnd;
                        }

                  svPtr->tpx = 1;

                  if (svPtr->xd[svPtr->xpx] == ',') {
                        svPtr->xpx++;
                        goto next_if_arg;
                        }

                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case WRITE:

                  if (svPtr->io != 5 && svPtr->out_file[svPtr->io] == NULL) goto io_error;

writeproc:

                  if (svPtr->xd[svPtr->xpx] == '!' || svPtr->xd[svPtr->xpx] == '#') {
                        // new line or page*/
                        i = 1;

                        if (svPtr->xd[svPtr->xpx] != '#') i = 2;

                        svPtr->xpx++;
                        goto iosub;
                        }

                  if (svPtr->xd[svPtr->xpx] == '*') { // "*" output
                        svPtr->xpx++;
                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        svPtr->pd1[svPtr->sdlim] = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);
                        svPtr->pd1[svPtr->sdlim + 1] = 0;

                        putchar (svPtr->pd1[svPtr->sdlim]);

                        i = 4;
                        goto iosub1;
                        }

                  if (svPtr->xd[svPtr->xpx] == '?') { // tab
                        svPtr->t2 = 1;
                        svPtr->xpx++;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        j = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);
                        i = 3;
                        goto iosub;
                        }

                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  i = 4;

iosub:

                  inout (&svPtr->pd1[svPtr->sdlim], i, j, svPtr);

iosub1:

                  if (svPtr->xd[svPtr->xpx] == ',') {
                        svPtr->xpx++;
                        goto writeproc;
                        }

                  if (svPtr->xd[svPtr->xpx] == 0 ||
                              svPtr->xd[svPtr->xpx] == ' ' ||
                              svPtr->xd[svPtr->xpx] == TAB) goto next_cmnd;

                  goto writeproc;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case JOB:

                  if (fork() != 0 ) goto next_line; // we are parent - continue executing - globals remain open

                  // treat argument as GOTO otherwise - drop thru


//*******************************************
// Close Globals or Disconnect from SQL server
//********************************************

#if defined(SQLITE)

                  XTAB[0]= '\t';
                  XTAB[1]= '\0';
                  tmp2[0]='\0'; // codes to sql()
                  svPtr->tpx=sql(CLOSE, svPtr, (char *) "", (char *) "", (char *) tmp2, XTAB);

#else

                  tmp2[0]='\0';
                  tmp3[0]='\0';
                  Mglobal(CLOSE,tmp2,tmp3,svPtr);  /* Close Globals */

#endif

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case GOTO:

                  svPtr->iorg = 1;
                  retz = GOTO;

                  while(gosub[gsbx][3] == FOR) {
                        cmdx=gosub[gsbx][2];
                        gsbx--;
                        }

// zap all stacking

                  forx=0;
                  goto lbl;

goto_return:

                  forx = 0;
                  pd1cur = ibr;
                  ExecLevel=1;
                  gsbx=0;
                  goto next_line;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case MERGE:

                  MergeFlag=1;
                  goto set;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case SET:  // SQL: and SHELL: COMMANDS TOO

                  MergeFlag=0;

//            if (toupper(svPtr->xd[p2px+1])!='Q' &&
//                    toupper(svPtr->xd[p2px+1])!='H')  goto set; // must be SET command

                  if ( toupper(svPtr->xd[p2px+1]) != 'Q' &&
                              toupper(svPtr->xd[p2px+1]) != 'H' ) {  // SET

//------------------------------------------------------------------------------------------------
//------------------------ S E T -----------------------------------------------------------------
//------------------------------------------------------------------------------------------------

set:

                        if ( svPtr->xd[svPtr->xpx] == '@' ) {	// indirection
                              svPtr->kflg = 1;
                              svPtr->t0px = 1;

                              if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                              svPtr->kflg = 0;
                              }

                        ftmp[0] = 0;

                        if (svPtr->xd[svPtr->xpx] == '(') {	// group LHS
                              svPtr->xpx++;
                              vd[1] = 0;
s1:
                              svPtr->t0px = 1;

                              if ( (svPtr->ierr = parse_ (svPtr)) ) goto parse_err;

                              strcat ( (char *) ftmp,  (const char *) &svPtr->v1d[1]);
                              strcat ( (char *) ftmp, " ");
                              svPtr->xpx++;

                              if (svPtr->xd[svPtr->xpx] != ')' && svPtr->xd[svPtr->xpx]) {
                                    svPtr->xpx++;
                                    goto s1;
                                    }
                              }

                        else {
                              svPtr->t0px = 1;
                              svPtr->setname[0] = 0;

                              if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                              strcpy ( (char *) &vd[1], (const char *) &svPtr->v1d[1]);
                              }

                        svPtr->xpx += 2;

                        if (MergeFlag) svPtr->t0px = 1;
                        else svPtr->t0px = 0;

                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        if (MergeFlag) {
                              strcpy ( (char *) &MergeRHS[1], (const char *) &svPtr->v1d[1]);
                              svPtr->xpx++;
                              }
                        else MergeRHS[1]=0;

                        retz = SET;
                        k = 0;

                        if (vd[1]) goto lhsref;

s2:
                        j = 1;

                        while ((vd[j++] = ftmp[k++]) != ' ');

                        vd[j - 1] = 0;
                        goto lhsref;

set_return:

                        if (ftmp[k]) goto s2;

                        if (svPtr->xd[svPtr->xpx] == ',') {
                              svPtr->xpx++;
                              goto set;
                              }

                        goto next_cmnd;

                        }

//------------------------------------------------------------------------------------------------
//-------------------- END SET -------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

                  if (toupper(svPtr->xd[p2px+1])=='Q' ||
                              toupper(svPtr->xd[p2px+1])=='H') {

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//	SHELL and SQL COMMANDS
//----------------------------------------------------------------------
//----------------------------------------------------------------------

                        char buf[STR_MAX];
                        char filename[256];
                        int bx=0,pflag=0;

                        buf[0]=0;
                        filename[0]=0;

                        if (toupper(svPtr->xd[p2px+1])=='Q') {
                              pflag=-1; // SQL undefferentiated
                              svPtr->bd[0]=0;
                              goto next_line;
                              }

//--------------------
//	Sql Codes go here
//--------------------

                        if (strncasecmp((char *) &svPtr->pd1[pd1Last], "shell", 5) != 0) {
                              prnterr (37,svPtr,pd1cur); // unrecognized command
                              goto abrt;
                              }

//*************************************************
// shell and sql command decode - comparable setups
//*************************************************

                        if (strncasecmp((const char *)&svPtr->pd1[pd1Last],"shell/p",7)==0 ) pflag=1;

                        if (strncasecmp((const char *)&svPtr->pd1[pd1Last],"shell/g",7)==0 ) pflag=2;

                        int pd1cur=pd1Last+svPtr->xpx-2;

                        while (svPtr->pd1[pd1cur]&&svPtr->pd1[pd1cur]!=' ') pd1cur++;

                        if (svPtr->pd1[pd1cur]) pd1cur++;

                        while (svPtr->pd1[pd1cur]) {
                              if (svPtr->pd1[pd1cur]=='&' && svPtr->pd1[pd1cur+1]=='~') {
                                    pd1cur+=2;

                                    for (i=1; svPtr->pd1[pd1cur]!='~'&&svPtr->pd1[pd1cur]!=0; i++)
                                          svPtr->xd[i]=svPtr->pd1[pd1cur++];

                                    svPtr->xd[i]=0;

                                    if (svPtr->pd1[pd1cur]=='~') pd1cur++;

                                    svPtr->t2=1;
                                    svPtr->xpx=1;

                                    if ((svPtr->ierr=parse_(svPtr))) goto parse_err;
                                    else {
                                          strcat(buf,(const char *) &svPtr->pd1[svPtr->sdlim]);
                                          bx=strlen(buf);
                                          }

                                    continue;
                                    }

                              buf[bx++]=svPtr->pd1[pd1cur];
                              buf[bx]=0;
                              pd1cur++;
                              }

                        pd1cur++;
                        buf[bx]=0;

                        if (!pflag) {  // SHELL command with no options
                              if (system(buf)<0) svPtr->tpx=0;
                              else svPtr->tpx=1;
                              }

//********
// SHELL/P
//********

                        else if (pflag == 1) { // SHELL/P command

                              svPtr->in_file[6]=popen(buf,"r");
                              }

//********
// SHELL/G
//********

                        else {  // SHELL/G

                              svPtr->out_file[6]=popen(buf,"w");

                              if (svPtr->out_file[6]==NULL) svPtr->tpx=0;
                              else svPtr->tpx=1;
                              }

                        goto next_line;
                        }

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case QUIT:

quit:

                  if (gsbx <= 0) goto restore;

                  if (gosub[gsbx][0] > 0) {
                        lblx = 0;                             /* zap labels */

                        memcpy (pd1p, swp[gosub[gsbx][0]], (size_t) gosub[gsbx][4]);
                        svPtr->pd1[gosub[gsbx][4] + 1] = 0;
                        swapextent = gosub[gsbx][0];

                        if (gosub[gsbx][5] > 0) svPtr->symlen = gosub[gsbx][5];
                        }

                  if (forx > 0 && gosub[gsbx][3] == FOR) {
                        cmdx = gosub[gsbx][2];
                        gsbx--;
                        forx--;
                        svPtr->xpx = STR_MAX;
                        goto next_cmnd;
                        }

quit1:

                  pd1cur = gosub[gsbx][1];

                  if (gsbx <= 0 ) {
                        printf("exit\n");
                        return 1;
                        }

                  cmdxold = cmdx;
                  cmdx = gosub[gsbx][2];

                  if (gosub[gsbx][3]==DO && BreakFlag) svPtr->xd[1]=0;
                  else strcpy ( (char *) &svPtr->xd[1], (const char *) &cmdstack[cmdx]);

                  svPtr->pd1len = gosub[gsbx][4];

                  if (forx > 0 ) {                          // patch to fix FOR after DO fixes
                        svPtr->xpx = 1;                       //commented out when fixing return value issue
                        }

                  retz = gosub[gsbx][3];
                  svPtr->tpx=gosub[gsbx][7];

// evaluate return args before killing symbol table - case xxx^yyy(a,b,c)

                  svPtr->args[0]=0;

                  if (retz!=FOR && svPtr->xd[svPtr->xpx] != ' ' && svPtr->xd[svPtr->xpx] != 0) {
                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

// copy processed return arg to state vector  - will be piped to parent

                        strcpy( (char *) svPtr->args, (char *) &svPtr->pd1[svPtr->sdlim]);

                        if (svPtr->fdes[0]>=0) {
                              write(svPtr->fdes[1], svPtr->args, STR_MAX);
                              close(svPtr->fdes[1]);
                              }
                        }

                  if (retz != FOR && retz != XECUTE) {
                        ExecLevNew[ExecLevel] = 0; // prevent 2 trips
                        _SymFree(1,svPtr);
                        }

                  gsbx--;

                  if (forx > 0 && retz == FOR) goto for_end;

                  if (retz == DO && ExecLevel > 1) {
                        if (ExecLevNew[ExecLevel]) {
                              _SymFree(1,svPtr); /* undo New */
                              }

                        ExecLevel--;
                        }

                  if (gosub[gsbx+1][6]) {
                        ExecLevel=gosub[gsbx+1][6];
                        gosub[gsbx+1][6]=0;
                        }

// old - permitted extra blank at EOL to cause loop

                  if (svPtr->xd[svPtr->xpx] == 0 ||
                              (svPtr->xd[svPtr->xpx] == ' ' &&
                               svPtr->xd[svPtr->xpx+1] == ' ') ) goto next_cmnd;

//        are we at end of line?

                  if (retz == XECUTE) goto xecute_return;

                  goto do_ret1;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case XECUTE:

xecute:

                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  ftmp[0] = TAB;
                  strcpy ( (char *) &ftmp[1], (const char *) &svPtr->pd1[svPtr->sdlim]);

                  if (svPtr->xd[svPtr->xpx] == ':') {
                        svPtr->xpx++;
                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        if ((k = atoi ((const char *) &svPtr->pd1[svPtr->sdlim])) == 0) {
                              if (svPtr->xd[svPtr->xpx] == ',') {
                                    svPtr->xpx++;
                                    goto xecute;
                                    }
                              else goto next_cmnd;
                              }
                        }

// #+ move line to be executed into code space above

                  l = svPtr->pd1len;
                  strcpy ( (char *) &svPtr->pd1[svPtr->pd1len + 2], (const char *) ftmp);
                  svPtr->pd1len = svPtr->pd1len + 2 + strlen ((const char *) ftmp);
                  gsbx++;

                  if (gsbx >= GOSUBMAX) {
                        goto stack_overflow;
                        }

                  gosub[gsbx][0] = 0;                       /* swap origin */
                  gosub[gsbx][1] = pd1cur;                  /* restore pd1cur */
                  gosub[gsbx][2] = cmdx;                    /* command stack address */
                  strcpy ( (char *) &cmdstack[cmdx], (const char *) &svPtr->xd[svPtr->xpx]);
                  cmdx = cmdx + strlen ((const char *) &svPtr->xd[svPtr->xpx]) + 1;
                  gosub[gsbx][4] = l;                       /* restore svPtr->pd1len */
                  gosub[gsbx][3] = XECUTE;
                  pd1cur = l + 2;
                  goto next_line_xecute;

// #+ return point - environment restored

xecute_return:

                  if (svPtr->xd[svPtr->xpx] == ' ' || svPtr->xd[svPtr->xpx] == 0) goto next_cmnd;

                  goto xecute;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case FOR:

                  /* stack set up */

                  forx++;

                  if (forx >= FORMAX) {
                        printf("OVERFLOW\n");
                        goto stack_overflow;
                        }

                  forit[forx] = 0;
                  forlt[forx] = 0;
                  forv[forx][0]= 0;

                  if (++gsbx >= GOSUBMAX) {
                        goto stack_overflow;
                        }

                  gosub[gsbx][0] = 0;                       /* swap */
                  gosub[gsbx][1] = pd1cur;
                  gosub[gsbx][2] = cmdx;
                  gosub[gsbx][3] = FOR;
                  gosub[gsbx][4] = svPtr->pd1len;

                  if (svPtr->xd[svPtr->xpx] == ' ' ) {
                        i=1;
                        strcpy( (char *) &forv[forx][0],"_dummy");
                        strcpy( (char *) &forin[forx][0],"0");
                        strcpy( (char *) &forlv[forx][0],"100");
                        svPtr->symflg = 0;                    /* store */
                        sym_ (0, (unsigned char *) &forv[forx][0], (unsigned char *) "1", svPtr);
                        forit[forx] = 1;
                        goto argless;
                        }

                  /* find local variable */

                  svPtr->t0px = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  svPtr->xpx += 2;
                  strcpy ( (char *) &forv[forx][0], (const char *) &svPtr->v1d[1]);

                  svPtr->symflg = 0;                        /* store */
                  dosave[0]=0;
                  sym_ (0, (unsigned char *) &forv[forx][0], (unsigned char *) dosave, svPtr);

nxt_arg:

                  forit[forx] = 0;
                  forlt[forx] = 0;

                  svPtr->t2 = 1;

                  if (svPtr->xd[svPtr->xpx]=='$') {         /* function */
                        forit[forx]=1;
                        forlt[forx]=1;
                        forlv[forx][0]=0; // function
                        strcpy((char *)forin[forx], (char *) &svPtr->xd[svPtr->xpx]);
                        }

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  forit[forx] = 0; // initial value
                  strcpy ( (char *) ftmp, (const char *) &svPtr->pd1[svPtr->sdlim]);

                  if (svPtr->xd[svPtr->xpx] == ',' ||
                              svPtr->xd[svPtr->xpx] == ' ' ||
                              svPtr->xd[svPtr->xpx] == 0 )     goto init_loop_var;

                  if (svPtr->xd[svPtr->xpx] != ':')    goto arg_list_error;

                        {
                        char xdtmp[STR_MAX];
                        int xdtmpx;
                        svPtr->symflg = 0;                    /* store */
                        sym_ ( 0, (unsigned char *) &forv[forx][0], (unsigned char *) ftmp, svPtr );
                        svPtr->t2 = 1;
                        svPtr->xpx++;
                        strcpy( xdtmp, (char *) &svPtr->xd[svPtr->xpx] );
                        xdtmpx = svPtr->xpx;

                        if ( (svPtr->ierr = parse_ (svPtr) ) ) goto parse_err;

                        xdtmpx = svPtr->xpx-xdtmpx;
                        xdtmp[xdtmpx] = 0;
                        forit[forx] = 1;
                        /* increment */
                        strcpy ( (char *) &forin[forx][0], (const char *) xdtmp );
                        }

                  if (svPtr->xd[svPtr->xpx] == ',' ||
                              svPtr->xd[svPtr->xpx] == 0   ||
                              svPtr->xd[svPtr->xpx] == ' ' ||
                              svPtr->xd[svPtr->xpx] == TAB) goto init_loop_var;

                  if ( svPtr->xd[svPtr->xpx] != ':') goto arg_list_error;

                  svPtr->t2 = 1;
                  svPtr->xpx++;

                  if ( (svPtr->ierr = parse_ (svPtr) ) ) goto parse_err;

                  forlt[forx] = 1; // limit
                  strcpy ( (char *) &forlv[forx][0], (const char *) &svPtr->pd1[svPtr->sdlim] );

                  if (svPtr->xd[svPtr->xpx] == ',' ||
                              svPtr->xd[svPtr->xpx] == ' ' ||
                              svPtr->xd[svPtr->xpx] == 0   ||
                              svPtr->xd[svPtr->xpx] == TAB) goto init_loop_var;

                  goto arg_list_error;

init_loop_var:

                  svPtr->symflg = 0;                        /* store */
                  // set value
                  sym_ (0, (unsigned char *) &forv[forx][0], (unsigned char *) ftmp,svPtr);

                  if (forin[forx][0]=='$') {

                        if ( strlen( (char*) ftmp) ==0 ) i = 1;
                        else i = 0;

                        forit[forx]=1;
                        goto argless;
                        }

                  else i = 0;

                  if (forlt[forx] == 1) {

                        if (numcomp (&forin[forx][0], (unsigned char *) "0") >= 0) {
                              if (numcomp (ftmp, &forlv[forx][0]) > 0) i = 1;
                              }

                        else {
                              if (numcomp (ftmp, &forlv[forx][0]) < 0) i = 1;
                              }
                        }

argless:

                  strcpy ( (char *) &cmdstack[cmdx], (const char *) &svPtr->xd[svPtr->xpx]);
                  cmdx = cmdx + strlen ( (const char *) &svPtr->xd[svPtr->xpx]) + 1;

byp:

                  while (svPtr->xd[svPtr->xpx] != 0 && svPtr->xd[svPtr->xpx] != ' ') {
                        svPtr->t2 = 1;
                        svPtr->xpx++;
                        parse_ (svPtr);
                        }

                  if (i == 0) goto next_cmnd;
                  else goto quit1;

for_end:                                  /***** end of for return *****/

                  if (forit[forx] != 1) {

for_quit:

                        if (svPtr->xd[svPtr->xpx] == ',') {
                              svPtr->xpx++;
                              gsbx++;

                              if (gsbx >= GOSUBMAX) goto stack_overflow;

                              goto nxt_arg;
                              }

                        forx--;
                        svPtr->xpx = STR_MAX;
                        goto next_cmnd;
                        }

                        {
                        /***** increment loop variable *****/

                        char itmp[STR_MAX];
                        svPtr->symflg = 1;                    /* retrieve */
                        sym_ (11, (unsigned char *) &forv[forx][0], (unsigned char *) itmp, svPtr);

                        char old[STR_MAX];
                        int oldxpx;

                        strcpy( old, (char *) &svPtr->xd[1] );
                        oldxpx = svPtr->xpx;
                        strcpy( (char *) svPtr->xd, (char *) &forin[forx][0] );
                        svPtr->xpx = 0;
                        svPtr->t2 = 1;

                        if ( ( svPtr->ierr = parse_ (svPtr) ) ) goto parse_err; // increment amount

                        unsigned char * dsve=&svPtr->pd1[svPtr->sdlim];
                        strcpy( (char *) &svPtr->xd[1], old );
                        svPtr->xpx = oldxpx;

                        if ( forin[forx][0] != '$' ) add ( (char *) itmp, (char *) dsve, (char *) ftmp);
                        else strcpy( (char *) ftmp, (char *) dsve);
                        }

                  if (BreakFlag) {
                        BreakFlag = 0;
                        goto for_quit;
                        }

                  if (forlt[forx] != 1) {
                        svPtr->symflg = 0;                    /* store */
                        sym_ (0, (unsigned char *) &forv[forx][0], (unsigned char *) ftmp,svPtr);
                        gsbx++;

                        if (gsbx >= GOSUBMAX) goto stack_overflow;

                        cmdx = cmdxold;

                        if (svPtr->xd[svPtr->xpx] == ',') {
                              i = 0;
                              goto byp;
                              }

                        goto next_cmnd;
                        }

                  /* limit test */

                  svPtr->symflg = 0;                        /* store */
                  sym_ (0, (unsigned char *) &forv[forx][0], (unsigned char *) ftmp,svPtr);

                  if (forin[forx][0]=='$') {

                        if (forlv[forx][0] == 0 ) {           // special case null string limit
                              if (strlen( (char*) ftmp) == 0 ) goto for_quit;
                              }
                        else {
                              if (strcmp((char *)ftmp,(char *)&forlv[forx][0])>=0) goto for_quit;
                              }
                        }
                  else if (numcomp ( (unsigned char *) &forin[forx][0],  (unsigned char *) "0") >= 0) {
                        if (numcomp (ftmp, &forlv[forx][0]) > 0) goto for_quit;
                        }
                  else {
                        if (numcomp (ftmp, &forlv[forx][0]) < 0) goto for_quit;
                        }

                  gsbx++;

                  if (gsbx >= GOSUBMAX) goto stack_overflow;

                  cmdx = cmdxold;

                  if (svPtr->xd[svPtr->xpx] == ',') {
                        i = 0;
                        goto byp;
                        }

                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case HALT:

                  if ((svPtr->xd[svPtr->xpx] == ' ' || svPtr->xd[svPtr->xpx] == 0)
                              && toupper(svPtr->xd[p2px+1])!='T' ) {
                        Mltoa(getpid(),tmp2);

                        if (svPtr->LockDone) cleanLocks((char *) tmp2);

                        Mglobal(CLOSE,tmp2,tmp3,svPtr);  /* Close Globals */
                        fflush(NULL);
                        _exit(EXIT_SUCCESS);
                        return EXIT_SUCCESS;
                        }

                  if (toupper(svPtr->xd[p2px+1])=='T') {   /* html command */

                        int pd1cur=pd1Last+svPtr->xpx-2;

                        while (svPtr->pd1[pd1cur]&&svPtr->pd1[pd1cur]!=' ') pd1cur++;

                        if (svPtr->pd1[pd1cur]) pd1cur++;

                        while (svPtr->pd1[pd1cur]) {
                              if (svPtr->pd1[pd1cur]=='&' && svPtr->pd1[pd1cur+1]=='~') {
                                    pd1cur+=2;

                                    for (i=1; svPtr->pd1[pd1cur]!='~'&&svPtr->pd1[pd1cur]!=0; i++)
                                          svPtr->xd[i]=svPtr->pd1[pd1cur++];

                                    svPtr->xd[i]=0;

                                    if (svPtr->pd1[pd1cur]=='~') pd1cur++;

                                    svPtr->t2=1;
                                    svPtr->xpx=1;

                                    if ((svPtr->ierr=parse_(svPtr))) goto parse_err;
                                    else printf("%s",&svPtr->pd1[svPtr->sdlim]);

                                    continue;
                                    }

                              if (svPtr->pd1[pd1cur]=='&' && svPtr->pd1[pd1cur+1]=='!') {
                                    pd1cur+=2;
                                    putchar('\n');
                                    continue;
                                    }

                              putchar(svPtr->pd1[pd1cur]);
                              pd1cur++;
                              }

                        pd1cur++;
                        goto next_line;
                        }

hang:

                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  j = atoi ( (char *) &svPtr->pd1[svPtr->sdlim]);

                  if (j > 0)

#ifdef NATIVE_SHARED
                        if ( 0 ) {  // deadlock prevention
                              Mglobal(PULSE,tmp2,tmp3,svPtr);
                              }

#endif

                  sleep (j);

                  if (svPtr->xd[svPtr->xpx] == ',') {
                        svPtr->xpx++;
                        goto hang;
                        }

                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case ELSE:

                  if (svPtr->xd[svPtr->xpx] == ' ') svPtr->xpx++;

                  if (svPtr->tpx == 0) goto next_cmnd;

                  svPtr->xpx = STR_MAX;

                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case DO: // or DATABASE

                  if (toupper(svPtr->xd[p2px+1])=='E') {    /* DECLARE command */
                        goto next_line; // ignore
                        }

                  if (toupper(svPtr->xd[p2px+1])=='A') {    /* DATABASE command */

                        svPtr->t2 = 1;

                        for (; svPtr->xd[svPtr->xpx]; svPtr->xpx++) if (svPtr->xd[svPtr->xpx]!=' ') break;

                        if ((svPtr->ierr=svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        if (strlen((const char *) &svPtr->pd1[svPtr->sdlim])>1000) goto file_error;

                        strcpy((char *) svPtr->UDAT, (const char *) &svPtr->pd1[svPtr->sdlim]);
                        strcat((char *) svPtr->UDAT,".dat");
                        strcpy((char *) svPtr->UKEY, (const char *) &svPtr->pd1[svPtr->sdlim]);
                        strcat((char *) svPtr->UKEY,".key");
                        goto next_line;
                        }

do_ret1:

                  argless=0;
                  NewFlag=0;

                  if (svPtr->xd[svPtr->xpx] == ' ' || svPtr->xd[svPtr->xpx] == 0) {
                        unsigned char tmp[1024];
                        strcpy ( (char *) tmp, "+0  ");
                        ExecLevel++;
                        ExecLevNew[ExecLevel]=0;
                        strcat ( (char *) tmp, (const char *) &svPtr->xd[svPtr->xpx]);
                        strcpy ( (char *) &svPtr->xd[1], (const char *) tmp);
                        svPtr->xpx = 1;

                        argless=1;
                        }

                  pd1hold1 = svPtr->pd1len;
                  pd1hold2 = pd1cur;
                  svPtr->iorg = 1;
                  shold = 0;
                  retz = DO;
                  goto lbl;

do_ret:

                  if (++gsbx >= GOSUBMAX) goto stack_overflow;

                  if (shold > 0) gosub[gsbx][0] = shold;
                  else gosub[gsbx][0] = 0;

                  gosub[gsbx][1] = pd1hold2;
                  gosub[gsbx][2] = cmdx;
                  gosub[gsbx][8]=NewFlag;
                  strcpy ( (char *) &cmdstack[cmdx], (const char *) &svPtr->xd[svPtr->xpx]);
                  cmdx = cmdx + strlen ( (const char *) &svPtr->xd[svPtr->xpx]) + 1;
                  gosub[gsbx][3] = DO;
                  gosub[gsbx][4] = pd1hold1;
                  pd1cur = ibr;
                  gosub[gsbx][5] = 0;
                  gosub[gsbx][7] = svPtr->tpx;

                  if (!argless) {
                        gosub[gsbx][6]=ExecLevel;
                        ExecLevel=1;
                        }

                  if (!SymInc) {  // flag indicates that inc was done already
                        svPtr->_Sym++;
                        svPtr->start[svPtr->_Sym]=NULL;
                        }

                  goto next_line;



// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case NEW:

                  svPtr->nflg=0;

new_again:

                  if (svPtr->xd[svPtr->xpx] == ' ' ||
                              svPtr->xd[svPtr->xpx] == 0 ||
                              svPtr->xd[svPtr->xpx] == TAB) {

                        /* new except none */
                        svPtr->symflg = 7;
                        svPtr->nstart = NULL;                 /* list freed in sym_() */
                        sym_ (7, (unsigned char *) "", (unsigned char *) "",svPtr);
                        svPtr->nstart = NULL;                 /* list freed in sym_() */
                        ExecLevNew[ExecLevel]=1;
                        goto next_cmnd;

                        }

                  /* ++++ New indirect */

                  if (svPtr->xd[svPtr->xpx] == '@') {
                        svPtr->kflg = 1;
                        svPtr->t0px = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        svPtr->kflg = 0;
                        }

                  /* ++++ New specific. new a,b,c */

                  if (svPtr->xd[svPtr->xpx] != '(') {
                        svPtr->t0px = 1;
                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;
                        sym_ (100, (unsigned char *) &svPtr->v1d[1], (unsigned char *) "",svPtr);
                        }

                  /* ++++ New all locals except. */
                  /* create linked lisk of var names not to be 'newed' */

                  else {
                        struct nmes *p1, *p2;
                        svPtr->nstart = NULL;
                        svPtr->xpx++;
new2:
                        svPtr->t0px = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        p1 = (struct nmes *) malloc (sizeof (struct nmes));

                        if (p1 == NULL) goto abrt;

                        p1->name = (char *) malloc (strlen ( (const char *) &svPtr->v1d[1]) + 1);
                        if (p1->name == NULL) goto abrt; // out of memory

                        keyfix (&svPtr->v1d[1]);        /* in case its an array */
                        strcpy ( (char *) p1->name, (const char *) &svPtr->v1d[1]);
                        p1->next = svPtr->nstart;
                        svPtr->nstart = p1;
                        svPtr->xpx++;

                        if (svPtr->xd[svPtr->xpx] != ')' && svPtr->xd[svPtr->xpx]) {
                              svPtr->xpx++;
                              goto new2;
                              }

                        svPtr->symflg = 7;
                        /* new except */
                        sym_ (7, (unsigned char *) "", (unsigned char *) "",svPtr);
                        svPtr->nstart = NULL;                 /* list freed in sym_() */
                        ExecLevNew[ExecLevel]=1;
                        }

                  if (svPtr->xd[++svPtr->xpx] == ',') {
                        svPtr->xpx++;
                        goto new_again;
                        }

                  goto next_cmnd;



            case DATAKILL:

// #++++ Kill all local variables

kill_again:

                  if (svPtr->xd[svPtr->xpx] == ' ' || svPtr->xd[svPtr->xpx] == 0 || svPtr->xd[svPtr->xpx] == TAB) {
                        svPtr->symflg = 4;
                        sym_ (4, (unsigned char *) "", (unsigned char *) "",svPtr);
                        goto next_cmnd;
                        }

// #++++ Kill indirect

                  if (svPtr->xd[svPtr->xpx] == '@') {
                        svPtr->kflg = 1;
                        svPtr->t0px = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        svPtr->kflg = 0;
                        }

// #++++ Kill specific - local or global

                  if (svPtr->xd[svPtr->xpx] != '(') {

                        svPtr->t0px = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        if (svPtr->v1d[1] != '^') {           /* kill a local */
                              svPtr->symflg = 2;                /* kill */
                              sym_ (2, (unsigned char *) &svPtr->v1d[1], (unsigned char *) "",svPtr);
                              }

                        else {                                /* globals */
                              keyfix (&svPtr->v1d[1]);
                              // original lngth - comp for start
                              k = strlen ( (const char *) &svPtr->v1d[1]) + 1;

                              if (svPtr->v1d[k-1] != 1 ) svPtr->v1d[k++]=1;

                              svPtr->v1d[k]=0;
                              k--;
                              strcpy ( (char *) tmp2, (const char *) &svPtr->v1d[1]);
                              /* kill first */
                              Mglobal (GKILL, &svPtr->v1d[1], svPtr->bd, svPtr);

#if !(defined(SQLITE))

k1:
                              /* kill others like it */
                              j = Mglobal (XNEXT, &svPtr->v1d[1], svPtr->bd, svPtr);

                              if (j && strncmp ((const char *) tmp2, (const char *) &svPtr->v1d[1], k) == 0) {
                                    Mglobal (GKILL, &svPtr->v1d[1], svPtr->bd, svPtr);
                                    goto k1;
                                    }

#endif

                              }
                        }

// #++++ Kill all locals except...

                  else {

                        struct nmes *p1, *p2;

                        svPtr->nstart = NULL;
                        svPtr->xpx++;

kill2:

                        svPtr->t0px = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        p1 = (struct nmes *) malloc (sizeof (struct nmes));

                        if (p1 == NULL) goto abrt;

                        p1->name = (char *) malloc (strlen ( (const char *) &svPtr->v1d[1]) + 1);

                        if (p1->name == NULL) goto abrt;

                        keyfix (&svPtr->v1d[1]);        /* in case its an array */
                        strcpy ( (char *) p1->name, (const char *) &svPtr->v1d[1]);
                        p1->next = svPtr->nstart;
                        svPtr->nstart = p1;
                        svPtr->xpx++;

                        if (svPtr->xd[svPtr->xpx] != ')' && svPtr->xd[svPtr->xpx]) {
                              svPtr->xpx++;
                              goto kill2;
                              }

                        svPtr->symflg = 5;
                        sym_ (5, (unsigned char *) "", (unsigned char *) "",svPtr);

                        svPtr->nstart = NULL;
                        }

                  if (svPtr->xd[++svPtr->xpx] == ',') {
                        svPtr->xpx++;
                        goto kill_again;
                        }

                  goto next_cmnd;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

            case FILECLOSE:

                  p2px++;

                  if (svPtr->xd[p2px] <= 90) svPtr->xd[p2px] += 32;  // lower case

                  if (svPtr->xd[p2px] == 'o') goto quit;

next_close_arg:

                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  i = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);

                  if (svPtr->in_file[i] == NULL && svPtr->out_file[i] == NULL) goto file_error;

                  if (svPtr->in_file[i] != NULL) fclose (svPtr->in_file[i]);

                  else fclose (svPtr->out_file[i]);

                  svPtr->in_file[i] = svPtr->out_file[i] = NULL;

                  if (svPtr->xd[svPtr->xpx] != ',') goto next_cmnd;

                  svPtr->xpx++;
                  goto next_close_arg;

            case USE:

                  svPtr->t2 = 1;

                  if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                  svPtr->io = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);

                  if (svPtr->io <= 0 || svPtr->io > 10) goto arg_list_error;

                  if (svPtr->in_file[svPtr->io] == NULL && svPtr->out_file[svPtr->io] == NULL) goto use_not_open;

                  goto next_cmnd;

// -----------------------------------------------------------
// -----------------------------------------------------------
// -----------------------------------------------------------
// #++++ remote block to proces lhs type references
// -----------------------------------------------------------
// -----------------------------------------------------------
// -----------------------------------------------------------

lhsref:

                  if (vd[1] == '^') {
                        strcpy ( (char *) svPtr->v1d, (const char *) &vd[1]);
                        strcpy ( (char *) svPtr->bd, (const char *) &svPtr->pd1[svPtr->sdlim]);

                        if (!MergeFlag) Mglobal (STORE, svPtr->v1d, svPtr->bd, svPtr);
                        else Merge(svPtr->v1d, (unsigned char *) &MergeRHS[1],svPtr);

                        }

                  else {
                        if (vd[1] == '$' && (vd[2] == 'p' || vd[2] == 'P')) { // $piece()
                              strcpy ( (char *) tmp3, (const char *) &svPtr->pd1[svPtr->sdlim]);
                              strcpy ( (char *) &svPtr->v1d[1], (const char *) &vd[1]);
                              svPtr->setpiece = 1;
                              fcn (svPtr);
                              svPtr->setpiece = 0;
                              goto lhsref1;
                              }

                        else if (vd[1] == '$' && (vd[2] == 'e' || vd[2] == 'E')) { // $extract()
                              strcpy ( (char *) tmp3, (const char *) &svPtr->pd1[svPtr->sdlim]);
                              strcpy ( (char *) &svPtr->v1d[1], (const char *) &vd[1]);
                              svPtr->setpiece = 1;
                              fcn (svPtr);
                              svPtr->setpiece = 0;
                              goto lhsref1;
                              }

                        else if (vd[1] == '$' && (vd[2] == 'z' || vd[2] == 'Z')) { // $z variable
                              strcpy ( (char *) tmp3, (const char *) &svPtr->pd1[svPtr->sdlim]);
                              strcpy ( (char *) &svPtr->v1d[1], (const char *) &vd[1]);
                              svPtr->setpiece = 1;
                              fcn (svPtr);
                              svPtr->setpiece = 0;
                              goto lhsref1;
                              }

                        else {
                              svPtr->symflg = 0;                /* store */
                              sym_ (0, (unsigned char *) &vd[1], (unsigned char *) &svPtr->pd1[svPtr->sdlim],svPtr);
                              }
                        }

lhsref1:

                  if (retz == READ) goto cont_read;
                  else goto set_return;

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// #++++ remote block to process DO and GOTO arguments.
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

lbl:

                  SymInc=0;

                  if (svPtr->xd[svPtr->xpx] == 0) goto lbl_error;

// *****************
// indirect argument
// *****************

                  if (svPtr->xd[svPtr->xpx] == '@') {

                        svPtr->t0px = 1;
                        svPtr->t2 = 2;

                        if ((svPtr->ierr = parse_ (svPtr))) goto lbl_error;

                        if (svPtr->t0px == 0) {
                              int i;                            // copy label part to do save.

                              for (i=1; svPtr->v1d[i] != 0 && svPtr->v1d[i] < 127; i++);

                              strncpy ( (char *) dosave, (const char *) &svPtr->v1d[1],i-1);
                              dosave[i-1]=0;

                              if (svPtr->v1d[i]!=0) {           // argumets follow the label xxx(a,b,c) not xxx^x.m(1,2,3)
                                    strcpy((char *) &svPtr->args, (char *) &svPtr->v1d[i+1]);
                                    }
                              }

                        else {

                              strcpy ( (char *) dosave, (const char *) &svPtr->pd1[svPtr->sdlim]);

                              }

                        if (svPtr->t0px == 0) svPtr->xpx++;

                        }

// ****************
// builtin function
// ****************

                  else if ( svPtr->xd[svPtr->xpx] == '$' ) { // builtin function
                        svPtr->t2 = 1;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        goto next_cmnd;
                        }

// ********************************
// extract label and offset from xd
// ********************************

                  else {
                        i = 0;
                        j = svPtr->xpx;

                        while (svPtr->xd[svPtr->xpx]) {
                              if (svPtr->xd[svPtr->xpx] == ',' || svPtr->xd[svPtr->xpx] == ' '
                                          || svPtr->xd[svPtr->xpx] == 0 || svPtr->xd[svPtr->xpx] == '+'
                                          || svPtr->xd[svPtr->xpx] == '('
                                          || svPtr->xd[svPtr->xpx] == '^' || svPtr->xd[svPtr->xpx] == ':') break;

                              dosave[i++] = svPtr->xd[svPtr->xpx++];
                              }

                        dosave[i] = 0;
                        }

// ********************************************************
// handles do xxx(a,b,c) - does not handle do xxx^yyy(a,b,c)
// ********************************************************

                  // fcn call with args
                  if (svPtr->xd[svPtr->xpx] == '(' || svPtr->xd[svPtr->xpx] == OPEN_PAREN ) {

                        if (svPtr->xd[svPtr->xpx+1] == ')' ) {

// empty argument list case ()

                              svPtr->args[0]=0;
                              svPtr->args[1]=0;
                              svPtr->args[2]=0;
                              svPtr->xpx += 2;
                              }

                        else {
                              svPtr->t0px = 1;
                              svPtr->t2 = 1;
                              svPtr->xpx-- ;
                              svPtr->xd[svPtr->xpx]='$';

                              if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                              svPtr->xpx++;
                              strcpy((char *) &svPtr->args, (char *) &svPtr->v1d[3]);
                              }
                        }

                  k = -1;

                  if (svPtr->xd[svPtr->xpx] == '+') {
                        svPtr->t2 = 2;

                        if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

                        k = atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]);

                        if (k < 0) goto lbl_error;
                        }

                  svPtr->v1d[1] = 0;

                  if (svPtr->xd[svPtr->xpx] != ' ' &&
                              svPtr->xd[svPtr->xpx] != ',' &&
                              svPtr->xd[svPtr->xpx]        &&
                              svPtr->xd[svPtr->xpx] != ':') {
                        svPtr->t0px = 1;
                        svPtr->t2 = 1;

                        if (svPtr->xd[svPtr->xpx] == '^' && svPtr->xd[svPtr->xpx + 1] == '@') {
                              svPtr->xpx++;
                              fileflg = 1;
                              }
                        else fileflg = 0;

                        if ((svPtr->ierr = parse_ (svPtr))) {
                              goto file_error;
                              }

                        if (fileflg) {
                              strcpy ( (char *) ftmp, (const char *) &svPtr->v1d[1]);
                              svPtr->v1d[1] = '^';
                              strcpy ( (char *) &svPtr->v1d[2], (const char *) ftmp);
                              }

                        if (svPtr->xd[svPtr->xpx] != ':') svPtr->xpx++;
                        }

                  if (svPtr->xd[svPtr->xpx] == ':') {
                        svPtr->xpx++;
                        svPtr->t2 = 1;
                        svPtr->t0px = 0;
                        strcpy ( (char *) ftmp, (const char *) &svPtr->v1d[1]);

                        if ((svPtr->ierr = parse_ (svPtr))) goto post_cond_error;

                        strcpy ( (char *) &svPtr->v1d[1], (const char *) ftmp);

                        if (atoi ( (const char *) &svPtr->pd1[svPtr->sdlim]) == 0) {
                              if (svPtr->xd[svPtr->xpx] == ',') {
                                    svPtr->xpx++;
                                    goto lbl;
                                    }
                              else goto next_cmnd;
                              }
                        }

                  if (dosave[0] == '^') {
                        strcpy ( (char *) &svPtr->v1d[1], (const char *) dosave);
                        dosave[0] = 0;
                        }

                  if (svPtr->v1d[1]) {
                        if (retz == DO) {
                              svPtr->iorg = 1;
                              shold = svPtr->pd1len;
                              swp[swapextent] = (char *) malloc (shold);

                              if (swp[swapextent] == NULL) goto stack_overflow;

                              memcpy (swp[swapextent], pd1p, (size_t) shold);
                              shold = swapextent;
                              swapextent++;

                              if (swapextent >= SWAP) goto stack_overflow;
                              }

                              {
                              // extract file name from line - avoids args

                              int i;
                              unsigned char ftmp1[512];         // limit on file name size

                              for (i=0; i<511 && isprint(svPtr->v1d[i+2]); i++)
                                    ftmp1[i]=svPtr->v1d[i+2];

                              ftmp1[i]=0;

                              /* load file */
                              readf_ (ftmp1, svPtr->iorg, svPtr);

                              if (!ftmp1[0]) goto file_error;

                              // remove file name leaving args if any

                                    {
                                    unsigned char *x=&svPtr->v1d[1];
                                    unsigned char *y=&svPtr->v1d[i+2];

                                    for (; *y != 0; x++,y++) *x = *y;

                                    *x = 0;
                                    }

//PROCESSED ARGS
// copy already processed args to arg string

                              strcpy((char *) &svPtr->args, (char *) &svPtr->v1d[2]);
                              }

                        svPtr->LineNumber = 0;
                        lblx = 0;

                        if (svPtr->v1d[2] == 0) goto file_error;
                        }

                  for (ibr = 0; ibr < lblx; ibr++) {
                        if (strcmp ((const char *) &lbls[ibr][0], (const char *) dosave) == 0) {
                              ibr = lbloff[ibr];
                              goto offset;
                              }
                        }

// ************************
// search for label in code
// ************************

                  ibr = 1;

                  if (dosave[0]) {
                        while (ibr < svPtr->pd1len) {

                              if (svPtr->pd1[ibr] == TAB
                                          || svPtr->pd1[ibr] == ' '
                                          || svPtr->pd1[ibr] == '#'
                                          || svPtr->pd1[ibr] == ';') { // no label
                                    while (svPtr->pd1[ibr++] != 0 && ibr < svPtr->pd1len); // scan for end of line
                                    }

                              else { // something in column 1

                                    j = 0;

                                    while (svPtr->pd1[ibr] == dosave[j] && dosave[j] != 0) { // compare dosave label with text
                                          j++;
                                          ibr++;
                                          }

                                    if ( (     svPtr->pd1[ibr] == TAB  	// label found
                                                || svPtr->pd1[ibr] == ' '	// delimiter
                                                || svPtr->pd1[ibr] == '(' 	// parameters
                                                || svPtr->pd1[ibr] == 0 )	// label alone on line
                                                && dosave[j] == 0) {

// add found label to table if room

                                          if (lblx < LBLS-1 && j < 10 && svPtr->pd1[ibr] !='(') {
                                                lblx++;
                                                strcpy ( (char *) &lbls[lblx][0], (const char *) dosave);
                                                lbloff[lblx] = ibr;
                                                }

                                          goto offset;
                                          }

                                    while (svPtr->pd1[ibr++] != 0 && ibr < svPtr->pd1len);
                                    }
                              }

                        goto lbl_error;
                        }

                  else {                                    // not do xxx^yyy but may be do xxx^yy(123)

                        while (svPtr->pd1[ibr] != ' ' &&
                                    svPtr->pd1[ibr] != TAB &&
                                    svPtr->pd1[ibr] != 0   &&
                                    svPtr->pd1[ibr] != '(' ) ibr++;

                        }

offset:

// pass arguments to subroutine

                  if ( svPtr->pd1[ibr] == '(' ) {

                        unsigned char var[256];
                        int j=ibr+1;
                        int i,iargs[10]= {0},
                                         nargs=0;
                        int count=0;

                        svPtr->exec++;                        // indicates subroutine call level - not ExecLevel

// advance to new symbol table level for subroutine and set
// flag for dotret: so that it won't do it again. other
// doret: returns need the sym increment but not sub call types

                        svPtr->_Sym++;
                        svPtr->start[svPtr->_Sym] = NULL;
                        SymInc=1;                             // flag to prevent inc from happing again

                        for (i = 0; svPtr->args[i] != 0; i++)
                              if (svPtr->args[i] == 207 || svPtr->args[i] == 208) {
                                    svPtr->args[i] = 0;
                                    iargs[++nargs] = i+1 ;
                                    }

                        i=0;

                        while ( svPtr->pd1[j]!=' ' && svPtr->pd1[j]!=0 ) {

                              if (svPtr->pd1[j]==0) goto lbl_error;

                              if (svPtr->pd1[j]==',' || svPtr->pd1[j] == ')') {

                                    if (count >= nargs) {         // arg count mismatch - pass zeros
                                          var[i]=0;
                                          }

                                    else {
                                          var[i]=0;
                                          // code 100 means store at this level only -> parameter
                                                {
                                                unsigned char *t,tmp[STR_MAX];
                                                int symflg=100;

                                                if (svPtr->args[iargs[count]] == '.') {
                                                      strcpy(svPtr->reference, (const char *) &svPtr->args[iargs[count]+1]);

//                                                      if ( sym_(SymRetrieve, &svPtr->args[iargs[count]+1], tmp, svPtr) == NULL) goto var_not_found;
							if ( sym_(SymRetrieve, &svPtr->args[iargs[count]+1], tmp, svPtr) == NULL) {
								printf("*** Var: %s\n", &svPtr->args[iargs[count] +1] );
								goto var_not_found;
								}


                                                      t=tmp;
                                                      symflg=200;
                                                      }

                                                else t=&svPtr->args[iargs[count]];

                                                sym_ (symflg, (unsigned char *) var, (unsigned char *) t, svPtr);
                                                }
                                          }

                                    i=0;
                                    j++;
                                    count++;

                                    if ( svPtr->pd1[j] == ')' ) break;

                                    continue;
                                    }

                              var[i++]=svPtr->pd1[j++];
                              }
                        }

// end argument passing section

                  if (k > 0) {                              // offset from label
                        for (i = 0; i < k; i++) {
                              while (svPtr->pd1[ibr++] != 0 && ibr < svPtr->pd1len);

                              if (ibr >= svPtr->pd1len) goto lbl_error;
                              }

                        while (svPtr->pd1[ibr] != TAB && svPtr->pd1[ibr] != ' ') ibr++;

                        if (ibr >= svPtr->pd1len) goto lbl_error;
                        }

                  else if (k == 0) ibr = pd1cur;

// else pd1cur = ibr;

                  if (svPtr->xd[svPtr->xpx] && svPtr->xd[svPtr->xpx] != ' ') svPtr->xpx++;

                  if (retz == GOTO) goto goto_return;
                  else goto do_ret;

err_call:
                  ernbr = 0;

                  if (svPtr->NOERR) goto abrt1;

                  fprintf (stderr, "\n*** %d Control-C\n", ernbr);
                  goto abrt1;

parse_err:
                  ernbr = svPtr->ierr;
                  prnterr (ernbr,svPtr,pd1cur);

                  if (svPtr->NOERR) {
                        svPtr->NOERR=-1;
                        goto next_cmnd;
                        }

                  goto abrt1;

post_cond_error:
                  ernbr = 6;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

lbl_error:
                  ernbr = 8;
                  prnterr (ernbr,svPtr,pd1cur);
                  printf(" %s\n",dosave);
                  goto abrt;

var_not_found:
                  ernbr=17;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt;

            default:
                  ernbr = 13;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

arg_list_error:
                  ernbr = 14;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

ref_err:
                  ernbr = 18;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

symtab_err:
                  ernbr = 23;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

file_error:
                  ernbr = 26;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

io_error:
                  ernbr = 20;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

program_size:
                  ernbr = 31;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

stack_overflow:
                  ernbr = 32;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

sql_error:
                  ernbr = 33;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

use_not_open:
                  ernbr = 36;
                  prnterr (ernbr,svPtr,pd1cur);
                  goto abrt1;

abrt1:
                  // if (!svPtr->NOERR) printf ("*** %s\n", &svPtr->pd1[pd1Last]);
                  goto abrt;

            }                                                 /* command switch */


restore:

//        Mglobal(CLOSE,tmp2,tmp3,svPtr);  /* Close Globals */
//	fflush(NULL);

      return 0;

      }                                                 /* function main() */

/*===========================================================================*
 *                                  strmove                                  *
 *===========================================================================*/

#include <mumpsc/inline.h>

/*===========================================================================*
 *                                   parse                                   *
 *===========================================================================*/


int parse_ (struct MSV * svPtr) {

      /* * #+ 206 = open; 207 = close; 208 = comma; 209 = colon */

      short ernbr, f, spx, adx, jpx, j, i, g;
      const static unsigned char cod209[2] = { 209, 0 };
      unsigned char s1p[40];
      unsigned char bbtyp, tmp2[4], tmp1[25], tmp3[STR_MAX], nxtchr;
      unsigned char *bp = &svPtr->bd[1];
      unsigned char *v1dp, *to, *from;
      int FcnArgToggle=0;

      const static unsigned char code[26] = {

            /*  0 */ 99, 99, 99, 99, 99, 99, 99, 99, 18, 19,
            /* 10 */ 99, 20, 18, 99, 99, 21, 22, 23, 99, 99,
            /* 20 */ 99, 99, 99, 99, 26, 27
            };

      const static unsigned char opcode[256] = {

            /*   0 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  30 */ 99, 99, 99, 25, 99, 14, 99, 99, 24, 12,
            /*  40 */  0, 99,  2,  4, 99,  3, 99,  1, 99, 99,
            /*  50 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  60 */ 11,  8,  9, 17, 28, 99, 99, 99, 99, 99,
            /*  70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  90 */ 99, 15, 13, 16, 99,  7, 99, 99, 99, 99,
            /* 100 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 110 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 120 */ 99, 99, 99, 99, 99, 99, 99, 99
            };

      const static unsigned char ncode[256] = {

            /*   0 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  30 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  40 */ 99, 99, 99, 99, 99, 99, 10, 99, 10, 10,
            /*  50 */ 10, 10, 10, 10, 10, 10, 10, 10, 99, 99,
            /*  60 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  90 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 100 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 110 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 120 */ 99, 99, 99, 99, 99, 99, 99, 99
            };

      const static unsigned char dcode[256] = {

            /*   0 */ 10, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  30 */ 99, 99, 10, 10, 99, 10, 99, 99, 10, 10,
            /*  40 */ 99, 10, 10, 10, 10, 10, 99, 10, 99, 99,
            /*  50 */ 99, 99, 99, 99, 99, 99, 99, 99, 10, 99,
            /*  60 */ 10, 10, 10, 10, 10, 99, 99, 99, 99, 99,
            /*  70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*  90 */ 99, 10, 10, 10, 10, 10, 99, 99, 99, 99,
            /* 100 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 110 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /* 120 */ 99, 99, 99, 99, 99, 99, 99, 99
            };

      const static unsigned char operand[256] = {

            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 1, 1, 2, 0,
            0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            2, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 2, 1, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 0
            };

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + initialization + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      svPtr->bd[0]=0;
      v1dp = &svPtr->v1d[1];
      svPtr->xpx--;
      spx = 0;
      svPtr->pd1[svPtr->pd1len + 2] = 0;
      svPtr->pd1[svPtr->pd1len + 3] = 0;
      svPtr->sdlim = svPtr->pd1len + 3;

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + main recursive internal entry point + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

start:
      spx++;
      s1p[spx] = PEMPTY;                            /* stack foundation */

nchar:

      if ((nxtchr = svPtr->xd[++svPtr->xpx]) == 0) goto finish;

      if (operand[nxtchr] == 1) goto scan_operand;

      if (nxtchr == CodedOpen) {
            unsigned char *p1, *p2;
            spx++;

            while (svPtr->pd1[svPtr->sdlim++]);

            p1 = &svPtr->v1d[1];
            p2 = &svPtr->pd1[svPtr->sdlim];

            while ((*p2++ = *p1++));

            *p2 = 0;
            p2--;
            *p2 = CodedOpen;
            s1p[spx] = OPENC;
            goto nchar;
            }

      if (nxtchr == ',') {
            if (s1p[spx - 1] != OPENC) {
                  if (svPtr->t2 > 0) goto finish;
                  return (16);
                  }

            /* concatenate stack tops and add coded comma */

            /* join tops */
            for (i = --svPtr->sdlim; (svPtr->pd1[i] = svPtr->pd1[i + 1]); i++);

            svPtr->pd1[i] = CodedComma;
            svPtr->pd1[i + 1] = 0;                    /* coded comma */
            svPtr->sdlim--;

            while (svPtr->pd1[svPtr->sdlim--]);       /* find new stack top start */

            svPtr->sdlim += 2;
            spx--;                                    /* stack pointer */
            goto nchar;
            }

      if (nxtchr == ')') {

            if (s1p[spx - 1] != POPEN) {
                  if (s1p[spx - 1] != OPENC) return (16);

                  /*
                   * fcn/array return section
                   */

                  svPtr->sdlim--;

                  for (i = svPtr->sdlim; (svPtr->pd1[i] = svPtr->pd1[i + 1]); i++);

                  /* join
                   * tops */
                  svPtr->pd1[i] = CodedClose;
                  svPtr->pd1[i + 1] = 0;                /* coded close */
                  svPtr->sdlim--;

                  while (svPtr->pd1[svPtr->sdlim--]);   /* find new stack top start */

                  svPtr->sdlim += 2;
                  spx--;                                /* stack pointer */
                  goto un_nest;
                  }

//--------------------------------
//       precedence close paren
//--------------------------------

            if (s1p[spx] != OPERAND) return (12);

            /* extract value from stack top */

            strmove (bp, &svPtr->pd1[svPtr->sdlim]);

            do
                  svPtr->sdlim--;

            while (svPtr->pd1[svPtr->sdlim - 1]);     /* compress stack */

            goto dec_stk;
            }

      if (nxtchr == ':') {
            i = svPtr->sdlim;

            do
                  i--;

            while (svPtr->pd1[i - 1]);

            if (svPtr->pd1[i] != '$' || (svPtr->pd1[i + 1] != 's' && svPtr->pd1[i + 1] != 'S'))
                  goto finish;

            svPtr->sdlim--;
            strmove (&svPtr->pd1[svPtr->sdlim], &svPtr->pd1[svPtr->sdlim + 1]);

            while (svPtr->pd1[svPtr->sdlim--]);

            svPtr->sdlim += 2;
            strcat ((char *) &svPtr->pd1[svPtr->sdlim], (char *) cod209);
            spx--;
            goto nchar;
            }

      if (nxtchr == ' ' || nxtchr == TAB) goto finish;

      if (nxtchr == '^' && svPtr->t2 == 2 && s1p[spx] == OPERAND) goto finish;

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + check for delimiters in list + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      spx++;

      if (nxtchr == QUOTE) {
            /* insert not based operator code */
            svPtr->xpx++;
            s1p[spx] = opcode[svPtr->xd[svPtr->xpx]];

            if (s1p[spx] != 99) {
                  s1p[spx] = code[opcode[svPtr->xd[svPtr->xpx]]];

                  // patch for ']]
                  if (svPtr->xd[svPtr->xpx+1] == ']') ++svPtr->xpx;

                  if (s1p[spx] != 99) goto nchar;
                  }

            svPtr->xpx--;
            }

      if (nxtchr == '*' && svPtr->xd[svPtr->xpx + 1] == '*') {
            // exponentiation
            svPtr->xpx++;
            s1p[spx] = EXPONEN;
            goto nchar;
            }

      if (nxtchr == ']' && svPtr->xd[svPtr->xpx + 1] == ']') {
            // collates after - becomes follows
            svPtr->xpx++;
            s1p[spx] = FOLLOWS;
            goto nchar;
            }

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + insert code + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      s1p[spx] = opcode[nxtchr];

      if (s1p[spx] != 99) goto nchar;

      svPtr->ErrChar=nxtchr;
      return (12);

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + scan for operand + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

scan_operand:

      jpx = svPtr->xpx;

      if (s1p[spx] == NOTPATTERN || s1p[spx] == PATTERN) {
            svPtr->bd[1] = 0;
            patrn1 (&jpx, &ernbr, svPtr);

            if (ernbr) return (ernbr);

            goto exec;
            }

      /***************************
          numeric operand
      ***************************/

      if (ncode[svPtr->xd[svPtr->xpx]] == 10) {

            if (svPtr->t0px>0 && svPtr->xd[svPtr->xpx] == '.' &&
                        ( isalpha(svPtr->xd[svPtr->xpx+1]) ||
                          svPtr->xd[svPtr->xpx+1] == '_' ||
                          svPtr->xd[svPtr->xpx+1] == '%') ) {
                  svPtr->bd[1]='.';
                  svPtr->xpx++;

                  for (i = 2; isalnum( (svPtr->bd[i] = svPtr->xd[svPtr->xpx]) ) ; (i++, svPtr->xpx++));

                  svPtr->bd[i] = 0;
                  svPtr->xpx--;
                  goto exec;
                  }

            for ( ; svPtr->xd[svPtr->xpx] == '0'; svPtr->xpx++); // remove leading zeros

            if (ncode[svPtr->xd[svPtr->xpx]] != 10 ) svPtr->xpx--; // all zeros

            for (i = 1; (svPtr->bd[i] = svPtr->xd[svPtr->xpx]) && ncode[svPtr->bd[i]] == 10; (i++, svPtr->xpx++));

            if (i == 1 && svPtr->bd[1] == '.') return (9);

            svPtr->bd[i] = 0;
            svPtr->xpx--;
            goto exec;
            }

      /***************************
          literal operand
      ***************************/

      if (svPtr->xd[svPtr->xpx] == '"') {
            j = 0;

            while (1) {
                  while (svPtr->xd[++svPtr->xpx] != '"') {
                        if ((svPtr->bd[++j] = svPtr->xd[svPtr->xpx]) == 0) return (2);
                        }

                  if (svPtr->xd[svPtr->xpx + 1] != '"') {
                        svPtr->bd[++j] = 0;
                        goto exec;
                        }

                  svPtr->xpx++;
                  svPtr->bd[++j] = '"';
                  }
            }

      /***************************
          variable name
      ***************************/

      svPtr->v1d[1] = svPtr->xd[svPtr->xpx++];
      j = 2;

      while (1) {

//------------------------------------------------------
//	indirect name and indirect indices case @x@(1,1)
//------------------------------------------------------

            if (svPtr->xd[svPtr->xpx]=='@' && svPtr->xd[svPtr->xpx+1]=='(' && s1p[spx]!=INDIRECT) {
                  strcpy((char *)&svPtr->xd[svPtr->xpx],(char *) &svPtr->xd[svPtr->xpx+1]);
                  goto idxin;
                  }

            if (dcode[svPtr->xd[svPtr->xpx]] == 10) {
                  svPtr->v1d[j] = 0;
                  svPtr->xpx--;
                  goto var1;
                  }

idxin:
            svPtr->v1d[j++] = svPtr->xd[svPtr->xpx];

            if (svPtr->xd[svPtr->xpx] == '(') {
                  svPtr->v1d[--j] = 0;
                  svPtr->xd[svPtr->xpx--] = CodedOpen;
                  goto start;                           /* recurse */
                  }

            svPtr->xpx++;
            }

un_nest:                                      /* copy answer from stack */

      while (svPtr->pd1[--svPtr->sdlim]);

      strmove (v1dp, &svPtr->pd1[svPtr->sdlim + 1]);

      while (svPtr->pd1[--svPtr->sdlim]);

      svPtr->sdlim++;
      spx -= 2;

var1:

      if (svPtr->t0px == 1 && spx == 1) {
            spx--;
            svPtr->t0px = 0;
            return (0);
            }

// $piece interface

      if (svPtr->pd1[svPtr->sdlim] == '$' && svPtr->t0px == 1 &&
                  (svPtr->pd1[svPtr->sdlim + 1] == 'P' || svPtr->pd1[svPtr->sdlim + 1] == 'p'))

            if (svPtr->setname[0] == 0) strmove (svPtr->setname, v1dp);

// $extract interface

      if (svPtr->pd1[svPtr->sdlim] == '$' && svPtr->t0px == 1 &&
                  (svPtr->pd1[svPtr->sdlim + 1] == 'E' || svPtr->pd1[svPtr->sdlim + 1] == 'e'))
            if (svPtr->setname[0] == 0) {
                  strmove(svPtr->setname, v1dp);
                  }

      if (svPtr->v1d[1] == '^') {                   /* global var */

            g = RETRIEVE;

// NO EVAL GLOBAL ZZ INTERFACE

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'Z' &&
                        toupper (svPtr->pd1[svPtr->sdlim + 2]) == 'Z') {
                  for(int i=0; v1dp[i]!=0; i++) if (v1dp[i]>127) v1dp[i]=1;
                  strcpy((char*)&svPtr->bd[1],(char *)v1dp);
                  goto exec;
                  }

            if (svPtr->pd1[svPtr->sdlim] == '$' && s1p[spx] != INDIRECT) {

                  FcnArgToggle ++;
                  FcnArgToggle %= 2;

//----------------------------------------------------------------------------------------
// some global references must be passed as references to subroutines without evaluation.
// this code hides the global ref in a static array and places the <gbl> token as the
// result of the 'evaluation'
// use with $name, $query $qlength $qsubscript
// $name function requires $na as minimum abbreviation while $next only requires $n
//----------------------------------------------------------------------------------------

                  if ( (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'N' &&
                              toupper (svPtr->pd1[svPtr->sdlim + 1]) != 'A' ) ||
                              toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'Q') {
                        g = NEXT;
                        /* hide global in a static array in BuildGlobal for later extraction */
                        BuildGlobal(0, g, v1dp, NULL, svPtr);
                        strcpy( (char *) bp,(const char *) "<gbl>");
                        goto exec;
                        }

//-----------
// $ORDER FCN
//-----------

                  if ( toupper(svPtr->pd1[svPtr->sdlim + 1]) == 'O') {

                        for (i = svPtr->sdlim; svPtr->pd1[i]; i++);

                        i--;

                        if (svPtr->pd1[i] == OPEN_PAREN ) {      // first argument
                              strcpy( (char *) bp,(const char *) v1dp);

                              for (int i=0; bp[i]; i++) if (bp[i]>127) bp[i]=1;

                              goto exec;
                              }

                        else {
                              f = Mglobal (g, v1dp, bp, svPtr);
                              goto exec;
                              }

                        }

                  if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'B') {
                        g = ORDERPREV;
                        BuildGlobal(0,g,v1dp,NULL,svPtr);
                        strcpy( (char *) bp,(const char *) "<gbl>");
                        goto exec;
                        }

                  }

            f = Mglobal (g, v1dp, bp, svPtr);

            /*******************************
                fix $order end string
            *******************************/

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'O')
                  if (strcmp ((char *) bp, "-1") == 0) svPtr->bd[1] = 0;   /* empty */

            /*
             * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
             * + + + check for $data + + +
             * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
             */

            if (svPtr->pd1[svPtr->sdlim] == '$' && s1p[spx] != INDIRECT &&
                        (svPtr->pd1[svPtr->sdlim + 1] == 'D' || svPtr->pd1[svPtr->sdlim + 1] == 'd')) {

                  if (f == 0) bbtyp = '0';
                  else bbtyp = '1';

                  i = strlen ((char *) v1dp);

#if !(defined(SQLITE))

// $DATA $data with NATIVE section

                  if (v1dp[i-1]!=1) {
                        v1dp[i++]=1;
                        v1dp[i++]=1;
                        v1dp[i++]=0;
                        }

                  v1dp[i] = 2;
                  v1dp[i + 1] = 1;
                  v1dp[i + 2] = 0;

                  f = Mglobal (NEXT, v1dp, svPtr->bd, svPtr);
#endif

#if (defined(SQLITE))

// $DATA $data RDBMS section

                  v1dp[i ] = 1;
                  v1dp[i+1 ] = 0;
                  v1dp[i + 2] = 0;

// determine if there is a child

                  f = Mglobal (ORDERNEXT, v1dp, svPtr->bd, svPtr);

#endif

                  svPtr->bd[2] = bbtyp;

                  if (f == 1)
                        svPtr->bd[1] = '1';
                  else
                        svPtr->bd[1] = '0';

                  if (svPtr->bd[1] == '0') {
                        svPtr->bd[1] = svPtr->bd[2];
                        svPtr->bd[2] = 0;
                        }

                  svPtr->bd[3] = 0;
                  goto exec;
                  }

            svPtr->tpx = f;
            goto exec;
            }

      /*
       * normal global
       */

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + built-in variables/fcns + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      if (svPtr->v1d[1] == '$') {

            if (svPtr->v1d[2] == 'd' ||
                        /*  svPtr->v1d[2] == 'n' ||
                            svPtr->v1d[2] == 'o' || svPtr->v1d[2] == 'O' ||
                            svPtr->v1d[2] == 'b' || svPtr->v1d[2] == 'B' || */
                        svPtr->v1d[2] == 'D' /* || svPtr->v1d[2] == 'N' */ ) {

                  /* open */
                  for (i = 1; svPtr->v1d[i] != CodedOpen; i++);

                  j = 0;

                  /* close */
                  while ((svPtr->bd[++j] = svPtr->v1d[++i]) != CodedClose);

                  svPtr->bd[j] = 0;
                  goto exec;
                  }

            if (svPtr->v1d[2] == 'z' || svPtr->v1d[2] == 'Z') {
                  zfcn (svPtr->v1d, &svPtr->bd[1],svPtr);
                  }
            else {
                  fcn (svPtr);
                  }

            if (svPtr->ierr == 0) goto exec;

            return (svPtr->ierr);
            }


      if (svPtr->pd1[svPtr->sdlim] == '$' && s1p[spx] == OPENC &&
                  ( svPtr->pd1[svPtr->sdlim + 1] == 'G' || svPtr->pd1[svPtr->sdlim + 1] == 'g') ) {

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'G') svPtr->symflg = SymName;
            else svPtr->symflg = SymNext;

            sym_(svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr);

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'N' && svPtr->bd[1] == 0)
                  strmove (bp, (unsigned char *) "-1");

            if (svPtr->symflg && s1p[spx] != INDIRECT) goto exec;

            return (27);
            }


      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + retrieve look-up + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      if (svPtr->v1d[1] == PREDICATEVAR) {
            strmove (bp, v1dp);
            goto exec;
            }

      if (svPtr->pd1[svPtr->sdlim]=='$' && s1p[spx]==OPENC) {

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'Q' ) { // query qlength qsubscript
                  g = NEXT;
                  BuildLocal(0,g,v1dp,NULL,svPtr);
                  strcpy( (char *) bp,(const char *) "<lcl>");
                  goto exec;
                  }

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'N' ) {
                  g = NEXT;
                  BuildLocal(0,g,v1dp,NULL,svPtr);
                  strcpy( (char *) bp,(const char *) "<lcl>");
                  goto exec;
                  }

            if (toupper (svPtr->pd1[svPtr->sdlim + 1]) == 'O') {

// fff for (int i=0; v1dp[i]; i++) printf("%d ",v1dp[i]);
// fff printf("\n\n");

                  FcnArgToggle ++;
                  FcnArgToggle %= 2;

                  if (FcnArgToggle ==1 ) {


                        g = ORDERNEXT;

// fff printf("before 111111 v1dp=%s bp=%s g=%d\n", v1dp, bp, g);

                        BuildLocal(0, g, v1dp, NULL, svPtr);

// fff printf("after 111111 v1dp=%s bp=%s g=%d\n", v1dp, bp, g);

                        strcpy( (char *) bp,(const char *) "<lcl>");
                        goto exec;
                        }
                  }
            }


      svPtr->symflg = SymRetrieve;

// if LHS $extract(), do not resolve variable

//    if (svPtr->pd1[svPtr->sdlim] == '$' && svPtr->t0px == 1 &&
//            (svPtr->pd1[svPtr->sdlim+1] == 'e' || svPtr->pd1[svPtr->sdlim+1] == 'E') ) {
//        svPtr->symflg=1;
//        strcpy((char *)bp,"");
//        }

// normal variable lookup

//    else  if (sym_ (svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr) == NULL)
      if (sym_ (svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr) == NULL)
            svPtr->symflg=0;

      else svPtr->symflg=1;

      if (svPtr->pd1[svPtr->sdlim] != '$') goto sym1;

      // * local variable $DATA

      if (s1p[spx] == OPENC && (svPtr->pd1[svPtr->sdlim + 1] == 'd' || svPtr->pd1[svPtr->sdlim + 1] == 'D')) {
            svPtr->symflg = SymData;
            sym_ (svPtr->symflg, (unsigned char *) v1dp, (unsigned char *) bp,svPtr);
            goto exec;
            }


      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + check for variable not found + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

sym1:

      if (svPtr->symflg == 0) {
            int i = 1, j, k = 1;

            while (1) {                                   /* look for a label of the same name */
                  for (j = i; svPtr->pd1[j] != TAB && svPtr->pd1[j] != ' '; j++);

                  if (strncmp ((char *) &svPtr->pd1[i], (char *) v1dp, j - i) == 0 &&
                              strlen ((char *) v1dp) == j - i) {
                        sprintf ((char *) bp, "%d", k);
                        goto exec;
                        }

                  i = i + strlen ((char *) &svPtr->pd1[i]) + 1;

                  if (i > svPtr->pd1len) break;

                  k++;
                  }

            return (17);
            }


      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       * + process value in bb or svPtr->bd + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

exec:

      if (s1p[spx] != PEMPTY && s1p[spx] != POPEN && s1p[spx] != OPENC) {
            goto nxt_expr;
            }

      spx++;

      while (svPtr->pd1[svPtr->sdlim++]);

      strmove (&svPtr->pd1[svPtr->sdlim], bp);
      s1p[spx] = OPERAND;
      goto nchar;

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + process expression - check for operator on stack top + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

nxt_expr:

      if (s1p[spx] == POPEN || s1p[spx] == OPERAND) {
            return (11);
            }

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + check for number under operator + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      if (s1p[spx - 1] != OPERAND) {

            switch (s1p[spx]) {

                  default:

                        return (1);

                  case INDIRECT:

                        strcat ((char *) bp, (char *) &svPtr->xd[svPtr->xpx + 1]);
                        strmove (&svPtr->xd[1], bp);
                        spx--;
                        svPtr->xpx = 0;

                        if (svPtr->kflg && spx <= 1) {
                              svPtr->xpx = 1;
                              return (0);
                              }

                        goto nchar;

                  case MINUS:                               /* unary minus */

                        strmove (tmp3, (unsigned char *) "-1");
                        mult ((char *) bp, (char *) tmp3, (char *) bp);
                        break;

                  case NOT_NOT:                                  /* not not */

                        svPtr->bd[1] = numcomp (bp, (unsigned char *) "0") ? '1' : '0';
                        svPtr->bd[2] = 0;
                        break;

                  case NOT:                                 /* unary not */

                        svPtr->bd[1] = numcomp (bp, (unsigned char *) "0") ? '0' : '1';
                        svPtr->bd[2] = 0;
                        break;

                  case PLUS:                                /* unary plus */

                        strmove (tmp3, (unsigned char *) "0");
                        add ((char *) bp, (char *) tmp3, (char *) bp);

                  }                                             /* switch */

            spx--;
            goto nxt_operator;
            }

//-----------------------------
// extract value under operator
//-----------------------------

      for (adx = svPtr->sdlim--; svPtr->pd1[svPtr->sdlim - 1]; svPtr->sdlim--);

//-------------------------------
// branch depending upon operator
//-------------------------------

      switch (s1p[spx]) {

            case OPERAND:
            case PEMPTY:
            case NOT:
            default:

                  return (12);

            case DIVIDE:

                  divx ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case EXPONEN:

                  expx ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case MULTIPLY:

                  mult ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case MINUS:

                  sub ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case PLUS:

                  add ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case CONCAT:

                  to = &svPtr->pd1[adx]; 			// start of left operand
                  while( *(to++) );			// find end of left operand
                  --to;					// retreat fron null
                  from = bp;				// start of right operand
                  while( *(to++) = *(from++) );		// concatenate
                  if ( to - &svPtr->pd1[adx] >= STR_MAX)
                        return 35;			// too long
                  *to=0;					// null
                  to=bp;					// address of target
                  from=&svPtr->pd1[adx];			// address of source
                  while (*(to++) = *(from++) );		// copy result to bp
                  break;

            case EQUALS:

                  svPtr->bd[1] = strcmp ((char *) &svPtr->pd1[adx], (char *) bp) ? '0' : '1';
                  svPtr->bd[2] = 0;
                  break;

            case NOTEQ:
                  svPtr->bd[1] = strcmp ((char *) &svPtr->pd1[adx], (char *) bp) ? '1' : '0';
                  svPtr->bd[2] = 0;
                  break;

            case GREATER:

                  svPtr->bd[1] = (numcomp (&svPtr->pd1[adx], bp) > 0) ? '1' : '0';
                  svPtr->bd[2] = 0;
                  break;

            case LESSTHAN:

                  svPtr->bd[1] = (numcomp (&svPtr->pd1[adx], bp) < 0) ? '1' : '0';
                  svPtr->bd[2] = 0;
                  break;

            case INTDIVIDE:

                  divi ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case MODULO:

                  modulo ((char *) &svPtr->pd1[adx], (char *) bp, (char *) bp);
                  break;

            case CONTAINS:

                  if (svPtr->pd1[adx] == 0 && bp == 0) {
                        svPtr->bd[1] = '1';
                        svPtr->bd[2] = 0;
                        break;
                        }

                  if (xindex ((unsigned char *) &svPtr->pd1[adx], (unsigned char *) bp, (short) 1) == 0)
                        svPtr->bd[1] = '0';
                  else
                        svPtr->bd[1] = '1';

                  svPtr->bd[2] = 0;
                  break;

            case FOLLOWS:

                  if (strcmp ((char *) &svPtr->pd1[adx], (char *) bp) <= 0)
                        svPtr->bd[1] = '0';
                  else
                        svPtr->bd[1] = '1';

                  svPtr->bd[2] = 0;
                  break;

            case PATTERN:

                  if (pm (&svPtr->pd1[adx], &svPtr->bd[0],svPtr) == 0)
                        svPtr->bd[1] = '0';
                  else
                        svPtr->bd[1] = '1';

                  svPtr->bd[2] = 0;
                  break;

            case NOTPATTERN:

                  if (pm (&svPtr->pd1[adx], &svPtr->bd[0],svPtr) == 0)
                        svPtr->bd[1] = '1';
                  else
                        svPtr->bd[1] = '0';

                  svPtr->bd[2] = 0;
                  break;

            case NOTGREATER:

                  if (numcomp (&svPtr->pd1[adx], bp) <= 0)
                        svPtr->bd[1] = '1';
                  else
                        svPtr->bd[1] = '0';

                  svPtr->bd[2] = 0;
                  break;

            case NOTLESS:

                  if (numcomp (&svPtr->pd1[adx], bp) >= 0)
                        svPtr->bd[1] = '1';
                  else
                        svPtr->bd[1] = '0';

                  svPtr->bd[2] = 0;
                  break;

            case NOTCONTAINS:

                  if (xindex ((unsigned char *) &svPtr->pd1[adx], (unsigned char *) bp, (short) 1) == 0)
                        svPtr->bd[1] = '1';
                  else
                        svPtr->bd[1] = '0';

                  svPtr->bd[2] = 0;
                  break;

            case NOTFOLLOWS:

                  if (strcmp ((char *) &svPtr->pd1[adx], (char *) bp) <= 0)
                        svPtr->bd[1] = '1';
                  else
                        svPtr->bd[1] = '0';

                  svPtr->bd[2] = 0;
                  break;

            case AND:

                  svPtr->bd[1] = numcomp (bp, (unsigned char *) "0")
                                 && numcomp (&svPtr->pd1[adx], (unsigned char *) "0") ? '1' : '0';
                  svPtr->bd[2] = 0;
                  break;

            case OR:
                  svPtr->bd[1] = numcomp (bp, (unsigned char *) "0")
                                 || numcomp (&svPtr->pd1[adx], (unsigned char *) "0") ? '1' : '0';
                  svPtr->bd[2] = 0;
                  break;

            case NOTAND:

                  if (numcomp (bp, (unsigned char *) "0")
                              && numcomp (&svPtr->pd1[adx], (unsigned char *) "0"))
                        svPtr->bd[1] = '0';
                  else
                        svPtr->bd[1] = '1';

                  svPtr->bd[2] = 0;
                  break;

            case NOTOR:

                  if (numcomp (bp, (unsigned char *) "0")
                              || numcomp (&svPtr->pd1[adx], (unsigned char *) "0"))
                        svPtr->bd[1] = '0';
                  else
                        svPtr->bd[1] = '1';

                  svPtr->bd[2] = 0;

            }


dec_stk:

      spx -= 2;

nxt_operator:

      if (s1p[spx] != POPEN && s1p[spx] != OPENC && s1p[spx] != PEMPTY)
            goto nxt_expr;

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + push answer + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

      spx++;
      svPtr->sdlim += strlen ((char *) &svPtr->pd1[svPtr->sdlim]) + 1;

      to = &svPtr->pd1[svPtr->sdlim];
      from = bp;
      while ( *(to++) = *(from++) );
      *to = 0;

      s1p[spx] = OPERAND;
      goto nchar;

      /*
       * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ + +
       * + exit sequence + + +
       * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       */

finish:

      if (s1p[spx - 1] != PEMPTY || s1p[spx] != OPERAND) return (16);
      else return (0);

      }


/*===========================================================================*
 *                                  patrn1                                   *
 *===========================================================================*/

void patrn1(short *jpx, short *ernbr, struct MSV * svPtr)

/* pattern argument [right side) set up */
      {
      const static unsigned char cod210[2] = { 210, 0 };
      unsigned char tmp1[2] = { 0, 0 };

a1285:

      if (svPtr->xd[*jpx] == '"') {
            strcat((char *) &svPtr->bd[0], "\"");

a1283:

            *jpx = (*jpx + 1);

            if (svPtr->xd[*jpx] == 0) {
                  *ernbr = 2;
                  return;
                  }

            if (svPtr->xd[*jpx] == '"') {
                  if (svPtr->xd[*jpx + 1] != '"')
                        goto a1281;

                  strcat((char *) &svPtr->bd[0], (char *) cod210);
                  *jpx = (*jpx + 1);
                  }

            else {
                  tmp1[0] = svPtr->xd[*jpx];
                  strcat((char *) &svPtr->bd[0], (char *) tmp1);
                  }

            goto a1283;
            }

a1281:

      tmp1[0] = svPtr->xd[*jpx];

      if (svPtr->xd[*jpx] != 0) {
            if (xindex((unsigned char *) " ,&:!''><=[)+-*/#_", (unsigned char *) tmp1, (short) 1)
                        == 0) {
                  strcat((char *) &svPtr->bd[0], (char *) tmp1);
                  *jpx = (*jpx + 1);
                  goto a1285;
                  }
            }

      svPtr->xpx = (*jpx - 1);
      *ernbr = 0;
      return;
      }


/*===========================================================================*
 *                                  getstr2                                  *
 *===========================================================================*/

short getstr2 (short int lun, unsigned char area[], struct MSV * svPtr) {

      short int i, chr;

      if (lun == 5) svPtr->in_file[lun] = stdin;

      for (i = 0; i < STR_MAX; i++) {               // restrict input line length
            chr = fgetc (svPtr->in_file[lun]);

            if (chr == EOF) {
                  area[0] = 0;
                  return (-1);
                  }

            area[i] = chr;

            if (area[i] == LINEFEED) {
                  area[i] = 0;
                  return (i);
                  }

            if (area[i] > 127) area[i] = ' ';

            if (area[i] == '\n') i--;
            else if (area[i] == '\r') i--;
            else if (area[i] == TAB) area[i] = TAB;
            else if (area[i] < 32) area[i] = ' ';
            }

      area[STR_MAX] = 0;                            // restricted length
      return (-1);
      }


/*===========================================================================*
 *                                   readf                                   *
 *===========================================================================*/

void readf_ (unsigned char zd[], short pd1org, struct MSV * svPtr) {

      short int i, j, k, l;
      FILE *infile;

      j = pd1org;
      strcpy(svPtr->CurrentFile,(char *)zd);
      infile = fopen ((const char *) zd, "r");

      if (infile == NULL) {
            zd[0] = 0;
            return;
            }

      l = j;

      for (i = j; i < svPtr->symlen; i++) {
            k = getc (infile);

            if (k > 127) k = 32;

            if (k =='\\') { // \ at the end of line is continuation
                  k = getc (infile);

                  if (k<1) {
                        printf("\n*** Input error in readf_()\n");
                        sigint(100);
                        }

                  if (k == '\n') {
                        while (k=getc(infile)) {
                              if (k==' ' || k=='\t') continue;

                              if (k=='\r') continue;

                              if (k<1) break;

                              if (k > 0) svPtr->pd1[i] = k;

                              break;
                              }
                        }
                  else {
                        svPtr->pd1[i++]='\\';
                        svPtr->pd1[i] = k;
                        continue;
                        }
                  }


            if (k == CRTN || k == CTRLZ) i--;

            else {
                  if (k > 0) svPtr->pd1[i] = k;
                  }

            if (k == LINEFEED) {
                  svPtr->pd1[i] = 0;
                  continue;
                  }

            if (k < 1) {
                  fclose (infile);
                  svPtr->pd1[i] = 0;
                  svPtr->pd1[i + 1] = 0;
                  svPtr->pd1len = i - 1;
                  return;
                  }

            }

      if (i>=svPtr->symlen)
            ErrorMessage("Interpreter cache overflow\n*** Increase ibuf in configure",
                         svPtr->LineNumber);
      }


/*===========================================================================*
 *                                   inout                                   *
 *===========================================================================*/
void inout (unsigned char text[], short cmnd, short col, struct MSV * svPtr) {
      short int i, j, dev;

      dev = svPtr->io;

      switch (cmnd) {

            case 1:                                   /* new page */

                  fputc ('\014', svPtr->out_file[dev]);
                  svPtr->hor[dev] = 1;
                  svPtr->ver[dev] = 0;
                  return;

            case 2:                                   /* skip line */

                  fprintf (svPtr->out_file[dev], "\n");
                  svPtr->hor[dev] = 0;
                  svPtr->ver[dev]++;
                  return;

            case 3:                                   /* tab */

                  if (col == svPtr->hor[dev]) return;

                  if (col < 1) return;

                  if (dev == 5) {
                        if (svPtr->hor[5]>col) {
                              putchar('\n');
                              svPtr->hor[5]=0;
                              }

                        for (i = svPtr->hor[dev]; i <= col - 1; i++) putchar(' ');
                        }

                  else {
                        if (svPtr->hor[dev]>col) fputc ('\n', svPtr->out_file[dev]);

                        for (j = svPtr->hor[dev]; j <= col - 1; j++)
                              fputc (' ', svPtr->out_file[dev]);
                        }

                  svPtr->hor[dev] = col;
                  return;

            case 4:                                   /* text print */

                  fprintf (svPtr->out_file[dev], "%s", text);

                  if (dev == 5) fflush(stdout);

                  svPtr->hor[dev] = svPtr->hor[dev] + strlen ( (const char *) text);
                  return;
            }
      }


/*===========================================================================*
 *                                  prnterr                                  *
 *===========================================================================*/
void prnterr (int i, struct MSV * svPtr, int pd1cur) {

      int n=1, nx=1, LN=1;

      for (n=1; n<PS; n++) {
            if (! svPtr->pd1[n] ) {
                  LN++;
                  }

            if ( n > pd1cur ) break;
            }

      if (svPtr->pd1[n]) nx=n;
      else nx=n-1;

      for (; svPtr->pd1[nx]&&nx>0; nx--);

      if (nx>1) {
            nx--;

            for (; svPtr->pd1[nx]&&nx>0; nx--);
            }

      svPtr->LineNumber=LN-1;

      if (svPtr->NOERR) return;

      fprintf (stderr, "\n\n*** in or near line %d:\n%s\n*** E%d ", svPtr->LineNumber, &svPtr->pd1[nx+1], i);

      switch (i) {

            case 1:
                  fprintf (stderr, "Multiple adjacent operators");
                  break;

            case 2:
                  fprintf (stderr, "Unmatched quotes");
                  break;

            case 3:
                  fprintf (stderr, "Global not found");
                  break;

            case 4:
                  fprintf (stderr, "Missing comma");
                  break;

            case 5:
                  fprintf (stderr, "Argument not permitted");
                  break;

            case 6:
                  fprintf (stderr, "Bad character after post-conditional");
                  break;

            case 7:
                  fprintf (stderr, "Invalid quote");
                  break;

            case 8:
                  fprintf (stderr, "label not found:");
                  break;

            case 9:
                  fprintf (stderr, "Too many/few fcn arguments");
                  break;

            case 10:
                  fprintf (stderr, "Invalid number");
                  break;

            case 11:
                  fprintf (stderr, "Missing operator");
                  break;

            case 12:
                  fprintf (stderr, "Unrecognized operator: %c",svPtr->ErrChar);
                  break;

            case 13:
                  fprintf (stderr, "Keyword");
                  break;

            case 14:
                  fprintf (stderr, "Argument list");
                  break;

            case 15:
                  fprintf (stderr, "Divide by zero");
                  break;

            case 16:
                  fprintf (stderr, "Invalid expression");
                  break;

            case 17:
                  fprintf (stderr, "Variable not found");
                  break;

            case 18:
                  fprintf (stderr, "Invalid reference");
                  break;

            case 19:
                  fprintf (stderr, "Logical table space overflow");
                  break;

            case 20:
                  fprintf (stderr, "I/O error");
                  break;

            case 23:
                  fprintf (stderr, "Symbol table full");
                  break;

            case 24:
                  fprintf (stderr, "Function argument error");
                  break;

            case 25:
                  fprintf (stderr, "Global not permitted");
                  break;

            case 26:
                  fprintf (stderr, "File error");
                  break;

            case 27:
                  fprintf (stderr, "$N error");
                  break;

            case 29:
                  fprintf (stderr, "<break> at line:");
                  break;

            case 30:
                  fprintf (stderr, "Function not found");
                  break;

            case 31:
                  fprintf (stderr, "Program space exceeded");
                  break;

            case 32:
                  fprintf (stderr, "Stack overflow");
                  break;

            case 33:
                  fprintf (stderr, "SQL error");
                  break;

            case 34:
                  fprintf (stderr, "Arg count mis-match");
                  break;

            case 35:
                  fprintf(stderr, "String length too long");
                  break;

            case 36:
                  fprintf(stderr, "Use unit not open");
                  break;

            case 37:
                  fprintf(stderr, "Unrecognized statement");
                  break;

            default:
                  fprintf (stderr, "Unknown error");
            }

      fprintf(stderr, "\n\n");
      return;
      }


int BuildGlobal(int code, int g, unsigned char * str, unsigned char * rslt, struct MSV * svPtr) {

      static unsigned char tmp[TSTACK][STR_MAX];
      static int stk=0;

      if (code<0) {
            stk=0;
            return 1;
            }

      if (code==0) {

            if (stk>=TSTACK) {
                  printf("*** Build Stack Overflow %d\n\n", svPtr->LineNumber);
                  sigint(100);
                  }

            strcpy( (char *) tmp[stk++],(const char *) str);
            return 1;
            }

      if (code==2) {
            if (stk==0) rslt[0]=0;
            else strcpy( (char *) rslt,(const char *) tmp[--stk]);

            return 1;
            }

// code 1

      stk--;

      if (g==NEXT) GlobalOrder(tmp[stk],rslt, (unsigned char *) "1", svPtr); //GlobalNext(tmp,rslt,svPtr);
      else if (g==ORDERNEXT) GlobalOrder(tmp[stk],rslt, (unsigned char *) "1",svPtr);
      else if (g==ORDERPREV) GlobalOrder(tmp[stk],rslt, (unsigned char *) "-1",svPtr);
      else return 0;

      return 1;
      }
