#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.hide.toggled.mps"," ",widget,!
 write $z~mdh~toggle~button~get~active(hide),!

 if $z~gtk~toggle~button~mdh~active(hide) set %=$z~mdh~widget~hide(scrolledWindow2)
 else  set %=$z~mdh~widget~show(scrolledWindow2)
