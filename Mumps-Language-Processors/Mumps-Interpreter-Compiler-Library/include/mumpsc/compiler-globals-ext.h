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

//	May 6, 2024

extern short setpiece;
extern unsigned char setname[128];
extern unsigned char bd[2048];
extern short int tpx;
extern short t2;
extern short pd1len;
extern short iorg;
extern long xpx;
extern short t0px;
extern short ierr;
extern short symlen;
extern short sdlim;
extern unsigned char pd1[PS1];
extern unsigned char xd[2048];
extern unsigned char v1d[2048];
extern short int ctrlc;
extern int gpad;
extern short ierr1;
extern struct nmes *nstart;
extern short Jflg;
extern short MAIN_flg;
extern char FunctionName[256];
extern short ArgMax;
extern char ErrChar;

struct nmes {
    char *name;
    struct nmes *next;
    } ;

struct txt {
    char *name;
    int lnbr;
    struct txt *next;
    } extern *LabelsStart, *TextStart, *p1, *p3;

//--------------------------
//	declared variables
//--------------------------

#define CVARMAX 100

struct dcls {
    char name[32];
    int type;
    } extern cvars[CVARMAX];

extern int cvarx;

extern int cvar (char *);

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

extern FILE  *Out,*Final;
extern short ExitCode;
extern short MaxTmp; /* max number of temp strrings used so far in this function */
extern int iiitmp; /* count on temp strings used so far in this stmt */
extern int   forxmax;
extern int   forxFLG,forLine,forx;
extern int   IF_flg;
extern int   IF_flg_max;
extern int   for_fcn_flg, for_fcn_flg2;
extern short ForBlocks[FBMAX];
extern short Blocks[FBMAX];
extern short FCN_flg;
extern int lines;


    extern short       pd1cur, i, j, k;
    extern short       l, parse_ ();

    extern short cmdx, pd1Last, POST_flg;
    extern short GBL_flg;
    extern short exportflag;
    extern short ExecLevel;
    extern short ExecName;
    extern short ExecStack[25];
    extern short ExecSym[25];
    extern short ExecX;
    extern short DoRet[1000];
    extern short DoRetX;
    extern short CX,EmitLabel;
    extern short JOB_flg;
    extern short at_flg, Set_flg;
    extern short LockFlag;
    extern short SubFlag;
    extern short SymIncrFlg;
    extern short for_mult;
    extern int F_flg;
    extern short SinglesFlg;
    extern int MergeFlg;
    extern int MergeTmp;
    extern int TryFlg;
    extern int CatchFlg;

    extern FILE *opnfile[7];
    extern FILE *in;
    extern unsigned char forv[256];
    extern int FOR_limit;
    extern unsigned char dosave[64];
    extern char cur_line[512];
    extern char *lhs[25];
    extern int lhsGF[25];
    extern int lhsx;
    extern int lhsf;
    extern long pid;


//-------------------------
//	cod208=208 comma
//	cod207=207 close
//	cod209=209 comma
//	cod210=210 colon
//	cod206=206 open
//-------------------------

    extern short retz, icmnd, p2px, ernbr;
    extern unsigned char output_file[1024], tmp2[1024], tmp3[1024], dirflg, vd[1024];
    extern int pl;
    extern char *p2;

    extern char *except[];

    extern int flg;

