extern "C" void on_findButton_clicked(GtkWidget *w) {

	printf("%s\n", "on.findButton.clicked.cpp");

	mdh_tree_store_clear (GTK_TREE_STORE(treeStore));

	char txt[4096];
	printf("\t search term: %s\n", mdh_entry_get_text(GTK_ENTRY(entry1), txt));

	global mesh("mesh");

	mstring mesh_entry;
	int i, j;

	mesh_entry = "^mesh(0)";

	while (1)  {	// show only 1st 100 entries
		mesh_entry = mesh_entry.Query();		// get next entry
		if (mesh_entry=="") break;			// no more entries
		if (mesh_entry.Piece("(",1) != "^mesh") break;	// array name must be ^mesh
		if (strcasestr(mesh_entry.Eval().c_str(), txt) 
			== NULL) continue; 
		i = mesh_entry.Qlength();			// number if array entry indices
		mdh_tree_level_add(treeStore, 			// add entry to table
			1, 					// tree add level 
			mesh_entry.c_str(),			// column 1
			mesh_entry.Eval().c_str()		// column 2
			);
		}
	}
