#!/usr/bin/mumps
# query2.mps Copyright 2014 Kevin C. O'Kane

 write "Enter search terms:",!

 set i=$zzInput("w")-1
 for j=0:1:i do
 . set a=$zlower(w(j))
 . set a=$zstem(a)
 . set query(a)=""

 set t1=$zd1

 for w=$order(query(w)) do
 . for d=$order(^index(w,d)) do
 .. write d,?8," ",^title(d),!

 write !,"Time elapsed: ",$zd1-t1,!
