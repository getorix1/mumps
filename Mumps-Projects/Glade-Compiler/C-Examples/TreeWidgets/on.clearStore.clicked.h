void on_clearStore_clicked(GtkWidget *w) {
	printf("%s\n", "on.clearStore.clicked.h");
	gtk_tree_store_clear (treeStore);
	}
