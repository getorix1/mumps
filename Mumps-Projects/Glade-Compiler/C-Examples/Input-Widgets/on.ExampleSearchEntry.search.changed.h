void on_ExampleSearchEntry_search_changed(GtkWidget *w) {
 printf("%s\n", "on.ExampleSearchEntry.search.changed.h");
 printf("%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
 }
