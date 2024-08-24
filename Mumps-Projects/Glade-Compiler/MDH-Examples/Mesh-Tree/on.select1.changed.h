extern "C" void on_select1_changed(GtkWidget *w) {

	printf("%s\n", "on.select1.changed.cpp");

	int col = 0;
	char txt[4096];

	printf("\tSelected: %s\n", 
		mdh_tree_selection_get_selected(select1, col, txt ));
	}
