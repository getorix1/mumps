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

void	idf_weights(STATE *SP) {

#include "globals.h"

//-------------------
//	IDF weights
//-------------------

	double dfx, avg;
	int doc_count = SP->docCount;
	int min_doc_count;
	char docNbr[32], tmp1[16], tmp2[16], word[2048];
	int d = 0, i;

	printf("\nBegin idf-weights.cpp\n");

	min_doc_count = atoi(SP->min_doc_word_count);

	printf("\tinitial IDF doc count = %d\n", doc_count);
	printf("\tMin IDF doc word count = %d\n", min_doc_count);

	FILE *f2 = fopen("idf-docfreq-totfreq-word.tmp", "w");

	strcpy(word, "");
	i = 0; avg = 0;

	while(1) {
		strcpy(word, df(word).Order());
		if (strlen(word) == 0) break;

		strcpy(tmp1, df(word)); // fprintf issue
		strcpy(tmp2, dict(word));

		fprintf(f2 , "%s %s %s\n",
			tmp1, tmp2, word); // docFreq, Dict, Word

		dfx = (double) df(word);
		dfx = log2(doc_count / dfx);
		idf(word) = dfx;
		avg = avg + dfx;
		i++;
		}

	fclose(f2);

	avg = avg / i / 2;
	printf("\tThreshold = %.3f\n", avg);

	FILE *f1 = fopen("idf-weights.tmp", "w");

	strcpy(word, "");

	while(1) {

		strcpy(word, idf(word).Order());

		if (strlen(word) == 0) break;

		if ( idf(word) > avg) {
			idf(word) = idf(word).Justify(5,3);
			fprintf(f1, "%s %s\n", (char *) idf(word), word);
			}

		else {
			idf(word).Kill();	
			dict(word).Kill();
			df(word).Kill();	
			index(word).Kill();
			}
		}
	
	fclose(f1);

	system("sort -n < idf-weights.tmp > idf-weights-sorted.txt");

	file_size("idf-weights-sorted.txt");

	bash_command("echo \"Low  IDF: `head -1 idf-weights-sorted.txt`\"");
	bash_command("echo \"High IDF: `tail -1 idf-weights-sorted.txt`\"");


	system("sort -k 1 -nr -k 2 -nr < idf-docfreq-totfreq-word.tmp > "
		"idf-docfreq-totfreq-word.txt");

	FILE *idfDelete = fopen("idf-deleted-words.txt", "w");

	strcpy(docNbr, "");

	while(1) { 

		strcpy(docNbr, doc(docNbr).Order(1));

		if (strlen(docNbr) == 0) break;

		strcpy(word, "");

		while(1) {

			strcpy(word, doc(docNbr,word).Order(1));
			if (strlen(word) == 0) break;

			if (idf(word).Data() == 0) {
				doc(docNbr,word).Kill();
				fprintf(idfDelete, "IDF delete %s in %s\n", word, docNbr);
				}
			}
		}

	strcpy(docNbr, ""); 

	while(1) {

		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;

		strcpy(word, "");
		strcpy(word, doc(docNbr,word).Order());

		if (strlen(word) == 0) {
			doc(docNbr).Kill();
			fprintf(idfDelete, "Delete doc %s\n", docNbr);
			}
		}

	fclose(idfDelete);

	file_size("idf-deleted-words.txt");

	d = 0;

	strcpy(docNbr, "");

	while(1) {
		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;
		d++;
		}

	DocCount(1) = SP->docCount = d;

	printf("\tNew number of documents after IDF: %d\n", d);	
	}
