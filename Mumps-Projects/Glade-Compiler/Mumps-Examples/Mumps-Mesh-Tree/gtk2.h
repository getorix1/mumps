#ifndef GTK2_H
#define GTK2_H

FILE *f=fopen("gtk4.h","w");
gtk_init(&argc, &argv);
builder = gtk_builder_new_from_file ("mumps.glade");

adjustment1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",adjustment1);
 	SymPut("adjustment1", tmp);
 	fprintf(f, " set adjustment1=\"%s\"\n", tmp);
 	}

check1 = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "check1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",check1);
 	SymPut("check1", tmp);
 	fprintf(f, " set check1=\"%s\"\n", tmp);
 	}

clearStore = GTK_BUTTON(gtk_builder_get_object(builder, "clearStore"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",clearStore);
 	SymPut("clearStore", tmp);
 	fprintf(f, " set clearStore=\"%s\"\n", tmp);
 	}

col0 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col0"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",col0);
 	SymPut("col0", tmp);
 	fprintf(f, " set col0=\"%s\"\n", tmp);
 	}
GtkCellRenderer *col0r;

col0r = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "col0r"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",col0r);
 	SymPut("col0r", tmp);
 	fprintf(f, " set col0r=\"%s\"\n", tmp);
 	}

gtk_tree_view_column_add_attribute(col0,col0r,"text",0);

col1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",col1);
 	SymPut("col1", tmp);
 	fprintf(f, " set col1=\"%s\"\n", tmp);
 	}
GtkCellRenderer *col1r;

col1r = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "col1r"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",col1r);
 	SymPut("col1r", tmp);
 	fprintf(f, " set col1r=\"%s\"\n", tmp);
 	}

gtk_tree_view_column_add_attribute(col1,col1r,"text",1);

col2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col2"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",col2);
 	SymPut("col2", tmp);
 	fprintf(f, " set col2=\"%s\"\n", tmp);
 	}
GtkCellRenderer *col2r;

col2r = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "col2r"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",col2r);
 	SymPut("col2r", tmp);
 	fprintf(f, " set col2r=\"%s\"\n", tmp);
 	}

gtk_tree_view_column_add_attribute(col2,col2r,"text",2);

entry1 = GTK_ENTRY(gtk_builder_get_object(builder, "entry1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",entry1);
 	SymPut("entry1", tmp);
 	fprintf(f, " set entry1=\"%s\"\n", tmp);
 	}

findButton = GTK_BUTTON(gtk_builder_get_object(builder, "findButton"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",findButton);
 	SymPut("findButton", tmp);
 	fprintf(f, " set findButton=\"%s\"\n", tmp);
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

loadTable = GTK_BUTTON(gtk_builder_get_object(builder, "loadTable"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",loadTable);
 	SymPut("loadTable", tmp);
 	fprintf(f, " set loadTable=\"%s\"\n", tmp);
 	}

loadTree = GTK_BUTTON(gtk_builder_get_object(builder, "loadTree"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",loadTree);
 	SymPut("loadTree", tmp);
 	fprintf(f, " set loadTree=\"%s\"\n", tmp);
 	}

radio1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radio1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",radio1);
 	SymPut("radio1", tmp);
 	fprintf(f, " set radio1=\"%s\"\n", tmp);
 	}

radio2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radio2"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",radio2);
 	SymPut("radio2", tmp);
 	fprintf(f, " set radio2=\"%s\"\n", tmp);
 	}

radio3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radio3"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",radio3);
 	SymPut("radio3", tmp);
 	fprintf(f, " set radio3=\"%s\"\n", tmp);
 	}

scrolledWindow2 = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledWindow2"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",scrolledWindow2);
 	SymPut("scrolledWindow2", tmp);
 	fprintf(f, " set scrolledWindow2=\"%s\"\n", tmp);
 	}

scrolledrWindow1 = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledrWindow1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",scrolledrWindow1);
 	SymPut("scrolledrWindow1", tmp);
 	fprintf(f, " set scrolledrWindow1=\"%s\"\n", tmp);
 	}

select1 = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "select1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",select1);
 	SymPut("select1", tmp);
 	fprintf(f, " set select1=\"%s\"\n", tmp);
 	}

show = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "show"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",show);
 	SymPut("show", tmp);
 	fprintf(f, " set show=\"%s\"\n", tmp);
 	}

spin1 = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",spin1);
 	SymPut("spin1", tmp);
 	fprintf(f, " set spin1=\"%s\"\n", tmp);
 	}

textbuffer1 = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",textbuffer1);
 	SymPut("textbuffer1", tmp);
 	fprintf(f, " set textbuffer1=\"%s\"\n", tmp);
 	}

textview1 = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "textview1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",textview1);
 	SymPut("textview1", tmp);
 	fprintf(f, " set textview1=\"%s\"\n", tmp);
 	}

toggle1 = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "toggle1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",toggle1);
 	SymPut("toggle1", tmp);
 	fprintf(f, " set toggle1=\"%s\"\n", tmp);
 	}

tree1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",tree1);
 	SymPut("tree1", tmp);
 	fprintf(f, " set tree1=\"%s\"\n", tmp);
 	}

treeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treeStore"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",treeStore);
 	SymPut("treeStore", tmp);
 	fprintf(f, " set treeStore=\"%s\"\n", tmp);
 	}

viewport1 = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport1"));
	{
	char tmp[128];
 	sprintf(tmp,"%p",viewport1);
 	SymPut("viewport1", tmp);
 	fprintf(f, " set viewport1=\"%s\"\n", tmp);
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
