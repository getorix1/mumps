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
#+ August 1, 2022
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


SHELL = /bin/sh

CPP = g++

dep =	/usr/bin/mumps mumps.glade gtk.mps

all:	GtkCompiler gtk

GtkCompiler:	../../GtkCompiler.mps $(dep)
	rm -f key.dat data.dat
	rm -f on*.mps
	../../GtkCompiler.mps "MUMPS" < mumps.glade

gtk:	gtk.mps $(dep)
	mumpsc gtk.mps

clean:
	rm -f *.tmp
	rm -f *.o
	rm -f key.dat data.dat nohup.out 

