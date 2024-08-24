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
//#+                                                              
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                                       +
//#+                      ####    #   #   ####    ##                       +
//#+                     #         # #   #       # #                       +
//#+                      ####      #     ####     #                       +
//#+                          #     #         #    #                       +
//#+                     #    #     #    #    #    #                       +
//#+                      ####      #     ####   #####                     +
//#+                                                                       +
//#+                                                                       +
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "memsize.h"
#include "externs.h"

#define GBLBUF 31
#define GBLROOT 30

#define LINEFEED '\012'
#define CR '\15'
#define CTRLZ '\032'
#define TAB '\011'

void cannon(unsigned char *);

errmod_(msgnbr,text,opnfile)

        FILE *opnfile[];
        short msgnbr;
        unsigned char text[];

{
        char msg[80];

        switch(msgnbr-1) {

case 0:         strcpy(msg,"multiple operators"); break;
case 1:         strcpy(msg,"unmatched quotes"); break;
case 2:         strcpy(msg,"global not found"); break;
case 3:         strcpy(msg,"expecting comma"); break;
case 4:         strcpy(msg,"argument not permitted"); break;
case 5:         strcpy(msg,"post-conditional error"); break;
case 6:         strcpy(msg,"invalid quote"); break;
case 7:         strcpy(msg,"label or file not found"); break;
case 8:         strcpy(msg,"too many/few fcn args"); break;
case 9:         strcpy(msg,"invalid nbr"); break;
case 10:        strcpy(msg,"missing operator"); break;
case 11:        strcpy(msg,"unrecognized operator"); break;
case 12:        strcpy(msg,"keyword"); break;
case 13:        strcpy(msg,"argument list"); break;
case 14:        strcpy(msg,"divide by zero"); break;
case 15:        strcpy(msg,"invalid expression"); break;
case 16:        strcpy(msg,"variable not found"); break;
case 17:        strcpy(msg,"invalid reference"); break;
case 18:        strcpy(msg,"logical space exceeded"); break;
case 19:        strcpy(msg,"I/O Error"); break;
case 20:        strcpy(msg,"21"); break;
case 21:        strcpy(msg,"22"); break;
case 22:        strcpy(msg,"symbol table full"); break;
case 23:        strcpy(msg,"function argument error"); break;
case 24:        strcpy(msg,"global not permitted"); break;
case 25:        strcpy(msg,"file error"); break;
case 26:        strcpy(msg,"Next/$Order error"); break;
case 27:        strcpy(msg,"28"); break;
case 28:        strcpy(msg,"break> at line:"); break;
case 29:        strcpy(msg,"function not found"); break;
case 30:        strcpy(msg,"program space exceeded"); break;
case 31:        strcpy(msg,"stack overflow"); break;
                }
        io=5;
        printf("\n>> %d %s",msgnbr,msg);
        printf("\n>> %s",text); return; }

xindex(source,ptrn,strt)

        unsigned char source[],ptrn[];
        short strt;

        {       short int i,j,k,l,istrt;


                for (i=0; ptrn[i]!=0; i++); --i;
                for (j=0; source[j]!=0; j++); --j;
                istrt=strt-1;
                if (strt<=1) istrt=0;
                for (k=istrt; k<=j; k++)
                        { for (l=0; l<=i; l++)
                                { if (ptrn[l]!=source[k+l]) goto outer; }
                        return(k+1);
outer:                  ; }
                return(0); }


fcn()  {       

static unsigned char tmp1[2]={0,0};
long int atol(),timex,day,fd;
double t1;
char *ctime();
int rand();
unsigned char tmp2[1024];
unsigned char tmp3[1024];
int rslt;
static unsigned char cod209[2]={209,0};
//void cannon(unsigned char *);

static unsigned char opcode[256] = {
/*0*/   99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*10*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*20*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*30*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*40*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*50*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*60*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*70*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*80*/  99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
/*90*/  99, 99, 99, 99, 99, 99, 99,  2, 99,  3,
/*100*/ 99,  1,  4, 99, 14, 13,  5, 99,  6, 99,
/*110*/ 99, 99,  7, 99, 10,  8,  9, 99, 99, 99,
/*120*/ 11, 12, 99, 99, 99, 99, 99, 99 } ;

        short int iargs[10],nargs,m,n,k,l;
        long i,j;

        ierr=0; nargs=0; iargs[0]=1;
        j=v1d[2]; /* hold function id */
        while (v1d[1]!=206&&v1d[1]!=0) strcpy(&v1d[1],&v1d[2]);
        if (v1d[1]==206) strcpy(&v1d[1],&v1d[2]);
        for (i=1; v1d[i]!=0; i++)
                if (v1d[i]==207|| v1d[i]==208) {
                        v1d[i]=0;
                        iargs[++nargs]=i+1; 
                        }

        if (j<=90) j+=32; /*lower case conversion*/
        if ((i=opcode[j])==99) { ierr=30; return; }

        switch(i) {

case 1: /* $extract */

        if (nargs==1) { bd[1]=v1d[1]; bd[2]=0; goto ex2; }
        if (nargs!=2&&nargs!=3) goto err;
        sscanf(&v1d[iargs[1]],"%d",&i);
        if (nargs!=3) j=i;
        else sscanf(&v1d[iargs[2]],"%d",&j);
        if (i>=iargs[1]||j<i) {bd[1]=0; goto ex2;}
        if (j>iargs[1]) j=iargs[1];
        if (i<1) i=1;
        k=1; for (i=i; i<=j; i++) bd[k++]=v1d[i];
        bd[k]=0; goto ex2;

case 2: /* $ascii */

        if (nargs>2) goto err;
        if (nargs==2) sscanf(&v1d[iargs[1]],"%d",&i); 
		else i=1;
        if (i>=iargs[1]||i<=0) {
		bd[1]='-'; 
		bd[2]='1'; 
		bd[3]=0; 
		return; 
		}
        rslt=v1d[i]; 
	if (rslt==0) 
	rslt= -1; 
	goto ex1;

case 3: /* $char        */

        bd[1]=0; j=1;
        for (i=1; i<=nargs; i++)
                { strcpy(tmp2,&v1d[j]); cannon((unsigned char *)tmp2); day=atol(tmp2); 
                if (day<=0) day=0;
                tmp1[0]=day; tmp1[1]=0; strcat(&bd[1],tmp1); j=iargs[i]; }
        return;

case 4: /* $find */

        if (nargs!=2&&nargs!=3) goto err;
        strcpy(tmp2,&v1d[1]);
        strcpy(tmp3,&v1d[iargs[1]]);
        if (nargs==2) i=1; else sscanf(&v1d[iargs[2]],"%d",&i);
        if (tmp3[0]!=0) { if ((i=xindex(tmp2,tmp3,i))>0) i+=strlen(tmp3); }
        if (i>strlen(tmp2)+1) i=0; rslt=i; goto ex1;

case 5: /* $justify */

        if (nargs==0) { /* $job */ 
		sprintf(&bd[1],"%X",getpid()); 
		goto ex2; 
		}

        if (nargs<2||nargs>3) goto err;
        l=atoi(&v1d[iargs[1]]);

        strcpy(&bd[1],&v1d[1]);
        if (nargs!=3) {
                k=strlen(&bd[1]);
                if (k>=l) goto ex2;
                if (l>255) l=255;
                lpad(&bd[1],&bd[1],l); goto ex2;
                }

        k=atoi(&v1d[iargs[2]]); /* arg 3 */
        sprintf(tmp2,"%c%d%c%df",'%',l,'.',k);
        t1=atof(&v1d[1]);
        sprintf(&bd[1],tmp2,t1);
        goto ex2;


case 6: /* $len */

        if (nargs==0) { bd[1]='0'; bd[2]=0; return; };
        strcpy(tmp2,&v1d[1]);
        if (nargs==2) {
                i=1; j=0;
                if ((k=strlen(&v1d[iargs[1]]))==0) { rslt=0; goto ex1; }
                while((i=xindex(tmp2,&v1d[iargs[1]],i))!=0) { j++; i+=k; }
                rslt=j+1; goto ex1; }
        if (nargs>1) goto err;
        rslt=strlen(tmp2);
ex1:    sprintf(&bd[1],"%d",rslt); return;

case 7: /* $piece */

        if (nargs==2) { k=1; l=1; goto p1; }
        if (nargs<2||nargs>4) goto err;
        if (nargs==2) { k=1; l=1; }
        else {
                strcpy(tmp2,&v1d[iargs[2]]); k=atoi(tmp2);
                l=k;
                if (nargs==4) {
                        strcpy(tmp2,&v1d[iargs[3]]);
                        l=atoi(tmp2); } }
        if (k<0) k=1;
p1:     strcpy(tmp3,&v1d[iargs[1]]); strcpy(tmp2,&v1d[1]);
        if (tmp2[0]==0||tmp3[0]==0||k>l||l<=0) { bd[1]=0; goto ex2; }
        m=0; n=0;
        while (n<k-1) { 
                m=xindex(tmp2,tmp3,m)+1; 
                if (m==1) { bd[1]=0; goto ex2; }
                n++; }
        if (k!=1) k=m+strlen(tmp3)-1; 
        while (n!=l) { 
                m=xindex(tmp2,tmp3,m)+1;
                if (m<=0) { m=strlen(tmp2)+1; goto piece1; }
                   else n++; 
		}
        m=m-k-1;
piece1: if (m==0&&setpiece==0) { bd[1]=0; goto ex2; }
        if (setpiece==1) {
                substr(tmp2,&bd[1],1,k-1);
                bd[k]=1; bd[k+1]=0;
                if (k+m-1<strlen(tmp2)) strcat(&bd[1],&tmp2[k+m-1]);
                goto ex2; }
        substr(tmp2,&bd[1],k,m);
        goto ex2;

case 8: /* $select */

        if (nargs==0) { /* $storage */

                rslt=symlen-pd1len; sprintf(&bd[1],"%d",rslt); goto ex2; }

        i=0;
sel1:   strcpy(tmp2,&v1d[iargs[i]]); j=xindex(tmp2,cod209,1)-1;
        if (j<=0) goto err;
        if (tmp2[j-1]=='0') { i++; if (i>=nargs) goto err; else goto sel1; }
        strcpy(&bd[1],&tmp2[j+1]); goto ex2;

case 9: /* $text */

        if (nargs==0) { /* $test */

                if (tpx==1) bd[1]='1'; 
		else bd[1]='0'; 
		bd[2]=0; 
		goto ex2; 
		}

        if (nargs!=1) goto err;

        i=atoi(&v1d[1]);
        if (i<=0) { 
		bd[1]=0; 
		goto ex2; 
		} 
        j=1; 
	k=1;
        while (k!=i) { 
                j=j+strlen(&pd1[j])+1;
                if (j>pd1len) { 
			bd[1]=0; 
			goto ex2; 
			} 
                k++; 
		}
        i=1;
        while(pd1[j]!=0) 
		if (pd1[j]!=TAB) bd[i++]=pd1[j++];
                	else { 
				bd[i++]='\t'; 
				j++; 
				}
        bd[i]=0; 
	goto ex2;

case 10: /* random */

	if (nargs!=1) goto err;
        j=atoi(&v1d[1]); 
        if (j<2) { bd[1]='0'; bd[2]=0; return; }
        rslt=rand(); 
	rslt=rslt/(RAND_MAX/(j));
        sprintf(&bd[1],"%d",rslt); 
	return;

case 11:        /* $x */

        sprintf(&bd[1],"%d",hor[io]); goto ex2;

case 12:        /* $y */

        sprintf(&bd[1],"%d",ver[io]); goto ex2;

case 13:        /* $io */

        sprintf(&bd[1],"%d",io); goto ex2;

case 14:        /* $horolog */

        timex=time(&timex); 
	day=timex/86400;
        timex=timex-(day*86400); day=47118+day; fd=day;
        sprintf(&bd[1],"%ld",fd); strcat(&bd[1],",");
        fd=timex; sprintf(tmp2,"%ld",fd);
        strcat(&bd[1],tmp2); return;

} // switch

ex2:    ierr=0; return;

err:    ierr=9; return;
                }

lpad(strin,strout,l)

        unsigned char strin[],strout[];
        short l;

        {       short int j,i,k;
                j=strlen(strin)-1;
                k=(l-j-1);
                if (k<=0) return;
                for (i=j; i>=0; i--) strout[i+k]=strin[i];
                strout[j+k+1]=0;
                for ( i=0; i<k; i++) strout[i]=' ';
                return; }

inout(popn,opnflg,opnfile,text,cmnd,col)

        FILE *opnfile[];
        unsigned char opnflg[];
        unsigned char text[];
        short cmnd,col;
        int popn[7][4];

{
        short int i,j,dev;

        dev=io;
        if (dev==5) opnfile[5]=stdout;
                switch (cmnd) {

case 1: /* new page     */

        fputc('\014',opnfile[dev]);
        hor[dev]=1; ver[dev]=0;
        return;

case 2: /* skip line    */

        fprintf(opnfile[dev],"\n"); 
	hor[dev]=0; 
	ver[dev]++;
        if (dev==5&&Tab!=0) {
                printf("\r");
                for (i=0; i<Tab; i++) printf(" ");
                }
        return;

case 3: /* tab */

        if (col==hor[dev]) return;
        if (col<1) return;
        if (dev==5) { for (i=hor[dev]; i<col-1; i++) printf(" "); }
        else { for (j=hor[dev]; j<=col-1; j++) fputc(' ',opnfile[dev]); }
        hor[dev]=col;
        return;

case 4: /* text print */

        fprintf(opnfile[dev],"%s",text); hor[dev]=hor[dev]+strlen(text);
        return;
        }
}

getstr1(popn,opnflg,opnfile,lun,area)

        FILE *opnfile[];
        unsigned char area[];
        unsigned char opnflg[];
        short int lun;
        int popn[7][4];

{

        short int i,chr;
        if (opnflg[lun]==3) {
                area[0]=27; area[1]='r'; area[2]=0;
                write(popn[lun][1],area,2); /* request data */
                i=read(popn[lun][2],area,1000); return(i); }
        if (lun==5) opnfile[lun]=stdin;
        for (i=0; i<1000; i++) { /* restrict input line length */
                chr=fgetc(opnfile[lun]);
                if (lun == 5 && chr == EOF) { 
			raise(SIGQUIT);
			area[0]=0; 
			return(-1); 
			}
                area[i]=chr;
                if (area[i]==LINEFEED) { 
			area[i]=0; 
			return(i); 
			}
                if (area[i]>127) area[i]=' ';
                if (area[i]=='\n') i--;
                else if (area[i]=='\r') i--;
                else if (area[i]=='\t') area[i]='\t'; 
		else if (area[i]<32) area[i]=' ';
                }
        area[1000]=0;  /* restricted length */
        return(-1);
}

substr(in,out,start,len)
        short start,len;
        unsigned char in[],out[];

        {       short i;

                if (len==0) { out[0]=0; return; }
                if (len<0) len=255; else if (len>255) len=255;
                start--; len--;
                for (i=0; i<=len; i++) out[i]=in[start++];
                out[len+1]=0; return; }

readf_(zd,pd1org,opnfile)
        short pd1org;
        FILE *opnfile[];
        unsigned char zd[];

        {
        short int i,j,k,l;
                FILE *infile;
                j=pd1org;
                infile=fopen(zd,"r");
                if (infile==NULL) { zd[0]=0; return; }
                l=j;
                for (i=j; i<symlen; i++) { 
                        k=getc(infile);
                        if (k>127) k=32;
                        if (k==CR||k==CTRLZ) i--;
                        else { if (k>0) pd1[i]=k; }
                        if (k==LINEFEED) {
                                pd1[i]=0;
                                if (pd1[l] == ' ' && pd1[l + 1] == 0 && l + 1 == i) i = l;
                                else if (pd1[l] == '\t' && pd1[l + 1] == ';') i=l;
                                else l=i+1;
                                continue;
                                }
                        if (k<1) { 
                                fclose(infile);
                                pd1[i]=0; pd1[i+1]=0; pd1len=i-1; return; } } }

match(pts,lx,ct,ad,rslt)
        short *rslt,pts[3][20],ct[],lx;
        unsigned char ad[];

{
        unsigned char tmp[2],tmp1[1024],tmp2[1024];
        short ptr,i,j;
        ptr=(-1);
        *rslt=0;
        tmp[1]=0;
        for (i=1; i<=lx; i++)
        {       if (ct[i]==0) continue;
                switch (bd[pts[1][i]])
        {

case 'C':
case 'c':       for(j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        if (ad[ptr]<32) continue;
                        if (ad[ptr]==127) continue;
                        return;
                                }
                break;

case 'N':
case 'n':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        if (ad[ptr]>='0'&&ad[ptr]<='9') continue;
                        return;
                        } /* for j=...*/
                break;

case 'P':
case 'p':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        tmp[0]=ad[ptr];
                        if
                        (xindex(" !\"#$%&''()*+,-./:;<=>?@[\\]^_`{|}~",tmp,1)>0)
                                 continue;
                        return;
                                } /* for j=...*/
                break;

case 'A':
case 'a':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        if (ad[ptr]>=65&&ad[ptr]<=90) continue;
                        if (ad[ptr]>=97&&ad[ptr]<=122) continue;
                        return;
                        } /* for j...*/
                break;

case 'L':
case 'l':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        if (ad[ptr]>=97&&ad[ptr]<=122) continue;
                        return;
                        } /*for j=...*/
                break;

case 'U':
case 'u':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        if (ad[ptr]>=65&&ad[ptr]<=90) continue;
                        return;
                        } /* for j=...*/
                break;

case 'E':
case 'e':       for (j=1; j<=ct[i]; j++)
                {       ptr++;
                        if (ptr>strlen(ad)) return;
                        if (ad[ptr]>127) return;
                        }
                break;

case '"':       for (j=1; j<=ct[i]; j++)
                {       
                        if (pts[2][i]==0) continue;
                        if (ptr+pts[2][i]-1>strlen(ad)) return;
                        ptr++;
                        substr(&ad[ptr],tmp1,1,pts[2][i]-1);
                        substr(&bd[1],tmp2,pts[1][i]+1,pts[2][i]-1);
                        if (strcmp(tmp2,tmp1)!=0) return;
                        ptr=ptr+strlen(tmp1)-1;
                        }
                break;

default:        return;         }
                                } /* for i=... */
        if (ptr+1!=strlen(ad)) return;
        *rslt=1;
        return;
                        } /* match() */

patrn(ad)
        unsigned char ad[];
{
        unsigned char indef,tmp[10];
        short iptr,k,j,i,m,pts[3][20],lx,count[20],ct2[20],stk[20],stx;
        short high[20],low[20],lad;

        indef=0; lx=0; lad=strlen(ad);
        for (i=0; i<20; i++)
                { high[i]=lad; low[i]=0; }
        for (i=1; i<=strlen(&bd[1]); i++) {

        lx++;
        if (bd[i]<'0'||bd[i]>'9') {
                if (bd[i]!='.') i--;
                count[lx]=(-1); indef=1; i++;
                if (bd[i]>='0'&&bd[i]<='9') {
                        j=i; while(bd[i]>='0'&&bd[i]<='9') i++;
                        substr(&bd[1],tmp,j,i-j);
                        cannon((unsigned char *) tmp);
                        high[lx]=atoi(tmp); }
                }

        else {  j=i;
                while(bd[i]>='0'&&bd[i]<='9') i++;
                substr(&bd[1],tmp,j,i-j);
                cannon((unsigned char *)tmp);
                count[lx]=atoi(tmp);
                if (bd[i]=='.') {
                        low[lx]=count[lx];
                        count[lx]= -1;
                        indef=1;
                        i++;
                        if (bd[i]>='0'&&bd[i]<='9') {
                                j=i;
                                while(bd[i]>='0'&&bd[i]<='9') i++;
                                substr(&bd[1],tmp,j,i-j);
                                cannon((unsigned char *)tmp);
                                high[lx]=atoi(tmp); }           
                        }
                }

        if (bd[i]!='"') {
                tmp[0]=bd[i]; tmp[1]=0;
                if (xindex("cnpalue",tmp,1)==0&&
                    xindex("CNPALUE",tmp,1)==0) goto err;
                pts[1][lx]=i; pts[2][lx]=1; continue; }
        j=i;
        if (bd[j+1]=='"') {
                j++; pts[1][lx]=i; pts[2][lx]=0; i=j; continue; }

a108:   if (bd[++j]==0) goto err;
        if (bd[j]!='"') { if (bd[j]==210) bd[j]='"'; goto a108; }
        pts[1][lx]=i; pts[2][lx]=j-i; i=j;
        }

	if (indef!=1) { match(pts,lx,count,ad,&j); ierr1=j; return; }
        
        stx=0;
        for ( i=1; i<=lx; i++) { if (count[i]<0) stk[++stx]=i; }
        for (i=1; i<=lx; i++)
                { if (count[i]>=0) ct2[i]=count[i]; else ct2[i]=low[i]; }
        iptr=stx; goto retry1;

retry:  ct2[stk[1]]++;
retry1: if (ct2[stk[1]]>lad||ct2[stk[1]]>high[stk[1]]) {
                ct2[stk[1]]=low[stk[1]]; m=2;
                if (stx==1) {ierr1=0; return; }
                ct2[stk[m]]++;
recount:        if (ct2[stk[m]]>lad||ct2[stk[m]]>high[stk[m]]) {
                        if (m>=stx) {ierr1=0; return; }
                        ct2[stk[m]]=low[stk[m]]; m++; ct2[stk[m]]++;
                        goto recount; } }
        k=0; for (j=1; j<=lx; j++) k=k+ct2[j];
        if (k>lad) goto retry;
        match(pts,lx,ct2,ad,&j);
        if ((ierr1=j)==1) return;
        goto retry;

err:    ierr1=99; return;
                        }
patrn1(jpx,ernbr)

        /* pattern argument [right side) set up */

        short *jpx,*ernbr;

        {
        static unsigned char tmp1[2],cod210[2]={210,0};
        tmp1[0]=0; tmp1[1]=0;

a1285:  if (xd[*jpx]=='"') {
                strcat(&bd[1],"\"");
a1283:          *jpx=(*jpx+1);
                if (xd[*jpx]==0) { *ernbr=2; return; }
                if (xd[*jpx]=='"') {
                        if (xd[*jpx+1]!='"') goto a1281;
                        strcat(&bd[1],cod210); *jpx=(*jpx+1); }
                else { tmp1[0]=xd[*jpx]; strcat(&bd[1],tmp1); }
                goto a1283; }

a1281:  tmp1[0]=xd[*jpx];
        if (xd[*jpx]!=0) {
                if (xindex(" ,&:!''><=[)+-*/#_",tmp1,1)==0) {
                        strcat(&bd[1],tmp1);
                        *jpx=(*jpx+1); goto a1285; } }
        xpx=(*jpx-1); *ernbr=0; return;
                        }

sigc()

        {
         ctrlc=1; return(1); }
itoa(val,str,rad)

        int val,rad;
        char *str;
{

        sprintf(str,"%d",val);
        return;
        }

strnset(string,chr,size)

        char string[];
        int chr,size;
{
        int i;
        for (i=0; i<size; i++) string[i]=chr;
        return;
        }
