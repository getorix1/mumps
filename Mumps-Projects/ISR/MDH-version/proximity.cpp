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

//	May 13, 2021

#include <mumpsc/libmpscpp.h>

#include "definitions.h"

void	proximity(STATE *SP) {

#include "globals.h"

	char w1[2048], w2[2048], k[2048], i[2048], j[2048], m[2048], n[2048];
	int ddx, p, proximity_min_count, prox_dist_factor;
	double xx;

	printf("\nBegin proximity.cpp\n");

	proximity_min_count = atoi(SP->proximity_min_count);
	prox_dist_factor = atoi(SP->prox_dist_factor);

	printf("\tprox_dist_factor = %d\n", prox_dist_factor );
	printf("\tproximity_min_count = %d\n", proximity_min_count);

//------------------------------------------------------------------
//	calculate term-term proximity coefficients within env words
//------------------------------------------------------------------

//----------------------------------------------------------------------
//	for each document k, sum the co-occurrences of words i and j
//----------------------------------------------------------------------

	strcpy(k, "");
	while(1) {
		strcpy(k, doc(k).Order());
		if(strlen(k) == 0) break;

//-------------------------------
//	for each term i in k
//-------------------------------

		strcpy(i, "");
		while(1) {
			strcpy(i, doc(k,i).Order());
			if (strlen(i) == 0) break;

//-------------------------------------
//	for each other term j in doc k
//-------------------------------------

			strcpy(j, i);
			while(1) {

				strcpy(j, doc(k,j).Order());
				if (strlen(j) == 0) break;

//---------------------------------------------
//	for each position m of term i in doc k
//---------------------------------------------

				strcpy(m, "");
				while(1) {
					strcpy(m, doc(k,i,m).Order());
					if (strlen(m) == 0) break;

//----------------------------------------------
//	for each position n of term j in doc k
//----------------------------------------------

				strcpy(n, "");
				while(1) {
					strcpy(n, doc(k,j,n).Order());
					if (strlen(n) == 0) break;
	
//----------------------------------------------------
//	calculate and store weight based on proximity
//----------------------------------------------------

					p = atoi(n) - atoi(m); // distance

					if (p < 6) { 

						xx = fabs ((double) p);
	
						ddx = (int) log(1.0/xx * prox_dist_factor + 1.0);

						if (ddx < 1) continue;

						if (ttp(i,j).Data()) {
							ttp(i,j)=ttp(i,j)+ddx; 
							ttp(i,j,1)=ttp(i,j,1)+p;
							}
						else {
							ttp(i,j)=ddx; 
							ttp(i,j,1)=p;
							}
						}
					}
				}
			}
		}
	}

	FILE *f1 = fopen("proximity.tmp", "w");

	strcpy(i, "");
	while(1) {
		strcpy(i, ttp(i).Order());
		if (strlen(i) == 0) break;
		strcpy(j, "");
		while(1) {
			strcpy(j, ttp(i,j).Order());
			if (strlen(j) == 0) break;
			if (atof(ttp(i,j)) < proximity_min_count) {
				ttp(i,j).Kill();
				continue;
				}

				strcpy(w1, stem(i,"").Order());
				strcpy(w2, stem(j,"").Order());

			fprintf(f1, "%s %s %s %d\n",
				(char *) ttp(i,j), 
				w1,
				w2,
				(int) ttp(i,j,1));
			}
		}

	fclose(f1);

	system("sort -rn < proximity.tmp > proximity.txt");

	file_size("proximity.txt");

	return;

	}
