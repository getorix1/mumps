#!/usr/bin/mumps
# loadTitles.mps Feb 14, 2014
# Copyright 2014 Kevin C. O'Kane

	zmain

	set x=0

	for  do
	. read t
	. if '$test break
	. set ^title($piece(t," ",1))=$piece(t," ",2,200)
	. set x=x+1

