extern "C" void on_entry1_changed(GtkWidget *w) {
	printf("%s\n", "on.entry1.changed.cpp");
	mdh_tree_store_clear (GTK_TREE_STORE(treeStore));
	char txt[4096];
	printf("\t search term: %s\n", mdh_entry_get_text(GTK_ENTRY(w), txt));
	}
