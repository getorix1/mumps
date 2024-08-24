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

short setpiece;
unsigned char setname[128];
unsigned char bd[2048];
short int tpx;
short t2;
short pd1len=0;
short iorg;
long xpx;
short t0px;
short ierr;
short symlen;
short sdlim;
unsigned char pd1[PS1];
unsigned char xd[2048];
unsigned char v1d[2048];
short int ctrlc;
int gpad;
short ierr1;
struct nmes *nstart;
short Jflg;
short MAIN_flg = 0;
char FunctionName[256];
short ArgMax = 0;
char ErrChar=' ';

struct nmes {
    char *name;
    struct nmes *next;
    } ;

struct txt {
    char *name;
    int lnbr;
    struct txt *next;
    } *LabelsStart=NULL, *TextStart=NULL, *p1=NULL, *p3=NULL;

//--------------------------
//	declared variables
//--------------------------

#define CVARMAX 100

struct dcls {
    char name[32];
    int type;
    } cvars[CVARMAX];

int cvarx;

int cvar (char *);

char * Extract2(unsigned char *);
char * Extract1(unsigned char *);
void ScanPastPostCond(long *xpx, char *xd, int *j);
int   xindex (unsigned char *, unsigned char *, short);
void  errmod_ (int msgnbr);
int   readf_ (unsigned char zd[], short pd1org);
void  inout (unsigned char text[], short cmnd, unsigned char col[]);
int   pindex (unsigned char source[], unsigned char ptrn[], short strt);
void  add (char *a, char *b, char *c);
int   numcomp (char aa[], char bb[]);
void  linepad (int, char *);
void  linepad2 (char *);
void  fcninit ();
void  epi (short, long);
void  EP (int, short, long);
int   getstr1 (FILE *, char[]);
int   LevCheck(short ExecName, short DoRetX, short DoRet[]);
int   doty(int j, short *ExecLevel, short *ExecName, short DoRet[], short DoRetX, short *pd1Last,
           short pd1cur, short ExecStack[], short *ExecX, short ExecSym[]);
void  LocalArray (unsigned char vd[], short *Set_flg);
void  GlobalArray(unsigned char v1d[], short *GBL_flg, unsigned char *vd);
int   atTest(unsigned char xd[], unsigned char pd1[], short *pd1Last, short lines, int xpx);
void  atCopy (int at_flg, unsigned char tmp[]);

FILE  *Out,*Final;
short ExitCode = 0;
short MaxTmp = 2; /* max number of temp strrings used so far in this function */
int iiitmp = 0; /* count on temp strings used so far in this stmt */
int   forxmax = 0;
int   forxFLG=0,forLine=0,forx = 0;
int   IF_flg = 0;
int   IF_flg_max = 0;
int   for_fcn_flg=0, for_fcn_flg2=0;
short ForBlocks[FBMAX];
short Blocks[FBMAX];
short FCN_flg = 0;
int lines;

    short       pd1cur, i, j, k;
    short       l, parse_ ();

    short cmdx, pd1Last, POST_flg = 0;
    short GBL_flg = 0;
    short exportflag=0;
    short ExecLevel = 0;
    short ExecName = 0;
    short ExecStack[25];
    short ExecSym[25] = { 0 };
    short ExecX = 0;
    short DoRet[1000];
    short DoRetX=0;
    short CX,EmitLabel=0;
    short JOB_flg = 0;
    short at_flg = 0, Set_flg = 0;
    short LockFlag=0;
    short SubFlag=0;
    short SymIncrFlg = 0;
    short for_mult = -1;
    int F_flg = 0;
    short SinglesFlg = 0;
    int MergeFlg=0;
    int MergeTmp=0;
    int TryFlg=0;
    int CatchFlg=0;

    FILE *opnfile[7];
    FILE *in;
    unsigned char forv[256];
    int FOR_limit;
    unsigned char dosave[64];
    char cur_line[512];
    char *lhs[25];
    int lhsGF[25];
    int lhsx=0;
    int lhsf=0;
//    long pid=getpid();
    long pid;


//-------------------------
//	cod208=208 comma
//	cod207=207 close
//	cod209=209 comma
//	cod210=210 colon
//	cod206=206 open
//-------------------------

    short retz, icmnd, p2px, ernbr;
    unsigned char output_file[1024]="", tmp2[1024], tmp3[1024], dirflg, vd[1024];
    int pl;
    char *p2;

    char *except[] = {
        "InputLengthException",
        "InterpreterException",
        ""
        };
    int flg=1;
