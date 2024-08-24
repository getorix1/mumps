void on_SampleColorButton_color_set(GtkWidget *w) {
 printf("%s\n", "on.SampleColorButton.color.set.h");
 GdkRGBA r;
 gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER(w), &r);
 printf("red=%f green=%f blue=%f alpha=%f\n", r.red, r.green, r.blue, r.alpha );
 }
