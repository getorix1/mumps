/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Information Storage and Retrieval Kit
#+
#+     Copyright (C) 2021, 2023 by Kevin C. O'Kane
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

//	April 14, 2023

#include "definitions.h"

void	idf_weights(STATE *SP) {

#include "globals.h"

//-------------------
//	IDF weights
//-------------------

	double dfx, avg;
	int doc_count = SP->docCount;
	char docNbr[32], tmp1[16], tmp2[16], word[2048];
	int d = 0, i;

	printf("\nBegin idf-weights.cpp\n");

	printf("\tinitial IDF doc count = %d\n", doc_count);

	FILE *f2 = fopen("idf-weights-sorted.dat", "r");

	char *pidf, *pword;
	
	while (fgets(word, 2048, f2) != NULL) {

		word[strlen(word)-1] = 0;
		pidf = strtok(word, " ");
		pword = strtok(NULL, " ");
		idf(pword) = pidf;
		}

	fclose (f2);

	}
