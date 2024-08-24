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

void	prune_doc(STATE *SP) {

#include "globals.h"

	char docNbr[32], word[2048];

//----------------------------------------------------------
//	delete words from index(word,doc) & doc(doc,word) 
//----------------------------------------------------------

	strcpy(docNbr, "");
	while(1) { 
		strcpy(docNbr, doc(docNbr).Order());
		if (strlen(docNbr) == 0) break;

		strcpy(word, "");
		while(1) {
			strcpy(word, doc(docNbr,word).Order());
			if (strlen(word) == 0) break;

			if (!df(word).Data()) {
				doc(docNbr,word).Kill();
				}
			}
		}
	}
