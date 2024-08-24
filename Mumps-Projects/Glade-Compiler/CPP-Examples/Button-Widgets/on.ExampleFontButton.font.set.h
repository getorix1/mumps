extern "C" void on_ExampleFontButton_font_set(GtkWidget *w) {
 printf("%s\n", "on.ExampleFontButton.font.set.h");
 printf("\tfont = %s\n", gtk_font_chooser_get_font(GTK_FONT_CHOOSER(w)));
 }
