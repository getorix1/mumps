#!/usr/bin/mumps
# hyperCluster.mps
# Copyright 2014, 2022 Kevin C. O'Kane

# Feb 25, 2022

	zmain

 kill ^hc
 set c=0,k=0

 if '$data(%1) write "Missing parameter",! halt
 if '$data(%2) write "Missing parameter",! halt

 set min=%1
 set wgt=%2

 if min="" set min=0.4
 if wgt="" set wgt=.4

#     read the level one clusters and build
#     centroid vectors

 for  do
 . read a
 . if '$test break
 . if a="" quit
 . set t=$p(a," ",1)
 . if t="cluster:" do  quit
 .. for w=$order(^hc(c,w)) set ^hc(c,w)=^hc(c,w)/k
 .. set c=c+1,k=0 
 .. quit
 . for w=$order(^doc(t,w)) do
 .. if $data(^hc(c,w)) set ^hc(c,w)=^hc(c,w)+^doc(t,w)
 .. else  set ^hc(c,w)=^doc(t,w)
 .. set k=k+1

 for i=1:1:c for w=$order(^hc(i,w)) if ^hc(i,w)<wgt kill ^hc(i,w)

#     write centroid vectors

 write !,"Centroid vectors",!!
 for i=1:1:c do
 . write i," "
 . for w=$order(^hc(i,w)) write w," (",$j(^hc(i,w),3,2),") "
 . write !!

 open 1:"hypertmp.tmp,new"

#     calculate cluster similarities

 write !!,"Cluster similarities:",!
 for i=1:1:c do
 . for j=i+1:1:c do
 .. s x=$zzCosine(^hc(i),^hc(j))
 .. if x<min quit
 .. use 5 write i," ",j," ",x,!
 .. use 1 write x," ",i," ",j,!

 use 5
 close 1

 kill ^clstr
 kill ^x

 shell sort -n -r < hypertmp.tmp > hypertmp.sorted.tmp

 open 1:"hypertmp.sorted.tmp,old"
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

 . if $d(^x(seq1)) for s=$order(^x(seq1,s)) do
 .. set ^clstr(s,seq2)=""
 .. set ^x(seq2,s)=""
 .. set f=0

 . if $d(^x(seq2)) for s=$order(^x(seq2,s)) do
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
 kill ^h


 for cx=$order(^clstr(cx)) do
 . open 1:"cterms.tmp,new"
 . for w=$order(^hc(cx,w)) do
 .. u 1 w ^hc(cx,w)," ",w,!
 . close 1
 . shell sort -nr <cterms.tmp > ctermssorted.tmp
 . set a=""
 . open 1:"ctermssorted.tmp,old"
 . u 1 for i=1:1:9 r x q:'$t  set a=a_" "_$o(^stem($p(x," ",2),""))
 . set ^clstr(cx)=a
 . close 1

 u 5

 write !!,"Number of clusters: ",c-1,!!
 for cx=$order(^clstr(cx)) do
 . write "cluster: ",cx,!
 . set ^h(cx)=^clstr(cx)
 . for seq1=$order(^clstr(cx,seq1)) do
 .. write "base cluster=",seq1,!
 .. set ^h(cx,seq1)="Cluster "_seq1
 .. for cz=$order(^ct(seq1,cz)) do
 ... write seq1,?8,$e(^title(cz),1,70),!
 ... set ^h(cx,seq1,cz)=""
 . write !
