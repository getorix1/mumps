#!/usr/bin/mumps
# idf-cutoff.mps Feb 14, 2014
# Copyright 2014 Kevin C. O'Kane

# Feb 22, 2022

	zmain

	if '$data(%1) write "Missing parameter",! halt

	set wmin=%1
	set x=0

	open 3:"idf-deleted-words.txt,new"

	use 3

	for w=$order(^dict(w)) do
	. if $data(^idf(w)) quit
	. write w,!
	. kill ^idf(w) kill ^mca(w) kill ^dict(w)

	close 3
	use 5

	for d=$order(^doc(d)) do
	. for w=$order(^doc(d,w)) do
	.. if '$data(^idf(w)) kill ^doc(d,w),^index(w,d)

	open 3:"idf-deleted-documents.txt,new"

	use 3

	for d=$order(^doc(d)) do
	. set z=0
	. for y=$order(^doc(d,y)) set z=z+1
	. if z<wmin do
	.. for y=$o(^doc(d,y)) kill ^doc(d,y),^index(y,d)
	.. kill ^doc(d)
	.. write "Doc ",d," deleted count=",z,!
	.. set x=x+1

	close 3
	use 5

	for w=$order(^index(w)) if $order(^index(w,""))="" kill ^index(w)

	write !,?8,"IDF total docs deleted: ",x,!

	set D=0
	for d=$order(^doc(d)) set D=D+1
	set ^DocCount(1)=D
	write ?8,"IDF remaining documents: ",^DocCount(1),!

	open 1:"DocCount,new" use 1 write D,! close 1 use 5

	open 1:"idfWeights.tmp,new"

	use 1

	set wcount=0

	for w=$order(^idf(w)) do
	. write ^idf(w)," ",w,!
	. set wcount=wcount+1

	use 5
	close 1

	write ?8,"IDF number of words remaining: ",wcount,!
	halt
