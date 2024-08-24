#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.ExampleCalendar.day.selected.mps"," ",widget,!
 write ?7,"date = ",$z~mdh~calendar~get~date(widget),!
