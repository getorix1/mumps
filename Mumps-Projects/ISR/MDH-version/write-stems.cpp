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

void	write_stems(STATE *SP) {

#include "globals.h"

	char word1[2048], word2[2048];;

//------------------------------------------------------
//	write stem dictionary
//------------------------------------------------------

	printf("\nBegin write-stems.cpp\n");

	FILE *Stems = fopen("stems.txt", "w");

	strcpy(word1, "");
	while(1) {
		strcpy(word1, stem(word1).Order());
		if (strlen(word1) == 0) break;

		fprintf(Stems,"%s  ", word1);

		strcpy(word2, "");
		while(1) {
			strcpy(word2, stem(word1,word2).Order());
			if (strlen(word2) == 0) break;

			fprintf(Stems, "%s ", word2);
			}
		fprintf(Stems, "\n\n");
		}

	fclose(Stems);

	file_size("stems.txt");
	}
