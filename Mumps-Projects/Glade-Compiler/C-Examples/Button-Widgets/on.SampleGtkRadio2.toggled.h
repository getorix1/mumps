void on_SampleGtkRadio2_toggled(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkRadio2.toggled.h");
 printf("\tButton SampleGtkRadio2 clicked\n");
 printf("\tSampleGtkRadio2"); 
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))==1)
 printf(" active\n"); else printf(" inactive\n");
 }
