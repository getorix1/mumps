#ifndef GTK2_H
#define GTK2_H

FILE *f=fopen("gtk4.h","w");
gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

ComboBoxExample = GTK_COMBO_BOX(gtk_builder_get_object(builder, "ComboBoxExample"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",ComboBoxExample);
 	SymPut("ComboBoxExample", tmp);
 	fprintf(f, " set ComboBoxExample=\"%s\"\n", tmp);
 	}

ExampleFontButton = GTK_FONT_BUTTON(gtk_builder_get_object(builder, "ExampleFontButton"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",ExampleFontButton);
 	SymPut("ExampleFontButton", tmp);
 	fprintf(f, " set ExampleFontButton=\"%s\"\n", tmp);
 	}

ExampleSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "ExampleSwitch"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",ExampleSwitch);
 	SymPut("ExampleSwitch", tmp);
 	fprintf(f, " set ExampleSwitch=\"%s\"\n", tmp);
 	}

SampleColorButton = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "SampleColorButton"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleColorButton);
 	SymPut("SampleColorButton", tmp);
 	fprintf(f, " set SampleColorButton=\"%s\"\n", tmp);
 	}

SampleGtkButton = GTK_BUTTON(gtk_builder_get_object(builder, "SampleGtkButton"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleGtkButton);
 	SymPut("SampleGtkButton", tmp);
 	fprintf(f, " set SampleGtkButton=\"%s\"\n", tmp);
 	}

SampleGtkCheckButton = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "SampleGtkCheckButton"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleGtkCheckButton);
 	SymPut("SampleGtkCheckButton", tmp);
 	fprintf(f, " set SampleGtkCheckButton=\"%s\"\n", tmp);
 	}

SampleGtkRadio1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "SampleGtkRadio1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleGtkRadio1);
 	SymPut("SampleGtkRadio1", tmp);
 	fprintf(f, " set SampleGtkRadio1=\"%s\"\n", tmp);
 	}

SampleGtkRadio2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "SampleGtkRadio2"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleGtkRadio2);
 	SymPut("SampleGtkRadio2", tmp);
 	fprintf(f, " set SampleGtkRadio2=\"%s\"\n", tmp);
 	}

SampleGtkRadio3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "SampleGtkRadio3"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleGtkRadio3);
 	SymPut("SampleGtkRadio3", tmp);
 	fprintf(f, " set SampleGtkRadio3=\"%s\"\n", tmp);
 	}

SampleGtkToggleButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "SampleGtkToggleButton"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",SampleGtkToggleButton);
 	SymPut("SampleGtkToggleButton", tmp);
 	fprintf(f, " set SampleGtkToggleButton=\"%s\"\n", tmp);
 	}

cbentry = GTK_ENTRY(gtk_builder_get_object(builder, "cbentry"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",cbentry);
 	SymPut("cbentry", tmp);
 	fprintf(f, " set cbentry=\"%s\"\n", tmp);
 	}

entrybuffer1 = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "entrybuffer1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",entrybuffer1);
 	SymPut("entrybuffer1", tmp);
 	fprintf(f, " set entrybuffer1=\"%s\"\n", tmp);
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

lbl4 = GTK_LABEL(gtk_builder_get_object(builder, "lbl4"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",lbl4);
 	SymPut("lbl4", tmp);
 	fprintf(f, " set lbl4=\"%s\"\n", tmp);
 	}

liststore1 = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",liststore1);
 	SymPut("liststore1", tmp);
 	fprintf(f, " set liststore1=\"%s\"\n", tmp);
 	}

page_0 = GTK_FIXED(gtk_builder_get_object(builder, "page_0"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",page_0);
 	SymPut("page_0", tmp);
 	fprintf(f, " set page_0=\"%s\"\n", tmp);
 	}

page_1 = GTK_FIXED(gtk_builder_get_object(builder, "page_1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",page_1);
 	SymPut("page_1", tmp);
 	fprintf(f, " set page_1=\"%s\"\n", tmp);
 	}

stack1 = GTK_STACK(gtk_builder_get_object(builder, "stack1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",stack1);
 	SymPut("stack1", tmp);
 	fprintf(f, " set stack1=\"%s\"\n", tmp);
 	}

stackSwitcher = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "stackSwitcher"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",stackSwitcher);
 	SymPut("stackSwitcher", tmp);
 	fprintf(f, " set stackSwitcher=\"%s\"\n", tmp);
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
