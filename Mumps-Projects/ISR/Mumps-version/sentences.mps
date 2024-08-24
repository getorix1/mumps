#!/usr/bin/mumps
# Copyright 2014, 2022, 2023, 2024 Kevin C. O'Kane

# reformat.mps April 13, 2024

	zmain

	set M=$zgetenv("in_docs")
	if M="" set M=1000000
	set M=1000000

	set D=0

	for  do  if D>M quit
	. set o=$ztell read line if '$test break
	. if $extract(line,1,2)="TI" do  quit
	.. set D=D+1
	.. set line=$zlower($e(line,7,2048))
	.. set line=$zblanks(line)
	.. w "*",D," "
	.. for i=1:1 do
	... s w=$p(line," ",i) if $l(w)=0 break
	... if $l(w)<3 quit
	... set w=$translate(w,"0123456789""`~!@#$%^&*()_+=?><,:;'{[}]")
	... w w," "
	.. w !
	. if $extract(line,1,2)="MH" quit
	. if $extract(line,1,13)="STAT- MEDLINE" set off=o write:D'=0 ! quit
	. if $extract(line,1,2)'="AB" quit
	. for  do
	.. set line=$zlower($e(line,7,2048))
	.. set line=$zblanks(line)
	.. for i=1:1 do
	... set w=$p(line," ",i)
	... set w=$translate(w,"0123456789""`~!@#$%^&*()_+=?><,:;'{[}]")
	... if $l(w)=0 break
	... if $l(w)<3 quit
	... if $e(w,$l(w),$l(w))="." do
	.... w $p(w,".",1),!
	... else w w," "
	.. read line if '$test w !! break
	.. if line="" w ! break

	write !
#	.... w:$p(line," ",i+1)="" D,"^ "

