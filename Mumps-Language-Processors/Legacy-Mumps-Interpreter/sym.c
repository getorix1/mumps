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

//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+								
//#+		  ####    #   #  #    #				
//#+		 #         # #   ##  ##				
//#+		  ####      #    # ## #				
//#+		      #     #    #    #				
//#+		 #    #     #    #    #				
//#+		  ####      #    #    #				
//#+							
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "memsize.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "keyfix.h"

struct stab {
	char * name;
	struct stab * next;
	char * data;
	int size;
	};

void sym_(unsigned char *a, unsigned char *b) {

  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  +								
  +	code 1 retrieve						
  +	code 0 store/create					
  +	code 2 delete explicit					
  +	code 3 $n on last argument				
  +	code 4 kill all						
  +	code 5 kill all except...				
  +	code 6 $d 						
  +								
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "externs.h"

char key[1024];

static struct stab *start=NULL;
struct stab *p1,*p2,*p3;
int i;

strcpy(key,a);
keyfix(key);

if (symflg==0) { /* store */
	p2=NULL;
	p3=start;
	while (p3!=NULL) {
		if (strcmp(p3->name,key)>=0) break;
		p2=p3;
		p3=p3->next;
		}

	if (p3==NULL && p2==NULL) { /* empty list */
		p1=(struct stab *) malloc (sizeof(struct stab));
		if (p1==NULL) { printf("Out of memory\n\n"); exit(1); }
		p1->name=(char *) malloc (strlen(key)+1);
		if (p1->name==NULL) { printf("Out of memory\n\n"); exit(1); }
		strcpy(p1->name,key);
		p1->size=strlen(key);
		p1->next=NULL;
		start=p1;
		p1->data=(char *) malloc(strlen(b)+1);
		if (p1->data==NULL) { printf("Out of memory\n\n"); exit(1); }
		strcpy(p1->data,b);
		symflg=1;
		return;
		}

	if (p3!=NULL && strcmp(p3->name,key)==0) {
		if (p3->size>=strlen(b)) strcpy(p3->data,b);
		else {
			free (p3->data);
			p3->data=(char *) malloc(strlen(b)+1);
			if (p3->data==NULL) { 
				printf("Out of memory\n\n"); exit(1); }
			strcpy(p3->data,b);
			p3->size=strlen(b);
			}
		symflg=1;
		return;
		}

	p1=(struct stab *) malloc (sizeof(struct stab));
	if (p1==NULL) { printf("Out of memory\n\n"); exit(1); }
	p1->name=(char *) malloc (strlen(key)+1);
	if (p1->name==NULL) { printf("Out of memory\n\n"); exit(1); }
	strcpy(p1->name,key);
	p1->size=strlen(key);
	p1->next=p3;
	if (p2!=NULL) p2->next=p1; else start=p1;
	p1->data=(char *) malloc(strlen(b)+1);
	if (p1->data==NULL) { printf("Out of memory\n\n"); exit(1); }
	strcpy(p1->data,b);
	symflg=1;
	return;
	}

if (symflg==1) { /* retrieve */
	p1=start;
	while (p1!=NULL) {
		if (strcmp(p1->name,key)==0) break;
		p1=p1->next;
		}
	if (p1==NULL) { symflg=0; return; }

	strcpy(b+1,p1->data);
	symflg=1;
	return;
	}

if (symflg==3) { /* next */
	p1=start;
	p2=NULL;
	i=strlen(key);
	if (key[i-3]=='-' && key[i-2]=='1' && key[i-1]==1) {
		key[i-3]=1;
		key[i-2]=0;
		i=i-2;
		}
	while (p1!=NULL) {
		if (strncmp(p1->name,key,i)>=0) break;
		p2=p1;
		p1=p1->next;
		}

	if (p1==NULL) { strcpy(b,"-1"); symflg=1; return; }

	if (strncmp(p1->name,key,i)>0) goto end;

	p1=p1->next;
	while (p1!=NULL) {
		if (strncmp(p1->name,key,i)!=0) break;
		p1=p1->next;
		}

	if (p1==NULL) { strcpy(b,"-1"); symflg=1; return; }

end:	i=strlen(p1->name)-2;
	for (; p1->name[i]!=1; i--);
	strcpy(b,p1->name+i+1);
	b[strlen(b)-1]=0;
	while (b[0]==' ') strcpy(b,&b[1]);
	symflg=1;
	return;
	}

if (symflg==2) { /* kill selected */

repeat:	p1=start;
	p2=NULL;
	i=0;

	i=strlen(key);

	while (p1!=NULL) {
		if (strncmp(p1->name,key,i)==0 && p1->name[i]==1) break;
		else if (strcmp(p1->name,key)==0) break;
		p2=p1;
		p1=p1->next;
		}

	if (p1==NULL) { symflg=1; return; }
	if (p2==NULL)  start=p1->next;
	else p2->next=p1->next;
	free (p1->data);
	free (p1->name);
	free (p1);
	goto repeat;
	}

if (symflg==4) { /* kill all */
	p1=start;
	while (p1!=NULL) {
		p2=p1->next;
		free (p1->data);
		free (p1->name);
		free (p1);
		p1=p2;
		}
	start=NULL;
	symflg=1;
	return;
	}

if (symflg==5) { /* kill all except... */

	struct nmes *np1;
	int flg;

	p1=start;
	p3=NULL;
	while (p1!=NULL) {
		np1=nstart;
		flg=0;
		while (np1!=NULL) {
			if (strcmp(np1->name,p1->name)==0) {
				p3=p1;
				p1=p1->next;
				flg=1;
				break;
				}
			np1=np1->next;
			}
		if (flg) continue;
		free (p1->data);
		free (p1->name);
		p2=p1->next;
		free (p1);
		if (p3==NULL) start=p2;	
		else p3->next=p2;
		p1=p2;
		}
	symflg=1;
	return;
	}

if (symflg==6) { /* $d */
	
	p1=start;

	while (p1!=NULL) {
		if (strcmp(p1->name,key)==0) break;
		p1=p1->next;
		}

	if (p1==NULL) { /* search fail */

		i=strlen(key);

		p1=start;
		while (p1!=NULL) {
			if (strncmp(p1->name,key,i)==0) break;
			p1=p1->next;
			}

		if (p1!=NULL) {
			b[0]='1';
			b[1]='0';
			b[2]=0;
			return;
			}

		b[0]='0'; 
		b[1]=0;
		return;
		}

	b[0]='1'; /* exists */

	p1=p1->next;

	if (p1==NULL) { /* no possible descendants */
		b[1]=0;
		return;
		}

	i=strlen(key);

	if (strncmp(key,p1->name,i)==0) {
		b[1]='1';
		b[2]=0;
		return;
		}

	b[1]=0;
	return;

	return;
	}
}
