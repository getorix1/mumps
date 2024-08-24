#!/usr/bin/mumps
# clusterdd.mps 
# Copyright 2014, 2022 Kevin C. O'Kane

# Feb 25, 2022

	zmain

 kill ^clstr
 kill ^clv
 kill ^x
 kill ^ct
 kill ^dc

 if '$data(%1) write "Missing parameter",! halt

 set min=%1
 if min="" set min=0.3

 open 1:"ddtmp.tmp,new"
 use 1
 for d1=$order(^dd(d1)) do
 . for d2=d1:$order(^dd(d1,d2)):"" do
 .. if ^dd(d1,d2)<min quit
 .. write ^dd(d1,d2)," ",d1," ",d2,!
 close 1
 shell sort -n -r < ddtmp.tmp > ddtmp.sorted.tmp
 open 1:"ddtmp.sorted.tmp,old"
 set c=1
 for  do
 . use 1
 . read a  // correlation doc1 doc2
 . if '$test break
 . set score=$p(a," ",1)
 . set seq1=$p(a," ",2)
 . set seq2=$p(a," ",3)
 . if seq1=seq2 quit
 . set f=1

# ^x() is a two dimensional array that contains, at the second level,
# a list of clusters to which the document number (seq1) belongs
# ^cluster() is the cluster matrix.  Each row (s) is a cluster
# numbered 1,2,3 ... The second level is a list of the document
# numbers of those documents in the cluster. The following
# code runs thru all the clusters first for doc1 (seq1) and
# adds seq2 (doc2) to those clusters doc1 belongs to.  It
# repeats the process for seq2 (doc2).  If a doc pair are not
# assigned to some cluster (f=1), they are assigned to a new
# cluster and the cluster number is incremented (c)

 . if $d(^x(seq1)) for s="":$order(^x(seq1,s)):"" do
 .. set ^clstr(s,seq2)=""
 .. set ^x(seq2,s)=""
 .. set f=0

 . if $d(^x(seq2)) for s="":$order(^x(seq2,s)):"" do
 .. set ^clstr(s,seq1)=""
 .. set ^x(seq1,s)=""
 .. set f=0

 . if f do
 .. set ^clstr(c,seq1)="" set ^x(seq1,c)=""
 .. set ^clstr(c,seq2)="" set ^x(seq2,c)=""
 .. set c=c+1

# print the clusters

 close 1
 use 5
 open 1:"document-clusters.txt,new"
 open 2:"document-clusters-table.txt,new"

 for cx=$order(^clstr(cx)) do
 . set mx=0
 . for d1=$order(^clstr(cx,d1)) do   // doc in cluster
 .. set x=0
 .. for w=$o(^doc(d1,w)) do
 ... set x=x+^idf(w)
 .. if x>mx set mx=x,dx=d1
 .. set ^ct(cx,d1)=""
 .. if $data(^dc(d1,cx)) set ^dc(d1,cx)=^dc(d1,cx)+1
 .. else  set ^dc(d1,cx)=1
 . u 1 w cx," ",mx," "
 . u 2 w "cluster: ",cx,!
 . u 1 write $e(^title(dx),1,70),!!
 . set ^clstr(cx)=^title(dx)
 . open 3:"tmp,new"
 . for w=$o(^doc(dx,w)) do
 .. use 1 write w,"(",^doc(dx,w),") " if $x>60 w !
 .. use 3 write ^doc(dx,w)," ",w,!
 . u 1 write !!
 . u 3 write ! close 3
 . shell sort -nr < tmp > tmp1
 . open 3:"tmp1,old"
 . for  do
 .. u 3 read t if '$t break
 .. u 1 w t," "
 . c 3
 . u 1 w !!
 . for d1=$order(^clstr(cx,d1)) do
 .. u 1 write d1," ",$e(^title(d1),1,70),!
 .. u 2 write d1," ",$e(^title(d1),1,70),!
 . u 1 w "-----------------",!!
 . u 2 w !

 close 1
 close 2

 open 1:"document-clusters-by-title.txt,new"
 use 1
 for d=$order(^dc(d)) do
 . write d," ",^title(d),!,?10
 . for c=$order(^dc(d,c)) do
 .. write c," "
 . write !

 close 1
 use 5
 
