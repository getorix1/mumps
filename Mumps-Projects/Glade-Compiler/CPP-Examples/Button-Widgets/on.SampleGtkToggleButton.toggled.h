extern "C" void on_SampleGtkToggleButton_toggled(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkToggleButton.toggled.h");
 printf("\ttoggled = %d\n", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));
 }
