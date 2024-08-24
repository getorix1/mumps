#!/usr/bin/mumps

	zmain

# Copyright 2014 Kevin C. O'Kane

# ttt.mps Feb 12, 2022

 kill ^ttt

 for w1=$order(^tt(w1)) do
 . for w2=$order(^tt(w1,w2)) do
 .. for w3=$order(^tt(w2,w3)) do
 ... if w1=w3 break
 ... if '$data(^ttt(w1,w3)) set ^ttt(w1,w3)=1
 ... else set ^ttt(w1,w3)=^ttt(w1,w3)+1

 open 1:"term-term-term.txt,new"
 use 1
 for w1=$order(^ttt(w1)) do
 . for w2=$order(^ttt(w1,w2)) do
 .. write ^ttt(w1,w2)," ",w1," ",w2,!
 close 1
