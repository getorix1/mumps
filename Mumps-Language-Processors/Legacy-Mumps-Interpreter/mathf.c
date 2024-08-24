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

//#*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+								
//#+		 #    #    ##     #####  #    #  ######		
//#+		 ##  ##   #  #      #    #    #  #		
//#+		 # ## #  #    #     #    ######  #####		
//#+		 #    #  ######     #    #    #  #		
//#+		 #    #  #    #     #    #    #  #		
//#+		 #    #  #    #     #    #    #  #		
//#+								
//#+								
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*

#include "memsize.h"

#include <stdio.h>
#include <string.h>

/******
******* floating point math package
******/

add(a,b,c)

	char *a,*b,*c;

	{
	static double aa,bb,cc;
	int i;
	double atof();

	for (i=0; a[i]!=0; i++) if (a[i]=='.') goto flt;
	for (i=0; b[i]!=0; i++) if (b[i]=='.') goto flt;

	sprintf(c,"%ld",atol(a)+atol(b));
	return;

flt:	aa=atof(a);
	bb=atof(b);
	cc=aa+bb;
	gcvt(cc,7,c);
	return;
	}

modulo(a,b)

	char *a,*b;

	{
	static double aa,bb,cc;
	int i;
	double atof();

	for (i=0; a[i]!=0; i++) if (a[i]=='.') goto flt;
	for (i=0; b[i]!=0; i++) if (b[i]=='.') goto flt;

	sprintf(b,"%ld",atol(a)%atol(b));
	return;

flt:	aa=atof(a);
	bb=atof(b);
	cc=fmod(aa,bb);
	gcvt(cc,7,b);
	return;
	}

sub(a,b,c)

	char a[],b[],c[];

	{
	double aa,bb,cc;
	int i;
	double atof();

	for (i=0; a[i]!=0; i++) if (a[i]=='.') goto flt;
	for (i=0; b[i]!=0; i++) if (b[i]=='.') goto flt;

	sprintf(c,"%ld",atol(a)-atol(b));
	return;

flt:	aa=atof(a);
	bb=atof(b);
	cc=aa-bb;
	gcvt(cc,7,c);
	return;
	}

mult(a,b,c)

	char a[],b[],c[];

	{
	double aa,bb,cc;
	double atof();

	aa=atof(a);
	bb=atof(b);
	cc=aa*bb;
	gcvt(cc,7,c);
	return;
	}

divx(a,b,c)

	char a[],b[],c[];

	{
	static double aa,bb,cc;
	double atof();

	aa=atof(a);
	bb=atof(b);
	cc=aa/bb;
	gcvt(cc,7,c);
	return;
	}

numcomp(aa,bb)

	char aa[],bb[];

	{ double a,b;
	long i,j;
	double atof();

	for (i=0; aa[i]!=0; i++) if (aa[i]=='.') goto flt;
	for (i=0; bb[i]!=0; i++) if (bb[i]=='.') goto flt;

	i=atol(aa); j=atol(bb);
	if (i<j) return(-1);
	if (i>j) return(1);
	return(0);

flt:	a=atof(aa);
	b=atof(bb);
	if (a<b) return(-1);
	if (a>b) return(1);
	return(0);
	}

void cannon(char a[])
{	int i,j,k;
	char tmp[25];

	i=0; j=0;
	while(a[i]=='+'||a[i]=='-') {
		if (a[i]=='-') if (j==0) j=1; else j=0; i++; }
	if (i>0) {
		if (j==0) strcpy(a,&a[i]);
			else { a[0]='-';
				if (i>1) strcpy(&a[1],&a[i]); } }
	j= -1;
	while(a[i]=='.'||(a[i]>='0'&&a[i]<='9')) {
		if (a[i]=='.') j=i; i++; }
	if (i==0) { a[0]='0'; a[1]=0; return; }
	if (a[i]!='E'&&a[i]!='e') a[i]=0;
	if (a[i]==0) {
		if (j>=0) {
				for (i--; a[i]=='0'; i--) a[i]=0;
				if (a[i]=='.') a[i]=0;
				if (i==0) { a[0]='0'; a[1]=0; } }
		if (a[0]=='-') j=1; else j=0;
		while (a[j]=='0') strcpy(&a[j],&a[j+1]);
		if (a[j]==0) { a[0]='0'; a[1]=0; }
		if (a[j]=='.'&&a[j+1]==0) a[j]='0';
		return; }
	if (a[i]!='e'&&a[i]!='E') return;
	a[i]=0; i++;
	if (a[i]=='-') { i++; j=1; }
		else { j=0; if (a[i]=='+') i++; }
	for (k=0; (tmp[k]=a[i])!=0&&(
		tmp[k]>='0'&&tmp[k]<='9'); (k++,i++));
	tmp[k]=0; i=atoi(tmp);
	if (i>0) if (j==0) for (j=1; j<=i; j++) mult(a,"10",a);
		else for (j=1; j<=i; j++) mult(a,".1",a);
	return; }

gcvt(x,i,a)

	char a[];
	double x;
	int i;

{
	sprintf(a,"%g",x);
	return;
	}
