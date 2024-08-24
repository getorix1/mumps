extern "C" void on_SampleGtkRadio1_toggled(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkRadio1.toggled.h");
 printf("\tButton SampleGtkRadio1 clicked\n");
 printf("\tSampleGtkRadio1"); 
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))==1)
 printf(" active\n"); else printf(" inactive\n");
 }
