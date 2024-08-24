void on_SampleGtkSpinButton_value_changed(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkSpinButton.value.changed.h");
 printf("spin = %d\n", (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(w)));
 }
