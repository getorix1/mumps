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
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                                       +
//#+                ######  ######   ####   #    #                         +
//#+                    #   #       #    #  ##   #                         +
//#+                   #    #####   #       # #  #                         +
//#+                  #     #       #       #  # #                         +
//#+                 #      #       #    #  #   ##                         +
//#+                ######  #        ####   #    #                         +
//#+                                                                       +
//#+                                                                       +
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "memsize.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#define RETRIEVE 0
#define NEXT 2
#define INIT 6
#define OPEN 206
#define CLOSE 207
#define COMMA 208
#define COLON 210
#define STORE 1
#define XNEXT 8

int global(short g, unsigned char key[], unsigned char bd[]);

void zfcn() {       

#include "externs.h"

        long int atol(),timex,fd;
	  time_t dtime;
        char *ctime();
	  static char linebuf[1024];
	  static int linebufx;
        int getpid(),rand();
        FILE *dump;
        char zcode,zcode1,zcode2,zcode3;
        double sq;
        unsigned char fn[32],tmp2[1024],tmp3[1024],tmp4[1024];
	  unsigned char gbltbl[128],gblstr[1024];
        int j,k,x;
        short int iargs[10],nargs;
        long i;
        short g;
        double rt;
        
        ierr=nargs=0; 
        iargs[0]=1;
	  for (k=0,j=1; v1d[j]!=OPEN&&v1d[j]!=0; j++) fn[k++]=v1d[j];
	  fn[k]=0;
	  for (k=0; fn[k]!=0; k++) fn[k]=tolower(fn[k]);

        zcode=v1d[3]; 
        zcode1=tolower(v1d[4]);
        zcode2=tolower(v1d[5]);
        zcode3=tolower(v1d[6]);

	  strcpy(&v1d[1],&v1d[j]);
        // while (v1d[1]!=OPEN&&v1d[1]!=0) strcpy(&v1d[1],&v1d[2]);

        if (zcode=='g'&&(zcode1=='a'||zcode1=='c'||zcode1=='s')) { /* zglobal */
                strcpy(&v1d[1],&v1d[2]);
                for (i=1; v1d[i]!=0; i++);
                if (v1d[i-1]!=CLOSE||v1d[i-2]!=CLOSE) goto err;
                v1d[i-1]=0;
                }

        else {
                if (v1d[1]==OPEN) strcpy(&v1d[1],&v1d[2]);

                for (i=1; v1d[i]!=0; i++)  /* locate arguments */
                        if (v1d[i]==CLOSE||v1d[i]==COMMA) { 
                                v1d[i]=0; 
                                iargs[++nargs]=i+1; 
                                }
                }


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       Branch table - go to appropriate fcn handler                    +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

     ierr=0;
     if (zcode<=90) zcode=zcode+32; /*lower case conversion */

     if (zcode=='a') {
			if (zcode1=='b') goto zabs; 
			goto zalter;
			}

        if (zcode=='c') goto zcondense;

        if (zcode=='d') {
                        if (zcode1=='1') goto zdate; 
                        if (zcode1=='2') goto zdate; 
                        if (zcode1=='3') goto zdate; 
                        if (zcode1=='4') goto zdate; 
                        if (zcode1=='5') goto zdate; 
                        if (zcode1=='6') goto zdate; 
                        if (zcode1=='7') goto zdate; 
                        if (zcode1=='8') goto zdate; 
                        if (zcode1=='9') goto zdate; 
			goto zdate;
                        }
        if (zcode=='e') goto z_err;
        if (zcode=='n') goto znormal;
        if (zcode=='b') goto zblnks;
        if (zcode=='p') goto zpad;
        if (zcode=='h') goto zhtml;
        if (zcode=='f') goto zfile;
        if (zcode=='g') goto zglobal;
        if (zcode=='l') goto zlog;
        if (zcode=='m') goto znxtrow;
        if (zcode=='r') goto zroot;
        if (zcode=='w') goto zword;
	  if (strcmp(fn,"$ztell")==0) goto ztell;
	
        if (zcode=='v') {
                        if (zcode1=='1') goto zv1; 
                        if (zcode1=='3') goto zv3; 
			      }

        if (zcode=='s') {
                if (zcode1=='q'&&zcode2=='r') goto square;
                goto zsystem;
                }

        goto err;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $Zv3(str) - convert to dictionary ref
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zv3:

	if (nargs!=1) goto err;
	strcpy(&bd[1],"@^patient(a1,\"~Dictionary\",\"");
	strcat(&bd[1],&v1d[1]);
	strcat(&bd[1],"\")");
	return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $Zv1(str) - check if string valid variable format
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zv1:

	if (nargs!=1) goto err;
	bd[2]=0;
	if (!isalpha(v1d[1])) { bd[1]=0; return; }
	for (i=2; v1d[i]!=0; i++) {
		if (isalnum(v1d[i])) continue;
		if (v1d[i]=='%') continue;
		bd[1]='0';
		return;
		}
	bd[1]='1';
	return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+									
+	$ZALTER(string)							
+	$ZNORMAL(string) - remove punctuation and make upper case	
+									
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zalter:
znormal: 

      j=1;
	for(i=1; v1d[i]!=0; i++) {
            if (v1d[i]=='\'') { bd[j]=0; break; }
		if (v1d[i]>='A'&&v1d[i]<='Z') bd[j++]=v1d[i];
		else if (v1d[i]>='a'&&v1d[i]<='z') bd[j++]=v1d[i]-32;
		else if ((v1d[i]>='0'&&v1d[i]<='9')
			||v1d[i]=='/') bd[j++]=v1d[i];
		else if (v1d[i]=='-' ) bd[j++]='_';
		}
	bd[j]=0;
	if (isdigit(bd[1])) { bd[1]=0; return; }
	if (bd[1]==0) return;

#if STEM==1
	stem(&bd[1],strlen(&bd[1])); 
#endif

	return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+									
+	$Ztell								
+									
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

ztell:
	if (io<0||io>4) {
		bd[1]=0;
		return;
		}

	i=ftell(cf);
	sprintf(&bd[1],"%ld",i);
	return;


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+								
+	$ZWORD - word/line buffer  				
+								
+	$ZWI(string) - initialize line buffer			
+	$ZWN - extract next word from line buffer		
+								
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zword:	if (zcode1=='i') { /* initialize */
		strcpy(linebuf,&v1d[1]);
		linebufx=0;
		bd[1]=0;
		return;
		}

	if (zcode1=='n') {
		if (linebuf[linebufx]==0) { /* empty */
			bd[1]=0;
			return;
			}

		while(linebuf[linebufx]!=0&&linebuf[linebufx]==' ') linebufx++;
		if (linebuf[linebufx]==0) { /* end of buffer */
			bd[1]=0;
			return;
			}
		i=1;
		while(1) { 	/* extract next word */
			bd[i]=linebuf[linebufx];
			if (linebuf[linebufx]==0||linebuf[linebufx]==' ') {
					bd[i]=0;
					return;
					}
			i++;
			linebufx++;
			}
		}

	if (zcode1=='p') { /* parse */

		if (linebuf[linebufx]==0) { /* empty */
			bd[1]=0;
			return;
			}

		while(linebuf[linebufx]!=0&&
			  linebuf[linebufx]==' ') linebufx++;

		if (linebuf[linebufx]==0) { /* end of buffer */
			bd[1]=0;
			return;
			}

		if (ispunct(linebuf[linebufx])) {
			bd[1]=linebuf[linebufx];
			bd[2]=0;
			linebufx++;
			return;
			}

		i=1;
		while(1) { 	/* extract next word */
			bd[i]=linebuf[linebufx];
			if (linebuf[linebufx]==0||
                ispunct(linebuf[linebufx])||
                linebuf[linebufx]==' ') {
					bd[i]=0;
					return;
					}
			i++;
			linebufx++;
			}
		}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+									
+	$ZBLANKS(string) - remove leading and double blanks		
+									
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zblnks:	

      if (nargs!=1) goto err;
	k=1;
	for (i=1; v1d[i]!=0; i++) if (v1d[i]!=' ') goto zbl1;
	bd[1]=0;
	return;

zbl1: 

     for (; v1d[i]!=0; i++) {
		bd[k++]=v1d[i];
		if (bd[k-2]==' '&&bd[k-1]==' '&&k>2) k--; /* double blank */
		}

	bd[k]=0;
	return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+									
+	$ZHTML(string) - convert codes for html transmission
+									
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zhtml:	if (nargs!=1) goto err;
	k=1;
	for (i=1; v1d[i]!=0; i++) {
		if (v1d[i]==' ') v1d[i]='+';
		else if (!isalnum(v1d[i])) {
			char t1[10],t2[512];
			sprintf(t1,"%%%2X\0",v1d[i]);
			v1d[i]=0;
			strcpy(t2,&v1d[1]);
			strcat(t2,t1);
			strcat(t2,&v1d[i+1]);
			strcpy(&v1d[1],t2);
			i=i+2;
			}
		}
	strcpy(&bd[1],&v1d[1]);
	return;
	
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZERROR - establish error condition execution text              +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

z_err:  if (nargs!=1) goto err;

        bd[1]='1'; 
        bd[2]=0; 
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZCONDENSE - Dump Global Arrays to Disk                         +
+                                                                       +
+       $ZCD[(start)] - dump to file ``dump''                           +
+       $ZCL - load from file ``dump''                                  +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zcondense:

        if (nargs>1) goto err;

        if (zcode1!='d') goto zc1;

	dtime=time(NULL);
	sprintf(tmp2,"%lu",dtime);
	if (strlen(tmp2)>8) tmp2[8]=0;
	strcat(tmp2,".dmp");

        dump=fopen(tmp2,"w+");
        if (dump==NULL) { 
                printf("dump err1\n"); 
                goto err; 
                }

        if (nargs==1) {  /* argument prvides starting point */
                strcpy(tmp2,&v1d[1]);
                }
        else {          /* default starting point */
                tmp2[0]=' '; /* lowest printable */
                tmp2[1]=1;
                tmp2[2]=1;
                tmp2[3]=0;
                }

        while(1) {      /* dump global arrays */
                g=XNEXT;
                i=global(g,tmp2,tmp3);
                if (i==0) break;
                g=RETRIEVE;
                global(g,tmp2,tmp4);
                for(i=0; tmp2[i]!=0; i++) {
                        if (tmp2[i]<' ') tmp3[i]='~'; else tmp3[i]=tmp2[i];
                        }
                tmp3[i]=0;
                fputs(tmp3,dump); fputs("\n",dump); /* key */
                fputs(tmp4,dump); fputs("\n",dump); /* data */
                }
        fclose(dump);
        bd[1]='1'; 
        bd[2]=0; 
        return;

zc1:    if (zcode1!='l') goto err;      /* load global arrays */

        dump=fopen("dump","r");
        if (dump==NULL) { 
                printf("dump err2\n"); 
                goto err; 
                }

        while(1) {                      /* load */
                if (fgets(tmp2,255,dump)==NULL) break;
                fgets(tmp3,255,dump);
                for (i=0; tmp3[i]!=0; i++) if (tmp3[i]<32) tmp3[i]=0;
                for (i=0; tmp2[i]!=0; i++) if (tmp2[i]=='~') tmp2[i]=1;
                                           else if (tmp2[i]<32) tmp2[i]=0;
                g=STORE;
                global(g,tmp2,tmp3);
                }
        fclose(dump);
        bd[1]='1'; 
        bd[2]=0; 
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       
+       $ZM(Global) - 							
+                                                                       
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

znxtrow:

        if (nargs==1) {  /* argument prvides starting point */
                strcpy(gblstr,&v1d[1]);
                strcpy(gbltbl,&v1d[1]);
                }
	strcpy(tmp2,gblstr); 

	g=XNEXT;
	i=global(g,tmp2,tmp3);
	if (i==0) {bd[1]=0; return; }
	g=RETRIEVE;
	global(g,tmp2,tmp4);
	for(i=0; tmp2[i]!=0; i++) {
		if (tmp2[i]<' ') tmp3[i]='~'; else tmp3[i]=tmp2[i];
		}
	tmp3[i]=0;
	for(i=0; gbltbl[i]==tmp3[i]&&gbltbl[i]!=0&&tmp3[i]!='~'; i++) ;
	if (gbltbl[i]==0&&tmp3[i]=='~') {
		for (i=0; (gblstr[i]=tmp2[i])!=0; i++);
		strcpy(&bd[1],tmp3);
	        return;
		}
	bd[1]=0;
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZAbs(nbr) - absolute value fcn                                 +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zabs:   if (nargs!=1) goto err;
        itoa(abs(atoi(&v1d[1])),&bd[1],10);
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $Zpad(str,len) - pad a string with blanks
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zpad:	if (nargs!=2) goto err;
	i=atol(&v1d[iargs[1]]);
	j=strlen(&v1d[1]);
	strcpy(&bd[1],&v1d[1]);
	for (j++; j<=i; j++) bd[j]=' ';
	bd[j]=0;
	return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $Zsqr(nbr) - square value fcn                                   +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

square: if (nargs!=1) goto err;
        sq=atof(&v1d[1]);
        gcvt(sq*sq,7,&bd[1]);
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZLOG(nbr) - natural log                                        +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zlog:   if (nargs>1) goto zlog1;
        sq=atof(&v1d[1]);
        sq=log((double)sq);
        if (sprintf(&bd[1],"%f",sq)!=EOF) return;
        goto err;

zlog1:  if (nargs>2) goto err;
        sscanf(&v1d[iargs[1]],"%d",&i);
        strcpy(&bd[1],&v1d[1]);

zlog2:  if (strlen(&bd[1])>i) return;
        strcat(&bd[1]," ");
        goto zlog2;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZDATE - date and time of day (Wed Jan 01, 1992 14:36:00)       +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zdate:
        timex=time(&timex); 

	if (zcode1=='1') sprintf(&bd[1],"%ld",timex); /* internal code */

	else if (zcode1=='2') { /* internal -> printable */
	        timex=atol(&v1d[1]);
		sprintf(&bd[1],"%s",ctime(&timex));
		}
	else if (zcode1=='3') {  /* $zd3(year,month,day) -> DayOfYear */

		struct tm date;
		int year,d,m;

		if (nargs!=3) goto err;

		year=atol(&v1d[1]);
		m=atol(&v1d[iargs[1]]);
		d=atol(&v1d[iargs[2]]);

		if (year>1900) date.tm_year  = year-1900;
		else date.tm_year  = year;
		date.tm_mon   = m-1; /* range is 0 to 11 */
		date.tm_mday  = d;
		date.tm_hour  = 0;
		date.tm_min   = 0;
		date.tm_sec   = 0;
		date.tm_yday  = 0;
		date.tm_isdst = 1;

		mktime( &date );

		sprintf(&bd[1],"%d",date.tm_yday+1); /* range is 0-365 */
		}

	else if (zcode1=='4') {  /* $zd4(year,dayofyear) -> gregorian */

		struct tm date;
		int year,d,m,dd;

		if (nargs!=2) goto err;

		year=atol(&v1d[1]);
		if (year>1900) year=year-1900;
		d=atol(&v1d[iargs[1]]);
		d--;
		m=0;
		dd=1;
		while (1) {
			date.tm_year  = year; 
			date.tm_mon   = m;
			date.tm_mday  = dd; 
			date.tm_hour  = 0;
			date.tm_min   = 0; 
			date.tm_sec   = 0;
			date.tm_yday  = 0; 
			date.tm_isdst = 1;
			mktime( &date );
			if (date.tm_yday==d) break;
			dd++;
			if (m==0 && dd>31) { m++; dd=1; } /* jan */
			else if (m==1 && (year%4)==0) {
				if (dd>29) { m++; dd=1; } /* feb */
				}
			else if (m==1 && (year%4)!=0) {
				if (dd>28) { m++; dd=1; } /* feb */
				}
			else if (m==2 && dd>31) { m++; dd=1; } /* mar */
			else if (m==3 && dd>30) { m++; dd=1; } /* apr */
			else if (m==4 && dd>31) { m++; dd=1; } /* may */
			else if (m==5 && dd>30) { m++; dd=1; } /* jun */
			else if (m==6 && dd>31) { m++; dd=1; } /* jul */
			else if (m==7 && dd>31) { m++; dd=1; } /* aug */
			else if (m==8 && dd>30) { m++; dd=1; } /* sep */
			else if (m==9 && dd>31) { m++; dd=1; } /* oct */
			else if (m==10 && dd>30) { m++; dd=1; } /* nov */
			else if (m==11 && dd>31) goto err; /* dec */
			}

		sprintf(&bd[1],"%d %d %d",
			date.tm_year,
			date.tm_mon+1,
			date.tm_mday);
		}

	else if (zcode1=='5') {  /* $zd5(year,mn,dy) -> yr,dy,dw (days since sunday) */

		struct tm date;
		int year,d,m;

		if (nargs!=3) goto err;

		year=atol(&v1d[1]);
		m=atol(&v1d[iargs[1]]);
		d=atol(&v1d[iargs[2]]);

		if (year>1900) date.tm_year  = year-1900;
		else date.tm_year  = year;
		date.tm_mon   = m-1; /* range is 0 to 11 */
		date.tm_mday  = d;
		date.tm_hour  = 0;
		date.tm_min   = 0;
		date.tm_sec   = 0;
		date.tm_yday  = 0;
		date.tm_isdst = 1;
		date.tm_wday = 0;

		mktime( &date );

		sprintf(&bd[1],"%d,%d,%d",
			date.tm_year,date.tm_yday,date.tm_wday);
		}

	else if (zcode1=='6') {  /* $zd6 -> HH:SS */
		strcpy(&bd[1],ctime(&timex)+11); 
		bd[6]=0;
	}

	else if (zcode1=='7' || zcode1=='8') { /* current as year,month,day */
	        if(nargs==1) timex=atol(&v1d[1]);
		strcpy(tmp2,ctime(&timex));
		bd[1]=tmp2[20];
		bd[2]=tmp2[21];
		bd[3]=tmp2[22];
		bd[4]=tmp2[23];
		bd[5]='-';
		tmp2[7]=0;
		if (strcmp(&tmp2[4],"Jan")==0) { bd[6]='0'; bd[7]='1';}
		else if (strcmp(&tmp2[4],"Feb")==0) { bd[6]='0'; bd[7]='2';}
		else if (strcmp(&tmp2[4],"Mar")==0) { bd[6]='0'; bd[7]='3';}
		else if (strcmp(&tmp2[4],"Apr")==0) { bd[6]='0'; bd[7]='4';}
		else if (strcmp(&tmp2[4],"May")==0) { bd[6]='0'; bd[7]='5';}
		else if (strcmp(&tmp2[4],"Jun")==0) { bd[6]='0'; bd[7]='6';}
		else if (strcmp(&tmp2[4],"Jul")==0) { bd[6]='0'; bd[7]='7';}
		else if (strcmp(&tmp2[4],"Aug")==0) { bd[6]='0'; bd[7]='8';}
		else if (strcmp(&tmp2[4],"Sep")==0) { bd[6]='0'; bd[7]='9';}
		else if (strcmp(&tmp2[4],"Oct")==0) { bd[6]='1'; bd[7]='0';}
		else if (strcmp(&tmp2[4],"Nov")==0) { bd[6]='1'; bd[7]='1';}
		else if (strcmp(&tmp2[4],"Dec")==0) { bd[6]='1'; bd[7]='2';}
		bd[8]='-';
		bd[9]=tmp2[8];
		bd[10]=tmp2[9];
		bd[11]=0;
		for (j=0; j<11; j++) if (bd[j]==' ') bd[j]='0';
		if (zcode1=='8') {
			bd[11]=',';
			strcpy(&bd[12],ctime(&timex)+11); 
			bd[17]=0;
			}
		}

	else if (zcode1=='9') { /* current as year,month,day,day of week */
	        if(nargs==1) timex=atol(&v1d[1]);
		strcpy(&bd[1],ctime(&timex));
		}
        else { 
		if (nargs!=0) goto err;
		strcpy(&bd[1],ctime(&timex)); 
        	bd[25]=0; 
		}
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZFILE(filename) - does file exist?                             +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


zfile:  fd=open(&v1d[1],0); 
        x=fd; 
        close(fd);
        if (x==-1) x=0; 
        else x=1; 
        goto a1821;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZGLOBAL - file system status                                   +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zglobal:

        g=5;    /* count empties */
        global(g,v1d,bd);  
        return;

a1821:  sprintf(&bd[1],"%d",x); 
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZROOT(nbr) - square root                                       +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zroot:  if (nargs!=1) goto err;
        sscanf(&v1d[1],"%lf",&rt);
        rt=sqrt(rt);
        sprintf(&bd[1],"%f",rt);
        return;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       $ZSYSTEM(command) - spawn shell and exec command                +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zsystem:

        if (nargs!=1) goto err;
        x=system(&v1d[1]);
        goto a1821;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+                                                                       +
+       Error exit                                                      +
+                                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

err:    ierr=9; return;
        }

#if STEM==0
int stem(char * word,int wl) { return 0; }
#endif

#if STEM==1
#include "stem.h"

/*========================================================================

	STEM

========================================================================*/

int stem(char *word,int wl)


/*********************************************************************
	if the ending of word[] is in endings.ending, it is removed and any
	replacement string is tacked on the end; search and replacement
	is controlled by endings.next.
**********************************************************************/

{
int i;

/*
i=0;
while (prefix[i].prefix[0]) {
	if (strncmp(word,prefix[i].prefix,prefix[i].offset)==0) {
		strcpy(tmp,prefix[i].replace);
		strcat(tmp,&word[prefix[i].offset]);
		strcpy(word,tmp);
		wl = strlen(word);
		break;
		}
	else i++;
	}
*/

/* check against possible endings */

i=wl-1;

switch (word[i]) {

case 'D': return escan(word,wl,D_endings);

case 'S': return escan(word,wl,S_endings);

case 'G': return escan(word,wl,G_endings);

case 'N': return escan(word,wl,N_endings);

case 'E': return escan(word,wl,E_endings);

case 'L': return escan(word,wl,L_endings);

case 'Y': return escan(word,wl,Y_endings);

case 'V': return escan(word,wl,V_endings);

case 'T': return escan(word,wl,T_endings);

case 'R': return escan(word,wl,R_endings);

default:  return wl;

}

}


int escan(char word[], int wl, struct Ends endings[])

{

int i=0;

while (endings[i].ending[0]) {

	if (strcmp(&word[wl-endings[i].offset],endings[i].ending)==0) {

		strcpy(&word[wl-endings[i].offset],endings[i].replace);
		wl += endings[i].replen-endings[i].offset;
		if (endings[i].next) break;
		i=0;
		}

	else i++;
	}

if (endings[i].next<0) return (wl);
if (word[wl-2]!='P' && word[wl-2]!='T' && word[wl-2]!='D') return (wl);

if (strcmp(&word[wl-2],"PP")==0) { word[wl-1]=0; return wl-1; }
if (strcmp(&word[wl-2],"TT")==0) { word[wl-1]=0; return wl-1; }
if (strcmp(&word[wl-2],"DD")==0) { word[wl-1]=0; return wl-1; }

return (wl);

}
#endif

