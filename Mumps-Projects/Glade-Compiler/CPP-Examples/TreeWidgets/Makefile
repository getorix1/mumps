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
#+      August 24, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


SHELL = /bin/bash

GCC = gcc
CPP = g++
MUMPSC = mumpsc

dep =	/usr/bin/mumps mumps.glade gtk.cpp

all:	GtkCompiler gtk

GtkCompiler:	../../GtkCompiler.mps $(dep)
#	rm -f on.*
	rm -f key.dat data.dat
	../../GtkCompiler.mps "CPP" < mumps.glade
	./BuildMeshTree.mps > mtree

gtk:	gtk.cpp $(dep)
	$(CPP) -O3 -fdiagnostics-color=never -fpermissive \
		-pthread `pkg-config --cflags --libs glib-2.0` \
		-o gtk gtk.cpp `pkg-config --libs glib-2.0` -lpthread \
		`pkg-config --cflags --libs gtk+-3.0` -export-dynamic
clean:
	rm -f *.tmp
	rm -f key.dat data.dat
	rm -f *.o
	rm -f nohup.out mtree

