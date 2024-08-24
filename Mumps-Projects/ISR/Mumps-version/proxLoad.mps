#!/usr/bin/mumps
# ~/Medline2012/proxLoad.mps 
# Copyright 2014, 2022 Kevin C. O'Kane

# Feb 15, 2022

	zmain

	open 1:"phrases.txt,new"

# input format: #co-occur w1 w2 direction

	for  do
	. use 5
	. read a
	. if '$test break
	. set wgt=$piece(a," ",1)
	. set w1=$piece(a," ",2)
	. set w2=$piece(a," ",3)
	. set dir=$piece(a," ",4)
	. set s=$zlog(wgt)
	. use 1
	. for d=$order(^index(w1,d)) do
	.. if $data(^index(w2,d)) do
	... set w1stem=w1
	... set w2stem=w2
	... if $data(^stem(w1)) set w1=$order(^stem(w1,""))
	... if $data(^stem(w2)) set w2=$order(^stem(w2,""))
	... if dir<0 set prs=w2_"-"_w1
	... else  set prs=w1_"-"_w2
	... write d," ",s," ",prs,!
	... set ^dict(prs)=wgt
	... set ^df(prs)=wgt
	... set ^phrase(w1stem,prs)=""
	... set ^phrase(w2stem,prs)=""
	... set ^doc(d,prs)=s
	... set ^idf(prs)=s
	... set ^index(prs,d)=s

	close 1
