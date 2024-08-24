#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.spin1.value.changed.mps"," ",widget,!
 write $z~mdh~spin~button~get~value(spin1),!
