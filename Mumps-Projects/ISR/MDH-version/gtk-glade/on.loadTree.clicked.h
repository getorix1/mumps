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

//	August 19, 2022

#include <mumpsc/libmpscpp.h>

	global clstr("clstr");
	global title("title");

extern "C" void	on_loadTree_clicked(GtkWidget *w) {

	printf("%s\n", "on.loadTree.clicked.cpp");

	mstring x, y;
	int i, j, k = 0;
	char tmp[1024];

 	mdh_tree_store_clear(treeStore);

	x = "";

//	set %=$z~mdh~tree~level~add(treeStore,treeDepth,col1,col2,col3)

	while(1) {
		x = clstr(x).Order(1);
		if (x == "") break;

		strcpy(tmp, "cluster ");
		strcat(tmp, x.c_str());

//		mdh_tree_level_add(treeStore, 1, tmp , "");
		y = "";
		y = clstr(x,y).Order(1);
		mdh_tree_level_add(treeStore, 1, tmp, title(y) );

		y = "";
		while(1) {
			y = clstr(x,y).Order(1);
			if (y == "") break;
			mdh_tree_level_add(treeStore, 2, y.c_str(), title(y) );
			}
		}
	}
