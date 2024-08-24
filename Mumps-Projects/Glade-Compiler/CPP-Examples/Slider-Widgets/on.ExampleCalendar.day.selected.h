extern "C" void on_ExampleCalendar_day_selected(GtkWidget *w) {
 printf("%s\n", "on.ExampleCalendar.day.selected.h");
 guint mn, dy, yr; gtk_calendar_get_date ((GtkCalendar *) w, &yr, &mn, &dy);
 printf("\tyear = %d month = %d day = %d\n", yr, mn, dy);
 printf("\tNote: the gtk_calendar_get_date() function is different in GTK4");
 }
