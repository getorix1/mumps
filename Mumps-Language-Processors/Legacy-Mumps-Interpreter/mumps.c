//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                                  
//#+    #     # #     # #     # ######   #####
//#+    ##   ## #     # ##   ## #     # #     #
//#+    # # # # #     # # # # # #     # #
//#+    #  #  # #     # #  #  # ######   #####
//#+    #     # #     # #     # #             #
//#+    #     # #     # #     # #       #     #
//#+    #     #  #####  #     # #        #####
//#+    
//#+    Mumps Interpreter                            
//#+
//#+    Copyright (C) 2022  Kevin C. O'Kane
//#+
//#+    Kevin C. O'Kane, Ph.D.
//#+    Computer Science Department
//#+    University of Northern Iowa
//#+    Cedar Falls, IA 50614-0507
//#+    kc.okane@gmail.com
//#+    https://threadsafebooks.com/
//#+    https://www.cs.uni.edu/~okane
//#+
//#+    This program is free software; you can redistribute it and/or modify
//#+    it under the terms of the GNU General Public License as published by
//#+    the Free Software Foundation; either version 2 of the License, or
//#+    (at your option) any later version.
//#+
//#+    This program is distributed in the hope that it will be useful,
//#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#+    GNU General Public License for more details.
//#+
//#+    You should have received a copy of the GNU General Public License
//#+    along with this program; if not, write to the Free Software
//#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+
//#+                                                              
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// code formatting
// astyle --style=banner
// bcpp -bcl -ybbi -i 5 -s < mumps.c

#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

#include "memsize.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <sys/stat.h>

#define GOSUBMAX 30
#define SWAP 30
#define STORE 1
#define RETRIEVE 0
#define NEXT 2
#define XNEXT 8
#define GKILL 3
#define INIT 6
#define GOPEN 7
#define EMPTY 5
#define GCLOSE 4

#define JOB 'j'
#define SET 's'
#define GOTO 'g'
#define WRITE 'w'
#define IF 'i'
#define FILE_OPEN 'o'
#define ZCMDS 'z'
#define READ 'r'
#define QUIT 'q'
#define XECUTE 'x'
#define TAB 9
#define FOR 'f'
#define HALT 'h'
#define ELSE 'e'
#define DO 'd'
#define KILL_VAR 'k'
#define FILE_CLOSE 'c'
#define USE 'u'
#define LOCK 'l'
#define CMMNT ';'
#define BREAK 'b'
#define HTML '<'
#define KILL2 20
#define FCN '$'
#define LBL 21

#include "defs.h"
#include "stateVector.h"
#include "keyfix.h"

int xindex(unsigned char *, unsigned char *, short);
void errmod_(short msgnbr, unsigned char text[],FILE *opnfile[]);
void fcn();
void readf_(unsigned char zd[], short pd1org, FILE *opnfile[]);
void inout(int popn[7][4], unsigned char opnflg[], FILE *opnfile[],
           unsigned char text[], short cmnd, short col);
int pindex(unsigned char source[], unsigned char ptrn[], short strt);
int global(short g, unsigned char key[], unsigned char bd[]);
void add(char *a, char *b, char *c);
int numcomp(char aa[], char bb[]);
void cannon(char a[]);
void sym_(unsigned char a[], unsigned char b[]);
void prnterr(int);
struct MSV * AllocSV(void);
void sigint(int i);

struct MSV *svPtr = NULL;

main(int argc, char *argv[]) {

    extern int errno;
    long int ilong,jlong;
    short pd1hold1,pd1hold2,ibr,pd1cur,i,mti=0,j,k;
    short l,new,parse_(),getstr1();
    static char *swp[SWAP]; /* swap pointers */

    /*----------------------------------------------
    |                                               |
    |       gosub stack                             |
    |                                               |
    |       0 - swapfile origin                     |
    |       1 - pd1cur                              |
    |       2 - addr in cmdstack                    |
    |       3 - return point [label]                |
    |       4 - original pd1len                     |
    |                                               |
    ------------------------------------------------*/

    static short gosub[GOSUBMAX][6],gsbx,cmdx,forx,cmdxold,pd1Last;
    static unsigned char cmdstack[2048];

    static char opnflg[6] = {0,0,0,0,0,0}; //* 0: not open; 1: input; 2: output
    FILE *opnfile[7];
    static int popn[7][4]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static unsigned char lbls[20][20];
    static int lbloff[20],lblx;
    static unsigned char forv[30][20],forit[30],forlt[30],ftmp[1024];
    char forin[30][13],forlv[30][13];
    static unsigned short swapextent,shold;
    static short ExecLevel=1;
    static short BreakFlag=0;
    static unsigned char dosave[64];
    static unsigned char cod206[2]= {206,0}; /* open */
    static unsigned char cod208=208; /* comma */
    static unsigned char cod207=207; /* close */
    static unsigned char cod209=209; /* comma */
    static unsigned char cod210=210; /* colon */
    static short retz,icmnd,p2px,ernbr,fileflg;
    static unsigned char tmp2[1024],tmp3[1024],dirflg,vd[1024];
    unsigned char * pd1p=&pd1[1];

    if (! svPtr ) {
        svPtr= AllocSV();
        }

    gpad=0;
    for (i=0; i<SWAP; i++) swp[i]=NULL;

//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//        init environment: io default io unit (5=terminal).
//                          pd1 is user partition (PS bytes long).
//                          gosub is stack for do's, for's and xecutes.
//                          symlen is bottom of symbol table (symbol table
//                                 grows down from top of user partition).
//                          PS is size of user partition.
//                          pd1cur is current location in user partition.
//                          pd1len is length of user pgm in partition.
//                          initialize random nbr generator.
//
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    io=5;
    mflag=0; // window enabled
    strnset(pd1,(int) ' ',PS);
    symlen=PS;
    pd1cur=1;
    pd1len=0;
    srand(1);
    for (i=0; i<GOSUBMAX; i++) for (j=0; j<=6; j++) gosub[i][j]=0;

#include "cgi.h"  // interface to web server

//****************************************************************************
//*******
//******* open or create/init global array file
//*******
//****************************************************************************

    global(GOPEN,v1d,bd);

//****************************************************************************
//*******
//******* set trap for <interrupt>.
//******* install default program to execute init.mps.
//*******
//****************************************************************************

    pd1cur=1;
    pd1[pd1len+1]=0;
    ctrlc=0;

//****************************************************************************
//*******
//******* re-enter here after error.
//******* re-set environment.
//******* if user error trap set, copy user error cmnd line to pgm area.
//*******
//****************************************************************************

restart:

    BreakFlag=0;
    ExecLevel=1;
    zerr=ernbr;
    ierr=0;
    opnfile[5]=stdin;
    if (ernbr==31) symlen=PS;
    Tab=0;
    io=5;
    t0px=0;
    t2=0;

    for (i=0; i<SWAP; i++) {  // clear the swap area
        if (swp[i]!=NULL) free(swp[i]);
        swp[i]=NULL;
        }

    swapextent=0;
    kflg=0;
    lblx=0;

restore:

//****************************************************************************
//*******
//******* write prompt to terminal and read a direct mode command into the
//******* partition.
//******* set pd1cur to this line.  init horizontal and vertical markers.
//******* kill gosubs and for's.  disable initial init.mps flag (mti).
//*******
//*****************************************************************************

    puts("\n");

    if (mti) {

        while(1) {
            char *line;

            line = readline (":> ");

            if (line == NULL) { // ^d - end of file
                raise(SIGQUIT);
                }

            if (strlen(line)==0) {
                free(line);
                continue;
                }

            add_history(line);

            if (strcasecmp(line,"h")==0 ||
                    strcasecmp(line,"\\h")==0 ||
                    strcasecmp(line,"\\q")==0 ||
                    strcasecmp(line,"\\quit")==0 ||
                    strcasecmp(line,"halt")==0 ) {
                raise(SIGQUIT);
                }
            pd1[1]=9;
            strcpy(&pd1[2], line);
            pd1len = strlen(&pd1[1]) + 1;
            break;
            }
        }

    BreakFlag=0;
    pd1cur=1;
    cmdx=0;
    setpiece=0;
    lblx=0;
    forx=0;
    mti=1;
    gsbx=0;
    kflg=0;
    pd1Last=0;

//****************************************************************************
//*******
//******* re-enter here for each new program command line
//******* extract next command into xd.
//******* re-enter here for running program.
//******* quit running program if at end.
//*******
//****************************************************************************

next_line:

    if (pd1cur>pd1len) goto quit;

/***************************************************************8
    if (pd1[pd1cur]=='!'||pd1[pd1cur]=='<') {
        if (pd1[pd1cur]=='<') putchar('<');
        pd1cur++;

write_html:

        while (pd1[pd1cur]) {
            if (pd1[pd1cur]=='&' && pd1[pd1cur+1]=='~') {
                pd1cur+=2;
                for (i=1; pd1[pd1cur]!='~'&&pd1[pd1cur]!=0; i++)
                    xd[i]=pd1[pd1cur++];
                xd[i]=0;
                if (pd1[pd1cur]=='~') pd1cur++;
                t2=1;
                xpx=1;
                if ((ierr=parse_())) goto parse_err;
                printf("%s",&pd1[sdlim]);
                continue;
                }
            if (pd1[pd1cur]=='&' && pd1[pd1cur+1]=='!') {
                pd1cur+=2;
                putchar('\n');
                continue;
                }
            putchar(pd1[pd1cur]);
            pd1cur++;
            }
        pd1cur++;
        goto next_line;
        }
************************************************************8**/

    if (pd1[pd1cur] == '#' || pd1[pd1cur] == '%') {
        pd1cur++;
        while (pd1[pd1cur]) pd1cur++;
        pd1cur++;
        goto next_line;
        }

    i=pd1cur;
    while (pd1[pd1cur] && pd1[pd1cur]!=TAB) pd1cur++;

//    if (pd1[pd1cur]==0) {
//        pd1cur=i;
//        goto write_html;
//        }

    /************
    multiple TABs
    ************/

    while (pd1[pd1cur]==TAB) pd1cur++;

    /*********************
    copy source line to xd
    *********************/

    if (pd1[pd1cur]=='.') {

        if (strncmp(&pd1[pd1cur],". ",2)==0) {
            if (ExecLevel!=2) {
                while(pd1[pd1cur++]);
                goto next_line;
                }
            pd1cur+=2;
            }

        else if (strncmp(&pd1[pd1cur],".. ",3)==0) {
            if (ExecLevel!=3) {
                while(pd1[pd1cur++]);
                goto next_line;
                }
            pd1cur+=3;
            }

        else if (strncmp(&pd1[pd1cur],"... ",4)==0) {
            if (ExecLevel!=4) {
                while(pd1[pd1cur++]);
                goto next_line;
                }
            pd1cur+=4;
            }

        else if (strncmp(&pd1[pd1cur],".... ",5)==0) {
            if (ExecLevel!=5) {
                while(pd1[pd1cur++]);
                goto next_line;
                }
            pd1cur+=5;
            }

        else if (strncmp(&pd1[pd1cur],"..... ",6)==0) {
            if (ExecLevel!=6) {
                while(pd1[pd1cur++]);
                goto next_line;
                }
            pd1cur+=6;
            }
        }

    pd1Last=pd1cur;
    xpx=j=1;
    while ((xd[j++]=pd1[pd1cur++])!=0);
    xd[j]=0;

//-----------------------------------------------------------------------------
//
//    re-enter here for each new command on current line
//    check for interrupt (crrlc)
//    extract and de-code command word
//
//-----------------------------------------------------------------------------

next_cmnd:      // continue line entry point

    while (1) {
        if (xd[xpx]==0||xpx>=1024)
            if (forx>0 && gosub[gsbx][3]==FOR) {
                if (ctrlc) {
                    ctrlc=0;
                    ernbr=29;
                    goto err_call;
                    }
                goto quit1;
                }
            else goto next_line;

        if (xd[xpx]!=' '&&xd[xpx]!='\t') break;
        xpx++;
        }

// decode command word

    p2px=xpx;
    t2=t0px=0;
    icmnd=tolower(xd[xpx]);
    while (isalpha(xd[++xpx])) ;                    // Ignore rest of word

    if (xd[xpx]==':') { // Postconditional
        xpx++;
        if ((ierr=parse_())) goto parse_err;
        if (!atoi(&pd1[sdlim])) {
            while (xd[xpx++]!=0) {
                if (xd[xpx]=='"') {
                    xpx++;
pc1:
                    while (xd[xpx]!='"'&&xd[xpx]!=0) xpx++;
                    xpx++;
                    if (xd[xpx]=='"') {
                        xpx++;
                        goto pc1;
                        }
                    }
                if (xd[xpx]==' ') goto next_cmnd;
                }
            xpx--;
            goto next_cmnd;
            }
        }

//-------------------------------------------------------------------------

// branch to correct command handler

    if (xd[xpx]) xpx++;                         // post conditional return

    switch (icmnd) {                              // go to command handler

        case CMMNT:
            goto next_line;              // comment command - do nothing

        case FCN:                                     // direct function execution

            xpx=p2px;
            t0px=0;
            t2=1;
            xd[xpx]-=32;
            if ((ierr=parse_())) goto parse_err;
            xpx++;
            goto next_cmnd;

        case BREAK:           // non-standard use - end a FOR loop from a DO group

            BreakFlag=1;
            goto quit;

        case LOCK:  // not meaningful - all files are locked.

            if (xd[xpx]==' '||xd[xpx]==0) {
                goto next_cmnd;
                }

            t0px=1;
            if ((ierr=parse_())) goto parse_err;
            tpx=1;

            if (xd[xpx+1]==':') {
                t2=1;
                xpx+=2;
                if ((ierr=parse_())) goto parse_err;
                xpx++;
                j=atoi(&pd1[sdlim]);
                // for (i=1; i<=j; i++) {  sleep(1); }
                tpx=0;
                goto next_cmnd;
                }

            xpx++;
            goto next_cmnd;

        case READ:

read_command:

            if (io<=4&&opnflg[io]!=1) goto io_error;

            dirflg=0;

            if (xd[xpx]=='@') {                                    // indirect
                t0px=1;
                if ((ierr=parse_())) goto parse_err;
                }

            if (xd[xpx]=='!'||xd[xpx]=='#') {              // carriage control
                if (xd[xpx]!='#') i=2;
                else i=1;
                goto start_io;
                }

            if (xd[xpx]=='?') {                                  // tab format
                t2=1;
                xpx--;
                if ((ierr=parse_())) goto parse_err;
                j=atoi(&pd1[sdlim]);
                i=3;
                goto start_io;
                }

            if (xd[xpx]=='"') {                    // text to be printed first
                t2=1;
                if ((ierr=parse_())) goto parse_err;
                i=4;
start_io:
                inout(popn,opnflg,opnfile,&pd1[sdlim],i,j);
                goto cont_read;
                }

            if (xd[xpx]=='*') {                                // direct mode
                xpx++;
                dirflg=1;
                }

            t0px=1;                       // extract variable name to be read
            if ((ierr=parse_())) goto parse_err;
            strcpy(&vd[1],&v1d[1]);

            hor[io]=0;
            ver[io]++;
            i=io;
            sdlim=pd1len+2;

            if (dirflg==1&&io==5) {

                pd1[sdlim]=0;

                }

            else {
                tpx=getstr1(popn,opnflg,opnfile,i,&pd1[sdlim]);
                if (tpx>=0) tpx=1;
                else tpx=0;
                }

store:
            retz=READ;
            goto lhsref;

cont_read:

            xpx++;
            dirflg=0;
            if (xd[xpx]==',') xpx++;
            if (xd[xpx]==' '||xd[xpx]==0||xd[xpx]=='\t') goto next_cmnd;
            goto read_command;


        case FILE_OPEN:

#include "open.h"

        case ZCMDS:

            p2px++;
            if (xd[p2px]<=90) xd[p2px]+=32; // lower case

            if (xd[p2px]=='g') { // zg
                v1d[0]=0;
                global(INIT,v1d,bd);
                goto next_cmnd;
                }

            if (xd[p2px]=='a') {
                gpad=0;
                goto next_cmnd;
                }
            if (xd[p2px]=='n') {
                gpad=1;
                goto next_cmnd;
                }

            if (xd[p2px]=='s') { /* zseek */
                long i;
                if (opnflg[io]==0) goto next_cmnd;
                t2=1;
                if ((ierr=parse_())) goto parse_err;
                i=atol(&pd1[sdlim]);
                fseek(opnfile[io],i,SEEK_SET);
                goto next_cmnd;
                }

            if (xd[p2px]=='e') { // zexit
abrt:
                global(GCLOSE,v1d,bd);
                return;
                }

            if (xd[p2px]=='p') { // zprolog

// zp:             t0px=1;
//                 if ((ierr=parse_())) goto parse_err;
//                 strcpy(tmp2,xd);
//                 i=xpx;
//                 logic(opnfile);
//                 if (ierr) goto restart;
//                 if (bd[1]=='1') tpx=1;
//                         else tpx=0;
//                 strcpy(xd,tmp2);
//                 xpx=i;
//                 if (xd[++xpx]==',') {
//                        xpx++;
//                        goto zp;
//                        }
                goto next_cmnd;
                }

            goto next_line; // all others

        case IF:

            if (xd[xpx]==' ') { // no argument form
                xpx++;
                if (tpx) goto next_cmnd;
                xpx=1024;
                goto next_cmnd;
                }

next_if_arg:

            t2=1;
            if ((ierr=parse_())) goto parse_err;

            if (numcomp(&pd1[sdlim],"0")==0) {
                tpx=0;
                xpx=1024;
                goto next_cmnd;
                }

            tpx=1;
            if (xd[xpx]==',') {
                xpx++;
                goto next_if_arg;
                }

            goto next_cmnd;

        case WRITE:

            if (io<=4&&opnflg[io]<2) goto io_error;

writeproc:

            if (xd[xpx]=='!'||xd[xpx]=='#') { // new line or page*/
                i=1;
                if (xd[xpx]!='#') i=2;
                xpx++;
                goto iosub;
                }

            if (xd[xpx]=='*') { // "*" output
                xpx++;
                t2=1;
                if ((ierr=parse_())) goto parse_err;
                pd1[sdlim]=atoi(&pd1[sdlim]);
                pd1[sdlim+1]=0;

                putchar(pd1[sdlim]);

                i=4;
                goto iosub1;
                }

            if (xd[xpx]=='?') { // tab
                t2=1;
                xpx++;
                if ((ierr=parse_())) goto parse_err;
                j=atoi(&pd1[sdlim]);
                i=3;
                goto iosub;
                }

            t2=1;
            if ((ierr=parse_())) goto parse_err;
            i=4;

iosub:
            inout(popn,opnflg,opnfile,&pd1[sdlim],i,j);

iosub1:
            if (xd[xpx]==',') {
                xpx++;
                goto writeproc;
                }

            if (xd[xpx]==0||xd[xpx]==' '||xd[xpx]=='\t') goto next_cmnd;
            goto writeproc;

        case JOB:

            goto next_line;

        case GOTO:

            iorg=1;
            retz=GOTO;
            goto lbl;

goto_return:

            forx=0;
            pd1cur=ibr;
            goto next_line;

        case SET:

set:
            if (xd[xpx]=='@') {
                kflg=1;
                t0px=1;
                if ((ierr=parse_())) goto parse_err;
                kflg=0;
                }

            ftmp[0]=0;
            if (xd[xpx]=='(') {
                xpx++;
                vd[1]=0;
s1:
                t0px=1;
                if ((ierr=parse_())) goto parse_err;
                strcat(ftmp,&v1d[1]);
                strcat(ftmp," ");
                xpx++;

                if (xd[xpx]!=')'&&xd[xpx]) {
                    xpx++;
                    goto s1;
                    }
                }

            else {
                t0px=1;
                setname[0]=0;
                if ((ierr=parse_())) goto parse_err;
                strcpy(&vd[1],&v1d[1]);
                }

            xpx+=2;
            t0px=0;
            t2=1;
            if ((ierr=parse_())) goto parse_err;
            retz=SET;
            k=0;
            if (vd[1]) goto lhsref;

s2:
            j=1;
            while((vd[j++]=ftmp[k++])!=' ');
            vd[j-1]=0;
            goto lhsref;

set_return:

            if (ftmp[k]) goto s2;
            if (xd[xpx]==',') {
                xpx++;
                goto set;
                }
            goto next_cmnd;

        case QUIT:

quit:
            if (gsbx<=0) goto restore;

            if (gosub[gsbx][0]>0) {
                lblx=0; /* zap labels */

                memcpy(pd1p,swp[gosub[gsbx][0]],(size_t)gosub[gsbx][4]);
                pd1[gosub[gsbx][4]+1]=0;
                swapextent=gosub[gsbx][0];
                if (gosub[gsbx][5]>0) symlen=gosub[gsbx][5];
                }

            if (forx>0&&gosub[gsbx][3]==FOR) {
                cmdx=gosub[gsbx][2];
                gsbx--;
                forx--;
                xpx=1024;
                goto next_cmnd;
                }

quit1:
            pd1cur=gosub[gsbx][1];
            cmdxold=cmdx;
            cmdx=gosub[gsbx][2];
            strcpy(&xd[1],&cmdstack[cmdx]);
            pd1len=gosub[gsbx][4];
            xpx=1;
            retz=gosub[gsbx][3];
            gsbx--;
            if (forx>0&&retz==FOR) goto for_end;
            if (retz==DO&&ExecLevel>1) ExecLevel--;
            if (xd[1]==' '||xd[1]==0) goto next_cmnd;
            if (retz==XECUTE) goto xecute_return;
            goto do_ret1;

        case XECUTE:

xecute:

            t2=1;
            if ((ierr=parse_())) goto parse_err;
            ftmp[0]=TAB;
            strcpy(&ftmp[1],&pd1[sdlim]);

            if (xd[xpx]==':') {
                xpx++;
                t2=1;
                if ((ierr=parse_())) goto parse_err;
                cannon(&pd1[sdlim]);

                if ((k=atoi(&pd1[sdlim]))==0) {
                    if (xd[xpx]==',') {
                        xpx++;
                        goto xecute;
                        }
                    else goto next_cmnd;
                    }
                }

//#+ move line to be executed into code space above

            l=pd1len;
            strcpy(&pd1[pd1len+2],ftmp);
            pd1len=pd1len+2+strlen(ftmp);
            gsbx++;
            if (gsbx>29) goto stack_overflow;
            gosub[gsbx][0]=0; /* swap origin */
            gosub[gsbx][1]=pd1cur; /* restore pd1cur */
            gosub[gsbx][2]=cmdx; /* command stack address */
            strcpy(&cmdstack[cmdx],&xd[xpx]);
            cmdx=cmdx+strlen(&xd[xpx])+1;
            gosub[gsbx][4]=l; /* restore pd1len */
            gosub[gsbx][3]=XECUTE;
            pd1cur=l+2;
            goto next_line;

//#+ return point - environment restored

xecute_return:

            xpx++;
            goto xecute;

        case FOR:

            /* find local variable */

            t0px=1;
            if ((ierr=parse_())) goto parse_err;
            if (++forx>29) goto stack_overflow;
            xpx+=2;
            strcpy(&forv[forx][0],&v1d[1]);

            /* stack set up */

            forit[forx]=0;
            forlt[forx]=0;
            if (++gsbx>29) goto stack_overflow;
            gosub[gsbx][0]=0; /* swap */
            gosub[gsbx][1]=pd1cur;
            gosub[gsbx][2]=cmdx;
            gosub[gsbx][3]=FOR;
            gosub[gsbx][4]=pd1len;

nxt_arg:

            t2=1;
            if ((ierr=parse_())) goto parse_err;
            forit[forx]=0;
            strcpy(ftmp,&pd1[sdlim]); /* initial */


            if (xd[xpx]==','||xd[xpx]==' '||xd[xpx]==0) goto init_loop_var;
            if (xd[xpx]!=':') goto arg_list_error;

            t2=1;
            xpx++;
            if ((ierr=parse_())) goto parse_err;
            forit[forx]=1;
            strcpy(&forin[forx][0],&pd1[sdlim]); /* increment */

            if (xd[xpx]==','||xd[xpx]==0||xd[xpx]==' '
                    ||xd[xpx]=='\t') goto init_loop_var;
            if (xd[xpx]!=':') goto arg_list_error;

            t2=1;
            xpx++;
            if ((ierr=parse_())) goto parse_err;

            forlt[forx]=1;
            strcpy(&forlv[forx][0],&pd1[sdlim]); /* limit */

            if (xd[xpx]==','||xd[xpx]==' '||xd[xpx]==0
                    ||xd[xpx]=='\t') goto init_loop_var;
            goto arg_list_error;

init_loop_var:

            symflg=0;  /* store */
            sym_(&forv[forx][0],ftmp);
            if (symlen<=pd1len) goto program_size;
            i=0;

            if (forlt[forx]==1) {
                if (numcomp(&forin[forx][0],"0")>=0) {
                    if (numcomp(ftmp,&forlv[forx][0])>0) i=1;
                    }

                else {
                    if (numcomp(ftmp,&forlv[forx][0])<0) i=1;
                    }
                }

            strcpy(&cmdstack[cmdx],&xd[xpx]);
            cmdx=cmdx+strlen(&xd[xpx])+1;

byp:
            while (xd[xpx]!=0&&xd[xpx]!=' ') {
                t2=1;
                xpx++;
                parse_();
                }

            if (i==0) goto next_cmnd;
            else goto quit1;

for_end:  /* end of line return */

            if (forit[forx]!=1) {

for_quit:
                if (xd[xpx]==',') {
                    xpx++;
                    gsbx++;
                    goto nxt_arg;
                    }

                forx--;
                xpx=1024;
                goto next_cmnd;
                }


                {
                /* increment loop variable */

                char itmp[64];
                symflg=1; /* retrieve */
                sym_(&forv[forx][0],itmp);
                strcpy(dosave,&forin[forx][0]);
                add(&itmp[1],dosave,ftmp);
                }

            if (BreakFlag) {
                BreakFlag=0;
                goto for_quit;
                }

            if (forlt[forx]!=1) {
                symflg=0; /* store */
                sym_(&forv[forx][0],ftmp);
                if (symlen<=pd1len) goto program_size;
                gsbx++;
                cmdx=cmdxold;

                if (xd[xpx]==',') {
                    i=0;
                    goto byp;
                    }

                goto next_cmnd;
                }

            /* limit test */

            if (numcomp(&forin[forx][0],"0")>=0) {
                if (numcomp(ftmp,&forlv[forx][0])>0) goto for_quit;
                }

            else {
                if (numcomp(ftmp,&forlv[forx][0])<0) goto for_quit;
                }

            symflg=0; /* store */
            sym_(&forv[forx][0],ftmp);
            if (symlen<=pd1len) goto program_size;
            gsbx++;
            cmdx=cmdxold;

            if (xd[xpx]==',') {
                i=0;
                goto byp;
                }

            goto next_cmnd;

        case HALT:

            if (xd[xpx]==' '||xd[xpx]==0) raise(SIGQUIT);

hang:

            t2=1;
            if ((ierr=parse_())) goto parse_err;
            j=atoi(&pd1[sdlim]);
            if (j>0) sleep(j);
            if (xd[xpx]==',') {
                xpx++;
                goto hang;
                }

            goto next_cmnd;

        case ELSE:

            if (xd[xpx]==' ') xpx++;
            if (tpx==0) goto next_cmnd;
            xpx=1024;
            goto next_cmnd;

        case DO:

do_ret1:

            if (xd[xpx]==' '||xd[xpx]==0) {
                unsigned char tmp[1024];
                strcpy(tmp,"+0 ");
                ExecLevel++;
                strcat(tmp,&xd[xpx]);
                strcpy(&xd[1],tmp);
                xpx=1;
                }

            pd1hold1=pd1len;
            pd1hold2=pd1cur;
            iorg=1;
            shold=0;
            retz=DO;
            goto lbl;

do_ret:

            if (++gsbx>29) goto stack_overflow;
            if (shold>0) gosub[gsbx][0]=shold;
            else gosub[gsbx][0]=0;
            gosub[gsbx][1]=pd1hold2;
            gosub[gsbx][2]=cmdx;
            strcpy(&cmdstack[cmdx],&xd[xpx]);
            cmdx=cmdx+strlen(&xd[xpx])+1;
            gosub[gsbx][3]=DO;
            gosub[gsbx][4]=pd1hold1;
            pd1cur=ibr;
            gosub[gsbx][5]=0;
            goto next_line;

        case KILL_VAR:

//#++++ Kill all local variables

kill_again:

            if (xd[xpx]==' '|| xd[xpx]==0 || xd[xpx]=='\t') {
                symflg=4;
                sym_("","");
                goto next_cmnd;
                }

//#++++ Kill indirect

            if (xd[xpx]=='@') {
                kflg=1;
                t0px=1;
                if ((ierr=parse_())) goto parse_err;
                kflg=0;
                }

//#++++ Kill specific */

            if (xd[xpx]!='(') {

                t0px=1;
                if ((ierr=parse_())) goto parse_err;

                if (v1d[1]!='^') {  /* kill a local */
                    symflg=2; /* kill */
                    sym_(&v1d[1],"");
                    }

                else { /* globals */
                    keyfix(&v1d[1]);
                    k=strlen(&v1d[1]); /* original lngth */
                    strcpy(tmp2,&v1d[1]);
                    global(GKILL,&v1d[1],bd); /* kill first */
k1:
                    j=global(XNEXT,&v1d[1],bd); /* kill others like it */
                    if (j&&strncmp(tmp2,&v1d[1],k)==0) {
                        global(GKILL,&v1d[1],bd);
                        goto k1;
                        }
                    }
                }

//#++++ Kill all locals except...

            else {

                struct nmes *p1,*p2;

                nstart=NULL;
                xpx++;

kill2:

                t0px=1;
                if ((ierr=parse_())) goto parse_err;
                p1=(struct nmes *) malloc(sizeof (struct nmes));
                if (p1==NULL) goto abrt;
                p1->name=(char *) malloc(strlen(&v1d[1])+1);
                if (p1->name==NULL) goto abrt;
                keyfix(&v1d[1]);  /* in case its an array */
                strcpy(p1->name,&v1d[1]);
                p1->next=nstart;
                nstart=p1;
                xpx++;

                if (xd[xpx]!=')' && xd[xpx]) {
                    xpx++;
                    goto kill2;
                    }

                symflg=5;
                sym_("","");
                p1=nstart;

                while (p1!=NULL) {
                    p2=p1->next;
                    free (p1->name);
                    free (p1);
                    p1=p2;
                    }

                nstart=NULL;
                }

            if (xd[++xpx]==',') {
                xpx++;
                goto kill_again;
                }

            goto next_cmnd;

        case FILE_CLOSE:

            p2px++;
            if (xd[p2px]<=90) xd[p2px]+=32; /*lower case*/
            if (xd[p2px]=='o') goto quit;

next_close_arg:

            t2=1;
            if ((ierr=parse_())) goto parse_err;
            i=atoi(&pd1[sdlim]);
            if (opnflg[i]==0) goto file_error;
            if (opnflg[i]==2) fprintf(opnfile[i],"\n");

            if (opnflg[i]==3) {
                tmp2[0]=27;
                tmp2[1]='h';
                tmp2[2]=0;
                write(popn[i][1],tmp2,2);
                }

            else fclose(opnfile[i]);
            opnflg[i]=0;
            if (xd[xpx]!=',') goto next_cmnd;
            xpx++;
            goto next_close_arg;

        case USE:

            t2=1;
            if ((ierr=parse_())) goto parse_err;
            io=atoi(&pd1[sdlim]);
            if (io<=0||io>5) goto arg_list_error;
            cf=opnfile[io];
            goto next_cmnd;

//#++++ remote block to proces lhs type references

lhsref:

            if (vd[1]=='^') {
                strcpy(v1d,&vd[1]);
                strcpy(bd,&pd1[sdlim]);
                global(STORE,v1d,bd);
                }

            else {
                if (vd[1]=='$'&&(vd[2]=='p'||vd[2]=='P')) {
                    strcpy(tmp3,&pd1[sdlim]);
                    strcpy(&v1d[1],&vd[1]);
                    setpiece=1;
                    fcn();
                    setpiece=0;
                    for (i=1; bd[i]!=0&&bd[i]!=1; i++);
                    if (bd[i]==0) goto ref_err;
                    sdlim=pd1len+2;
                    l=1;
                    j=sdlim;
                    while (l<i) pd1[j++]=bd[l++];
                    pd1[j]=0;
                    strcat(&pd1[sdlim],tmp3);
                    strcat(&pd1[sdlim],&bd[i+1]);
                    strcpy(&vd[1],setname);
                    goto lhsref;
                    }

                else {
                    symflg=0; /* store */
                    sym_(&vd[1],&pd1[sdlim]);
                    if (symlen<=pd1len) goto program_size;
                    if (symflg==0) goto symtab_err;
                    }
                }

            if (retz==READ) goto cont_read;
            else goto set_return;

//#++++ remote block to process DO and GOTO arguments.

lbl:

            if (xd[xpx]==0) goto lbl_error;

            if (xd[xpx]=='@') {
                t0px=1;
                t2=2;
                if ((ierr=parse_())) goto lbl_error;
                if (t0px==0) strcpy(dosave,&v1d[1]);
                else strcpy(dosave,&pd1[sdlim]);
                if (t0px==0) xpx++;
                }

            else {
                i=0;

                while (xd[xpx]) {
                    if (xd[xpx]==','||xd[xpx]==' '||xd[xpx]==0||
                            xd[xpx]=='+'||xd[xpx]=='^'||xd[xpx]==':')
                        break;
                    dosave[i++]=xd[xpx++];
                    }

                dosave[i]=0;
                }

            k=-1;

            if (xd[xpx]=='+') {
                t2=2;
                if ((ierr=parse_())) goto parse_err;
                cannon(&pd1[sdlim]);
                k=atoi(&pd1[sdlim]);
                if (k<0) goto lbl_error;
                }

            v1d[1]=0;
            if (xd[xpx]!=' '&&xd[xpx]!=','&&xd[xpx]&&xd[xpx]!=':') {
                t0px=1;
                t2=1;

                if (xd[xpx]=='^'&&xd[xpx+1]=='@') {
                    xpx++;
                    fileflg=1;
                    }

                else fileflg=0;

                if ((ierr=parse_())) goto file_error;

                if (fileflg) {
                    strcpy(ftmp,&v1d[1]);
                    v1d[1]='^';
                    strcpy(&v1d[2],ftmp);
                    }

                if (xd[xpx]!=':') xpx++;
                }

            if (xd[xpx]==':') {
                xpx++;
                t2=1;
                t0px=0;
                strcpy(ftmp,&v1d[1]);
                if ((ierr=parse_())) goto post_cond_error;
                strcpy(&v1d[1],ftmp);

                if (atoi(&pd1[sdlim])==0) {
                    if (xd[xpx]==',') {
                        xpx++;
                        goto lbl;
                        }

                    else goto next_cmnd;

                    }
                }

            if (dosave[0]=='^') {
                strcpy(&v1d[1],dosave);
                dosave[0]=0;
                }

            if (v1d[1]) {
                if (retz==DO) {
                    iorg=1;
                    shold=pd1len;
                    swp[swapextent]=(char *) malloc(shold);
                    if (swp[swapextent]==NULL) goto stack_overflow;
                    memcpy(swp[swapextent],pd1p,(size_t)shold);
                    shold=swapextent;
                    swapextent++;
                    if (swapextent>=SWAP) goto stack_overflow;
                    }

                readf_(&v1d[2],iorg,opnfile); /* load file */
                lblx=0;
                if (v1d[2]==0) goto file_error;
                }

            for(ibr=0; ibr<=lblx; ibr++)
                if (strcmp(&lbls[ibr][0],dosave)==0) {
                    ibr=lbloff[ibr];
                    goto offset;
                    }
            ibr=1;
            if (dosave[0]) {
                while(ibr<pd1len) {
                    if (pd1[ibr]==9) while(pd1[ibr++]!=0&&ibr<pd1len);
                    else {
                        j=0;

                        while(pd1[ibr]==dosave[j]) {
                            j++;
                            ibr++;
                            }

                        if (pd1[ibr]==9&&dosave[j]==0) {
                            if (lblx<19&&j<10) {
                                lblx++;
                                strcpy(&lbls[lblx][0],dosave);
                                lbloff[lblx]=ibr;
                                }
                            goto offset;
                            }
                        while(pd1[ibr++]!=0&&ibr<pd1len);

                        }
                    }
                goto lbl_error;
                }

offset:

            if (k>0) {

                for (i=0; i<k; i++) {
                    while(pd1[ibr++]!=0&&ibr<pd1len);
                    if (ibr>=pd1len) goto lbl_error;
                    }

                while(pd1[ibr]!=9) ibr++;
                if (ibr>=pd1len) goto lbl_error;

                }

            else if (k==0) ibr=pd1cur;

            if (xd[xpx]&&xd[xpx]!=' ') xpx++;

            if (retz==GOTO) goto goto_return;
            else goto do_ret;

err_call:
            ernbr=0;
            printf("\n*** %d Control-C\n",ernbr);
            goto abrt1;

parse_err:
            ernbr=ierr;
            prnterr(ernbr);
            printf("\n*** %d Expression error\n",ernbr);
            goto abrt1;

post_cond_error:
            ernbr=6;
            prnterr(ernbr);
            goto abrt1;

lbl_error:
            ernbr=8;
            prnterr(ernbr);
            goto abrt1;

        default:
            ernbr=13;
            prnterr(ernbr);
            goto abrt1;

arg_list_error:
            ernbr=14;
            prnterr(ernbr);
            goto abrt1;

ref_err:
            ernbr=18;
            prnterr(ernbr);
            goto abrt1;

symtab_err:
            ernbr=23;
            prnterr(ernbr);
            goto abrt1;

file_error:
            ernbr=26;
            prnterr(ernbr);
            goto abrt1;

io_error:
            ernbr=20;
            prnterr(ernbr);
            goto abrt1;

program_size:
            ernbr=31;
            prnterr(ernbr);
            goto abrt1;

stack_overflow:
            ernbr=32;
            prnterr(ernbr);

abrt1:
            printf("*** %s\n",&pd1[pd1Last]);

            if (cgi_flag) goto abrt;
            goto restore;

        } /* command switch */

    } /* function main() */


void prnterr(int i) {

    printf("\n*** ");

    switch(i) {

        case 1:
            printf("Multiple adjacent operators");
            break;
        case 2:
            printf("Unmatched quotes");
            break;
        case 3:
            printf("Global not found");
            break;
        case 4:
            printf("Missing comma");
            break;
        case 5:
            printf("Argument not permitted");
            break;
        case 6:
            printf("Bad character after post-conditional");
            break;
        case 7:
            printf("Invalid quote");
            break;
        case 8:
            printf("Label not found");
            break;
        case 9:
            printf("Too many/few fcn arguments");
            break;
        case 10:
            printf("Invalid number");
            break;
        case 11:
            printf("Missing operator");
            break;
        case 12:
            printf("Unrecognized operator");
            break;
        case 13:
            printf("Keyword");
            break;
        case 14:
            printf("Argument list");
            break;
        case 15:
            printf("Divide by zero");
            break;
        case 16:
            printf("Invalid expression");
            break;
        case 17:
            printf("Variable not found");
            break;
        case 18:
            printf("Invalid reference");
            break;
        case 19:
            printf("Logical table space overflow");
            break;
        case 20:
            printf("I/O error");
            break;
        case 23:
            printf("Symbol table full");
            break;
        case 24:
            printf("Function argument error");
            break;
        case 25:
            printf("Global not permitted");
            break;
        case 26:
            printf("File error");
            break;
        case 27:
            printf("$N error");
            break;
        case 29:
            printf("<break> at line:");
            break;
        case 30:
            printf("Function not found");
            break;
        case 31:
            printf("Program space exceeded");
            break;
        case 32:
            printf("Stack overflow");
            break;
        default:
            printf("Unknown error");
        }
    return;
    }

/*===========================================================================*
 *                                  sigint                                   *
 *===========================================================================*/

void sigint(int i) {

#ifndef SIGKILL
#define SIGKILL 101
#endif
#ifndef SIGHUP
#define SIGHUP 102
#endif
#ifndef SIGINT
#define SIGINT 103
#endif
#ifndef SIGQUIT
#define SIGQUIT 104
#endif
#ifndef SIGILL
#define SIGILL 105
#endif
#ifndef SIGTRAP
#define SIGTRAP 106
#endif
//#ifndef SIGABRT
//#define SIGABRT 107
//#endif
#ifndef SIGBUS
#define SIGBUS 108
#endif
#ifndef SIGFPE
#define SIGFPE 109
#endif
#ifndef SIGKILL
#define SIGKILL 110
#endif
#ifndef SIGUSR1
#define SIGUSR1 111
#endif
#ifndef SIGSEGV
#define SIGSEGV 112
#endif
#ifndef SIGUSR2
#define SIGUSR2 113
#endif
#ifndef SIGPIPE
#define SIGPIPE 114
#endif
#ifndef SIGALRM
#define SIGALRM 115
#endif
#ifndef SIGTERM
#define SIGTERM 116
#endif
#ifndef SIGSTKFLT
#define SIGSTKFLT 117
#endif
#ifndef SIGCHLD
#define SIGCHLD 118
#endif
#ifndef SIGIOT
#define SIGIOT 119
#endif
#ifndef SIGCONT
#define SIGCONT 120
#endif
#ifndef SIGSTOP
#define SIGSTOP 121
#endif
#ifndef SIGTSTP
#define SIGTSTP 122
#endif
#ifndef SIGTTIN
#define SIGTTIN 123
#endif
#ifndef SIGTTOU
#define SIGTTOU 124
#endif
#ifndef SIGURG
#define SIGURG 125
#endif
#ifndef SIGXCPU
#define SIGXCPU 126
#endif
#ifndef SIGXFSZ
#define SIGXFSZ 127
#endif
#ifndef SIGVTALRM
#define SIGVTALRM 128
#endif
#ifndef SIGPROF
#define SIGPROF 129
#endif
#ifndef SIGWINCH
#define SIGWINCH 130
#endif
#ifndef SIGIO
#define SIGIO 131
#endif
#ifndef SIGPWR
#define SIGPWR 132
#endif
#ifndef SIGSYS
#define SIGSYS 133
#endif

    char tmp[2];
    char tmp1[2];
    struct MSV * svPtr;
    svPtr=AllocSV(); /* dummy vector */

    if (svPtr==NULL) {
        printf("Memory error exit\n");
        fflush(stdout);
        raise(SIGKILL); // cannot be trapped
        }

    tmp[0] = 0;
    tmp1[0] = 0;

    /* Close Globals */

    global (CLOSE, (unsigned char*) tmp, (unsigned char*) tmp1);

    printf("\nGoodbye!\n");
    printf("Global arrays closed.\n");

    printf("Termination message %d: ",i);

    switch (i) {
        case SIGHUP:
            printf("Hangup (POSIX)\n");
            break;

        case SIGINT:
            printf("Interrupt (ANSI)\n");
            break;

        case SIGQUIT:
            printf("Quit (POSIX)\n");
            break;

        case SIGILL:
            printf("Illegal instruction (ANSI)\n");
            break;

        case SIGTRAP:
            printf("Trace trap (POSIX)\n");
            break;

        case SIGABRT:
            printf("Abort (ANSI)\n");
            break;

        case SIGBUS:
            printf("BUS error (4.2 BSD)\n");
            break;

        case SIGFPE:
            printf("Floating-point exception (ANSI)\n");
            break;

        case SIGKILL:
            printf("Kill, unblockable (POSIX)\n");
            break;

        case SIGUSR1:
            printf("User-defined signal 1 (POSIX)\n");
            break;

        case SIGSEGV:
            printf("Segmentation violation (ANSI)\n");
            break;

        case SIGUSR2:
            printf("User-defined signal 2 (POSIX)\n");
            break;

        case SIGPIPE:
            printf("Broken pipe (POSIX)\n");
            break;

        case SIGALRM:
            printf("Alarm clock (POSIX)\n");
            break;

        case SIGTERM:
            printf("Termination (ANSI)\n");
            break;

        case SIGSTKFLT:
            printf("Stack fault\n");
            break;

        case SIGCHLD:
            printf("Child status has changed (POSIX)\n");
            break;

        case SIGCONT:
            printf("Continue (POSIX)\n");
            break;

        case SIGSTOP:
            printf("Stop, unblockable (POSIX)\n");
            break;

        case SIGTSTP:
            printf("Keyboard stop (POSIX)\n");
            break;

        case SIGTTIN:
            printf("Background read from tty (POSIX)\n");
            break;

        case SIGTTOU:
            printf("Background write to tty (POSIX)\n");
            break;

        case SIGURG:
            printf("Urgent condition on socket (4.2 BSD)\n");
            break;

        case SIGXCPU:
            printf("CPU limit exceeded (4.2 BSD)\n");
            break;

        case SIGXFSZ:
            printf("File size limit exceeded (4.2 BSD)\n");
            break;

        case SIGVTALRM:
            printf("Virtual alarm clock (4.2 BSD)\n");
            break;

        case SIGPROF:
            printf("Profiling alarm clock (4.2 BSD)\n");
            break;

        case SIGWINCH:
            printf("Window size change (4.3 BSD, Sun)\n");
            break;

        case SIGIO:
            printf("I/O now possible (4.2 BSD)\n");
            break;

        case SIGPWR:
            printf("Power failure restart (System V)\n");
            break;

        case SIGSYS:
            printf("Bad system call\n");
            break;

        case 100:
            printf("Software Error\n");
            break;

        default:
            printf("default signal\n");
        }

    printf("\n");
    exit(EXIT_FAILURE);
    }

/*===========================================================================*
 *                                  AllocSV                                   *
 *===========================================================================*/

struct MSV * AllocSV(void) {
    static struct MSV *p1=NULL;
    int i;
    char *dbp;

    if (p1 == NULL) {

        signal(SIGHUP,SIG_IGN);
        signal(SIGINT,sigint);
        signal(SIGQUIT,&sigint);
        signal(SIGILL,&sigint);
        signal(SIGTRAP,&sigint);
        signal(SIGABRT,sigint);
        signal(SIGIOT,&sigint);
        signal(SIGBUS,&sigint);
        signal(SIGFPE,&sigint);
        signal(SIGKILL,&sigint);
        signal(SIGUSR1,&sigint);
        signal(SIGSEGV,&sigint);
        signal(SIGUSR2,&sigint);
        signal(SIGPIPE,&sigint);
//      signal(SIGALRM,&sigint);
        signal(SIGTERM,&sigint);
        signal(SIGSTKFLT,&sigint);
        signal(SIGXCPU,&sigint);
        signal(SIGXFSZ,&sigint);
        signal(SIGPWR,&sigint);
        signal(SIGSYS,&sigint);

        }

    p1 = (struct MSV *) malloc(sizeof(struct MSV));

    if (p1==NULL) {
        printf("*** Unable to allocate MSV\n");
        return NULL;
        }

    p1->LockDone=0;
    p1->io=5;
    p1->ierr=0;
    memset (p1->bd,'\0', STR_MAX);
    memset (p1->pd1,'\0', PS1);
    memset (p1->v1d,'\0', STR_MAX);
    memset (p1->xd,'\0', STR_MAX);
    memset (p1->setname,'\0', 256);
    p1->symflg=0;
    p1->ITRAP=0;
    p1->setpiece=0;
    p1->t0px=0;
    p1->xpx=0;
    p1->nstart=NULL;
    p1->ctrlc=0;
    p1->iorg=1;
    p1->symlen=0;
    p1->NOERR=0;

    for (i=0; i<10; i++) {
        p1->in_file[i]=NULL; /* file ptrs */
        p1->out_file[i]=NULL; /* file ptrs */
        p1->hor[i]=0; /* horizontal cursors */
        p1->ver[i]=1; /* vertical cursors */
        }

    p1->in_file[5]=stdin;
    p1->out_file[5]=stdout;
    p1->LineNumber=0;
    p1->tpx=0;
    p1->_Sym=0;
    p1->pd1len=0;
    p1->t2=0;
    p1->nflg=0;
    p1->Rargsx=0;
    p1->ERROR=0;
    p1->gpad=0;
    strcpy( (char * ) p1->naked,"");

    for (i=0; i<SYM_MAX; i++) p1->start[i]=NULL;

    p1->order=0;
    p1->_SymPush=0;
    p1->hit=0;
    p1->count=0;
    p1->ScanMinWordSize=3;
    p1->ScanMaxWordSize=25;
    p1->fdes[0]=-1;
    p1->fdes[1]=-1;
    p1->exec=0;
    p1->reference[0]=0; // call by ref original name
    strcpy(p1->CurrentFile,"");

#if defined(SQLITE)

    strcpy(p1->Col_Names,"");
    strcpy(p1->pgsql_msg,"");
    strcpy(p1->Table,"mumps");
    p1->TabSize=10;
    // remote host defaults to 127.0.0.1
    strcpy(p1->Connection,"host=@remotehost@ dbname=mumps user=mumps password=  ");
    p1->ConOpen=0;
    p1->grsltsx=0;

    for (int i=0; i< 21; i++) p1->grslts[i]= (char *) malloc(64);

    p1->grslts[21]= (char *) malloc(STR_MAX);
    sprintf(p1->sqloutput,"%d.tmp",getpid());

#else

    if ((dbp=getenv("MumpsNativeDatabase")) !=NULL) {
        strcpy( (char * ) p1->cfgdata,dbp);
        strcat( (char * ) p1->cfgdata,".dat");
        strcpy( (char * ) p1->cfgkey,dbp);
        strcat( (char * ) p1->cfgkey,".key");
        strcpy(p1->UDAT,dbp);
        strcat(p1->UDAT,".dat");
        strcpy(p1->UKEY,dbp);
        strcat(p1->UKEY,".key");
        }

    else {
        strcpy( (char * ) p1->cfgdata,"data.dat");
        strcpy( (char * ) p1->cfgkey,"key.dat");
        strcpy(p1->UDAT,"data.dat");
        strcpy(p1->UKEY,"key.dat");
        }

#endif

    return p1;
    }

