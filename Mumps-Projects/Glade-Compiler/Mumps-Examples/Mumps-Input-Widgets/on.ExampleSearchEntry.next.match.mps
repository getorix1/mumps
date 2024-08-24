#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.ExampleSearchEntry.next.match.mps"," ",widget,!
 write ?7,"Search for: ",$z~mdh~entry~get~text(ExampleSearchEntry),!
