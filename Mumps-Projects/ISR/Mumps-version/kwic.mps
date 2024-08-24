#!/usr/bin/mumps
# kwic.mps Copyright 2014, 2022 Kevin C. O'Kane

# Feb 16, 2022

	zmain

        set %=$zStopInit("stop.words")

	for  do
	. set i=$zzinput("w")
	. if i=0 break
	. set i=i-1
	. set doc=w(0)
	. for j=1:1:i do
	.. s w(j)=$translate(w(j),"""`~!@#$%^&*()_-+=?/>.<,:;'{[}]")
	.. if $l(w(j))<4 quit
	.. if $l(w(j))>24 quit
	.. if w(j)?1n.e quit
	.. if $zStopLookup(w(j)) quit
	.. set tw=$zlower(w(j))
	.. write tw,?26,doc,": "
	.. set x=""
	.. for k=j+1:1:i set x=x_w(k)_" "
	.. set y=""
	.. for k=j-1:-1:1 set y=w(k)_" "_y quit:$l(y)>25
	.. write ?(80-$l(y)),y,?80,w(j)," ",x,!

	halt
