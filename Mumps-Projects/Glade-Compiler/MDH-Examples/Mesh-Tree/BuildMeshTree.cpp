// #+
// #+ Copyright (C) 2022 by Kevin C. O'Kane
// #+
// #+ Kevin C. O'Kane, Ph.D.
// #+ Professor Emeritus
// #+ Computer Science Department
// #+ University of Northern Iowa
// #+ Cedar Falls, IA 50614-0507
// #+ kc.okane@gmail.com
// #+ https://threadsafebooks.com/
// #+ https://www.cs.uni.edu/~okane
// #+
// #+ This program is free software; you can redistribute it and/or modify
// #+ it under the terms of the GNU General Public License as published by
// #+ the Free Software Foundation; either version 2 of the License, or
// #+ (at your option) any later version.
// #+
// #+ This program is distributed in the hope that it will be useful,
// #+ but WITHOUT ANY WARRANTY; without even the implied warranty of
// #+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// #+ GNU General Public License for more details.
// #+
// #+ You should have received a copy of the GNU General Public License
// #+ along with this program; if not, write to the Free Software
// #+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// 
// #+      Feb 2, 2022


#include <mumpsc/libmpscpp.h>

	global mesh("mesh");

int	main() {

	mesh().Kill();

	ifstream in;
	in.open ("mtrees2003.txt");

	if (!in.is_open())  { 
		cout << "mtrees2003.txt not found" << endl;
		cout << "terminating " << endl;
		return EXIT_FAILURE;
		}	

	mstring a;
	mstring key, code, x[25], z, set = "set ";
	char aa[256];
	int i, j, k=0;

	while (1) {

		in.getline(aa, 256);

		a = aa;

		key = a.Piece(";", 1);
		code = a.Piece(";", 2);

		if (key.Length() == 0 || code.Length() == 0) break;

		for (i = 1; i < 25; i++ ) {

			x[i] = code.Piece(".",i);
			if (x[i].Length() == 0) { break; }
			}

		i --;

//#-----------------------------------------------------------------------
//#     build a reference like ^mesh("A01","047","025","600)
//#     by concatenating quotes, codes, quotes, and commas onto z
//#-----------------------------------------------------------------------

		z = "^mesh("; 	// begin building global reference

		for (j = 1; j < i ; j++) 

			z = z & "\"" & x[j] & "\"," ;

		z = set & z & "\"" & x[i] & "\")=\"" & key & "\"";

//#-----------------------------------------------------------------------
//#     z now looks like set ^mesh("A01","047")="Abdomen"
//#     now execute the text
//#-----------------------------------------------------------------------

//		cout << z << endl;

		xecute(z);
		}

	return EXIT_SUCCESS;
	}
