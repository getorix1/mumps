#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Internet Radio Automation & Encoding Toolkit
#+
#+     Copyright (C) 2018, 2019, 2020, 2021 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
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
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#       May 27, 2024

dep	=	/usr/bin/mumps

check:	backref checkout fcns for1 for2 for3 globals1 go1 kill \
	lclOrder new01 new02 new03 ops qtst remote s16a s6a \
	s6b s6c s6d s6 zfunctions pieceTests extractTests interp2 \
	interp1 numTests

chop : chop.mps $(dep)
	mumpsc chop.mps

numTests : numTests.mps $(dep)
	mumpsc numTests.mps

interp1 : interp1.mps $(dep)
	mumpsc interp1.mps

interp2 : interp2.mps $(dep)
	mumpsc interp2.mps

backref : backref.mps $(dep)
	mumpsc backref.mps

checkout : checkout.mps $(dep)
	mumpsc checkout.mps

pieceTests : pieceTests.mps $(dep)
	mumpsc pieceTests.mps

extractTests : extractTests.mps $(dep)
	mumpsc extractTests.mps

fcns : fcns.mps $(dep)
	mumpsc fcns.mps

for1 : for1.mps $(dep)
	mumpsc for1.mps

for2 : for2.mps $(dep)
	mumpsc for2.mps

for3 : for3.mps $(dep)
	mumpsc for3.mps

globals1 : globals1.mps $(dep)
	mumpsc globals1.mps

go1 : go1.mps $(dep)
	mumpsc go1.mps

kill : kill.mps $(dep)
	mumpsc kill.mps

lclOrder : lclOrder.mps $(dep)
	mumpsc lclOrder.mps

new01 : new01.mps $(dep)
	mumpsc new01.mps

new02 : new02.mps $(dep)
	mumpsc new02.mps

new03 : new03.mps $(dep)
	mumpsc new03.mps

ops : ops.mps $(dep)
	mumpsc ops.mps

qtst : qtst.mps $(dep)
	mumpsc qtst.mps

remote : remote.mps $(dep)
	mumpsc remote.mps

s16a : s16a.mps $(dep)
	mumpsc s16a.mps

s6a : s6a.mps $(dep)
	mumpsc s6a.mps

s6b : s6b.mps $(dep)
	mumpsc s6b.mps

s6c : s6c.mps $(dep)
	mumpsc s6c.mps

s6d : s6d.mps $(dep)
	mumpsc s6d.mps

s6 : s6.mps $(dep)
	mumpsc s6.mps

zfunctions : zfunctions.mps $(dep)
	mumpsc zfunctions.mps


clean:
	rm -f *.tmp
	rm -f *.txt
	rm -f *.cpp
	rm -f *.o
	rm -f backref checkout fcns for1 for2 for3 globals1 go1 kill \
		lclOrder new01 new02 new03 ops qtst remote s16a s6a \
		s6b s6c s6d s6 zfunctions
	rm -f key.dat data.dat nohup.out
	rm -r -f segment*
	rm -f *.orig
