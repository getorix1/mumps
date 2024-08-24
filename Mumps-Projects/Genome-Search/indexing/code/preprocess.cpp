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

//      May 4, 2023

#include "definitions.h"

void    preprocess(STATE *SP) {

#include "globals.h"

	int i, D, min;
	char tmp[STR_MAX];
	mstring line, w, ws, tmps;

	off_t off;

        ofstream f2("wordlist.tmp");

	strcpy(tmp, SP->DB);
	strcat(tmp, ".converted");
        ifstream in(tmp, ios_base::in);
	printf("	input %s\n",tmp);

	strcpy(tmp, SP->DB);
	strcat(tmp, ".stemmed");
	FILE *out = fopen(tmp,"w");
	printf("	output %s\n",tmp);

	D = 0;
	min = atoi(SP->min_word_length);

	printf("	Min word length %d\n",min);

	char *work, *p1, *p2, *p3, *save;

        while (1) {

                if (!line.ReadLine(in)) break;
		work = line.c_str();
		save = NULL;
		p1 = strtok_r(work, " ", &save);
		p1 = strtok_r(NULL, " ", &save);
		D = atol(p1);
		off = ftello(out);
		fprintf(out, "%ld %ld", off, D);

		for (i=3; ; i++) {
			p1 = strtok_r(NULL, " ", &save);
			if (p1 == NULL) break;
			if (strlen(p1) < min) continue;
			fprintf(out, " %s", p1);
			f2 << p1 << endl;
			}

		fprintf(out,"\n");
		}

	f2.close();
	fclose(out);
	in.close();

	system("sort < wordlist.tmp | uniq -c > stemmed-vocabulary.tmp");

        cout << "\tNumber unique words: "; file_size1("stemmed-vocabulary.tmp");
        file_size("db.stemmed");

	return;
	}
