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

//	June 10, 2024

#include "definitions.h"

void	extract_titles(STATE *SP) {

#include "globals.h"

	char line[STR_MAX];

	off_t filePtr, fpTmp;

	printf("\nBegin extract-titles.cpp\n");

//-----------------------
//	Extract titles
//-----------------------
	
	FILE *mainInput = fopen("ohsu.medline", "r");
	FILE *Titles = fopen("titles.list", "w");

	int d = 0;

	while(fgets(line,STR_MAX, mainInput) != NULL) {

		line[strlen(line) - 1] = 0;

		fpTmp = ftello(mainInput);

		if (strncmp(line,"TI",2) == 0) {
			d++;
			if (d > SP->max_docs) break;
			fprintf(Titles, "%s\n", &line[6]);
			title(d) = (char *) &line[6];
			continue;
			}

		if (strncmp(line,"MH",2) == 0) continue; 

		if (strncmp(line,"AB",2) == 0) continue; 

		if (strncmp(line,"STAT- MEDLINE",13) == 0) {
			filePtr = fpTmp;
			continue;
			}
		}

	fclose(Titles);
	fclose(mainInput);

	}
