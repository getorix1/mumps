#ifndef GTK3_H
#define GTK3_H

extern "C" void on_ComboBoxExample_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.ComboBoxExample.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_ExampleFontButton_font_set(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.ExampleFontButton.font.set.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_ExampleSwitch_state_set(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.ExampleSwitch.state.set.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleColorButton_color_set(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleColorButton.color.set.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkButton_clicked(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkButton.clicked.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkCheckButton_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkCheckButton.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkRadio1_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkRadio1.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkRadio2_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkRadio2.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkRadio3_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkRadio3.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_SampleGtkToggleButton_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.SampleGtkToggleButton.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_cbentry_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.cbentry.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_stack_switcher_button_release_event(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.stack.switcher.button.release.event.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }


#endif
