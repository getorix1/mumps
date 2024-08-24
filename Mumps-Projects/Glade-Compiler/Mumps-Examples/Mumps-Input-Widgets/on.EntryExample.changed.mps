#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.EntryExample.changed.mps"," ",widget,!
 write ?7,"Entered text: ",$z~mdh~entry~get~text(EntryExample),!
