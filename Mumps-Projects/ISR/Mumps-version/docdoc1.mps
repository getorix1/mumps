#!/usr/bin/mumps
# Copyright 2014, 2022 Kevin C. O'Kane

# Feb 23, 2022

	zmain

 kill ^dd
 kill ^vx

 if '$data(%1) write "Missing parameter",! halt

 if '$d(%1) set min=5
 else  set min=%1

 if '$d(%2) set cmin=.5
 else  set cmin=%2

 open 1:"dd.tmp,new" 

 use 1

# ^vx(x) is the count of words in doc x

 for d=$o(^doc(d)) do
 . set i=0
 . for w=$o(^doc(d,w)) w w," ",d,! set i=i+1
 . set ^vx(d)=i

 u 5
 c 1

 shell sort < dd.tmp > dda.tmp
 shell join -1 1 -2 1 dda.tmp dda.tmp > ddc.tmp
 shell ./chop.mps < ddc.tmp > ddb.tmp
 shell sort ddb.tmp > dde.tmp
 shell uniq -c < dde.tmp > ddf.tmp
 shell sort -nr < ddf.tmp > dd1.tmp

 open 1:"dd1.tmp,old"

# d1 is doc 1
# d1 is doc 2
# c is the number of terms in common

 open 2:"docdoc1.tmp,new"

 for  do 
 . use 1 read a 
 . if '$test break 
 . set a=$zblanks(a) 
 . set c=+a 
 . if c<min break 
 . set d1=$piece(a," ",2) 
 . set d2=$piece(a," ",3) 
 . if d1=d2 quit
 . set v1=^vx(d1)
 . set v2=^vx(d2)
 . set c=c/$zsqrt(v1*v2)
 . if c<cmin quit
 . set c=$j(c,1,3)
 . u 2 w d1," ",d2," ",c,!
 . set ^dd(d1,d2)=c
 . set ^dd(d1,d2)=c

 use 5
 close 2 
 close 1 
