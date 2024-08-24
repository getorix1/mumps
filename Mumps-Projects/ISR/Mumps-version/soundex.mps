#!/usr/bin/mumps
# ~/Medline2012/stems.mps Nov 11, 2014
# Copyright 2014 Kevin C. O'Kane
# convert data base to word stems
# August 26, 2022

	zmain

	kill ^sndx

	for w=$order(^dict(w)) do
	. set sndx=$zzsoundex(w)
	. set ^sndx(sndx,w)=""

	for x=$order(^sndx(x)) do
	. write x,": "
	. for w=$order(^sndx(x,w)) write " ",w
	. write !!
