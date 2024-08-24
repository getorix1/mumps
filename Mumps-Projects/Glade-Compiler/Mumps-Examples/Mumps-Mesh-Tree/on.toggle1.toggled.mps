#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.toggle1.toggled.mps"," ",widget,!
 write $z~mdh~toggle~button~get~active(toggle1),!
 set %=$z~mdh~entry~set~text(entry1,"test test test")
 set %=$z~mdh~spin~button~set~value(spin1,32)
 set %=$z~mdh~dialog~new~with~buttons(window,"test test test")
 write "dialog returns: ",%,!
