#ifndef GTK2_H
#define GTK2_H

FILE *f=fopen("gtk4.h","w");
gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

EntryExample = GTK_ENTRY(gtk_builder_get_object(builder, "EntryExample"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",EntryExample);
 	SymPut("EntryExample", tmp);
 	fprintf(f, " set EntryExample=\"%s\"\n", tmp);
 	}

ExampleSearchEntry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "ExampleSearchEntry"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",ExampleSearchEntry);
 	SymPut("ExampleSearchEntry", tmp);
 	fprintf(f, " set ExampleSearchEntry=\"%s\"\n", tmp);
 	}

FileChooserExample = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "FileChooserExample"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",FileChooserExample);
 	SymPut("FileChooserExample", tmp);
 	fprintf(f, " set FileChooserExample=\"%s\"\n", tmp);
 	}

adjustment1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",adjustment1);
 	SymPut("adjustment1", tmp);
 	fprintf(f, " set adjustment1=\"%s\"\n", tmp);
 	}

adjustment2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",adjustment2);
 	SymPut("adjustment2", tmp);
 	fprintf(f, " set adjustment2=\"%s\"\n", tmp);
 	}

adjustment3 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment3"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",adjustment3);
 	SymPut("adjustment3", tmp);
 	fprintf(f, " set adjustment3=\"%s\"\n", tmp);
 	}

fixed1 = GTK_FIXED(gtk_builder_get_object(builder, "fixed1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",fixed1);
 	SymPut("fixed1", tmp);
 	fprintf(f, " set fixed1=\"%s\"\n", tmp);
 	}

label1 = GTK_LABEL(gtk_builder_get_object(builder, "label1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",label1);
 	SymPut("label1", tmp);
 	fprintf(f, " set label1=\"%s\"\n", tmp);
 	}

label2 = GTK_LABEL(gtk_builder_get_object(builder, "label2"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",label2);
 	SymPut("label2", tmp);
 	fprintf(f, " set label2=\"%s\"\n", tmp);
 	}

label3 = GTK_LABEL(gtk_builder_get_object(builder, "label3"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",label3);
 	SymPut("label3", tmp);
 	fprintf(f, " set label3=\"%s\"\n", tmp);
 	}

window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",window);
 	SymPut("window", tmp);
 	fprintf(f, " set window=\"%s\"\n", tmp);
 	}

g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
gtk_builder_connect_signals(builder, NULL);
gtk_widget_show(GTK_WIDGET(window));
fclose(f);

#endif
