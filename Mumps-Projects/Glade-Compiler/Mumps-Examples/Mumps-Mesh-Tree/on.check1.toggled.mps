#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.check1.toggled.mps"," ",widget,!
 write $z~mdh~toggle~button~get~active(check1),!
