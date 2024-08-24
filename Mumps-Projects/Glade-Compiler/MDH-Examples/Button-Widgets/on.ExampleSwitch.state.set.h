extern "C" void on_ExampleSwitch_state_set(GtkWidget *w) {
 printf("%s\n", "on.ExampleSwitch.state.set.h");
 printf("\tActive: %d\n", gtk_switch_get_active (GTK_SWITCH(w)));
 }
