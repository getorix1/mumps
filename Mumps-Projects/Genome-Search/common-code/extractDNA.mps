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
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	zmain

	set D=0
#	open 1:"titles.list,new"
	for  do
#	. if D>10000 break
	. read line
	. if '$t break
	. set line=$zblanks(line)
	. set lab=$p(line," ",1)
	. if lab="ACCESSION" set acc=$p(line," ",2) quit
	. if lab="DEFINITION" set def=$p(line," ",2,99) quit
	. if lab="VERSION" set ver=$p(line," ",2) quit
	. if lab="ORIGIN" do
	.. write ">gi|",ver,"| ",acc," ",def,! set off=$ztell use 5
#	.. write off," ",D," " set D=D+1
	.. set max=0
	.. set k=0
	.. for i=1:1 do
	... read line
	... if '$test break
	... if line="//" break
	... set line=$e(line,11,100)
	... write $translate(line," "),!

#	close 1
