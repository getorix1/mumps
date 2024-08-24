#!/usr/bin/mumps
# Copyright 2014, 2022, 2023, 2024 Kevin C. O'Kane

# reformat.mps June 1, 2024
# Wikipedia file version

	zmain

	set M=$zgetenv("in_docs")
	if M="" set M=1000000
	set M=1000000
	write "number of docs: ",M,!
	open 1:"../titles.data,new"
	kill ^map

	set D=0

	for  do  if D>M quit
	. read line if '$test break
	. set line=$zlower(line)
	. if $p(line,"""",2)="id" do
	.. write D," ",$p(line,"""",4),!
	. if $p(line,"""",2)="text" do
	.. write $p(line,"""",4)," "
	. if $p(line,"""",2)="title" do
	.. write $p(line,"""",4),!
	.. use 1
	.. write D," ",$p(line,"""",4),!
	.. use 5
	.. set D=D+1


	write !
	close 1
