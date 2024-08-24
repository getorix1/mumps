#!/usr/bin/mumps
# document-term-matrix.mps 
# Copyright 2017, 2022 Kevin C. O'Kane

# Feb 21, 2022

	zmain

 kill ^df
 kill ^dict
 kill ^doc

 if '$data(%1) write "Missing parameter 1",! halt
 if '$data(%2) write "Missing parameter 2",! halt

 set wmin=%1
 set mindf=%2

 set %=$zStopInit("basic.stop.words")

 set d=0

 open 1:"dict.tmp,new"
 open 2:"doc.tmp,new"
 open 3:"document-term-matrix.txt,new"

 for  do
 . use 5 read line
 . if '$t break
 . set off=$p(line," ",1),doc=$p(line," ",2),^doc(doc)=off
 . for j=3:1 do
 .. set w=$p(line," ",j)
 .. if $l(w)=0 break
 .. if $zStopLookup(w) quit
 .. set ^doc(doc,w,j)=""
 .. use 2 write doc," ",w,!
 .. use 1 write w,!

 use 5
 close 1
 close 2

 shell sort < dict.tmp | uniq -c > dict.sorted.tmp

 open 1:"dict.sorted.tmp,old"
 use 1

 for  do
 . read a
 . if '$test break
 . set a=$zblanks(a)
 . set ^dict($piece(a," ",2))=$piece(a," ",1)

 use 5
 close 1

 shell sort < doc.tmp | uniq -c > doc.sorted.tmp

 open 1:"doc.sorted.tmp,old"

 use 1

 for  do
 . read a
 . if '$test break
 . set a=$zblanks(a)
 . set c=$p(a," ",1)
 . set a2=$piece(a," ",2),a3=$piece(a," ",3)
 . set ^doc(a2,a3)=c
 . set ^index(a3,a2)=c

 use 5
 close 1

 open 1:"df.tmp,new"
 use 1

 for d=$order(^doc(d)) do
 . set c=0
 . for w=$order(^doc(d,w)) set c=c+1
 . if c<wmin do
 .. for w=$order(^doc(d,w)) kill ^index(w,d)
 .. kill ^doc(d)
 . else  do
 .. for w=$order(^doc(d,w)) write w,!

 close 1

 shell sort < df.tmp | uniq -c | sort -nr > df.sorted.tmp

 open 1:"df.sorted.tmp,old"

 use 1
 for  do
 . read a
 . if '$test break
 . set a=$zblanks(a)
 . if +a<mindf break
 . set ^df($piece(a," ",2))=+a

 close 1
 use 5

 for w=$order(^dict(w)) if '$data(^df(w)) kill ^dict(w)

 for d=$order(^doc(d)) do
 . set c=c+1 for w=$order(^doc(d,w)) do
 .. if '$d(^dict(w)) kill ^doc(d,w) quit
 .. set c=c+1
 . if c<wmin do
 .. for w=$order(^doc(d,w)) kill ^index(w,d)
 .. kill ^doc(d)

 set c=0
 for d=$order(^doc(d)) set c=c+1
 set ^DocCount(1)=c

 open 1:"DocCount,new"
 use 1 write c,!

 close 1

# display doc-term matrix

 use 3

 for d=$order(^doc(d)) do
 . write "Doc ",d,": ",$e(^title(d),1,70),!
 . for w=$order(^doc(d,w)) do
 .. write w," (",^doc(d,w),") "
 .. if $x>60 write !
 . write !!

 close 3

# display document frequency vector

 open 1:"term-frequencies.unsorted.tmp,new"
 use 1

 for w=$order(^df(w)) write ^df(w)," ",^dict(w)," ",w,!

 close 1
 use 5

 shell sort -nr < term-frequencies.unsorted.tmp > term-frequencies.txt

 halt
