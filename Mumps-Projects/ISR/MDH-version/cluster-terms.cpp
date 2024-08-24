/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Information Storage and Retrieval Kit
#+
#+     Copyright (C) 2021 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     https://www.cs.uni.edu/~okane
#+     https://threadsafebooks.com/
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

//	May 9, 2022

#include "definitions.h"

//-------------------------
//	CLUSTER TERMS
//-------------------------

void	cluster_terms(STATE *SP) {

#include "globals.h"

//--------------------------
//	cluster documents
//--------------------------

	char cx[32], s[32], w1[2048], w2[2048], score[32], line[8192];
	int f, cNbr;
	mstring sword;
	double min_tt_cluster;

	min_tt_cluster = atof(SP->min_tt_cluster);

	printf("\nBegin cluster-terms.cpp\n");
	printf("\tmin_tt_cluster = %5.3f\n", min_tt_cluster);

	FILE *TTtmp = fopen("tt-tmp.tmp", "w");

	strcpy(w1, "");
	while(1) {
		strcpy(w1, tt(w1).Order());
		if (strlen(w1) == 0) break;

		strcpy(w2, "");
		while(1) {
			strcpy(w2, tt(w1,w2).Order());
			if (strlen(w2) == 0) break;
			if (atof(tt(w1,w2)) < min_tt_cluster) break;
			fprintf(TTtmp, "%s %s %s\n", (char *) tt(w1,w2), w1, w2);
			}
		}

	fclose(TTtmp);

	system("sort -n -r < tt-tmp.tmp > tt-tmp.sorted.tmp");

	TTtmp = fopen("tt-tmp.sorted.tmp", "r");

	if (TTtmp == NULL) {
		printf("Missing tt-tmp.sorted.tmp\n");
		abort();
		}

	cNbr = 1;
	while(fgets(line,16384,TTtmp) != NULL) {
		line[strlen(line)-1] = 0;

		strcpy(score, strtok(line, " "));
		strcpy(w1, strtok(NULL, " "));
		strcpy(w2, strtok(NULL, " "));

		if (strcmp(w1, w2) == 0) continue;
		f = 1;

//-------------------------------------------------------------------------
//	^x() is a two dimensional array that contains, at the second level,
//	a list of clusters to which the word (w1) belongs
//	^cluster() is the cluster matrix.  Each row (s) is a cluster
//	numbered 1,2,3 ... The second level is a list of the words
//	in the cluster. 
//	
//	The following code runs thru all the clusters first for w1 (w1) and
//	adds w2 to those clusters w1 belongs to.  It
//	repeats the process for w2.  If a word pair are not
//	assigned to some cluster (f=1), they are assigned to a new
//	cluster and the cluster number is incremented (c)
//-------------------------------------------------------------------------

		if (tx(w1).Data()) {
			strcpy(s, "");
			while(1) {
				strcpy(s, tx(w1,s).Order());
				if (strlen(s) == 0) break;
				tclstr(s,w2) = "";
				tx(w2,s) = "";
				f = 0;
				}
			}
		if (tx(w2).Data()) {
			strcpy(s, "");
			while(1) {
				strcpy(s, tx(w2,s).Order());
				if (strlen(s) == 0) break;
				tclstr(s,w1) = "";
				tx(w1,s) = "";
				f = 0;
				}
			}
		if (f) {
			sprintf(cx, "%d", cNbr);
			tclstr(cx,w1)=""; 
			tx(w1,cx)="";
			tclstr(cx,w2)=""; 
			tx(w2,cx)="";
			cNbr = cNbr + 1;
			}
		}

	fclose(TTtmp);

	TTtmp = fopen("tt-clusters.txt", "w");

	fprintf(TTtmp, "Number of clusters: %d\n", cNbr);

//	strcpy(cx, "");
	for (int i = 0; i < cNbr; i++) {
//		strcpy(cx, tclstr(cx).Order());
//		if (strlen(cx) == 0) break;

		sprintf(cx, "%d", i);

//		fprintf(TTtmp, "cluster: %s\n", cx);

		strcpy(w1, "");
		while(1) {
			strcpy(w1, tclstr(cx,w1).Order());
			if (strlen(w1) == 0) break;
//			fprintf(TTtmp, "%s ",w1);
			fprintf(TTtmp, "%s ", stem(w1,"").Order().c_str());
			}
		fprintf(TTtmp, "\n");
		}

	fclose(TTtmp);

	file_size("tt-clusters.txt");
	
			


/************************************

 halt

 kill ^dt
 kill ^dtt

 for d=$order(^doc(d)) do
 . for w=$order(^doc(d,w)) do
 .. if $data(^x(w)) do
 ... for c=$order(^x(w,c)) do
 .... if $data(^dt(d,c)) set ^dt(d,c)=^dt(d,c)+1
 .... else  set ^dt(d,c)=1

 for d=$order(^dt(d)) do
 . for c=$order(^dt(d,c)) do
 .. for w=$order(^clstr(c,w)) do
 ... if $data(^dtt(d,w)) set ^dtt(d,w)=^dtt(d,w)+1
 ... else  set ^dtt(d,w)=1

 for d=$order(^dtt(d)) do
 . write ^title(d),!
 . for w=$order(^dtt(d,w)) write w," "
 . write !
******************************************/

	}

