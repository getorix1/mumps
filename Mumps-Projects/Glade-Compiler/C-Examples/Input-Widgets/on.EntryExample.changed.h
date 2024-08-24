void on_EntryExample_changed(GtkWidget *w) {
 printf("%s\n", "on.EntryExample.changed.h");
 printf("\t%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
 }
