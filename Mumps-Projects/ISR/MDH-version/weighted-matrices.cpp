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

//	May 11, 2022

#include "definitions.h"

void	weighted_matrices(STATE *SP) {

#include "globals.h"

//--------------------------------------
//	weighted doc and index matrices
//--------------------------------------

	double wgt;
	int lineCount = 0, i, deldocs = 0, delwords = 0;
	char docNbr[32], word[2048], tmp[32];

	double min_doc_weight = atof(SP->min_doc_weight);
	int min_doc_word_count = atoi(SP->min_doc_word_count);
	delwords = 0;

	printf("\nBegin weighted-matrices.cpp\n");

	FILE *dtWgted = fopen("doc-term-idf-weighted-matrix.txt", "w");
	FILE *tdWgted = fopen("term-doc-idf-weighted-matrix.txt", "w");

	strcpy(docNbr, "");

	while (1) { // for each doc
		int lineCount;

		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;

		fprintf(dtWgted, "\n\nDoc=%s ", (char *) docNbr);
		fprintf(dtWgted, "%s\n", (char *) title(docNbr));

		lineCount = 0;
		strcpy(word, "");

		while(1) { // for each word
			char tmp[16];
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;

			wgt = idf(word) * doc(docNbr,word);

			if (wgt < min_doc_weight) {
				delwords++; 
				doc(docNbr,word).Kill();
				continue;
				}

			sprintf(tmp, "%-.3f", wgt);

			doc(docNbr,word) = tmp;
			index(word,docNbr) = tmp;

			fprintf(dtWgted, "%s(%s) ", word, tmp);
			lineCount++;

			if (lineCount > 5) {
				fprintf(dtWgted, "\n");
				lineCount = 0;
				}
			}

		if (! lineCount) fprintf(dtWgted, "\n");
		}

	strcpy(docNbr, "");
	while (1) { // for each doc

		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;

		i = 0;
		strcpy(word, "");
		while(1) { // for each word
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;
			i++;
			}

		if (i < min_doc_word_count) {
			doc(docNbr).Kill();
			deldocs++;
			}
		}

	strcpy(word, "");
	while(1) { // print t-d matrix

		strcpy(word, index(word).Order());
		if (strlen(word) == 0) break;

		fprintf(tdWgted, "term=%s ", (char *) word);

		strcpy(docNbr, "");
		while(1) {
			strcpy(docNbr, index(word,docNbr).Order());
			if (strlen(docNbr) == 0) break;

			fprintf(tdWgted, "%s(%s) ", (char *) docNbr, 
				(char *) index(word,docNbr));
			}

		fprintf(tdWgted, "\n");
		}

	fprintf(dtWgted, "\n");

	fclose(dtWgted);
	fclose(tdWgted);

	file_size("doc-term-idf-weighted-matrix.txt");
	file_size("term-doc-idf-weighted-matrix.txt");
	printf("	words deleted: %d\n", delwords);
	printf("	docs deleted:  %d\n", deldocs);

	}
