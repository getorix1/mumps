#!/usr/bin/mumps
# Copyright 2014, 2022, 2023 Kevin C. O'Kane

# preprocess.mps June 10, 2024

	zmain

	set %=$zStopInit("basic.stop.words")

	set min=3

	for  do
	. set w=$zzscanalnum
	. if '$t break
	. write w," "
	. set w=$zzscanalnum
	. if '$t break
	. write w
	. for  do
	.. set w=$zzscanalnum
	.. if '$t break
	.. if w="eoleol" write ! break
	.. if $l(w)<min quit
	.. if $zStopLookup(w) quit
	.. set x=$zstem(w)
	.. set ^stem(x,w)=""
	.. write " ",x

	halt

L1	read line
	if '$test goto L2
	write $p(line," ",1,2)
#	set line=$translate(line,"0123456789""`~!@#$%^&*()_+=?>.<,:;'{[}]")
#	set line=$zblanks(line)
	set i=2

L3	set i=i+1
	s w=$p(line," ",i)
	if $l(w)=0 write ! goto L1
	if $l(w)<min goto L3
	if $zStopLookup(w) goto L3
	if $e(w,1,1)="-" set w=$e(w,2,99)
	set x=$zstem(w)
	set ^stem(x,w)=""
	write " ",x
	goto L3

L2	halt
