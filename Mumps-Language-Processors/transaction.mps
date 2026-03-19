#!/usr/bin/mumps

	s i=999
	s ^a(1)=1
	set t1=$zd1
	w "page size ",$zsqlite("pragma","page_size"),!
	s i=$zsqlite("pragma","mmap_size=20000000")
	s i=$zsqlite("pragma","cache_size=-1000000")
	s i=$zsqlite("pragma","journal_mode=off")
	s i=$zsqlite("begin transaction")

	for i=1:1:10000 s ^a(i)=i
	for i=1:10:1000 w ^a(i),!
	for i=1:1:10000 s ^a(i)=-i
	for i=1:10:1000 w ^a(i),!

	s i=$zsqlite("commit transaction")

	write "time ",$zd1-t1,!
