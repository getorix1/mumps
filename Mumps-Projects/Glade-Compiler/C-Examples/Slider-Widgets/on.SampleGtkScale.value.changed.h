void on_SampleGtkScale_value_changed(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkScale.value.changed.h");
 GtkAdjustment *adj = gtk_range_get_adjustment ( (GtkRange*) w);
 printf("Scale = %d\n", (int) gtk_adjustment_get_value(adj));
 }
