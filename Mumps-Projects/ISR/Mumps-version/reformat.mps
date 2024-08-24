#!/usr/bin/mumps
# Copyright 2014, 2022, 2023, 2024 Kevin C. O'Kane

# reformat.mps June 10, 2024

	zmain

	set M=$zgetenv("in_docs")
	if M="" set M=1000000
	set M=1000000
	open 1:"../titles.list,new"
	kill ^map

	set D=0

	for  do  if D>M quit
	. set off=$ztell read line if '$test break
	. set line=$translate(line,"0123456789""`~!@#$%^&*()_+=?>.<,:;'{[}]")
	. set line=$translate(line,"-"," ")
	. if $extract(line,1,2)="TI" do  quit
	.. set D=D+1
	.. use 1 write D," ",$extract(line,7,200),! use 5
	.. write off," ",D
	.. set ^map(D)=off
	.. set line=$zlower($e(line,7,2048))
	.. set line=$zblanks(line)
	.. for i=1:1 do
	... s w=$p(line," ",i) if $l(w)=0 break
	... w " ",w
	. if $extract(line,1,2)="MH" quit
	. if $extract(line,1,13)="STAT  MEDLINE" write:D'=0 ! quit
	. if $extract(line,1,2)'="AB" quit
	. for  do
	.. set line=$zlower($e(line,7,2048))
	.. set line=$zblanks(line)
	.. for i=1:1 do
	... set w=$p(line," ",i) if $l(w)=0 break
	... w " ",w
	.. read line if '$test break
	.. set line=$translate(line,"0123456789""`~!@#$%^&*()_+=?>.<,:;'{[}]")
	.. if line="" write " ","eoleol" break
	.. if $find(line,"ABSTRACT") set line=$piece(line,"ABSTRACT",1)

	write !
	close 1
