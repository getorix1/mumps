extern "C" void on_select1_changed(GtkWidget *w) {
 printf("%s\n", "on.select1.changed.h");
GtkTreeSelection *s;
GtkTreeModel *m;
gchar *value;
GtkTreeIter iter;
if (gtk_tree_selection_get_selected (GTK_TREE_SELECTION(w), &m, &iter) == FALSE) return; 
gtk_tree_model_get(m, &iter, 0, &value,  -1); printf("col 1 = %s\n", value);
 }
