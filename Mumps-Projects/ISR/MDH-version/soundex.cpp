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

//	May 13, 2021

#include <mumpsc/libmpscpp.h>

#include "definitions.h"

void	soundex(STATE *SP) {

#include "globals.h"

	mstring w, sdx, sdx1;

	cout << "\nBegin soundex.cpp\n";

	sndx("").Kill();

	ofstream f1("soundex-codes.txt");

	w = "";
	while(1) {
		w = dict(w).Order(1);
		if (w.Length() == 0) break;
		sdx = w.Soundex();
		sndx(sdx,w) = "";
		}

	sdx1 = "";
	while(1) {
		sdx1 = sndx(sdx1).Order(1);
		if (sdx1.Length() == 0) break;
		f1 << sdx1 << ": " ;
		w = "";
		while(1) {
			w = sndx(sdx1,w).Order(1);
			if (w.Length() == 0) break;
			f1 << w << " ";
			}
		f1 << endl;
		}

	f1.close();

	return 1;
	}

