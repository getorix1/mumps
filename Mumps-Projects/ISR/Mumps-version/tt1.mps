#!/usr/bin/mumps
# Copyright 2014, 2022 Kevin C. O'Kane
# tt1.mps Feb 15, 2022

	zmain

 if '$d(%1) set min=0.25
 else  set min=%1

 if '$d(%2) set min1=5
 else  set min1=%2

 open 1:"tt.tmp,new"

 use 1
 for d=$order(^doc(d)) do
 . for w=$order(^doc(d,w)) do
 .. for w1=w:$order(^doc(d,w1)):"" do
 ... if w1=w quit
 ... write w," ",w1,!

 close 1

 shell sort < tt.tmp | uniq -c | sort -nr -k 1 > tt.sorted.tmp

 open 1:"tt.sorted.tmp,old"
 open 2:"tt.tmp,new"

# input format: #co-occur w1 w2
# output format: relationship w1 w1 #co-occur ^df(w1) ^df(w2)

 kill ^tt

# input format: co-occur w1 w2 
# out format: wgt w1 w2 co-occur df(w1) df(w2)

 for  do
 . use 1
 . read a
 . if '$test break
 . set a=$zblanks(a)
 . set w1=$piece(a," ",2)
 . set w2=$piece(a," ",3)
 . set co=+a
 . if co<min1 quit
 . set c1=co/(^df(w1)+^df(w2)-co)
 . if c1<min quit
 . set c1=$j(c1,1,3)
 . set ^tt(w1,w2)=c1
 . set ^tt(w2,w1)=c1
 . use 2
 . write ^tt(w1,w2)," ",w1," ",w2," ",co," ",^df(w1)," ",^df(w2),!

 close 1,2
 use 5
 shell sort -n < tt.tmp > term-term-matrix-sorted.txt
