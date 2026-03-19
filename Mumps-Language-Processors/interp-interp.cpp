//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+ Mumps Compiler Run-Time Support Functions
//+ Copyright (c) 2001 - 2025 by Kevin C. O'Kane
//+
//+ kc.okane@gmail.com
//+ okane@uni.edu
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

//	Dec 20, 2025

/*************************************************************************
   interp.cpp - Mumps Runtime Library Mumps interpretation routines - can
   be used for indirection operations in the compiled code.
	astyle --style=banner --indent=tab interp-interp.cpp.in
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

#define SQLITE



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

//--------------------------------
//	 comments or special code
//--------------------------------

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

//--------------------------------------------
//	skip label - find 1st blank or TAB
//--------------------------------------------

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

	svPtr->xpx = 1;

	to = &svPtr->xd[1];
	from = &svPtr->pd1[pd1cur];

	while ( *(to++) = *(from++) );
	*to = 0;
	pd1cur = from - svPtr->pd1;

// -----------------------------------------------------------------------------
//
// re-enter here for each new command on current line
// check for interrupt (crrlc)
// extract and de-code command word
//
// -----------------------------------------------------------------------------

next_cmnd:       // continue line entry point

	BuildLocal(-1, 0, NULL, NULL, NULL); // stack cleaner
	BuildGlobal(-1, 0, NULL, NULL, NULL); // stack cleaner

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

	icmnd = tolower (svPtr->xd[svPtr->xpx]); // command letter

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

		case BREAK:      // non-standard use - end a FOR loop from a DO group

			BreakFlag = 1;
			goto quit;

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
					svPtr->tpx = TimeOut_getstr1(svPtr->io,
					                             &svPtr->pd1[svPtr->sdlim], timeout, 0);
					}
				else {
					unsigned char i;
					svPtr->tpx = DirectRead(svPtr->io, &i);
					sprintf((char *) &svPtr->pd1[svPtr->sdlim],"%d",i);
					}

				goto store;
				}

			svPtr->hor[svPtr->io] = 0;
			svPtr->ver[svPtr->io]++;
			i = svPtr->io;
			svPtr->sdlim = svPtr->pd1len + 2;

			if (dirflg == 1) {

				unsigned char i;
				svPtr->tpx = DirectRead(svPtr->io, &i);
				sprintf((char *) &svPtr->pd1[svPtr->sdlim], "%d", i);

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

		case JOB:

			if (fork() != 0 ) goto next_line; // we are parent - continue executing - globals remain open

			// treat argument as GOTO otherwise - drop thru


//----------------------------------------------
// Close Globals or Disconnect from SQL server
//----------------------------------------------

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

		case MERGE:

			MergeFlag=1;
			goto set;

		case SET:  // SQL: and SHELL: COMMANDS TOO

			MergeFlag=0;

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

				if (toupper(svPtr->xd[p2px+1]) == 'Q') {
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

//--------------------------------------------------------
//	evaluate return args before killing symbol table
//	case xxx^yyy(a,b,c)
//--------------------------------------------------------

			svPtr->args[0]=0;

			if (retz!=FOR && svPtr->xd[svPtr->xpx] != ' ' && svPtr->xd[svPtr->xpx] != 0) {
				svPtr->t2 = 1;

				if ((svPtr->ierr = parse_ (svPtr))) goto parse_err;

//----------------------------------------------------------------------------
//	copy processed return arg to state vector  - will be piped to parent
//----------------------------------------------------------------------------

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

		case ELSE:

			if (svPtr->xd[svPtr->xpx] == ' ') svPtr->xpx++;

			if (svPtr->tpx == 0) goto next_cmnd;

			svPtr->xpx = STR_MAX;

			goto next_cmnd;

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

	return 0;

	}                                                 /* function main() */

