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

#+	Jan 15, 2022

#	'tree' is the global array reference to load.
#	It may be a top level array reference or
#	a node within the global array.
#	treeStore is assumed to be empty.
#	only 100 rows are loaded

        for i=1:1:100 do
        . set tree=$query(@tree)
        . if tree="" break
	. set %=$z~mdh~tree~level~add(treeStore,1,tree,@tree,"null")
