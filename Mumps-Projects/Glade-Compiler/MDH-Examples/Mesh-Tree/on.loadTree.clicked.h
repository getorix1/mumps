// #+-------------------------------------------------------------------------------
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
// #+
// #+      July 21, 2022
// #+-------------------------------------------------------------------------------

extern "C" void on_loadTree_clicked(GtkWidget *w) {

	global mesh("mesh");

	printf("%s\n", "on.loadTree.clicked.cpp");

	mdh_tree_store_clear (GTK_TREE_STORE(treeStore));

	mstring mesh_entry;
	int i, j, k = 0;

	mesh_entry = "^mesh(0)";	// initial reference

	for (j = 0; j< 500; j++) {
		mesh_entry = mesh_entry.Query();
		if (mesh_entry == "") break;
		if (mesh_entry.Piece("(",1) != "^mesh") break;
		i = mesh_entry.Qlength();
		mdh_tree_level_add(treeStore, 
			i, 
			mesh_entry.c_str(),
			mesh_entry.Eval().c_str()
			);
		}

	}
