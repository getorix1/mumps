#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.loadTree.clicked.mps"," ",widget,!
 set i=$z~mdh~tree~store~clear(treeStore)
 set tree="^mesh(0)" ; both of these work
 set tree="^mesh" ; both of these work
 do ^LoadTreeView.mps
