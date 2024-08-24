extern "C" void on_SampleGtkScrollBar_value_changed(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkScrollBar.value.changed.h");
GtkAdjustment *adj = gtk_range_get_adjustment ( (GtkRange*) w);
 printf("Scroll = %d\n", (int) gtk_adjustment_get_value(adj));
 }
