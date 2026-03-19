#!/usr/bin/mumps

      s i=$zsqlite("pragma","journal_mode=off")
      s i=$zsqlite("pragma","synchronous=off")

	zmain
	s t=$zd1
	for i=1:1:100000 s ^a(i)=i
	for i=1:1:100000 s k=^a(i)
	w $zd1-t,!
