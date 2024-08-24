
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

//	April 1, 2021

#include "definitions.h"

void	write_doc_term(STATE *SP) {

#include "globals.h"

	char docNbr[32], word[2048];

//------------------------------------------------------
//	write document term matirx
//------------------------------------------------------

	printf("Begin write-doc-term.cpp\n");

	FILE *docTerm = fopen("doc-term-matrix-unweighted.txt", "w");

	strcpy(docNbr, "");
	while(1) {
		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;
		fprintf(docTerm, "Doc %s: ", docNbr);
		strcpy(word, "");
		while (1) {
			strcpy(word, doc(docNbr,word).Order());	
			if (strlen(word) == 0) break;
			fprintf(docTerm, "%s(%s) ", word, (char *) doc(docNbr,word));
			}
		fprintf(docTerm, "\n\n");
		}

	fclose(docTerm);
	}
