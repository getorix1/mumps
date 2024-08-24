#!/usr/bin/mumps
# kwoc.mps Copyright 2014, 2022 Kevin C. O'Kane

# Feb 13, 2022


	zmain

        set %=$zStopInit("stop.words")

	for  do
	. read line
	. if '$t break
	. set doc=$p(line," ",1)
	. for j=2:1 do
	.. s w=$p(line," ",j)
	.. s w=$translate(w,"""`~!@#$%^&*()_-+=?/>.<,:;'{[}]")
	.. if $l(w)=0 break
	.. if $l(w)<4 quit
	.. if $l(w)>24 quit
        .. if $zStopLookup(w) quit
	.. if w?1n.e quit
	.. set W=$zlower(w)
	.. write W,?25,$justify(doc,6),": "
	.. write $p(line," ",2,99)
	.. set ^kwoc(W)=""
	.. set ^kwoc(W,doc)=line
	.. write !
