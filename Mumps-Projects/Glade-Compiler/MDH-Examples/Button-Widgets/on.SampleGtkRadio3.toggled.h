extern "C" void on_SampleGtkRadio3_toggled(GtkWidget *w) {
 printf("%s\n", "on.SampleGtkRadio3.toggled.h");
 printf("\tButton SampleGtkRadio3 clicked\n");
 printf("\tSampleGtkRadio3"); 
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))==1)
 printf(" active\n"); else printf(" inactive\n");
 }
