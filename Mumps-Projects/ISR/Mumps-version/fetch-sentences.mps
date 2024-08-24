#!/usr/bin/mumps
# Copyright 2014, 2022, 2023, 2024 Kevin C. O'Kane

# fetch-sentences.mps April 14, 2024

	zmain

	w "$$$ ",%1,!
	set %=$zseek(^map(%1))

	for  do
	. read line if '$t break
	. if $extract(line,1,2)="TI" do  quit
	.. set line=$zlower($e(line,7,2048))
	.. set line=$zblanks(line)
	.. for i=1:1 do
	... s w=$p(line," ",i) if $l(w)=0 break
	... if $l(w)<3 quit
	... set w=$translate(w,"0123456789""`~!@#$%^&*()_+=?><,:;'{[}]")
	... w w," "
	.. w !
	. if $extract(line,1,2)="MH" quit
	. if $extract(line,1,13)="STAT- MEDLINE" write ! quit
	. if $extract(line,1,2)'="AB" quit
	. for  do
	.. set line=$zlower($e(line,7,2048))
	.. set line=$zblanks(line)
	.. for i=1:1 do
	... set w=$p(line," ",i)
#	... set w=$translate(w,"0123456789""`~!@#$%^&*()_+=?><,:;'{[}]")
	... if $l(w)=0 break
#	... if $l(w)<3 quit
	... if $e(w,$l(w),$l(w))="." do
	.... w $p(w,".",1),!
	... else w w," "
	.. read line if '$test w !! break
	.. if line="" w ! halt


