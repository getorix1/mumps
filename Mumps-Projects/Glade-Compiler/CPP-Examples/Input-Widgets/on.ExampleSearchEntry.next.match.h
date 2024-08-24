extern "C" void on_ExampleSearchEntry_next_match(GtkWidget *w) {
 printf("%s\n", "on.ExampleSearchEntry.next.match.h");
 printf("%s\n", gtk_entry_get_text(GTK_ENTRY(w)));
 }
