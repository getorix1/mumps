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

//	May 13, 2022

#include "MDH-interface.h"

#include "definitions.h"

int	main() {

#include "globals.h"

	mstring prs, dx, line, word, word1, word3, sdx, sword;
	struct STATE *SP;
	double cos;

	SP = (STATE *) malloc(sizeof(STATE));

	printf( "\nBegin medlineRetrieve searching\n\n") ;

	load_params(SP);
	
	query("").Kill();
        ans("").Kill();
        Tmp("").Kill();

	while(1) {

		if (!line.ReadLine(stdin)) break;
	
		for (int i = 1; ; i++) {

			word = line.Piece(" ",i);

			if (word.Length() == 0) break;

			word1 = Stem(word);

			if (!dict(word1).Data()) {
				cout << "unknown search term: " << word1 << endl;
				break;
				}

			cout << endl << "query term: " << word << endl;

			if (stem(word1).Data()) {

				cout << "    words in this stem group are: ";

				for (word3 = stem(word1,nul).Order(1);
					word3.Length();
					word3 = stem(word1,word3).Order(1)) 
					{

					cout << word3 << " ";

					}

				cout << endl;

				}

			cout << "    adding related phrase words: ";

			if (!phrase(word1).Data()) {
				cout << "none" << endl;
				}

			else {

				for (	prs = phrase(word1,nul).Order(1); 
					prs.Length(); 
					prs = phrase(word1,prs).Order(1)) {

					cout << prs << " ";
					query(prs) = 2;
					}

				cout << endl;
				}

			cout << "    words sounding similar to " << word << ": ";

			sdx = word.Soundex();

			if (!sndx(sdx).Data()) cout << " none" << endl;

			else {

				for (	sword = sndx(sdx,nul).Order(1);
					sword.Length(); 
					sword = sndx(sdx, sword).Order(1)) {

					cout << sword << " ";
					}

				cout << endl;
				}

			query(word1) = 3;

			dx = "";
			while(1) {
				dx = index(word1,dx).Order(1);
				if (dx.Length() == 0) break;
				Tmp(dx) = "";
				}
			}
		}
		

	pid_t px = getpid();
	char tmp[STR_MAX];
	sprintf(tmp, "%d.tmp", px);
	FILE *f1 = fopen(tmp, "w");

	int f = 0;

	for (	dx = Tmp(nul).Order(1); 
		dx.Length(); 
		dx = Tmp(dx).Order(1)) {

		cos = Cosine(doc(dx), query(nul));

		if ( cos > 0) {
			fprintf(f1, "%.3f %s\n", cos, dx.c_str());
			f++;
			}
		}

	fclose(f1);

	if (f == 0) return;

	char tmp2[1024];
	strcpy(tmp2, "sort -nr < ");
	strcat(tmp2, tmp);
	strcat(tmp2, "> sorted_");
	strcat(tmp2, tmp);

	system(tmp2);

	strcpy(tmp2, "sorted_");
	strcat(tmp2, tmp);

	f1 = fopen(tmp2, "r");

	cout << endl << f << " documents found" << endl << endl;
	cout << "   Doc  Rel   Title" << endl;

	for (int i = 0; i < 10; i++) {

		if (fgets(tmp, STR_MAX, f1) == NULL) break;

		tmp[strlen(tmp) - 1] = 0;

		char * p1 = strtok(tmp, " ");
		char * p2 = strtok(NULL, " ");

		printf("%6s %6s ", p2, p1);
		cout << title(p2).Extract(1,70) << endl;
		}

	printf("End...\n");
	GlobalClose; fflush(stdout); return 1;
	}

