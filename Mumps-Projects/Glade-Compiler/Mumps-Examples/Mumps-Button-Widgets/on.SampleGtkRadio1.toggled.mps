#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.SampleGtkRadio1.toggled.mps"," ",widget,!
 write ?7,"Active: ",$z~mdh~toggle~button~get~active(widget),!
