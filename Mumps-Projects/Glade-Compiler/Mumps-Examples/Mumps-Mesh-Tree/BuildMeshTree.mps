#!/usr/bin/mumps
# ~/Medline2012/Mesh/BuildMeshTree.mps Dec 30, 2021
# Copyright 2014, 2021 Kevin C. O'Kane

	kill ^mesh

	open 1:"mtrees2003.txt,old"

	if '$test write "mtrees2003.txt not found",! halt
	for  do
	. use 1
	. read a
	. if '$test break
	. set key=$piece(a,";",1)  // text description
	. set code=$piece(a,";",2) // everything else
	. if key=""!(code="") break

	. for i=1:1 do
	.. set x(i)=$piece(code,".",i)  // extract code numbers
	.. if x(i)="" break

	. set i=i-1
	. use 5
	. set z="^mesh("        // begin building a global reference

#-----------------------------------------------------------------------
#     build a reference like ^mesh("A01","047","025","600)
#     by concatenating quotes, codes, quotes, and commas onto z
#-----------------------------------------------------------------------

	. for j=1:1:i-1 set z=z_""""_x(j)_""","
	. set z="set "_z_""""_x(i)_""")="""_key_""""

#-----------------------------------------------------------------------
#     z now looks like set ^mesh("A01","047")="Abdomen"
#     now execute the text
#-----------------------------------------------------------------------

#	. write z,!
	. xecute z

	close 1
	use 5
	write "done",!
	quit
