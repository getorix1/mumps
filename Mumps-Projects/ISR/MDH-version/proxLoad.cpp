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

//	May 10, 2021

#include <mumpsc/libmpscpp.h>

#include "definitions.h"

void	proxLoad(STATE *SP) {

#include "globals.h"

	mstring line, wgt, w1, w2, d, prs, w1stem, w2stem;
	double s;
	int dir;

	cout << endl << "Begin proxLoad" << endl;

	ofstream f1("phrases.txt", ios_base::out);
	ifstream in("proximity.txt", ios_base::in);

	while(1) {
		if (!line.ReadLine(in)) break;
		wgt = line.Piece(" ",1);
		w1 = line.Piece(" ",2);
		w2 = line.Piece(" ",3);
		dir = line.Piece(" ",4);
		s = log10(atof(wgt.c_str()));
		d = "";
		while(1) {
			d = index(w1,d).Order(1);
			if (d.Length() == 0) break;
			if (index(w2,d).Data()) {
				w1stem = w1;
				w2stem = w2;
				if (stem(w1).Data()) w1 = stem(w1,(mstring) "").Order(1);
				if (stem(w2).Data()) w2 = stem(w2,(mstring) "").Order(1);
				if (dir < 0) prs = w2 & "-" & w1;
				else prs = w1 & "-" & w2;
				f1 << d << " " << s << " " << prs << " " << endl;
				dict(prs) = wgt;
				df(prs) = wgt;
				phrase(w1stem,prs)="";
				phrase(w2stem,prs)="";
				doc(d,prs)=s;
				idf(prs)=s;
				index(prs,d)=s;
				}
			}
		}

	f1.close();

	return 1;
	}
