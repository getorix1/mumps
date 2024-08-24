extern "C" void on_SampleGtkCheckButton_toggled(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkCheckButton.toggled.h");
 printf("\tchecked = %d\n", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));
 }
