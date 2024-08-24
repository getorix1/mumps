#ifndef GTK3_H
#define GTK3_H

extern "C" void on_check1_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.check1.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_clearStore_clicked(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.clearStore.clicked.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_entry1_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.entry1.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_findButton_clicked(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.findButton.clicked.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_loadTable_clicked(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.loadTable.clicked.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_loadTree_clicked(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.loadTree.clicked.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_radio1_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.radio1.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_radio2_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.radio2.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_radio3_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.radio3.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_select1_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.select1.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_show_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.show.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_spin1_value_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.spin1.value.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_toggle1_toggled(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.toggle1.toggled.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }


#endif
