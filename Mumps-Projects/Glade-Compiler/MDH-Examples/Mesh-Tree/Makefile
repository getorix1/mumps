#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     GTK Compiler
#+
#+     Copyright (C) 2022 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     okane@uni.edu
#+     https://www.cs.uni.edu/~okane
#+     https://threadsafebooks.com/
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
#+ August 24, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


SHELL = /bin/bash

CPP = g++
MUMPSC = mumpsc

dep =	/usr/bin/mumps mumps.glade gtk.cpp

all:	GtkCompiler gtk BuildMeshTree

GtkCompiler:	../../GtkCompiler.mps $(dep)
	rm -f data.dat key.dat
	../../GtkCompiler.mps "MDH" < mumps.glade

gtk:	gtk.cpp $(dep)
	$(MUMPSC) gtk.cpp

BuildMeshTree:	BuildMeshTree.cpp $(dep)
	$(MUMPSC) BuildMeshTree.cpp
	rm -f key.dat data.dat
	./BuildMeshTree

clean:
	rm -f *.tmp
	rm -f key.dat data.dat
	rm -f *.o
	rm -f key.dat data.dat nohup.out 

