#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.clearStore.clicked.mps"," ",widget,!
 set i=$z~mdh~tree~store~clear(treeStore)
