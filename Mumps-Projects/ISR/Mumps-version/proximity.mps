#!/usr/bin/mumps
# ~/Medline2012/proximity.mps 
# Copyright 2014, 2022, 2023 Kevin C. O'Kane

# May 12, 2023

	zmain

# ttx term-term correlation matrix
# calculate term-term proximity coefficients within env words

# %1 is the minumum prox score; 
# %2 is the distance facttor
# %3 is the log adjustment factor

 if '$data(%1) write "Missing parameter",! halt
 if '$data(%2) write "Missing parameter",! halt
 if '$data(%3) write "Missing parameter",! halt

 set av=0,avi=0

 kill ^ttp  //* delete any old term-term correlation matrix

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# for each document k, sum the co-occurrences of words i and j
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# for each document k

 declare k,i,j,m,n

 set k=""
 for  do
 . set k=$order(^doc(k))
 . if k="" break

# for each term i in p k

 . set i=""
 . for  do
 .. set i=$order(^doc(k,i))
 .. if i="" break

# for each other term j in doc k

 .. set j=i
 .. for  do
 ... set j=$order(^doc(k,j))
 ... if j="" break

# for each position m of term i in doc k
 ... set m=""
 ... for  do
 .... set m=$order(^doc(k,i,m))
 .... if m="" break

# for each position n of term j in doc k

 .... set n=""
 .... for  do
 ..... set n=$order(^doc(k,j,n))
 ..... if n="" break

# calculate and store weight based on proximity

 ..... set %%=$zabs(m-n)
 ..... if %%<%2 quit
 ..... set dd=$zlog(1/$zabs(m-n)*%3+1)\1
 ..... if dd<1 quit
 ..... set av=av+dd,avi=avi+1
 ..... if '$data(^ttp(i,j)) set ^ttp(i,j)=dd,^ttp(i,j,1)=n-m
 ..... else  set ^ttp(i,j)=^ttp(i,j)+dd,^ttp(i,j,1)=^ttp(i,j,1)+(n-m)

 open 1:"term-term-proximity-similarities.tmp,new"
 use 1

 set i=""
 for  do
 . set i=$order(^ttp(i))
 . if i="" break
 . set j=""
 . for  do
 .. set j=$order(^ttp(i,j))
 .. if j="" break
 .. if ^ttp(i,j)<%1 kill ^ttp(i,j) quit
 .. write ^ttp(i,j)," ",i," ",j," ",^ttp(i,j,1),!

 close 1

 use 5
 write ?8,"Average score = ",av/avi," ",av,"/",avi,!

