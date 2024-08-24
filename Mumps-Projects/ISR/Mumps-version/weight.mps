#!/usr/bin/mumps
# weight.mps 

# Feb 23, 2022

	zmain
 declare n1,d,w,delwords,x,deldocs

 if '$d(%1) set m1=10.
 else  set m1=%1

 if '$d(%2) set m2=10.
 else  set m2=%2

 kill ^index

 set delwords=0
 set deldocs=0

 open 2:"document-term-matrix-weighted.txt,new"

 for d=$order(^doc(d)) do
 . use 2 write !,"doc=",d,?15
 . use 2 write ^title(d),!,?15
 . for w=$order(^doc(d,w)) do
 .. set x=^idf(w)*^doc(d,w)
 .. if x<m1 kill ^doc(d,w) set delwords=delwords+1 quit
 .. set ^doc(d,w)=x
 .. set ^index(w,d)=x
 .. write w,"(",x,") "
 . write !
 close 2

 for d=$o(^doc(d)) do
 . set i=0
 . for w=$o(^doc(d,w)) do
 .. set i=i+1
 . if i<m2 kill ^doc(d) set deldocs=deldocs+1

 open 2:"term-document-matrix-weighted.txt,new"
 use 2
 for w=$order(^index(w)) do
 . write w,?26
 . for d=$order(^index(w,d)) do
 .. write d,"(",^index(w,d),") "
 . write !

 use 5
 close 2

 write ?8,delwords," document word instances with weight < ",m1," deleted ",!
 write ?8,deldocs," docs with less than ",m2," words deleted",!!

