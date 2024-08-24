#ifndef GTK2_H
#define GTK2_H

gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

adjustment1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));

clearStore = GTK_BUTTON(gtk_builder_get_object(builder, "clearStore"));

col0 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col0"));
GtkCellRenderer *col0r;

col0r = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "col0r"));

gtk_tree_view_column_add_attribute(col0,col0r,"text",0);

col1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col1"));
GtkCellRenderer *col1r;

col1r = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "col1r"));

gtk_tree_view_column_add_attribute(col1,col1r,"text",1);

col2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col2"));
GtkCellRenderer *col2r;

col2r = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "col2r"));

gtk_tree_view_column_add_attribute(col2,col2r,"text",2);

fixed1 = GTK_FIXED(gtk_builder_get_object(builder, "fixed1"));

loadTable = GTK_BUTTON(gtk_builder_get_object(builder, "loadTable"));

loadTree = GTK_BUTTON(gtk_builder_get_object(builder, "loadTree"));

scrolledWindow2 = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledWindow2"));

select1 = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "select1"));

textbuffer1 = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer1"));

tree1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree1"));

treeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treeStore"));

window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
gtk_builder_connect_signals(builder, NULL);
gtk_widget_show(GTK_WIDGET(window));

#endif
