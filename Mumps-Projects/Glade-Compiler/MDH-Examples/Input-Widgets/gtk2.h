#ifndef GTK2_H
#define GTK2_H

gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

EntryExample = GTK_ENTRY(gtk_builder_get_object(builder, "EntryExample"));

ExampleSearchEntry = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "ExampleSearchEntry"));

FileChooserExample = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "FileChooserExample"));

adjustment1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));

adjustment2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));

adjustment3 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment3"));

fixed1 = GTK_FIXED(gtk_builder_get_object(builder, "fixed1"));

label1 = GTK_LABEL(gtk_builder_get_object(builder, "label1"));

label2 = GTK_LABEL(gtk_builder_get_object(builder, "label2"));

label3 = GTK_LABEL(gtk_builder_get_object(builder, "label3"));

window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
gtk_builder_connect_signals(builder, NULL);
gtk_widget_show(GTK_WIDGET(window));

#endif
