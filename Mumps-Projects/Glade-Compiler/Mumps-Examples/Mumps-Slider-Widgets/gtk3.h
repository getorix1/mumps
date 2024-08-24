#ifndef GTK3_H
#define GTK3_H

extern "C" void on_ExampleCalendar_day_selected(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.ExampleCalendar.day.selected.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkScale_value_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkScale.value.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkScrollBar_value_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkScrollBar.value.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkSpinButton_value_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkSpinButton.value.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }


#endif
