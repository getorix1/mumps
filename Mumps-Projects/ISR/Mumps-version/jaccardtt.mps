#!/usr/bin/mumps
# jaccard-tt.mps 
# Copyright 2014, 2022 Kevin C. O'Kane

# Feb 23, 2022

	zmain

      if '$data(%1) set JMIN=0
      else  set JMIN=%1

# input format: wgt word1 word2 co-occur-ww-w2 ^df(w1) ^df(w2)
# output format: jaccardtt word1 word2

	open 1:"term-term-jaccard-similarities.tmp,new"

	for  do
	. use 5 read a
	. if '$test break
	. set w1=$piece(a," ",2)
	. set w2=$piece(a," ",3)
	. set co=$piece(a," ",4)
	. set d1=$piece(a," ",5)
	. set d2=$piece(a," ",6)
	. set jc=co/(d1+d2-co)
	. if jc<JMIN quit
	. set jc=$j(jc,1,3)
	. use 1 write jc," ",w1," ",w2,!
	. set ^jtt(w1,w2)=jc
	. set ^jtt(w2,w1)=jc

	use 5
	close 1
