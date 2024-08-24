#ifndef GTK2_H
#define GTK2_H

gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

fixed1 = GTK_FIXED(gtk_builder_get_object(builder, "fixed1"));

window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
gtk_builder_connect_signals(builder, NULL);
gtk_widget_show(GTK_WIDGET(window));

#endif
