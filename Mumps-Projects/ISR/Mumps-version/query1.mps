#!/usr/bin/mumps
# query1.mps Copyright 2014, 2022 Kevin C. O'Kane

 zmain

 write !,"Enter search terms:",!

 read line

 if $len(line)=0 halt

 write !

 kill ^query

 for i=1:1 do
 . set w=$piece(line," ",i)
 . if $len(w)=0 break
 . set w=$zlower(w)
 . if $data(^stem(w)) set w=$zstem(w)
 . set ^query(w)=""

 set t1=$zd1

 for d=$order(^doc(d)) do
 . for w=$order(^query(w)) do
 .. if $data(^doc(d,w)) write d,?8," ",$extract(^title(d),1,72),! break

 write !,"Time elapsed: ",$zd1-t1,!
 halt
