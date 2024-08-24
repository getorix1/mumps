#!/usr/bin/mumps
# Copyright 2014, 2022, 2023 Kevin C. O'Kane

# preprocess.mps May 15, 2022

	zmain

	set %=$zStopInit("basic.stop.words")

#	open 1:"stem-words.tmp,new"
#	open 2:"wordlist.tmp,new"

	set min=%1

	for  do
	. read line if '$test break
	. u 5 write $p(line," ",1,2)
	. set line=$translate(line,"0123456789""`~!@#$%^&*()_+=?>.<,:;'{[}]")
	. set line=$zblanks(line)
	. for i=3:1 do
	.. s w=$p(line," ",i)
	.. if $l(w)=0 break
	.. if $l(w)<min quit
	.. if $zStopLookup(w) quit
	.. if $e(w,1,1)="-" set w=$e(w,2,99)
	.. set x=$zstem(w)
	.. set ^stem(x,w)=""
#	.. write " ",x use 2 write w,! use 1 write x,! use 5
	.. write " ",x
	. u 5 w !

#	close 1
#	close 2
