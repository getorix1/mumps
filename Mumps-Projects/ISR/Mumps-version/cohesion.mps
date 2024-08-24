#!/usr/bin/mumps
# ~/Medline2012/cohesion.mps 
# Copyright 2016, 2022 Kevin C. O'Kane

# Feb 12, 2022

	zmain

# phrase construction

 for  do
 . read a
 . if '$test break
 . set w1=$piece(a," ",2)
 . set w2=$piece(a," ",3)
 . set co=$piece(a," ",4)
 . set ch=co/(^dict(w1)*^dict(w2))*100000\1
 . if ch>0 write ch," ",w1," ",w2,!

