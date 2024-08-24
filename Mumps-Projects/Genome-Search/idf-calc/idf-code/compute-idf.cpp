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

//	May 3, 2023

#include "definitions.h"

void	compute_idf(STATE *SP) {

#include "globals.h"

	int i, j, k;

	int wmin_kill = 0;

	long max_docs = atol(SP->max_docs);

	printf("\nBegin doc-term-matrix.cpp\n");
	printf("\tmax_docs %d\n", max_docs);

	idf("").Kill();

	int wmin = atoi(SP->min_doc_vector_word_count);
	int mindf = atoi(SP->min_doc_word_count);
	int min_len = atoi(SP->min_word_length);

	cout << "\tDelete Docs with fewer than " << wmin << " words\n";
	cout << "\tDelete Words in fewer than " << mindf << " Docs\n";

        char stemmed[2048];
        sprintf(stemmed, "%s.converted", SP->DB);

        ifstream in(stemmed, ios_base::in);

 	ofstream f2("doc.tmp", ios_base::out);

	mstring line, off, docx, w;
	char *work, *save,  *p_count, *p_w, *p_off, *p_docx, *p1;
	long doc_count = 0;

	while (1) {

		if (!line.ReadLine(in)) break;

                work = line.c_str();
                save = NULL;

		p_off = strtok_r(work, " ", &save);
		p_docx = strtok_r(NULL, " ", &save);

		doc_count++;
		
		doc(p_docx) = p_off;

		for (int i = 3; ; i++) {
			p_w = strtok_r(NULL, " ", &save);
			if (p_w == NULL) break;
			if (strlen(p_w) < min_len) continue;
			f2 << p_w << " " << p_docx << endl;
			}
		}

	f2.close();
	in.close();

//-----------------------------------
//	unique occurences with count 
//-----------------------------------

	system("sort < doc.tmp | uniq > doc-sorted.tmp");

	system("cut -d' ' -f1-1 < doc-sorted.tmp | uniq -c | sort -nr > df.sorted.tmp");

	in.open("df.sorted.tmp", ios_base::in);

	double dfx, avg = 0.0;
	long ii = 0;
	char *pa1, *pa2;

	while(1) {
		if (!line.ReadLine(in)) break;

                work = line.c_str();
                save = NULL;

		for (p1 = work; *p1 != ' '; p1++);

		pa1 = strtok_r(p1, " ", &save);		// count
		if (atoi(pa1) < mindf) break;
		pa2 = strtok_r(NULL, " ", &save);	// word

		dfx = atof(pa1);

		dfx = log2(doc_count / dfx);

		idf(pa2) = dfx;

		avg = avg + dfx;
		ii++;
		}

	in.close();

	avg = avg / ii;

	printf("\tAvg = %.3f\n", avg);

	double threshold = avg / 2;
	printf("\tAvg= %.3f Threshold = %.3f\n", avg, threshold);

	FILE *fx1 = fopen("idf-weights.tmp", "w");

	char word[4096];

	strcpy(word, "");

	mstring idftmp;

	while(1) {

		strcpy(word, idf(word).Order());

		if (strlen(word) == 0) break;

		if ( idf(word) > 0) {
			idftmp = idf(word).Justify(5,3);
			fprintf(fx1, "%s %s\n", (char *) idftmp.c_str(), word);
			}
		}
	
	fclose(fx1);

	system("sort -n < idf-weights.tmp > idf-weights-sorted.dat");

	file_size("idf-weights-sorted.dat");

	bash_command("echo \"Low  IDF: `head -1 idf-weights-sorted.dat`\"");
	bash_command("echo \"High IDF: `tail -1 idf-weights-sorted.dat`\"");

	return;
	}

