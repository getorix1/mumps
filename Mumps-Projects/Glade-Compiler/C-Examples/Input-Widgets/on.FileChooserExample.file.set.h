void on_FileChooserExample_file_set(GtkWidget *w) {
 printf("%s\n", "on.FileChooserExample.file.set.h");
 printf("%s\n", gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(w)));
 }
