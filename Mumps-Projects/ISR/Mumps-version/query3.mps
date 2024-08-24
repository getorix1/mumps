#!/usr/bin/mumps
# query3.mps Copyright 2014 Kevin C. O'Kane

 kill ^tmp

 write "Enter search terms:",!
 set i=$zzInput("w")-1
 for j=0:1:i do
 . set a=$zlower(w(j))
 . set a=$zstem(a)
 . set query(a)=""

 if $order(query(""))="" halt

 set t1=$zd1

 for w=$order(query(w)) do
 . for i=$order(^index(w,i)) do
 .. if $data(^tmp(i)) set ^tmp(i)=^tmp(i)+^index(w,i)
 .. else  set ^tmp(i)=^index(w,i)

 set fn=$job_",new"
 open 1:fn
 use 1
 for i=$order(^tmp(i)) do
 . use 1 write $justify(^tmp(i),6,2)," ",^title(i),!
 close 1
 use 5
 set cmd="sort -n <"_$job_" | tail -10 ; rm "_$job
 shell &~cmd~

 write !,"Elapsed time: ",$zd1-t1,!
 halt
