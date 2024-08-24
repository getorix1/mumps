#!/usr/bin/mumps

	zmain

# piece and tests

	write "--------------------------",!
	write "$piece() tests",!
	write "--------------------------",!

	set x="123.456.789"
	set $p(x,".",2)="abc"_"def" set j=1
	write "1 $p(x,""."",2)=abc 123.abcdef.789 ",x,!

	set x="123.456.789"
	s $p(x,".",1,2)="abc"
	write "2 $p(x,""."",1,2)=abc abc.789 ",x,!

	s ^a(2)="123.456.789"
	s i="3-1"
	s $p(^a(@i),".",2)="abc"
	write "3 $p(^a(@i),""."",2)=abc 123.abc.789 ",^a(@i),!

	k ^a
	s i="4-1"
	s ^a(3)="123.456.789"
	s $p(^a(@i),".",1,2)="abc"
	write "4 $p(^a(@i),""."",1,2)=abc  abc.789 ",^a(@i),!

	k ^a
	s i="4-1"
	s j=1,k=2
	s ^a(3)="123.456.789"
	s $p(^a(@i),".",j,k)="abc"
	write "5 $p(^a(@i),""."",j,k)=abc  abc.789 ",^a(@i),!

