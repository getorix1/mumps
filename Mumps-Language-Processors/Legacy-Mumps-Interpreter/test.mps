#!/home/okane/Desktop/mumps/Legacy-Mumps-Interpreter/mumps

	f i=1:1:10 w i,!
	halt

	set x-$zd1
	f i=1:1:1000000 s ^a(i)=i i i#10000=0 w i,!
	write !,$zd1-x,!
	halt

	f i=1:1:100000000 i i#10000=0 w i,!
	halt

	f i=1:1:5000000 w ^a(i)," "
	halt

