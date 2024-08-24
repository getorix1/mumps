#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.SampleGtkSpinButton.value.changed.mps"," ",widget,!
 write ?7,"Spin=",$j($z~mdh~spin~button~get~value(widget),4,1),!
