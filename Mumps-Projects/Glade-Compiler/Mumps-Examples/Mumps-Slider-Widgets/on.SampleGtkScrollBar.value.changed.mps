#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.SampleGtkScrollBar.value.changed.mps"," ",widget,!
 write ?7,"Scroll = ",$z~mdh~range~get~adjustment(widget),!
