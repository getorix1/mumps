#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.ExampleFontButton.font.set.mps"," ",widget,!
 write ?7,"font = ",$z~mdh~font~chooser~get~font(widget),!
