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

void	term_term_cohesion(STATE *SP) {

#include "globals.h"

//-----------------------
//	term-term matrix
//	term cohesion
//-----------------------

	char line[8192], *p1, *p2, *p3, word[2048], word1[2048], docNbr[32];
	int ttMin;
	double minCohesion;

	ttMin = atoi(SP->min_tt_cooccur);
	minCohesion = .250;

	printf("\nBegin term-term-cohesion.cpp\n");
	printf("\tttMin = %d\n", ttMin);
	printf("\tminCohesion = %5.3f\n", minCohesion);

	FILE *ttTmp = fopen("tt-co-occur.tmp", "w");

	strcpy(docNbr, "");

	while(1) {
		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0 ) break;

		strcpy(word, "");
		while(1) {
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;

			strcpy(word1, word);
			while(1) {
				strcpy(word1, doc(docNbr,word1).Order());
				if (strlen(word1) == 0) break;
				if (strcmp(word,word1) == 0) continue;
				fprintf(ttTmp, "%s %s\n", word, word1); // terms co-occur
				}
			}
		}

	fclose(ttTmp);

	system("sort < tt-co-occur.tmp | uniq -c | sort -nr > tt-co-occur-sorted.txt");

	ttTmp = fopen("tt-cohesion.tmp", "w");

	FILE *ttTmpSorted = fopen("tt-co-occur-sorted.txt", "r");

	if (ttTmpSorted == NULL) {
		printf("*** Missing tt-co-occur-sorted.txt\n");
		abort();
		}

//------------------------------------------------------
//	term term cohesion
//	input format: #co-occur w1 w2
//------------------------------------------------------

	double cohes;
	int tttmp;
	char tmpNbr[32];

	while(fgets(line,16384,ttTmpSorted) != NULL) {

		char freq2[16], freq3[16];

		line[strlen(line) - 1] = 0;

		p1 = strtok(line, " "); // co-occur
		p2 = strtok(NULL, " "); // word1
		p3 = strtok(NULL, " "); // word2

		tttmp = atoi(p1);

		if (tttmp < ttMin) break; // input file is sorted

		cohes = tttmp / ((double) df(p2) + (double) df(p3) - tttmp);

		if (cohes < minCohesion) continue;

		sprintf(tmpNbr, "%5.3f", cohes);

		tt(p2,p3) = tmpNbr;
		tt(p3,p2) = tmpNbr;

		strcpy(freq2, df(p2)); // fprintf issue
		strcpy(freq3, df(p3));

		fprintf(ttTmp, "%s %s %s %s %s %s\n",
			tmpNbr, p2, p3, p1, freq2, freq3);

		}

	fclose(ttTmp);
	fclose(ttTmpSorted);

//-------------------------
//	sort by cohesion
//-------------------------

	system("sort -nr < tt-cohesion.tmp > tt-cohesion-sorted.txt");

	file_size("tt-cohesion-sorted.txt");

	}
