#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.loadTable.clicked.mps"," ",widget,!
 set i=$z~mdh~tree~store~clear(treeStore)
 set tree="^mesh"
 do ^LoadTable.mps
 
