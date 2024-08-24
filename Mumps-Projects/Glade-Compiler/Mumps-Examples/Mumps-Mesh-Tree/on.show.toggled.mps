#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.show.toggled.mps"," ",widget,!
 if $z~mdh~toggle~button~get~active(show) do
 . set %=$z~mdh~widget~hide(scrolledWindow2)
 else set %=$z~mdh~widget~show(scrolledWindow2)
