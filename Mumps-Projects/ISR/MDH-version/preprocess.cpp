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

//      May 8, 2022

#include "definitions.h"

void    preprocess(STATE *SP) {

#include "globals.h"

	int i, M, D, min;
	char tmp[STR_MAX];
	mstring line, w, ws;

	M = atoi(SP->max_docs);

	printf("\nBegin preprocess %d documents\n", M );

	StopInit("basic.stop.words");

        ofstream f1("stem-words.tmp");
        ofstream f2("wordlist.tmp");

	strcpy(tmp, SP->DB);
	strcat(tmp, ".converted");
        ifstream in(tmp, ios_base::in);
	printf("	input %s\n",tmp);

	strcpy(tmp, SP->DB);
	strcat(tmp, ".stemmed");
        ofstream out(tmp);
	printf("	output %s\n",tmp);

	D = 0;
	min = atoi(SP->min_word_length);

	printf("	Min word length %d\n",min);

        while (1) {

                if (!line.ReadLine(in)) break;
		D = line.Piece(" ",2);
//		if (D > M) break;
		out << line.Piece(" ", 1, 2);

		line = line.Translate("0123456789\"`~!@#$%^&*()_+=?>.<,:;'{[}]");

		line = line.zBlanks();
		for (i=3; ; i++) {
			w = line.Piece(" ",i);
			if (w.Length() == 0) break;
			if (w.Length() < min) continue;
			if (w.Extract(1,1) == "-") w = w.Extract(2,99);
			if (StopLookup(w)) continue;
			ws = Stem(w);
			stem(ws,w) = "";
			out << " " << ws;
			f2 << w << endl;
			f1 << ws << endl;
			}
		out << endl;
		}

	f1.close();
	f2.close();
	out.close();
	in.close();

	system("sort < stem-words.tmp | uniq -c > stemmed-vocabulary.tmp");

        file_size("wordlist.tmp");
        file_size("ohsu.stemmed");
        file_size("stem-words.tmp");

	return;
	}
