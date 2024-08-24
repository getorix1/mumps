
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

#include <stdlib.h>

#include <memsize.h>

/******
******* bcd math package
******/

add(a,b,c)

	char a[13],b[13],c[13];

	{ short as,bs,i,j,k,l,m,id,jd;
	char ina[25],inb[25],buf[25];

	id= -1; jd= -1;
	cannon(a); cannon(b);
	if (a[0]=='-') as=1; else as=0;
	if (b[0]=='-') bs=1; else bs=0;
	if (as!=bs) {
		if ((i=numcomp(&a[as],&b[bs]))==0) { c[0]='0'; c[1]=0; return; }
		if (i>0) {
			if (as==1) { sub(&a[as],&b[bs],ina); strcpy(c,"-");
				strcat(c,ina); return; }
			sub(&a[as],&b[bs],c); return; }
		if (bs==1) { sub(&b[bs],&a[as],ina); c[0]='-'; c[1]=0;
			strcat(c,ina); return; }
		if (bs==1) { c[0]='-'; c[1]=0; sub(&b[bs],&a[as],ina); return; }
		sub(&b[bs],&a[as],c); return; }
	for (i=0; a[i]!=0; i++) if (a[i]=='.') id=i; 
	for (j=0; b[j]!=0; j++) if (b[j]=='.') jd=j;
	for (k=0; k<24; k++) { buf[k]='0'; ina[k]='0'; inb[k]='0'; }
	buf[24]=0; ina[24]=0; inb[24]=0;
	if (id<0) { if (i>12) { strcpy(c,"****"); return; } k=11; }
		else { if (i-id>12) i=id+13; k=i-(id+1)+11; }
	l=k;
	for (; i>as; k--) if (id!=--i) ina[k]=a[i]; else { k++; }
	m=k;
	if (jd<0) { if (j>12) { strcpy(c,"****"); return; } k=11; }
		else { if (j-jd>12) j=jd+13; k=j-(jd+1)+11; }
	if (k>l) l=k;
	for (; j>bs; k--) if (jd!=--j) inb[k]=b[j]; else { k++; }
	if (k<m) m=k;
	for (i=l; i>m-1; i--) {
		if ((buf[i]=ina[i]+inb[i]-48)>57)
			{ buf[i]-=10; ina[i-1]++; } }
	if (m==11&&buf[11]=='0') {
		c[0]='.';
		for (i=12; i<=24; i++) c[i-11]=buf[i];	
		for (j=i-13; c[j]=='0'; j--) c[j]=0;
		if (c[1]=='0'&&c[2]==0) {
			c[0]='0'; c[1]=0; return; }
		if (as==1) { buf[0]='-'; buf[1]=0;
			strcat(buf,c); strcpy(c,buf); }
		return; }
	j=0;
	for (i=0; buf[i]=='0'&&i<=11; i++);
	for (; i<=11; i++) c[j++]=buf[i];
	c[j++]='.';
	for (i=12; i<=l&&j<=11; i++) c[j++]=buf[i];
	c[j--]=0;
	while (c[j]=='0') c[j--]=0;
	if (c[j]=='.') c[j]=0;
	if (c[0]==0) { c[0]='0'; c[1]=0; return; }
	if (as==1) { buf[0]='-'; buf[1]=0;
		strcat(buf,c); strcpy(c,buf); }
	return;
	}

sub(a,b,c)

	char a[],b[],c[];

	{ short as,bs,i,j,k,l,m,id,jd;
	char ina[25],inb[25],buf[25];

	id= -1; jd= -1;
	cannon(a); cannon(b);
	if (a[0]=='-') { as=1; strcpy(a,&a[1]); } else as=0;
	if (b[0]=='-') { bs=1; strcpy(b,&b[1]); } else bs=0;
	if (as==bs&&numcomp(a,b)>=0) goto s1;
	if (as==bs&&numcomp(a,b)<0) {
		strcpy(ina,a); strcpy(a,b); strcpy(b,ina);
		if (as==1) as=0; else as=1; goto s1; }
	if (as==0) { add(a,b,c); return; }
	add(a,b,c); strcpy(buf,"-"); strcat(buf,c); strcpy(c,buf);
	return;
	
s1:	for (i=0; a[i]!=0; i++) if (a[i]=='.') id=i;
	for (j=0; b[j]!=0; j++) if (b[j]=='.') jd=j;
	for (k=0; k<24; k++) { buf[k]='0'; ina[k]='0'; inb[k]='0'; }
	buf[24]=0; ina[24]=0; inb[24]=0;
	if (id<0) { if (i>12) { strcpy(c,"****"); return; } k=11; }
		else { if (i-id>12) i=id+13; k=i-(id+1)+11; } 
	l=k;
	for (; i>0; k--) if (id!=--i) ina[k]=a[i]; else { k++; }
	m=k;
	if (jd<0) { if (j>12) { strcpy(c,"****"); return; } k=11; }
		else { if (j-jd>12) j=jd+13; k=j-(jd+1)+11; }
	if (k>l) l=k;
	for (; j>0; k--) if (jd!=--j) inb[k]=b[j]; else { k++; }
	if (k<m) m=k;
	for (i=l; i>m; i--) {
		if ((buf[i]=ina[i]-inb[i]+48)<48) { buf[i]+=10; ina[i-1]--; } }
	if (m==11) { 
		c[0]='.';
		for (i=12; i<=24; i++) c[i-11]=buf[i];	
		for (j=i-13; c[j]=='0'; j--) c[j]=0;
		if ((c[1]=='0'&&c[2]==0)||c[1]==0) {
			c[0]='0'; c[1]=0; return; }
		if (as==1) { buf[0]='-'; buf[1]=0;
			strcat(buf,c); strcpy(c,buf); }
		return; }
	j=0;
	for (i=0; buf[i]=='0'&&i<=11; i++);
	for (; i<=11; i++) c[j++]=buf[i];
	c[j++]='.';
	for (i=12; i<=l&&j<=11; i++) c[j++]=buf[i];
	c[j--]=0;
	while (c[j]=='0') c[j--]=0;
	if (c[j]=='.') c[j]=0;
	if (c[0]==0) { c[0]='0'; c[1]=0; return; }
	if (as==1) { strcpy(buf,"-"); strcat(buf,c); strcpy(c,buf); }
	return;
	}

mult(a,b,c)

	char a[],b[],c[];

	{ short as,bs,i,j,k,l,m,id,jd;
	char ina[25],inb[25],buf[25],tmp[25];

	id= -1; jd= -1;
	cannon(a); cannon(b);
	if (a[0]=='-') { as=1; strcpy(a,&a[1]); } else as=0;
	if (b[0]=='-') { bs=1; strcpy(b,&b[1]); } else bs=0;
	for (i=0; a[i]!=0; i++) if (a[i]=='.') id=i;
	for (j=0; b[j]!=0; j++) if (b[j]=='.') jd=j;
	for (k=0; k<24; k++) { buf[k]='0'; ina[k]='0'; inb[k]='0'; }
	buf[24]=0; ina[24]=0; inb[24]=0;
	if (id<0) { if (i>10) { strcpy(c,"****"); return; } k=11; }
		else { if (i-id>7) i=id+7; k=i-(id+1)+11; }
	l=k;
	for (; i>0; k--) if (id!=--i) ina[k]=a[i]; else { k++; }
	m=k;
	if (jd<0) { if (j>10) { strcpy(c,"****"); return; } k=11; }
		else { if (j-jd>7) j=jd+7; k=j-(jd+1)+11; }
	if (k>l) l=k;
	for (; j>0; k--) if (jd!=--j) inb[k]=b[j]; else { k++; }
	if (k<m) m=k;

	buf[0]='0'; buf[1]=0;
	for (i=0; i<=23; i++) {
	for (j=0; j<=23; j++) {
		k=ina[i]-48; l=inb[j]-48; k=k*l;
		if (k==0) continue;
		l=11-i+11-j; l=11-l;
		for (m=0; m<=23; m++) tmp[m]='0'; tmp[24]=0;
		if (l<0) { strcpy(c,"****"); return; }
		sprintf(&tmp[l-1],"%02d",k);
		tmp[l+1]='0';
		if (l<=11) tmp[12]=0;
		else {
			for(k=24; k>11; k--) tmp[k]=tmp[k-1];
			tmp[12]='.'; tmp[l+2]=0; }
		add(buf,tmp,buf); } }

	if (m==11) {
		c[0]='.';
		for (i=12; i<=24; i++) c[i-11]=buf[i];
		if (as!=bs) {
			strcpy(buf,c); strcpy(c,"-"); strcat(c,buf); }
		return; }
	j=0;
	for (i=0; buf[i]=='0'&&i<=11; i++);
	for (; i<=11; i++) c[j++]=buf[i];
	c[j++]='.';
	for (i=12; i<=l&&j<=11; i++) c[j++]=buf[i];
	c[j]=0;
	if (c[j-1]=='.') c[j-1]=0;
	if (c[0]==0) { c[0]='0'; c[1]=0; return; }
	if (as!=bs) {
		strcpy(buf,c); strcpy(c,"-"); strcat(c,buf); }
	return;
	}

divx(a,b,c)

	char a[],b[],c[];

	{ short i,j,id,jd,as,bs;
	long x,y,z,atol();
	char ina[25],buf[25];

	id= -1; jd= -1;
	cannon(a); cannon(b);
	if (a[0]=='-') { as=1; strcpy(a,&a[1]); } else as=0;
	if (b[0]=='-') { bs=1; strcpy(b,&b[1]); } else bs=0;
	for (i=0; a[i]!=0; i++) if (a[i]=='.') id=i;
		else if (a[i]<'0'||a[i]>'9') { a[i]=0; break; }
	for (j=0; b[j]!=0; j++) if (b[j]=='.') jd=j;
		else if (b[j]<'0'||b[j]>'9') { b[j]=0; break; }
	if (id>9||jd>9) {strcpy(c,"****"); return;}
	if (id>=0) { /* remove decimal point */
		a[9]=0; /* nbr of digits limit */
		for (i=id; a[i]!=0; i++) a[i]=a[i+1];
		id=strlen(&a[id]); }
	else id=0;
	if (jd>=0) { /* remove decimal point */
		b[9]=0; /* nbr of digits limit */
		for (j=jd; b[j]!=0; j++) b[j]=b[j+1];
		jd=strlen(&b[jd]); }
	else jd=0;
	if (strlen(a)>12||strlen(b)>12) { strcpy(c,"****"); return; }
	x=atol(a); y=atol(b); ina[0]=0;
	if (x==0) { c[0]='0'; c[1]=0; return; }
	for (i=0; i<20; i++) {
		z=x/y;
		sprintf(buf,"%ld",z);
		strcat(ina,buf);
		z=x%y;
		if (z==0) break;
		x=z*10;
		if (i==0) strcat(ina,".");
		}

	if (id==jd) { /* same decimal point location */
		if (ina[0]=='0') strcpy(c,&ina[1]);
			else strcpy(c,ina);
		j=0;
		for(i=0; c[i]!=0; i++) if (c[i]=='.') j=1;
		if (j==1) c[12]=0; else c[12]=0; /* result limits */
		if (as!=bs) { /* signs */
			strcpy(ina,c); strcpy(c,"-"); strcat(c,ina); }
		return; }

	strcpy(c,ina);

	if (jd>id) { jd=jd-id; id=0; } else { id=id-jd; jd=0; }
	j=0;
	for (i=0; c[i]!=0; i++) if (c[i]=='.') { j=i; break; }
	if (j==0) { /* add a decimal point if none present */
			j=i; strcat(c,".");
			for (i=0; i<=jd; i++) strcat(c,"0"); }
	else while(strlen(&c[j+1])<jd) strcat(c,"0"); /* pad zeros */

	if (jd>0) { /* shift decimal point */
		for (i=j; i<=j+jd-1; i++) c[i]=c[i+1];
		c[j+jd]='.'; }
	if (id>0) { /* shift decimal point */
		for (i=j; i>j-id; i--)
			if (i>0) c[i]=c[i-1];
				else c[i]='0';
		if (j-id>=0) c[j-id]='.';
			else {
				for(i=strlen(c); i>=0; i--) c[i+1]=c[i];
				c[0]='.'; } }
	if (c[0]==0) { c[0]='0'; c[1]=0; return; }
	for (i=0; c[i]!=0; i++);
	while (c[--i]=='0') c[i]=0;
	if (c[i]=='.') c[i]=0;
	while (c[0]=='0'&&c[1]!='.') strcpy(c,&c[1]); /* remove leading 0's */
	c[12]=0; /* length restriction on result */
	if (as!=bs) { strcpy(ina,c); strcpy(c,"-"); strcat(c,ina); }
	return;
	}

numcomp(aa,bb)

	char aa[],bb[];

	{ short as,bs,i,j,k,l,m,id,jd;
	char a[25],b[25],ina[25],inb[25];

	id= -1; jd= -1;
	strcpy(a,aa); strcpy(b,bb);
	cannon(a); cannon(b);
	if (a[0]=='-') { strcpy(a,&a[1]); as=1; } 
		else { if (a[0]=='+') strcpy(a,&a[1]); as=0; }
	if (b[0]=='-') { strcpy(b,&b[1]); bs=1; }
		else { if (b[0]=='+') strcpy(b,&b[1]); bs=0; }
	if (as>bs) return(-1);
	if (as<bs) return(1);
	for (i=0; a[i]!=0; i++) if (a[i]=='.') id=i;
	for (j=0; b[j]!=0; j++) if (b[j]=='.') jd=j;
	for (k=0; k<24; k++) { ina[k]='0'; inb[k]='0'; }
	ina[24]=0; inb[24]=0;
	if (id<0) k=11; else k=i-(id+1)+11; l=k;
	for (; i>0; k--) if (id!=--i) ina[k]=a[i]; else { k++; }
	m=k;
	if (jd<0) k=11; else k=j-(jd+1)+11; if (k>l) l=k;
	for (; j>0; k--) if (jd!=--j) inb[k]=b[j]; else { k++; }
	if (k<m) m=k;
	for (i=m; i<=l; i++) {
		if (ina[i]==inb[i]) continue;
		if (ina[i]<inb[i]) 
			if (as==1) return(1); 
			else return(-1);
		else if (as==1) return(-1);
			else return(1); }
	return(0);
	}
cannon(a)

	char a[];

{	int i,j,k;
	char tmp[25];

	i=0; j=0;
	while(a[i]=='+'||a[i]=='-') {
		if (a[i]=='-') if (j==0) j=1; else j=0; i++; }	
	if (i>0) {
		if (j==0) strcpy(a,&a[i]);
			else { a[0]='-'; 
				if (i>1) strcpy(&a[1],a[i]); } }
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
