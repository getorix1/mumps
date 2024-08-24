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

#include <mumpsc/libmpscpp.h>

#include "definitions.h"


void	cluster_docs(STATE *SP) {

#include "globals.h"

//--------------------------
//	cluster documents
//--------------------------

	char s[128], docNbr[32], docNbr1[32], line[8192], *p1;
	char word[2048], word1[2048], cx[128], score[32], seq1[16], seq2[16];
	int cl = 1, f, clusMinWordCount = 5;
	double cdMin;

	printf("\nBegin cluster-docs.cpp\n");

	cdMin = atof(SP->min_cluster_cosine);

	printf("\tcdMin = %5.3f\n", cdMin);
	printf("\tclusMinWordCount = %d\n", clusMinWordCount);

	FILE *ddTmp = fopen("doc-clusters-dd.tmp", "w");

	strcpy(docNbr, "");
	while(1) {
		strcpy(docNbr, dd(docNbr).Order());
		if (strlen(docNbr) == 0) break;
	
		strcpy(docNbr1, "");
		while(1) {
			strcpy(docNbr1, dd(docNbr,docNbr1).Order());
			if (strlen(docNbr1) == 0) break;

			if (atof(dd(docNbr,docNbr1)) < cdMin) continue;

			fprintf(ddTmp, "%s %s %s\n", 
				(char *) dd(docNbr,docNbr1), docNbr, docNbr1);
			}
		}

	fclose(ddTmp);

	system("sort -n -r < doc-clusters-dd.tmp > doc-clusters-dd-sorted.tmp");

	ddTmp = fopen("doc-clusters-dd-sorted.tmp", "r");

	while(fgets(line,16384,ddTmp) != NULL) {

		line[strlen(line) - 1] = 0;

		p1 = strtok(line, " ");
		strcpy(score, p1); // correlation score

		p1 = strtok(NULL, " ");
		strcpy(seq1, p1);

		p1 = strtok(NULL, " ");
		strcpy(seq2, p1);

		if (strcmp(seq1, seq2) == 0) continue;

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
		strcpy(s, "");
		while(1) {
			strcpy(s, x(seq1,s).Order());
			if (strlen(s) == 0) break;
			clstr(s,seq2) = "";
			x(seq2,s) = "";
			f = 0;
			}

		}

	if (x(seq2).Data()) {
		strcpy(s, "");
		while(1) {
			strcpy(s, x(seq2,s).Order());
			if (strlen(s) == 0) break;
			clstr(s,seq1) = "";
			x(seq1,s) = "";
			f = 0;
			}
		}

	if (f) {
		clstr(cl,seq1) = "";
		x(seq1,cl) = "";

		clstr(cl,seq2) = "";
		x(seq2,cl) = "";

		cl++;
		}
	}

	fclose(ddTmp);

//----------------------
//	print clusters
//----------------------

	FILE *clusFile = fopen("doc-clusters-with-titles.txt", "w");

	FILE *clusTerms = fopen("doc-clusters-terms.tmp", "w"); // cluster nbr term

	fprintf(clusFile, "Document Clusters with Doc Nbrs and Titles\n\n");

	for (int i = 1; i < cl; i++) {

		sprintf(cx, "%d", i);

		fprintf(clusFile, "Cluster: %s\n", cx);

		strcpy(seq1, "");
		while(1) {
			strcpy(seq1, clstr(cx,seq1).Order());
			if (strlen(seq1) == 0) break;

			fprintf(clusFile, "%6.6s %-100.100s\n", seq1, (char *) title(seq1));

			strcpy(word, "");
			while (1) {
				strcpy(word, doc(seq1,word).Order());
				if (strlen(word) == 0) break;

//---------------------------------------------------
//	show first non-stemmed version of the word
//---------------------------------------------------

				fprintf(clusTerms, "%s %s\n", cx, word); // stemmed word

				}

			ct(cx,seq1) = ""; // cluster , doc
			dca(seq1,cx) = ""; // doc , cluster
			}
		fprintf(clusFile, "\n"); // line between clusters
		}

	fclose(clusFile);
	fclose(clusTerms);

//--------------------------------------------------------------------------------
//	(cluster, word) tuples are instances of a word in a cluster doc.
//	these tuples are counted to give (count, cluster, word) tuples
//	showing the number of times a word from a doc is used in a cluster
//
//	count clusterNbr word
//--------------------------------------------------------------------------------

	system("sort -n < doc-clusters-terms.tmp | uniq -c | "
		"sort -k2 -n -k1 -n  >  doc-clusters-terms1.tmp");

	clusTerms = fopen("doc-clusters-terms1.tmp", "r");

	FILE *f1 = fopen("doc-clusters.tmp", "w");

	char clusterNbr[32];

	strcpy(clusterNbr, "");

	while (fgets(line, 16384,  clusTerms) != NULL) {

		char *clst, *count, *word;

		line[strlen(line)-1] = 0;

		count = strtok(line, " "); 
		clst = strtok(NULL, " ");
		word = strtok(NULL, " ");

//---------------------------------------------------------------------------
//	Don't show words that appear in cluster docs < clusMinWordCount
//---------------------------------------------------------------------------

		if (atoi(count) < clusMinWordCount) continue;

		if (strcmp(clusterNbr, clst) != 0) {
			if (strlen(clusterNbr) != 0) fprintf(f1, "\n");
			strcpy(clusterNbr, clst);
			fprintf(f1, "%s ", clusterNbr);
			}

		fprintf(f1, "%s ", word);

		if (clwgt(clst).Data() == 0) clwgt(clst)=atof(count)*idf(word);
		else clwgt(clst)=clwgt(clst)+atof(count)*idf(word);
		}

	fprintf(f1, "\n");

	fclose(f1);
	fclose(clusTerms);

	system("echo \"clusterNbr and clusterTerms\" > doc-clusters-terms.txt ");
	system("echo >> doc-clusters-terms.txt ");
	system("sort -n < doc-clusters.tmp >> doc-clusters-terms.txt");

	clusTerms = fopen("doc-clusters-per-document.tmp", "w");

	strcpy(docNbr, "");
	while(1) {

		strcpy(docNbr, dca(docNbr).Order());
		if (strlen(docNbr) == 0) break;

		int d = 0;
		strcpy(cx, "");
		while(1) {

			strcpy(cx,dca(docNbr,cx).Order());
			if (strlen(cx) == 0) break;
			d++; // cluster count per document
			}

		fprintf(clusTerms, "%d ",d);

		fprintf(clusTerms, "%6.6s %-100.100s\n", docNbr, (char *) title(docNbr));

		}

	fclose(clusTerms);

	system("echo \"number of clusters    document number      title\" "
		"> doc-clusters-per-doc.txt ");
	system("echo >> doc-clusters-per-doc.txt ");
	system("sort -nr < doc-clusters-per-document.tmp >> doc-clusters-per-doc.txt");

	file_size("doc-clusters-per-doc.txt");

	}
