#!/usr/bin/mumps
# discrim3.mps 
# Copyright 2014, 2022 Kevin C. O'Kane

# Feb 14, 2022

	zmain

 set D=^DocCount(1)  // number of documents
 set sq=0
 kill ^mca

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# calculate centroid vector ^c() for entire collection and
# the sum of the squares (needed in cos calc but should only be done once)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 for w=$order(^dict(w)) do
 . set ^c(w)=^dict(w)/D   // centroid is composed of avg word usage
 . set sq=^c(w)**2+sq     // The sum of the squares is needed below.

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Calculate total similarity of doc for all words (T) space by
# calculating the sum of the similarities of each document with the centroid.
# Remember and store contribution of each document in ^dc(dn).
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 set T=0
 for i=$order(^doc(i)) do
 . set x=0
 . set y=0

 . for w=$order(^doc(i,w)) do
 .. set d=^doc(i,w)
 .. set x=d*^c(w)+x                       // numerator of cos(c,doc) calc
 .. set y=d*d+y                           // part of denominator

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Calculate and store the cos(c,doc(i)).
# Remember in ^dc(i) the contribution that this document made to the total.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 . if y=0 quit
 . set ^dc(i)=x/$zsqrt(sq*y)              // cos(c,doc(i))
 . set T=^dc(i)+T                         // sum the cosines

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# calculate similarity of doc space with words removed
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 for W=$order(^dict(W)) do

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# For each document containing W, calculate sum of the contribution
# of the cosines of these documents to the total (T).  ^dc(i) is
# the original contribution of doc i.  Sum of contributions is stored in T1.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 . set T1=0,T2=0
 . for i=$order(^index(W,i)) do            // row of doc nbrs for word
 .. set T1=^dc(i)+T1                       // use prevsly calc'd cos

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# For each word in document i, recalculate cos(c,doc) but without word W
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 .. set x=0
 .. set y=0
 .. for w=$order(^doc(i,w)) do
 ... if w'=W do                          // if W not w
 .... set d=^doc(i,w)
 .... set x=d*^c(w)+x                   //  d*^c(w)+x
 .... set y=d**2+y

 .. if y=0 quit
 .. set T2=x/$zsqrt(sq*y)+T2               // T2 sums cosines without W

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# subtract original contribution with W (T1) and add contribution
# without W (T2) and calculate r - the change, and store in ^mca(W)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#     if old (T1) big and new (T2) small, density declines

 . set r=T2-T1*10000\1
 . write $j(r,6)," ",W," ",^idf(W),!

 use 5
 halt
