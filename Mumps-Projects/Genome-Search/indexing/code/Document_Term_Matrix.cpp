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

//	May 8, 2023

#include "definitions.h"

void	doc_term_matrix(STATE *SP) {

#include "globals.h"

	int i, j, k;

	int wmin_kill = 0;

	long max_docs = atol(SP->max_docs);

	printf("\nBegin doc-term-matrix.cpp\n");
	printf("\tmax_docs %d\n", max_docs);

	doc("").Kill();

	int wmin = atoi(SP->min_doc_vector_word_count);
	int mindf = atoi(SP->min_doc_word_count);

	cout << "\tDelete Docs with fewer than " << wmin << " words\n";
	cout << "\tDelete Words in fewer than " << mindf << " Docs\n";

	char stemmed[2048];
	sprintf(stemmed, "%s.stemmed", SP->DB);

	ifstream in(stemmed, ios_base::in);

 	ofstream f2("doc.tmp", ios_base::out);

	mstring line, off, docx, w;
	char *work, *p_count, *p_w, *p_off, *p_docx, *p1, *save;

	while (1) {

		if (!line.ReadLine(in)) break;	// db.stemmed

		save = NULL;

		work = line.c_str();

		p_off = strtok_r(work, " ", &save);	// offset
		p_docx = strtok_r(NULL, " ", &save);	// doc number
		
		doc(p_docx) = p_off;

		for (int i = 3; ; i++) {
			p_w = strtok_r(NULL, " ", &save);
			if (p_w == NULL) break;
			if (idf(p_w).Data() == 0) continue;
			f2 << p_docx << " " << p_w << endl;	// doc.tmp
			}
		}

	f2.close();
	in.close();

//-----------------------------------
//	unique occurences with count 
//-----------------------------------

	system("sort < doc.tmp | uniq -c > doc.sorted.tmp");

	in.open("doc.sorted.tmp", ios_base::in);

	char *pa1, *pa2, *pa3;
	mstring a1, a2, a3;

	while(1) {
		if (!line.ReadLine(in)) break;

		strcpy(work, line.c_str());
		for (p1 = work; *p1 == ' '; p1++);

		pa1 = strtok(p1, " ");		// count
		pa2 = strtok(NULL, " ");	// doc
		pa3 = strtok(NULL, " ");	// word
		doc(pa2, pa3) = pa1;
		index(pa3, pa2) = pa1;
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
			w = "";
			while(1) {
				w = doc(docx,w).Order(1);
				if (w == "") break;
				index(w,docx).Kill();
				}
			wmin_kill ++;
			doc(docx).Kill();
			}
		}


	docx = "";
	i = 0;
	while(1) {
		docx = doc(docx).Order(1);
		if (docx == "") break;
		i++;
		}


	printf("	Revised doc count %d\n",i);

	SP->docCount = i;

	DocCount(1) = i;

	cout << "\tDocs deleted for having too few words: " << wmin_kill << endl;

	return;
	}
