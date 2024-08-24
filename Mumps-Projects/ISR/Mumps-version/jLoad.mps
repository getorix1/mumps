#!/usr/bin/mumps
# ~/Medline2012/jLoad.mps December 10, 2014
# Copyright 2014 Kevin C. O'Kane

	zmain

# input format: #co-occur w1 w2

	for  do
	. use 5
	. read a
	. if '$test break
	. set wgt=$piece(a," ",1)
	. set w1=$piece(a," ",2)
	. set w2=$piece(a," ",3)
	. set ^jtt(w1,w2)=wgt
