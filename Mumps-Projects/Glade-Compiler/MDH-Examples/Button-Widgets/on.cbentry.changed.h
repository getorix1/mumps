extern "C" void on_cbentry_changed(GtkWidget *w) {
 printf("%s\n", "on.cbentry.changed.h");
 printf("\t%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
 }
