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
#+      August 27, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


SHELL = /bin/bash

GCC = gcc

dep =	/usr/bin/mumps mumps.glade gtk.c

all:	GtkCompiler gtk

GtkCompiler:	../../GtkCompiler.mps $(dep)
	rm -f on.*
	rm -f key.dat data.dat
	rm -f gtk.css
	ln -s ../../gtk.css gtk.css
	../../GtkCompiler.mps "C" < mumps.glade

gtk:	gtk.c $(dep)
	$(GCC) -O3 -fdiagnostics-color=never \
		-pthread `pkg-config --cflags --libs glib-2.0` \
		-o gtk gtk.c `pkg-config --libs glib-2.0` -lpthread \
		`pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean:
	rm -f *.tmp
	rm -f key.dat data.dat
	rm -f *.o
	rm -f nohup.out 

