#ifndef GTK3_H
#define GTK3_H

extern "C" void on_EntryExample_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.EntryExample.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_ExampleSearchEntry_next_match(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.ExampleSearchEntry.next.match.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_ExampleSearchEntry_search_changed(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.ExampleSearchEntry.search.changed.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }

extern "C" void on_FileChooserExample_file_set(GtkWidget *w) {
	struct MSV * Ptr = AllocSV(); char tmp[512]; 
	sprintf(tmp,"set widget=\"%p\" g ^on.FileChooserExample.file.set.mps",w); 
	Interpret((const char *) tmp, Ptr); free(Ptr); }


#endif
