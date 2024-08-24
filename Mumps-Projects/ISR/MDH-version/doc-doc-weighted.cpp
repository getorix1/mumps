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

void	doc_doc_weighted_matrix(STATE *SP) {

#include "globals.h"

//-----------------------------
//	doc-doc weighted matrix
//-----------------------------

	char line[8192], *p1, *p2, *p3, word[2048], docNbr[32], docNbr1[32];

	int min_doc_doc_cooccur = atoi(SP->min_doc_doc_cooccur);
	double min_dd_cohesion = atof(SP->min_dd_cohesion);

	printf("\nBegin doc-doc-weighted-matrix.cpp\n");
	printf("\tmin_doc_doc_cooccur = %d\n", min_doc_doc_cooccur);
	printf("\tmin_dd_cohesion = %5.3f\n", min_dd_cohesion);

	FILE *ddTmp = fopen("dd.tmp", "w");

	strcpy(word, "");
	while(1) {
		strcpy(word, index(word).Order());
		if (strlen(word) == 0) break;

		strcpy(docNbr, "");
		while(1) { // doc1 on word 'word'
			strcpy(docNbr,index(word,docNbr).Order());
			if (strlen(docNbr) == 0) break;

			strcpy(docNbr1, docNbr);
			while(1) { // doc2 on word 'word'
				strcpy(docNbr1, index(word,docNbr1).Order());
				if (strlen(docNbr1) == 0) break;
				if (strcmp(docNbr,docNbr1) == 0) continue;
				fprintf(ddTmp, "%s %s\n", docNbr, docNbr1);
				}
			}
		}

	fclose(ddTmp);

	printf("\tsort dd.tmp\n");

	system("sort < dd.tmp | uniq -c | sort -rn > dd1.tmp");

//----------------------------------------------
//	format: nbr words in common, doc1, doc2
//----------------------------------------------
	
	ddTmp = fopen("dd1.tmp", "r");

	FILE *cosTmp = fopen("doc-doc-matrix-cohesion.tmp", "w");

	double cos;

	while(fgets(line,16384,ddTmp) != NULL) {
	
		char tmp[32];

		line[strlen(line) - 1] = 0;

		p1 = strtok(line, " "); // count

		if (atoi(p1) <  min_doc_doc_cooccur) break;

		p2 = strtok(NULL, " "); // doc1
		p3 = strtok(NULL, " "); // doc2

		if (strcmp(p2,p3) == 0) continue;

//		cos = Cosine(doc(p2),doc(p3));
//		if (cos < SP->cosineMin) continue;
//		sprintf(tmp,"%5.3f",cos);

		strcpy(tmp, p1);

		int d1count = 0;
		strcpy(docNbr, p2);
		strcpy(word, "");
		while(1) {
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;
			d1count ++;
			}

		int d2count = 0;
		strcpy(docNbr, p3);
		strcpy(word, "");
		while(1) {
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;
			d2count ++;
			}

		double coh = atof(p1) / (d1count + d2count - atof(p1));

		if (coh < min_dd_cohesion) continue;

		sprintf(tmp, "%5.3f", coh);

		dd(p2,p3) = tmp;

		fprintf(cosTmp, "%s %s %s\n", tmp, p2, p3);
		}

	fclose(ddTmp);
	fclose(cosTmp);

	printf("\tsort Doc-Doc-Matrix.tmp\n");

	system("sort -nr < doc-doc-matrix-cohesion.tmp > doc-doc-matrix-cohesion.txt");

	file_size("doc-doc-matrix-cohesion.txt");

	}

