#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h

 write "on.findButton.clicked.mps"," ",widget,!

 set x=$z~mdh~entry~get~text(entry1)
 set i=$z~mdh~tree~store~clear(treeStore)

 set tree="^mesh("
 for i=1:1 do
 . set z=$p(x," ",i)
 . if z="" break
 . set tree=tree_""""_z_""","

 set tree=$e(tree,1,$l(tree)-1)_")"
 write "Looking for ",tree,!

 do ^LoadTreeView.mps


