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

//	May 10, 2022

#include "definitions.h"

void	hyper_clusters(STATE *SP) {

#include "globals.h"

	char docNbr[32], word[2048], cluster[32], cluster1[32];;

	double hyper_min = atof(SP->hyper_min);

	printf("\nBegin hyper-clusters.cpp\n");
	printf("\thyper_min = %5.3f\n", hyper_min);

//---------------------------------------------
//	read the level one clusters and build
//	centroid vectors
//---------------------------------------------

	strcpy(cluster,"");
	while(1) {
		strcpy(cluster,ct(cluster).Order());
		if (strlen(cluster) == 0) break;
		strcpy(docNbr,"");
		while(1) {
			strcpy(docNbr,ct(cluster,docNbr).Order());
			if (strlen(docNbr) == 0) break;
			strcpy(word,"");
			while(1) {
				strcpy(word,doc(docNbr,word).Order());
				if (strlen(word) == 0) break;
				if (hc(cluster,word).Data()) 
					hc(cluster,word)=doc(docNbr,word)+hc(cluster,word);
				else
					hc(cluster,word)=doc(docNbr,word);
				}
			}
		}

	FILE *f1 = fopen("Hypercluster-vectors.txt", "w");

	strcpy(cluster,"");
	while(1) {
		strcpy(cluster,hc(cluster).Order());
		if (strlen(cluster) == 0) break;
		fprintf(f1, "Hyper Cluster %s\n", cluster);
		strcpy(word,"");
		while(1) {
			strcpy(word, hc(cluster,word).Order());
			if (strlen(word) == 0) break;
			if (atof(hc(cluster,word)) < hyper_min) {
				hc(cluster,word).Kill();
				}
			else {
				fprintf(f1, "%s(%s) ",word, (char*) hc(cluster,word));
				}
			}
		fprintf(f1, "\n\n");
		}

	fclose (f1);

//---------------------------------------
//     calculate cluster similarities
//---------------------------------------

	f1 = fopen("hypertmp.tmp", "w");

	strcpy(cluster,"");
	while(1) {
		strcpy(cluster,hc(cluster).Order());
		if (strlen(cluster) == 0) break;
		strcpy(cluster1,cluster);
		while(1) {
			double cos;
			strcpy(cluster1,hc(cluster1).Order());
			if (strlen(cluster1) == 0) break;
			cos = Cosine(hc(cluster),hc(cluster1));
			if (cos < 0.01) continue;
			fprintf(f1, "%s %s %f\n", cluster, cluster1, cos);
			}
		}
	fclose (f1);

	system("sort -n -r < hypertmp.tmp > hypertmp.sorted.tmp");

	f1 = fopen("hypertmp.sorted.tmp", "r");

	x("").Kill();

	char s[32], line[1024], score[32], seq1[32], seq2[32];
	int cx, f;

	cx = 1;

	while(fgets(line, 32000, f1) != NULL) {

		strcpy(seq1, strtok(line," "));
		strcpy(seq2, strtok(NULL," "));
		strcpy(score, strtok(NULL," "));

		if (strcmp(seq1,seq2) == 0) continue;
		f = 1;

//-------------------------------------------------------------------------
//	^x() is a two dimensional array that contains, at the second level,
//	a list of clusters to which the document number (seq1) belongs
//	^cluster() is the cluster matrix.  Each row (s) is a cluster
//	numbered 1,2,3 ... The second level is a list of the document
//	numbers of those documents in the cluster. The following
//	code runs thru all the clusters first for doc1 (seq1) and
//	adds seq2 (doc2) to those clusters doc1 belongs to.  It
//	repeats the process for seq2 (doc2).  If a doc pair are not
//	assigned to some cluster (f=1), they are assigned to a new
//	cluster and the cluster number is incremented (c)
//-------------------------------------------------------------------------

		if (x(seq1).Data()) {
			strcpy(s,"");
			while(1) {
				strcpy(s,x(seq1,s).Order());
				if (strlen(s) == 0) break;
				hclstr(s,seq2)="";
				x(seq2,s)="";
				f=0;
				}
			}

		if (x(seq2).Data()) {
			strcpy(s,"");
			while(1) {
				strcpy(s,x(seq2,s).Order());
				if (strlen(s) == 0) break;
				hclstr(s,seq1)="";
				x(seq1,s)="";
				f=0;
				}
			}


		if (f) {
			hclstr(cx,seq1)="";
			x(seq1,cx)="";
			hclstr(cx,seq2)="";
			x(seq2,cx)="";
			cx=cx+1;
			}
		}

	f1 = fopen("Hyperclusters.txt", "w");

	fprintf(f1, "Number of clusters: %d\n\n",cx-1);

	strcpy(cluster,"");
	while(1) {
		strcpy(cluster,hclstr(cluster).Order());
		if (strlen(cluster) == 0) break;
		fprintf(f1, "Hypercluster: %s \n", cluster);
		strcpy(seq1,"");
		while(1) {
			strcpy(seq1,hclstr(cluster,seq1).Order());
			if (strlen(seq1) == 0) break;
			fprintf(f1, "Subcluster: %s\n", seq1);
			strcpy(docNbr,"");
			while(1) {
				strcpy(docNbr,ct(seq1,docNbr).Order());
				if (strlen(docNbr) == 0) break;
				fprintf(f1, "%6.6s %-100.100s\n", docNbr, (char*) title(docNbr));
				}
			}
		fprintf(f1, "\n\n");
		}

	fclose(f1);

	file_size("Hypercluster-vectors.txt");

	return;

	}
