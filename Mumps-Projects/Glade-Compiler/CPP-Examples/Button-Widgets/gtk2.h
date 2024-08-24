#ifndef GTK2_H
#define GTK2_H

gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

ComboBoxExample = GTK_COMBO_BOX(gtk_builder_get_object(builder, "ComboBoxExample"));

ExampleFontButton = GTK_FONT_BUTTON(gtk_builder_get_object(builder, "ExampleFontButton"));

ExampleSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "ExampleSwitch"));

SampleColorButton = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "SampleColorButton"));

SampleGtkButton = GTK_BUTTON(gtk_builder_get_object(builder, "SampleGtkButton"));

SampleGtkCheckButton = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "SampleGtkCheckButton"));

SampleGtkRadio1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "SampleGtkRadio1"));

SampleGtkRadio2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "SampleGtkRadio2"));

SampleGtkRadio3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "SampleGtkRadio3"));

SampleGtkToggleButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "SampleGtkToggleButton"));

cbentry = GTK_ENTRY(gtk_builder_get_object(builder, "cbentry"));

entrybuffer1 = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "entrybuffer1"));

fixed1 = GTK_FIXED(gtk_builder_get_object(builder, "fixed1"));

label1 = GTK_LABEL(gtk_builder_get_object(builder, "label1"));

label2 = GTK_LABEL(gtk_builder_get_object(builder, "label2"));

label3 = GTK_LABEL(gtk_builder_get_object(builder, "label3"));

lbl4 = GTK_LABEL(gtk_builder_get_object(builder, "lbl4"));

liststore1 = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));

page_0 = GTK_FIXED(gtk_builder_get_object(builder, "page_0"));

page_1 = GTK_FIXED(gtk_builder_get_object(builder, "page_1"));

stack1 = GTK_STACK(gtk_builder_get_object(builder, "stack1"));

stackSwitcher = GTK_STACK_SWITCHER(gtk_builder_get_object(builder, "stackSwitcher"));

window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
gtk_builder_connect_signals(builder, NULL);
gtk_widget_show(GTK_WIDGET(window));

#endif
