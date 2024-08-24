extern "C" void on_loadTable_clicked(GtkWidget *w) {
	printf("%s\n", "on.loadTable.clicked.h");
        gtk_tree_store_clear (treeStore);
#include "table-load.h"
 }
