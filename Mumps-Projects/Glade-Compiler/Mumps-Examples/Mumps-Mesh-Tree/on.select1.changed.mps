#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.select1.changed.mps"," ",widget,!
 set x=$z~mdh~tree~selection~get~selected(select1,0)
 write "selection ",x,!
 set %=$z~mdh~text~buffer~set~text(textbuffer1,x)
