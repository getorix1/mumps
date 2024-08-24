#!/usr/bin/mumps

#+ Copyright (C) 2022 by Kevin C. O'Kane
#+
#+ Kevin C. O'Kane, Ph.D.
#+ Professor Emeritus
#+ Computer Science Department
#+ University of Northern Iowa
#+ Cedar Falls, IA 50614-0507
#+ kc.okane@gmail.com
#+ http://www.cs.uni.edu/~okane
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

#+	Jan 24, 2022

#	'tree' is the global array reference to load.
#	It may be a top level array reference or
#	a node within the global array.
#	treeStore is assumed to be empty.

#	how many indices in tree? (depth)

        set tmp=$query(@tree)   ; get the next node
        set depth=$qlength(tmp) ; how many indices in next node?

#	if tree reference greater that top level, add each
#	node of original query to tree.

	if depth>1 do
	. set tmp=tree          ; hold for future reference
	. for j=1:1:depth-1 do  ; next node depth
	.. set %=$z~mdh~tree~level~add(treeStore,j,tree,@tree)

#	set %=$z~mdh~tree~level~add(treeStore,treeDepth,col1,col2,col3)

        for  do
        . set tree=$query(@tree)
        . if tree="" break
        . set i=$qlength(tree)
	. set %=$z~mdh~tree~level~add(treeStore,i,tree,@tree,$qsubscript(tree,i))
	. if depth>1&(i=(depth-1)) if tmp'=tree break ; we've move beyond the query
