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
//#+    Copyright (C) MM  Kevin C. O'Kane
//#+
//#+    Kevin C. O'Kane
//#+    Computer Science Department
//#+    University of Northern Iowa
//#+    Cedar Falls, IA 50614-0507
//#+    Tel 319 273 7322
//#+    okane@cs.uni.edu
//#+    http://www.cs.uni.edu/~okane
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

#include "memsize.h"
#include <stdio.h>
#include <fcntl.h>

#define DEBUG 0
#define GBLBUF 31
#define GBLROOT 30
#define FETCH 0
#define STORE 1
#define NEXT 4
#define NOMATCH 0
#include "externs.h"

	static int itab={0};
	short inxt,iend,iext;

logic(opnfile)

	FILE *opnfile[7];

{
#if LOGIC==1
	static union {
		unsigned char wc[4000];
		short wi[2000];
		} work;

	short parse();
	static short in_entry[100],flg1,f,i,ii,j,k;
	static long int icur,iprior,inxt1,init={0};
	static unsigned char lclid[10],hold[40],table[512];
	static short local;
	unsigned char temp[256],temp1[256],val[256];
	static unsigned char cod1[2]={1,0};
	static unsigned char tmpbuf[256];
	static short g[4],inptrs[100][4],inx;
	static unsigned char inold[100][40],inrslt[100][40];
	unsigned char dtmp[255];

	if (init==0) {
		lclid[0]='\037'; lclid[5]=0;
		local=2; sprintf(&lclid[1],"%04d",local);
		for (i=0; i<=1999; i++) work.wi[i]=0; init=1; }

/*	 ______________________________________________________                 
	|         |         |       |               |          |
  	| text(v) |iprior(2)|inxt(2)|symtab count(1)|lcl symtab|  (the stack)   
  	|_________|_________|_______|_______________|__________|             */


	if (strcmp(&v1d[1],&work.wc[1])==0) goto re_enter;

	itab=0; for (i=0; i<=1999; i++)  work.wi[i]=0; inx=0;

	while (local!=2) { 
		sprintf(&lclid[1],"%04d",local);
		symflg=2; sym_(lclid,""); local--; }

	strcpy(&work.wc[1],&v1d[1]); /* copy initial goal into work area */
	icur=1; iprior=1; iend=strlen(&work.wc[1])+2;

/*	pointers into stack:

	icur	- points to next goal or sub-goal in workstack work[icur].wc
	iprior	- points to text of parent goal or sub-goal.
	inxt	- points to 1st byte beyond current local symtab.
	iend	- points to next local symtab byte.
	iext	- points to current local table extent field (contains inxt).

									    */
	/* set up trailer structure for current goal text */

goal:	iend=((iend+1)/2)*2;		/* align to word boundary */
	work.wi[(iend)/2]=iprior;	/* back pointer to parent goal */
	iend=iend+2;			/* advance to next available byte */
	iext=iend;			/* ptr to extent count */
	iend=iend+2;			/* past extent counter */
	inxt=iend+1;			/* first byte of lcl sym tab */
	work.wi[(iext)/2]=inxt;		/* extent */
	work.wc[iend]=0;		/* count field - no lcl sym tab */
	strcpy(&temp[1],&work.wc[icur]); /* move current goal to temp */

/*	inspect goal in temp for ~... variables.  for each, make an entry
	in the local symtab at work[iend] containing the ~... name and
	lclid given to it.  if the ~... var is in "table", this means
	that a value is being passed in.  if this value is a constant,
	store the constant in the mps symtab under the lclid.  if
	the value is a lclid, store in the mps symtab the name of the new
	lclid under the old lclid.  thus, the outer points to the inner.
	as the pgm exits, the outer name will be used to find inner values.   */

	cre_lcl_tbl(temp,work.wc,&local,lclid,val,table); 
	if (lclid[1]==0) goto finish; /* stack overflow */

sub_goal:

	g[3]=FETCH;
	strcpy(&v1d[1],&work.wc[icur]);	/* goal(s) */
	if ((j=index(&v1d[1]," ",1))>0) v1d[j]=0;
	icur=icur+strlen(&v1d[1]);

	/* expand ~... references in text */

substituted:

	i=1;
	while ((j=index(&v1d[i],"~",1))>0) {

		i=i+j-1; j=i+1;
		while ((v1d[j]>='a'&&v1d[j]<='z')||(v1d[j]>='A'&&v1d[j]<='Z')||
		       (v1d[j]>='0'&&v1d[j]<='9')) j++;

/*		if char prior to "~" is ":" then this is assignment form  -
		set flag to lookup for only partial substitution.            */

		if (v1d[i-1]==':') v1d[i]=':';	/* flag to substituter */
		substr(&v1d[i],&temp1[1],1,j-i); /* temp1 is ~... variable */

/*		replace ~... variable with its current value.  find ~...
		value in current local symtab and replace by its current local
		local value.  if current local value is lclid, attempt to
		resolve the name by calling the mps symtab with it for
		alternative definitions calculated at lower levels.  this
		may result either in an explicit value or a lclid name.	     */

		lookup(&work.wc[iend],temp1,val); /* val returned with rslt */

		if (val[1]==0) { errmod_(16,&temp[1],opnfile); goto finish; }

		/* substitute value of ~... or $... into current goal */

		strcat(&val[1],&v1d[j]); strcpy(&v1d[i],&val[1]); i++; }

	switch (v1d[1]) {

case '|':	/* cut goal - succeeds at once */

		inx++; strcpy(&inold[inx][1],"|"); goto fact;

default:	/* mps expression found */

		strcpy(&xd[1],&v1d[1]); xpx=1; t2=1;
		if ((ierr=parse_()))
			{ errmod_(ierr,&xd[1],opnfile); goto finish; }

		if (xd[xpx]==':') { /* assignment form */

			strcpy(temp,&pd1[sdlim]); /*save rslt*/
			strcpy(&xd[1],&xd[xpx+1]); xpx=1; t0px=1;
			if ((ierr=parse_())) /* fetch target name */
				{ errmod_(ierr,&xd[1],1); goto finish; }

			if (v1d[1]=='^') {
				g[3]=STORE; strcpy(&bd[1],temp);
				f=global(g,v1d,bd); g[3]=FETCH; goto fact; }

			symflg=0; /* insert rslt in symtab */
			while(temp[0]==' ') strcpy(temp,&temp[1]);
			sym_(&v1d[1],temp); goto fact; }

		/* expression only - not assignment form */
		/* if numeric result, goal succeeds if not 0, fail otherwise */
		/* if string form, substitute as new goal and try again */

		if (pd1[sdlim]<'0'||pd1[sdlim]>'9')

			{ strcpy(&v1d[1],&pd1[sdlim]); goto substituted; }

		if (numcomp(&pd1[sdlim],"0")!=0) goto fact; else goto re_enter;

case '^':	/* global array goal */

	for (i=1; v1d[i]!=0; i++) if (v1d[i]>127|| v1d[i]=='('||
		    v1d[i]==')'||v1d[i]==',') v1d[i]=1;

	g[3]=FETCH; strcpy(tmpbuf,&v1d[1]); f=global(g,v1d,bd); 
/*	if (f==0&&direc1[0]!=0) { /* try alt directory
		strcpy(dtmp,direc); strcpy(direc,direc1);
		g[3]=FETCH; f=global(g,v1d,bd); strcpy(direc,dtmp); } */
	if (f==0) goto not_found; } /* end of switch */

found:	if (strlen(&bd[1])!=0) { /* if rhs exists: extract & make new 
				    goal level under current sub-goal */

		strcpy(&work.wc[inxt],&bd[1]); /* copy rhs to stack */
		iprior=icur; icur=inxt; /* set pointers */
		iend=strlen(&work.wc[inxt])+inxt+1;
		in_entry[inx]=local; /* remember lclid at entry to rhs */
		goto goal; /* finish stack build for new goal */ }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +									     +
  +	   simple global; return from sub-level;			     +
  +	   true expression; cut; or assignment stmt			     +
  +									     +
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

fact:	while (work.wc[icur]==' ') icur++; /* advance to next sub-goal */

/* 	if more sub-goals, process them.  of end, return to parent */
	if (work.wc[icur]!=0) goto sub_goal; /* another sub-goal found */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	entire level of sub-goals succeeded -
	restore parent rhs - check if parent is root goal.

  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	icur++;			/* past zero byte */
	icur=((icur+1)/2)*2;	/* align to word boundary */
	iprior=work.wi[(icur)/2];
	icur=iprior+strlen(&work.wc[iprior])+1;
	icur=((icur+1)/2)*2; icur=icur+4; iend=icur; icur=iprior;
	if (iprior!=1) goto fact; /* back in parent except if root */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	root goal restored - exit sequence

  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	inxt1=iend+1; j=work.wc[iend]; /* symtab count field */

	while (j!=0) { /* for each ~... variable in local table */

		i=inxt1+strlen(&work.wc[inxt1])+1; /* i pts to $... equiv fld */
alias:		symflg=1; sym_(&work.wc[i],"");	/* fetch val of $... */
		if (pd1[symadr]=='\037') { /* $... fld pts to another $... */
			/* copy new $... on top of old */
			strcpy(&work.wc[i],&pd1[symadr]); goto alias; }

		strcpy(temp,&pd1[symadr]);	/* extract value of $... */
		symflg=0;		/* copy to "~a" variable */
		while(temp[0]==' ') strcpy(temp,&temp[1]);
		sym_(&work.wc[inxt1+1],temp); /* avoid ~ char */
		inxt1=i+strlen(&work.wc[i])+1;	/* next ~... var */
		j--; }

	bd[1]='1'; bd[2]=0; ierr=0; return; /* return happy */

not_found:	/* global goal did not exist */

	flg1=0;
	if (index(&v1d[1],"\037",1)==0) { /* no $... vars in test fact */

		strcpy(&hold[1],&v1d[1]); /* save global goal */

		while (1) {

			g[3]=NEXT; /* global alters v1d - tmpbuf set above */
			f=global(g,v1d,bd); 
			if (tmpbuf[0]!='^') strcpy(&v1d[1],tmpbuf); 
			   else strcpy(&v1d[1],&tmpbuf[1]);

/*			if ((f==2||ianme(&v1d[1],&bd[1])==NOMATCH)
			    &&direc1[0]!=0) {
				strcpy(dtmp,direc); strcpy(direc,direc1);
				g[3]=NEXT; f=global(g,v1d,bd);
				strcpy(&v1d[1],tmpbuf); strcpy(direc,dtmp); } */

			if (f==2||ianme(&v1d[1],&bd[1])==NOMATCH)
				break; /* no next: back-up and re-try */

			if (index(&bd[1],"~",1)==0&&inx>0)
				break; /* no ~... in next and stack present -
					  try new fact against stacked goal */

			/* if array name match and ~... vars present
			   try for match */

			if (index(&bd[1],"~",1)>0) goto fetch_rhs;
			strcpy(&v1d[1],&bd[1]); /* try next one */ }

re_enter:	if (inx==0) goto finish; /* no stack */
		if (inold[inx][1]=='|') { /* cut - find start of rule */
			while (in_entry[inx]==0&&inx>=0) inx--;
			goto re_enter; }

		strcpy(&hold[1],&inold[inx][1]);	/* prior goal */
		strcpy(&v1d[1],&inrslt[inx][1]);	/* former result */
		icur=inptrs[inx][1]; inxt=inptrs[inx][2]; iend=inptrs[inx][3];
		inx--; flg1=1; goto next_candidate; }

	strcpy(&hold[1],&v1d[1]); /* save original goal */

next_candidate:

	g[3]=NEXT; strcpy(tmpbuf,&v1d[1]); f=global(g,v1d,bd);
/*	if ((f==2||ianme(&v1d[1],&bd[1])==NOMATCH)&&direc1[0]!=0) {
		strcpy(dtmp,direc); strcpy(direc,direc1);
		g[3]=NEXT; f=global(g,v1d,bd);
		strcpy(direc,dtmp); strcpy(&v1d[1],tmpbuf); } */

	if (ianme(&v1d[1],&bd[1])==NOMATCH||f==2) { /* cancel any variables */

		strcpy(&v1d[1],&hold[1]);
		if (flg1==1&&in_entry[inx+1]>0) goto re_enter;

		while ((i=index(&v1d[1],"\037",1))>0) {
			for (j=0; v1d[i]!=1; j++) temp[j]=v1d[i++]; temp[j]=0;
			symflg=2; sym_(temp,""); /* delete $... variable */
			strcpy(&v1d[1],&v1d[i+1]); }
		goto re_enter; }

	/* fetch rhs using new lhs */

fetch_rhs:

	strcpy(&v1d[1],&bd[1]); g[3]=FETCH; f=global(g,v1d,bd);

	/* check match between goal (temp1) and candidate lhs (temp) */

	strcpy(&temp[1],&v1d[1]); strcpy(&temp1[1],&hold[1]);
	i=1; j=1;

	while (1) { /*compare goal with candidate lhs for match*/

	i=index(&temp1[1],cod1,1); j=index(&temp[1],cod1,1);
	if (i+j==0) break; /* end - args have matched */
	if (i==0||j==0) goto re_enter; /* arg count mis-match */
	if (temp[1]!='~'&&temp1[1]!='~'&&temp[1]!='\037'&&temp1[1]!='\037') {
		temp[j]=0; temp1[i]=0; /* check constants match */
		if (strcmp(&temp[1],&temp1[1])!=0) goto next_candidate; }
	strcpy(&temp[1],&temp[j+1]); strcpy(&temp1[1],&temp1[i+1]); }

	/* goal matches found lhs */

	inx++; in_entry[inx]=0; /* marker to start of evaluation of lhs */
	strcpy(&inold[inx][1],&hold[1]); /* goal */
	strcpy(&inrslt[inx][1],&v1d[1]); /* save candidate lhs */
	inptrs[inx][1]=icur; inptrs[inx][2]=inxt; inptrs[inx][3]=iend;

	/* build header table giving ~... names in candidate lhs */
	/* and corresponding values supplied by goal header */

	itab=1; j=0; table[1]=0;
	i=index(&v1d[1],cod1,1); /* find open paren marker */
	strcpy(&temp[1],&v1d[1]); /* copy original lhs with variables */

next_arg:

	strcpy(&v1d[1],&v1d[i+1]); /* lhs header array name */
	j++; /* arg count */
	if (v1d[1]=='~') goto var_found; else goto no_var; /* variable? */

nxt_lhs:

	i=index(&v1d[1],cod1,1); /* comma */
	if (i<strlen(&v1d[1])) goto next_arg; 

		strcpy(&v1d[1],&temp[1]); /* restore candidate lhs */
		goto found; /* done creating table */

var_found:

	for (k=1; v1d[k]!=0&&v1d[k]!=1; k++); /* find end of var name */
	substr(&v1d[1],&table[itab],1,k-1); /* copy lhs var name to table */
	itab=itab+k; table[itab]=0; /* account for variable name */

no_var:	/* scan goal for corresponding argument */

	i=1; for (k=1; k<=j; k++) i=index(&hold[i],cod1,1)+i; /*count args*/
	for (k=i; hold[k]!=0&&hold[k]!=1; k++); /*find end*/
	hold[k]=0; strcpy(&table[itab],&hold[i]); /* arg from goal hdr */
	hold[k]=1; /* restore marker */

/*	if lhs arg is ~..., then continue table build */

	if (v1d[1]=='~') {
		itab=itab+strlen(&table[itab])+1;
		table[itab]=0; goto nxt_lhs; }

/*	if goal arg is $..., link it to lhs constant in symtab */

	if (table[itab]!='\037') goto nxt_lhs;

	/* update */

	symflg=0; i=index(&v1d[1],cod1,1); v1d[i]=0;
	for (k=1; v1d[k]==' '&&v1d[k]!=0; k++);
	sym_(&table[itab],&v1d[k]);
	v1d[i]=1; goto nxt_lhs;

	/* termination */

finish:	bd[1]='0'; bd[2]=0; work.wc[1]=0; /* no more retries */
	while (local!=2) {
		sprintf(&lclid[1],"%04d",local);
		symflg=2; sym_(lclid,""); local--; }

	return; }

/*+++++++++++++++++++++++++++++++++++++++++++++++
  +						+
  +	create local symbol table		+ 
  +						+
  +     for each ~... name in goal, make an 	+
  +	entry in the local table after the	+
  +	goal.					+
  +						+
  +	for each ~... name, create a lclid	+
  +	symbol and enter both in the local	+
  +	symbol table.				+
  +						+
  +	if ~... name in "table", then the	+
  +	associated "table" entry contains	+
  +	either a value being passed in from the +
  +	invoking goal or the name of a		+
  +	lclid variable from the invoking goal.	+
  +						+
  +	if a constant value is being passed in, +
  +	set the symtab value for the new lclid  +
  +	to this value.  if the item passed in	+
  +	if a lclid variable, set the value in	+
  +	symtab of the incoming lclid to the	+
  +	name of the new, internal lclid. thus,	+
  +	the outer lclid variable has as its	+
  +	value the name of the inner lclid var-	+
  +	iable in a chain for out-passing of 	+
  +	results.  				+
  +						+
  +++++++++++++++++++++++++++++++++++++++++++++++*/

cre_lcl_tbl(temp,work,local,lclid,val,table)

	unsigned char temp[],work[],val[],table[],lclid[];
	short int *local;

	{
	union { char cc[2]; short iww; } ww;
	short int i,j,ii,jj;
	unsigned char temp1[256],c206={206},c207={207},c208={208};
	i=1;

	while ((j=index(&temp[i],"~",1))>0) {

	j=j+i; i=j-1;

	while ((temp[j]>='a'&&temp[j]<='z')||
	       (temp[j]>='0'&&temp[j]<='9')||
	       (temp[j]>='A'&&temp[j]<='Z')) j++;

	/* extract name */

	substr(&temp[i],&temp1[1],1,j-i);
	lookup(&work[iend],temp1,val); /* check if it exists */
	if (val[1]==0) { /*does not exist*/

		work[iend]=work[iend]+1; /* increment lcl sym tab count */
		substr(&temp[i],&work[inxt],1,j-i); /* copy local variable */
		ii=inxt;
		(*local)++; /* build lcl id */
		if (*local>999) { lclid[1]=0; errmod_(19," ",1); return; }
		inxt=inxt+strlen(&work[inxt])+1; /* advance lcl sym tab ptr */
		sprintf(&lclid[1],"%04d",*local);
		strcpy(&work[inxt],lclid); /* move lcl id name into table */
		jj=inxt;
		inxt=inxt+strlen(&work[inxt])+1; /* increment ptr */
		ww.iww=inxt; /* extent */
		work[iext]=ww.cc[0]; work[iext+1]=ww.cc[1];
		i=1;
		while (i<=itab) { /*scan tbl for ~... name*/

			if (strcmp(&table[i],&work[ii])!=0) { 
				i=i+strlen(&table[i])+1; /* ~... name */
				i=i+strlen(&table[i])+1; /* val */ }

			else { /* found */

				symflg=0;
				i=i+strlen(&table[i])+1; /*-> ~...*/
				if (table[i]=='\037') {
					while (work[jj]==' ') 
						strcpy(&work[jj],&work[jj+1]);
					sym_(&table[i],&work[jj]); }
				   else {
					while (table[i]==' ')
						strcpy(&table[i],&table[i+1]);
					sym_(&work[jj],&table[i]); }
				break; }
			}
		}
	i=j; if (temp[i++]==0) return;
	}
	return; }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +									 +
  +	translate ~... variables to their current local symtab value	 +
  +									 +
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

lookup(work,temp1,val)

	unsigned char work[],temp1[],val[];

{

	short int i,j,k; unsigned char flg;

	i=work[0]; /* nbr of entries */ j=1; flg=0;

	/* if 1st byte of temp1 is ":" then do not mke complete */
	/* substitution.  this is the assignment form. */

	if (temp1[1]==':') { flg=1; temp1[1]='~'; }

	for (k=1; k<=i; k++) {
	
		if (strcmp(&work[j],&temp1[1])==0) goto found;
		j=j+strlen(&work[j])+1; j=j+strlen(&work[j])+1; }

	val[1]=0; return;

found:	j=j+strlen(&work[j])+1;
	strcpy(&val[1],&work[j]); /* internal name */
	if (flg) return; /* assignment form - no substitute value */

	while (1) {
		symflg=1; sym_(&val[1],""); if (symflg==0) return;
		strcpy(&val[1],&pd1[symadr]);
		if (val[1]!='\037') return; /* trace back to origin */ }
	}

ianme(a,b)

	unsigned char a[],b[];

	{ short int i,j,in;
	i=index(a,"\01",1)-1; j=index(b,"\01",1)-1;
	a[i]=0; b[j]=0;
	if (strcmp(a,b)==0) in=1; else in=0;
	a[i]=1; b[j]=1; return(in); }
#if DEBUG
pxt(a)
	unsigned char a[];
{
	int i;
	for (i=0; a[i]!=0; i++) {
		if (a[i]=='\n'||(a[i]>' '&&a[i]<=127)) 
			printf("%c",a[i]); else printf(" "); }
	printf(" ");
	}
pyt(a)
	unsigned char a[];
{
	int i;
	printf("\n"); for (i=0; a[i]!=0; i++) printf("%d ",a[i]);
	}
#endif
#endif
#if LOGIC==0
}
#endif
