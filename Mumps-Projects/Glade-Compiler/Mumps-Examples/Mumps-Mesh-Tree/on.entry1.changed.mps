#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.entry1.changed.mps"," ",widget,!
 write $z~mdh~entry~get~text(entry1),!
