#!/usr/bin/mumps

	zmain

#	watch for null $p(line2," ",1)

	write !,"Results for: ",%1,!!
	for  do
	. read line
	. if '$t break
	. write $p(line,"###",1),!
	. set line2=$piece(line,"###",2)
	. write ?10
	. for i=2:1 do
	.. set w=$p(line2," ",i)
	.. if w="" break
	.. write w," "
	.. if $x>60 write ?10
	. write !!
