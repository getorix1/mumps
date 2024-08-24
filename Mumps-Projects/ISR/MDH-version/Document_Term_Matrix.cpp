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

//	June 11, 2024

#include "definitions.h"

void	doc_term_matrix(STATE *SP) {

#include "globals.h"

	int i, j, k;

	printf("\nBegin doc-term-matrix.cpp\n");

	df("").Kill();
	dict("").Kill();
	doc("").Kill();

	int wmin = atoi(SP->min_doc_vector_word_count);
	int mindf = atoi(SP->min_doc_word_count);

	StopInit("stop.words");

	char stemmed[2048];
	sprintf(stemmed, "%s.stemmed", SP->DB);

	ifstream in(stemmed, ios_base::in);

	ofstream f1("dict.tmp", ios_base::out);
 	ofstream f2("doc.tmp", ios_base::out);

	mstring line, off, docx, w;

	while (1) {

		if (!line.ReadLine(in)) break;

		off = line.Piece(" ", 1);
		docx = line.Piece(" ", 2);
		doc(docx) = off;

		for (int i = 3; ; i++) {
			w = line.Piece(" ", i);
			if (w.Length() == 0) break;
			if (StopLookup(w)) continue;
			doc(docx, w, i) = "";
			f2 << docx << " " << w << endl;
			f1 << w << endl;
			}
		}

	f1.close();
	f2.close();
	in.close();

	system("sort < dict.tmp | uniq -c > dict.sorted.tmp");

	in.open("dict.sorted.tmp", ios_base::in);

	while(1) {
		if (!line.ReadLine(in)) break;
		line = line.zBlanks();
		dict(line.Piece(" ",2)) = line.Piece(" ",1);
		}

	in.close();

	system("sort < doc.tmp | uniq -c > doc.sorted.tmp");

	in.open("doc.sorted.tmp", ios_base::in);

	mstring a1, a2, a3;

	while(1) {
		if (!line.ReadLine(in)) break;
		line = line.zBlanks();
		a1 = line.Piece(" ",1);
		a2 = line.Piece(" ",2);
		a3 = line.Piece(" ",3);
		doc(a2, a3) = a1;
		index(a2, a2) = a1;
		}

	in.close();

	f1.open("df.tmp", ios_base::out);


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
			doc(docx).Kill();
			}
		else {
			w = "";
			while(1) {
				w = doc(docx,w).Order(1);
				if (w == "") break;
				f1 << w << endl;
				}
			}
		}

	f1.close();

	system("sort < df.tmp | uniq -c | sort -nr > df.sorted.tmp");

	in.open("df.sorted.tmp", ios_base::in);

	while(1) {
		if (!line.ReadLine(in)) break;
		line = line.zBlanks();
		a1 = line.Piece(" ",1);
		if (a1 < mindf) break;
		df(line.Piece(" ",2)) = a1;
		}

	in.close();

	w = "";
	while(1) {
		w = dict(w).Order(1);
		if (w == "") break;
		if (!df(w).Data()) dict(w).Kill();
		}

	docx = "";
	i = 0;
	while(1) {
		docx = doc(docx).Order(1);
		if (docx == "") break;
		w = "";
		while(1) {
			w = doc(docx,w).Order(1);
			if (w == "" ) break;
			if (!dict(w).Data()) {
				doc(docx,w).Kill();
				continue;
				}
			i++;
			}
		if (i < wmin) {
			w = "";
			while(1) {
				w = doc(docx,w).Order(1);
				if (w == "" ) break;
				index(w,docx).Kill();
				}
			doc(docx).Kill();
			}
		}


	ofstream f3("document-term-matrix.txt");

	docx = "";
	i = 0;
	while(1) {
		docx = doc(docx).Order(1);
		if (docx == "") break;
		i++;
		f3 << "Doc " << docx << ": " << title(docx) << endl;
		w = "";
		while(1) {
			w = doc(docx,w).Order(1);
			if (w == "" ) break;
			f3 << w << " (" << doc(docx,w) << ") ";
			}
		f3 << endl;
		}

	f3.close();

	printf("	Revised doc count %d\n",i);

	SP->docCount = i;

	DocCount(1) = i;

	f3.open("term-frequencies.unsorted.tmp", ios_base::out);

	w = "";
	while(1) {
		w = df(w).Order(1);
		if (w == "") break;
		f3 << df(w) << " " << dict(w) << " " << w << endl;
		}

	f3.close();

	system("sort -nr < term-frequencies.unsorted.tmp > term-frequencies.txt");

        file_size("document-term-matrix.txt");
        file_size("dict.tmp");
        file_size("doc.tmp");
        file_size("dict.sorted.tmp");
        file_size("doc.sorted.tmp");
        file_size("df.tmp");
        file_size("df.sorted.tmp");
        file_size("term-frequencies.unsorted.tmp");
        file_size("term-frequencies.txt");

	return;
	}
