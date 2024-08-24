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

//	May 2, 2023

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
	char work[16000], *p_count, *p_w, *p_off, *p_docx, *p1;

	while (1) {

		if (!line.ReadLine(in)) break;

		strcpy(work, line.c_str());

		p_off = strtok(work, " ");
		p_docx = strtok(NULL, " ");
		
		doc(p_docx) = p_off;

		for (int i = 3; ; i++) {
			p_w = strtok(NULL, " ");
			if (p_w == NULL) break;
			if (strlen(p_w) < min_len) continue;
//			f2 << p_docx << " " << p_w << endl;
			f2 << p_w << " " << p_docx << endl;
			}
		}

	f2.close();
	in.close();

//-----------------------------------
//	unique occurences with count 
//-----------------------------------

	system("sort < doc.tmp | uniq > doc-sorted.tmp");

	system("cut -d' ' -f1-1 < doc-sorted.tmp | uniq -c | sort -nr > doc-counted.tmp");

return;


	system("sort < doc.tmp | uniq -c > doc.sorted.tmp");

	in.open("doc.sorted.tmp", ios_base::in);

	char *pa1, *pa2, *pa3;
	mstring a1, a2, a3;

	while(1) {
		if (!line.ReadLine(in)) break;

		strcpy(work, line.c_str());
		for (p1 = work; *p1 != ' '; p1++);

		pa1 = strtok(p1, " ");		// count
		pa2 = strtok(NULL, " ");	// doc
		pa3 = strtok(NULL, " ");	// word
		doc(pa2, pa3) = pa1;
		}

	in.close();


	for (	docx = doc(nul).Order(1);
		docx.Length();
		docx = doc(docx).Order(1)) 
		{

		i = 0;
		w = "";
		while(1) {
			w = doc(docx,w).Order(1);
			if (w == "") break;
			i++;
			}
		if (i < wmin) {
			wmin_kill ++;
			doc(docx).Kill();
			}
		}

	long doc_count = 0;

	for (	docx = doc(nul).Order(1);
		docx.Length();
		docx = doc(docx).Order(1)) 

		doc_count++;

	system ("sort < doc.tmp | uniq | cut -d \" \" -f 2 | sort | uniq -c > df.sorted.tmp");

	in.open("df.sorted.tmp", ios_base::in);

	double dfx, avg;
	long ii = 0;

	while(1) {
		if (!line.ReadLine(in)) break;

		strcpy(work, line.c_str());
		for (p1 = work; *p1 != ' '; p1++);

		pa1 = strtok(p1, " ");		// count
		if (atoi(pa1) < mindf) break;
		pa2 = strtok(NULL, " ");	// word

		dfx = atof(pa1);

		dfx = log2(doc_count / dfx);

		idf(pa2) = dfx;

		avg = avg + dfx;
		ii++;
		}

	in.close();

	printf("\tAvg = %.3f\n", avg/ii);
	avg = avg / ii / 2;
	printf("\tThreshold = %.3f\n", avg);

	FILE *fx1 = fopen("idf-weights.tmp", "w");

	char word[4096];

	strcpy(word, "");

	mstring idftmp;

	while(1) {

		strcpy(word, idf(word).Order());

		if (strlen(word) == 0) break;

		if ( idf(word) > avg) {
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

