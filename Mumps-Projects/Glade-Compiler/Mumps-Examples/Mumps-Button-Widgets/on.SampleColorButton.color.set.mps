#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.SampleColorButton.color.set.mps"," ",widget,!
 write ?7,"Colors (RGBA): ",$z~mdh~color~chooser~get~rgba(widget),!
