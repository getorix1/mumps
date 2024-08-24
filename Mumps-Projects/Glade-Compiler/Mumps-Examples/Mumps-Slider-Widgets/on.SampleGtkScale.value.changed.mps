#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.SampleGtkScale.value.changed.mps"," ",widget,!
 write ?7,"Scale = ",$z~mdh~range~get~adjustment(widget),!
