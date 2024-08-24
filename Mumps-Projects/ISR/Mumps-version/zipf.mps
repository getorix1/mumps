#!/usr/bin/mumps
# zipf.mps Copyright 2014 Kevin C. O'Kane
# input is from sdin 
# output is to sdout

	zmain

	if '$data(%1) set d=1000
	else  set d=%1

	write $zd," Zipf Table Rank*Freq/d",!!
	for i=1:1 do
	. read a
	. if '$test break
	. set a=$zblanks(a)
	. set f=$piece(a," ",1)
	. set w=$piece(a," ",2)
	. set t=i*f/d
	. write $justify(t,6,0)," ",w,!
