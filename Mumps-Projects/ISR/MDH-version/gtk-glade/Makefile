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
#+      August 25, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SHELL = /bin/bash

GCC = gcc

all:	gtk

gtk:	gtk.cpp mumps.glade
	rm key.dat data.dat
	../../../Glade-Compiler/GtkCompiler.mps "CPP" < mumps.glade
	mumpsc gtk.cpp
	rm key.dat data.dat
	ln -s ../data.dat data.dat
	ln -s ../key.dat key.dat

clean:
	rm -f *.tmp
	rm -f *.o
	rm -f nohup.out 

