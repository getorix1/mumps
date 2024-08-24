#!/usr/bin/mumps

	zmain

#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     GTK Compiler
#+
#+     Copyright (C) 2022 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     https://www.cs.uni.edu/~okane
#+     https://threadsafebooks.com/
#+
#+ This program is free software; you can redistribute it and/or modify
#+ it under the terms of the GNU General Public License as published by
#+ the Free Software Foundation; either version 2 of the License, or
#+ (at your option) any later version.
#+
#+ This program is distributed in the hope that it will be useful,
#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+ GNU General Public License for more details.
#+
#+ You should have received a copy of the GNU General Public License
#+ along with this program; if not, write to the Free Software
#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#+ August 25, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if '$d(%1) write "***** MISSING PARM *****",! halt
	set C=%1

	write !,"Mumps GTK Application Builder ",$zd,!
	write "-----------",!

	open 1:"gtk1.h,new"
	use 1
		write "#ifndef GTK1_H",!
		write "#define GTK1_H",!!

	open 2:"gtk2.h,new"
	use 2
		write "#ifndef GTK2_H",!
		write "#define GTK2_H",!!

	open 3:"gtk3.h,new"
	use 3
		write "#ifndef GTK3_H",!
		write "#define GTK3_H",!!

	use 1
		write "#include <gtk/gtk.h>",!
		write "#include <gtk/gtkx.h>",!
		if C="MDH" write "#include <mumpsc/libmpscpp.h>",!
		write "GtkBuilder *builder;",!

	if C="MUMPS" do
	. use 2
	. write "FILE *f=fopen(""gtk4.h"",""w"");",!

	use 2
		write "gtk_init(&argc, &argv);",!
		write "builder = gtk_builder_new_from_file (""mumps.glade"");",!

	kill ^gtk

	for  do

	. use 5
	. read line
	. if '$test break

	. set iclass=$find(line,"object class=")

	. if iclass'=0 do  quit
	.. set p1=$extract(line,iclass,255)
	.. set widget=$piece(p1,"""",2)
	.. set id=$p(p1,"""",4)
	.. if id="" quit
	.. set ^gtk(id)=widget
	.. quit

	. set p=$find(line,"property name=""stack"">")

	. if p'=0 do  quit
	.. set stk=$e(line,p,255)
	.. set stk=$p(stk,"<")
	.. set ^gtk(id,"stack")=stk
	.. quit

	. set sn=$find(line,"signal name=")

	. if sn'=0 do
	.. set sig=$piece(line,"""",4,4)
	.. set ^gtk(id,"sig",sig)=""

	. if '$find(line,"</object>") quit

	for id=$o(^gtk(id)) do
	. use 5
	. set widget=^gtk(id)
	. write id," ",widget,! ; write widget name and data type
	. do widget
	. if $d(^gtk(id,"stack")) write ?5,"stack=",^gtk(id,"stack"),!
	. for sig=$o(^gtk(id,"sig",sig)) do
	.. write ?5,"signal=",sig,!
	.. if $d(^gtk(id,"stack")) set stack=^gtk(id,"stack")
	.. do signals

	use 2
		write !,"g_signal_connect(window, ""destroy"", G_CALLBACK(gtk_main_quit), NULL);",!
		write "gtk_builder_connect_signals(builder, NULL);",!
		write "gtk_widget_show(GTK_WIDGET(window));",!

	if C="MUMPS" do
	. use 2
	. write "fclose(f);",!

	use 1
		write !,"#endif",!
		close 1

	use 2
		write !,"#endif",!
		close 2

	use 3
		write !,"#endif",!
		close 3

	use 5

	write "-----------",!!

	halt

###################################################

widget

	if widget="GtkCellRendererText" do
	. use 2
	. write "GtkCellRenderer *",id,";",!

	else if widget="GtkAlignment" quit

	else  do
	. use 1
	. write widget," *",id,";",! ; Gtk type and widget name

	use 2
	write !,id," = "

	if widget="GtkWindow" write "GTK_WINDOW("
	if widget="GtkFixed" write "GTK_FIXED("
	if widget="GtkFrame" write "GTK_FRAME("
	if widget="GtkButton" write "GTK_BUTTON("
	if widget="GtkStackSwitcher" write "GTK_STACK_SWITCHER("
	if widget="GtkStack" write "GTK_STACK("
	if widget="GtkIconView" write "GTK_ICON_VIEW("
	if widget="GtkComboBox" write "GTK_COMBO_BOX("
	if widget="GtkSeparator" write "GTK_SEPARATOR("
	if widget="GtkInfoBar" write "GTK_INFO_BAR("
	if widget="GtkButtonBox" write "GTK_BUTTON_BOX("
	if widget="GtkBox" write "GTK_BOX("
	if widget="GtkMenu" write "GTK_MENU("
	if widget="GtkMenuButton" write "GTK_MENU_BUTTON("
	if widget="GtkLockButton" write "GTK_LOCK_BUTTON("
	if widget="GtkEntryBuffer" write "GTK_ENTRY_BUFFER("
	if widget="GtkSwitch" write "GTK_SWITCH("
	if widget="GtkLevelBar" write "GTK_LEVEL_BAR("
	if widget="GtkVolumeButton" write "GTK_VOLUME_BUTTON("
	if widget="GtkImage" write "GTK_IMAGE("
	if widget="GtkProgressBar" write "GTK_PROGRESS_BAR("
	if widget="GtkSpinner" write "GTK_SPINNER("
	if widget="GtkDrawingArea" write "GTK_DRAWING_AREA("
	if widget="GtkCalendar" write "GTK_CALENDAR("
	if widget="GtkFontButton" write "GTK_FONT_BUTTON("
	if widget="GtkAdjustment" write "GTK_ADJUSTMENT("
	if widget="GtkRadioButton" write "GTK_RADIO_BUTTON("
	if widget="GtkScale" write "GTK_SCALE("
	if widget="GtkFileChooserButton" write "GTK_FILE_CHOOSER_BUTTON("
	if widget="GtkColorButton" write "GTK_COLOR_BUTTON("
	if widget="GtkSpinButton" write "GTK_SPIN_BUTTON("
	if widget="GtkScrollbar" write "GTK_SCROLLBAR("
	if widget="GtkLabel" write "GTK_LABEL("
	if widget="GtkToggleButton" write "GTK_TOGGLE_BUTTON("
	if widget="GtkEntry" write "GTK_ENTRY("
	if widget="GtkSearchEntry" write "GTK_SEARCH_ENTRY("
	if widget="GtkViewport" write "GTK_VIEWPORT("
	if widget="GtkScrolledWindow" write "GTK_SCROLLED_WINDOW("
	if widget="GtkTextView" write "GTK_TEXT_VIEW("
	if widget="GtkTextBuffer" write "GTK_TEXT_BUFFER("
	if widget="GtkCheckButton" write "GTK_CHECK_BUTTON("
	if widget="GtkTreeView" write "GTK_TREE_VIEW("
	if widget="GtkTreeSelection" write "GTK_TREE_SELECTION("
	if widget="GtkTreeViewColumn" write "GTK_TREE_VIEW_COLUMN("
	if widget="GtkCellRendererText" write "GTK_CELL_RENDERER("
	if widget="GtkListStore" write "GTK_LIST_STORE("
	if widget="GtkTreeStore" write "GTK_TREE_STORE("

	write "gtk_builder_get_object(builder, """,id,"""));",!

	if C="MUMPS" do
	. write "	{",!
	. write "	char tmp[128];",!
	. write " 	sprintf(tmp,""%p"",",id,");",!
	. write " 	SymPut(""",id,""", tmp);",!
	. write " 	fprintf(f, "" set ",id,"=\""%s\""\n"", tmp);",!
	. write " 	}",!

	if widget="GtkCellRendererText" do
	. set c=$p(id,"col",2) set c=$p(c,"r",1)
	. use 2
	. write !,"gtk_tree_view_column_add_attribute(",$p(id,"r",1),",",id,",""text"",",c,");",!

	use 5
	quit

signals

	if C="MUMPS" do
	. set sig1=$translate(sig,"_",".")_".mps"
	. use 3
	. write "extern ""C"" void ",sig,"(GtkWidget *w) {",!
	. write "	struct MSV * Ptr = AllocSV(); char tmp[512]; ",!
	. write "	sprintf(tmp,""set widget=\""%p\"" g ^",sig1,""",w); ",!
	. write "	Interpret((const char *) tmp, Ptr); free(Ptr); }",!!

	if C="CPP"!(C="MDH")!(C="C") do
	. set sig1=$translate(sig,"_",".")_".h"
	. use 3
	. write "#include """,sig1,"""",!

	if '$zfile(sig1) do
	. use 5
	. write "Creating signal handler: ",sig1,!
	. set sig2=sig1_",new"
	. open 4:sig2

	. if C="MUMPS" do
	.. use 4 write "#!/usr/bin/mumps",!
	.. use 4 write !,"#	Mumps GTK Signal Handler",!
	.. use 4 write !," do ^gtk4.h",!
	.. use 4 write " write """,sig1,""","" "",widget,!",!

	. if C="CPP"!(C="MDH") do
	.. use 4 write "extern ""C"" void ",sig,"(GtkWidget *w) {",!
	.. use 4 write " printf(""%s\n"", """,sig1,""");",!

	. if C="C" do
	.. use 4 write "void ",sig,"(GtkWidget *w) {",!
	.. use 4 write " printf(""%s\n"", """,sig1,""");",!

# ------------------------------------------------------------------------------------------------------

	. if C="MUMPS" do

	.. if widget="GtkScrollbar" do
	... use 4
	... write " write ?7,""Scroll = "",$z~mdh~range~get~adjustment(widget),!",!

	.. else  if widget="GtkScale" do
	... use 4
	... write " write ?7,""Scale = "",$z~mdh~range~get~adjustment(widget),!",!

	.. else  if widget="GtkCalendar" do
	... use 4
	... write " write ?7,""date = "",$z~mdh~calendar~get~date(widget),!",!

	.. else  if widget="GtkCalendar" do
	... use 4
	... write " write ?7,""date = "",$z~mdh~calendar~get~date(widget),!",!

	.. else  if widget="GtkRadioButton" do
	... use 4
	... write " write ?7,""Active: "",$z~mdh~toggle~button~get~active(widget),!",!

	.. else  if widget="GtkTreeSelection" do
	... use 4 do
	... write " write $z~mdh~tree~selection~get~selected("_id_",0),!",!

	.. else  if widget="GtkToggleButton" do
	... use 4 do
	... write " write ?7,""Active: "",$z~mdh~toggle~button~get~active(widget),!",!

	.. else  if widget="GtkSpinButton" do
	... use 4
	... write " write ?7,""Spin="",$j($z~mdh~spin~button~get~value(widget),4,1),!",!

	.. else  if widget="GtkFontButton" do
	... use 4
	... write " write ?7,""font = "",$z~mdh~font~chooser~get~font(widget),!",!

	.. else  if widget="GtkCheckButton" do
	... use 4
	... write " write ?7,""Active: "",$z~mdh~toggle~button~get~active(widget),!",!

	.. else  if widget="GtkSearchEntry" do
	... use 4
	... write " write ?7,""Search for: "",$z~mdh~entry~get~text("_id_"),!",!

	.. else  if widget="GtkFileChooserButton" do
	... use 4
	... write " write ?7,""File selected: "",$z~mdh~file~chooser~get~filename(widget),!",!

	.. else  if widget="GtkColorButton" do
	... use 4
	... write " write ?7,""Colors (RGBA): "",$z~mdh~color~chooser~get~rgba(widget),!",!

	.. else  if widget="GtkEntry" do
	... use 4
	... write " write ?7,""Entered text: "",$z~mdh~entry~get~text("_id_"),!",!

	.. else  if widget="GtkSwitch" do
	... use 4 do
	... write " write ?7,""Active: "",$z~mdh~switch~get~active(widget),!",!

# ------------------------------------------------------------------------------------------------------

	. if C="CPP"!(C="MDH")!(C="C") do

	.. if widget="GtkCalendar" do
	... use 4
	... write " guint mn, dy, yr; gtk_calendar_get_date ((GtkCalendar *) w, &yr, &mn, &dy);",!
	... write " printf(""\tyear = %d month = %d day = %d\n"", yr, mn, dy);",!
	... write " printf(""\tNote: the gtk_calendar_get_date() function is different in GTK4"");",!

	.. else  if widget="GtkScale" do
	... use 4
	... write " GtkAdjustment *adj = gtk_range_get_adjustment ( (GtkRange*) w);",!
	... write " printf(""Scale = %d\n"", (int) gtk_adjustment_get_value(adj));",!

	.. else  if widget="GtkScrollbar" do
	... use 4
	... write "GtkAdjustment *adj = gtk_range_get_adjustment ( (GtkRange*) w);",!
	... write " printf(""Scroll = %d\n"", (int) gtk_adjustment_get_value(adj));",!

	.. else  if widget="GtkButton" do
	... use 4
	... write " printf(""\tButton ",$p(sig,".",2)," clicked\n"");",!

	.. else  if widget="GtkStackSwitcher" do
	... use 4
	... write " printf(""\tButton ",$p(sig,".",2)," clicked\n"");",!
	... write " printf(""\tVisible Child: %s\n"",gtk_stack_get_visible_child_name(GTK_STACK(",stack,")));",!

	.. else  if widget="GtkRadioButton" do
	... use 4
	... write " printf(""\tButton ",$p(sig1,".",2)," clicked\n"");",!
	... write " printf(""\t",$p(sig1,".",2),"""); ",!
	... write "if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))==1)",!
	... write " printf("" active\n""); else printf("" inactive\n"");",!

	.. else  if widget="GtkTreeSelection" do
	... use 4 
	... write "GtkTreeSelection *s;",!,"GtkTreeModel *m;",!,"gchar *value;",!,"GtkTreeIter iter;",!
        ... write "if (gtk_tree_selection_get_selected (GTK_TREE_SELECTION(w), &m, &iter) == FALSE) return; ",!
        ... write "gtk_tree_model_get(m, &iter, 0, &value,  -1); printf(""col 1 = %s\n"", value);",!

	.. else  if widget="GtkToggleButton" do
	... use 4
	... write " printf(""\ttoggled = %d\n"", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));",!

	.. else  if widget="GtkSpinButton" do
	... use 4
	... write " printf(""spin = %d\n"", (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(w)));",!

	.. else  if widget="GtkFontButton" do
	... use 4
	... write " printf(""\tfont = %s\n"", gtk_font_chooser_get_font(GTK_FONT_CHOOSER(w)));",!

	.. else  if widget="GtkCheckButton" do
	... use 4
	... write " printf(""\tchecked = %d\n"", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));",!

	.. else  if widget="GtkEntry" do
	... use 4
	... write " printf(""\t%s\n"", gtk_entry_get_text(GTK_ENTRY(w)));",!

	.. else  if widget="GtkSearchEntry" do
	... use 4
	... write " printf(""%s\n"", gtk_entry_get_text(GTK_ENTRY(w)));",!

	.. else  if widget="GtkFileChooserButton" do
	... use 4
	... write " printf(""%s\n"", gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(w)));",!

	.. else  if widget="GtkColorButton" do
	... use 4
	... write " GdkRGBA r;",!
	... write " gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER(w), &r);",!
	... write " printf(""red=%f green=%f blue=%f alpha=%f\n"", r.red, r.green, r.blue, r.alpha );",!

	.. else  if widget="GtkSwitch" do
	... use 4
	... write " printf(""\tActive: %d\n"", gtk_switch_get_active (GTK_SWITCH(w)));",!

	.. use 4
	.. write " }",!

	. close 4

	use 5

	quit
