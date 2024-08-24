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

//	May 15, 2024

int set_code(int);
int for_code();
int error_code();
int abtrx();
int if_code();
int lock_code();
void copyright_notice(FILE *);

#define INTERPRET fprintf (Out, "	_jj = Interpret( (CCHR) \n"); \
		  fprintf (Out, "		\"%s\", svPtr); \n", tmp); \
                  fprintf (Out, "	svPtr->ERROR = _jj; \n"); \
                  fprintf (Out, "	if (!svPtr->NOERR && _jj < 0) \n"); \
                  fprintf (Out, "		ErrorMessage(\"Indirection error\", "\
				"svPtr->LineNumber); \n");

#define SymCheck if (ExecSym[ExecX]) \
	{ fprintf(Out,"	_SymFree(1,svPtr); /* Remove symbol table layer */\n"); \
	ExecSym[ExecX]=0; }

#define Sysout(x) fprintf(Out,x)

#define Sysout_skip fprintf(Out,"\n")

#define SWAP 30
#define STORE 1
#define RETRIEVE 0
#define NEXT 2
#define XNEXT 8
#define GKILL 3
#define INIT 6
#define GOPEN 7
#define EMPTY 5
#define CLOSE 4

//-------------------------
//	error return codes
//-------------------------

#define PARSE_ERROR 1
#define ARG_LIST_ERROR 2
#define NO_GLOBAL_PERMITTED 3
#define MEM_ERROR 4
#define TOO_MANY_LHS 5
#define ABRTX 6
#define KILL_DCL_ERROR 7
#define NEXT_LINE 8
#define BAD_LOCK 9

//----------------------------
//	entry points in set
//----------------------------

#define SET 0
#define SET_STORE 1

//-----------------
//	commands
//-----------------

#define JOB       'j'
#define SET       's'
#define MERGE     'm'
#define GOTO      'g'
#define WRITE     'w'
#define IF        'i'
#define NEW       'n'
#define NEW1      'N'
#define OPEN      'o'
#define ZCMDS     'z'
#define READ      'r'
#define QUIT      'q'
#define XECUTE    'x'
#define FOR       'f'
#define HALT      'h'
#define ELSE      'e'
#define EXPORTS   'y'
#define DO        'd'
#define KILL      'k'
#define CLOSE     'c'
#define CONTINUE  'b'
#define USE       'u'
#define LOCK      'l'
#define CMMNT     ';'
#define BREAK     'b'
#define HTML      '<'
#define KILL2     20
#define FCN       'D'
#define LBL       21
#define CLSQ      '}'
#define OPSQ      '{'
#define TRY       't'
#define CATCH     'c'

#define FBMAX 4000 /* max number of FOR blocks */

#ifdef NOTABS
#define TAB ' '
#else
#define TAB 9
#endif
