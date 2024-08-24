#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.FileChooserExample.file.set.mps"," ",widget,!
 write ?7,"File selected: ",$z~mdh~file~chooser~get~filename(widget),!
