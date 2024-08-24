#!/usr/bin/mumps
# map.mps December 11, 2014
# Copyright 2014 Kevin C. O'Kane

	zmain

 set M=$zgetenv("max_docs")
 if M="" set M=1000000
 open 1:"titles.list,new"
 kill ^map

 set D=0

 for  do  if D>M quit
 . use 5
 . set o=$ztell read line
 . if '$test break
 . if $extract(line,1,2)="TI" do  quit
 .. set D=D+1
 .. use 1 write D," ",$extract(line,7,200),!
 .. use 5
 .. set ^map(D)=off
 .. quit
 . if $extract(line,1,2)="MH" quit
 . if $extract(line,1,13)="STAT- MEDLINE" set off=o quit
 . if $extract(line,1,2)'="AB" quit
 . for  do
 .. read line
 .. if '$test break
 .. if line="" break

 close 1
