#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+   
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2004, 2022 by Kevin C. O'Kane  
#+
#+     Kevin C. O'Kane
#+     okane@cs.uni.edu
#+
#+ This program is free software; you can redistribute it and/or modify
#+ it under the terms of the GNU General Public License as published by
#+ the Free Software Foundation; either version 2 of the License, or
#+ (at your option) any later version.
#+ 
#+ This program is distributed in the hope that it will be useful,
#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+ GNU General Public License for more details.
#+ 
#+ You should have received a copy of the GNU General Public License
#+ along with this program; if not, write to the Free Software
#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#+ Nov 8, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	zmain

#	number of sequences
	if '$data(%1) set M=100000
	else  set M=%1

#	starting sequence number
	if $data(%2) set D=%2
	else  set D=0

#	sequences processed
	set MM=0

	open 1:"titles.list,new"
	if '$t write "open error",! halt

#	initial line

	read title
	if '$t halt
	set off=$ztell

	set dna=""

	for  do

	. for  do
	.. use 5
	.. read line
	.. if '$t set eof=1 break
	.. else  set eof=0
	.. if $e(line,1,1)=">" break
	.. if $length(dna)+$length(line)>16000 quit
	.. set dna=dna_line

	. use 1
	. write D," ",$e(title,1,72),!
	. use 5
	. set off=$ztell
	. write off," ",D," "
	. set D=D+1,MM=MM+1
	. set j=0
	. set dna=$zlower(dna)
	. for i=1:1:$length(dna) do
	.. write $e(dna,i,i)
	.. if j#11=10 write " "
	.. set j=j+1
	. write !
	. set dna=""
	. set title=line
	. if eof close 1 halt
	. if MM>M close 1 halt

