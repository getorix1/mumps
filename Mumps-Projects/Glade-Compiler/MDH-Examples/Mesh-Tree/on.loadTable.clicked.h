extern "C" void on_loadTable_clicked(GtkWidget *w) {

	global mesh("mesh");

	printf("%s\n", "on.loadTree.clicked.cpp");

	mdh_tree_store_clear (GTK_TREE_STORE(treeStore));

	mstring mesh_entry;
	int i, j;

	mesh_entry = "^mesh(0)";

	for(int k = 0; k < 100; k++) {	// show only 1st 100 entries
		mesh_entry = mesh_entry.Query();		// get next entry
		if (mesh_entry=="") break;			// no more entries
		if (mesh_entry.Piece("(",1) != "^mesh") break;	// array name must be ^mesh
		i=mesh_entry.Qlength();				// number if array entry indices
		mdh_tree_level_add(treeStore, 			// add entry to table
			1, 					// tree add level 
			mesh_entry.c_str(),			// column 1
			mesh_entry.Eval().c_str()		// column 2
			);
		}

	}
