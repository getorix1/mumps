#!/usr/bin/mumps
# clustertt.mps 
# Copyright 2014 Kevin C. O'Kane

# Feb 15, 2022

	zmain

 kill ^clstr
 kill ^x

 if '$data(%1) set min=.1
 else set min=%1

 open 1:"TTtmp.tmp,new"
 use 1
 for w1=$order(^tt(w1)) do
 . for w2=w1:$order(^tt(w1,w2)):"" do
 .. if ^tt(w1,w2)<min quit
 .. write ^tt(w1,w2)," ",w1," ",w2,!
 close 1

 shell sort -n -r < TTtmp.tmp > TTtmp.sorted.tmp

 open 1:"TTtmp.sorted.tmp,old"

 set c=1
 for  do
 . use 1
 . read a  // correlation word1 word2
 . if '$test break
 . set score=$p(a," ",1)
 . set w1=$p(a," ",2)
 . set w2=$p(a," ",3)
 . if w1=w2 quit
 . set f=1

# ^x() is a two dimensional array that contains, at the second level,
# a list of clusters to which the word (w1) belongs
# ^cluster() is the cluster matrix.  Each row (s) is a cluster
# numbered 1,2,3 ... The second level is a list of the words
# in the cluster. 

# The following
# code runs thru all the clusters first for w1 (w1) and
# adds w2 to those clusters w1 belongs to.  It
# repeats the process for w2.  If a word pair are not
# assigned to some cluster (f=1), they are assigned to a new
# cluster and the cluster number is incremented (c)

 . if $d(^x(w1)) for s=$order(^x(w1,s)) do
 .. set ^clstr(s,w2)=""
 .. set ^x(w2,s)=""
 .. set f=0

 . if $d(^x(w2)) for s=$order(^x(w2,s)) do
 .. set ^clstr(s,w1)=""
 .. set ^x(w1,s)=""
 .. set f=0

 . if f do
 .. set ^clstr(c,w1)="" set ^x(w1,c)=""
 .. set ^clstr(c,w2)="" set ^x(w2,c)=""
 .. set c=c+1

# print the clusters

 close 1
 use 5
 write "number of clusters: ",c,!!
 for cx=$order(^clstr(cx)) do
 . write "cluster: ",cx,!
 . for w1=$order(^clstr(cx,w1)) do
 .. use 5 write w1," "
 . write !!

 halt

 kill ^dt
 kill ^dtt

 for d=$order(^doc(d)) do
 . for w=$order(^doc(d,w)) do
 .. if $data(^x(w)) do
 ... for c=$order(^x(w,c)) do
 .... if $data(^dt(d,c)) set ^dt(d,c)=^dt(d,c)+1
 .... else  set ^dt(d,c)=1

 for d=$order(^dt(d)) do
 . for c=$order(^dt(d,c)) do
 .. for w=$order(^clstr(c,w)) do
 ... if $data(^dtt(d,w)) set ^dtt(d,w)=^dtt(d,w)+1
 ... else  set ^dtt(d,w)=1

 for d=$order(^dtt(d)) do
 . write ^title(d),!
 . for w=$order(^dtt(d,w)) write w," "
 . write !

