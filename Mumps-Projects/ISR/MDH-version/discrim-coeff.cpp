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

//	May 4, 2022

#include "definitions.h"

void	discrim_coef(STATE *SP) {

#include "globals.h"

//--------------------
//	discrim coeff
//--------------------

	printf("Discrimination coefficients\n");

	int D, d = 0;
	double r, sq = 0.0, ddx, xx, y, T, T1, T2;
	char *p1, *p2, line[8192], word[2048], word1[2048], docNbr[32];
	FILE *discrimIn;

	FILE *Discrim = fopen("discrimination-coeff.tmp", "w");

	D = DocCount(1);  // number of documents

//---------------------------------------------------------------------------------
// 	Calculate centroid vector ^c() for entire collection and
//	the sum of the squares (needed in cos calc but should only be done once)
//---------------------------------------------------------------------------------

	strcpy(word, "");
	while(1) {
		strcpy(word, dict(word).Order());
		if (strlen(word) == 0) break;

		xx = (double) dict(word) / D;
		c(word) = xx ;
		sq = xx * xx + sq;
		}

//----------------------------------------------------------------------------------
//	Calculate total similarity of doc for all words (T) space by
//	calculating the sum of the similarities of each document with the centroid.
//	Remember and store contribution of each document in ^dc(dn).
//----------------------------------------------------------------------------------

	T = 0.0;

	strcpy(docNbr, "");
	while(1) {

		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;

		xx = 0.0;
		y = 0.0;

		strcpy(word, "");
		while(1) {
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;

			ddx = doc(docNbr,word);
			xx = ddx * c(word) + xx;
			y = ddx * ddx + y;
			}

//---------------------------------------------------------------------------------
//	Calculate and store the cos(c,doc(i)).
//	Remember in ^dc(i) the contribution that this document made to the total.
//---------------------------------------------------------------------------------

		if (y == 0.0) {	
			dc(docNbr)=0.0;
			continue;
			}
		dc(docNbr) = (double) xx / sqrt(sq * y);	// cos(c,doc(i))
		T = dc(docNbr) + T;		// sum of the cosines
		}

//-----------------------------------------------------------
//	calculate similarity of doc space with words removed
//-----------------------------------------------------------

	strcpy(word, "");
	while(1) {
		strcpy(word, dict(word).Order());
		if (strlen(word) == 0) break;

//----------------------------------------------------------------------------------
//	For each document containing W, calculate sum of the contribution
//	of the cosines of these documents to the total (T).  ^dc(i) is
//	the original contribution of doc i.  Sum of contributions is stored in T1.
//----------------------------------------------------------------------------------

		T1 = 0.0;
		T2 = 0.0;

		strcpy(docNbr, "");
		while(1) {	// row of docNbrs for word
			strcpy(docNbr, index(word,docNbr).Order());
			if (strlen(docNbr) == 0) break;
			T1 = dc(docNbr) + T1;	// previously calculated cos


//-----------------------------------------------------------------------------
//	For each word in document i, recalculate cos(c,doc) but without word W
//-----------------------------------------------------------------------------
	
			x = 0.0; y = 0.0;

			strcpy(word1, "");
			while (1) {
				strcpy(word1, doc(docNbr,word1).Order()); 
				if (strlen(word1) == 0) break;

				if (strcmp(word, word1) != 0) {
					d = (int) doc(docNbr,word1);
					x = d * (double) c(word1) + x;
					y = d * d + y;
					}
				}

			if (y == 0.0) continue;
			T2 = x / sqrt(sq * y) + T2;	// sum of cosines without word
			}

//----------------------------------------------------------------------
//	subtract original contribution with W (T1) and add contribution
//	without W (T2) and calculate r - the change, and store in ^mca(W)
//----------------------------------------------------------------------

		r = ((T2 - T1) * 10000.0); 

		fprintf(Discrim, "%d %s \n", (int) r, word);
//		printf("%d %s \n", (int)r, word);
		}

	fclose(Discrim);

	system("sort -n < discrimination-coeff.tmp > Discrimination-Coefficients.txt");

	discrimIn = fopen("Discrimination-Coefficients.txt", "r");

	if (discrimIn == NULL) {
		printf("Missing discrimination-coeff.txt in discrim-coeff.cpp\n");
		abort();
		}

	while(fgets(line,16384,discrimIn) != NULL) {

		line[strlen(line) - 1] = 0;

		p1 = strtok(line, " ");
		p2 = strtok(NULL, " ");

		discrim(p1) = p1;

		}

	fclose(discrimIn);
	}
