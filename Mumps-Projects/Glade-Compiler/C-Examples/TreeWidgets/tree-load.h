//#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+
//#+     GTK Compiler
//#+
//#+     Copyright (C) 2018, 2019, 2020, 2021 by Kevin C. O'Kane
//#+
//#+     Kevin C. O'Kane
//#+     kc.okane@gmail.com
//#+     https://www.cs.uni.edu/~okane
//#+     https://threadsafebooks.com/
//#+
//#+ This program is free software; you can redistribute it and/or modify
//#+ it under the terms of the GNU General Public License as published by
//#+ the Free Software Foundation; either version 2 of the License, or
//#+ (at your option) any later version.
//#+
//#+ This program is distributed in the hope that it will be useful,
//#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#+ GNU General Public License for more details.
//#+
//#+ You should have received a copy of the GNU General Public License
//#+ along with this program; if not, write to the Free Software
//#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+
//#+ August 7, 2022
//#+
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef TREE_LOAD
#define TREE_LOAD

//-------------------------------------------------------------------------
//	The tree structure is in the first column only
//	Subsequent column(s) are tabular only.
//	When you create a tree, each added item that is added at a greater
//	depth, must be only one level deeper than the prior depth. That is,
//	when creating lower levels, you can't skip one (or more).
//
//	Depth numbers begin at 1 and can extend to 10.
//
//	GtkTreeLevelAdd(treeStore, columns, depth, tree, data [,more data columns]);
//-------------------------------------------------------------------------

	FILE *in;
	in = fopen("mtree", "r");
	char *p1, dpth[64], word[1024], code[1024], line[1024];
	int dep;

	while (fgets(line,1024,in)) {
		line[strlen(line) - 1] = 0;	// remove newline

		p1 = strtok(line, " ");		// first token
		dep = atoi(p1);			// depth in tree
		strcpy(dpth, p1);

		p1 = strtok(NULL, " ");		// second token
		strcpy(code, p1);		// MeSH codes placed in second table column

		p1 = strtok(NULL, " ");		// third token
		strcpy(word, p1);		// code word(s) placed in first column tree

		GtkTreeLevelAdd(treeStore, 3, dep, word, code, dpth);
		}
#endif
