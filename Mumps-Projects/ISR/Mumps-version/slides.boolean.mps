#!/usr/bin/mumps
# Copyright 2014 Kevin C. O'Kane
# boolean.mps February 14, 2014
# assumes that ^titles(docnbr) exists

	read "Enter query terms ",query

	set query=$zlower(query)

	set exp=""

	for i=1:1 do
	. set w=$piece(query," ",i)
	. if w="" break
	. if $find("()",w) set exp=exp_w continue
	. if w="|" set exp=exp_"!" continue
	. if w="~" set exp=exp_"'" continue
	. if w="&" set exp=exp_"&" continue
	. set exp=exp_"$f(line,"""_w_""")"

	write !,"Mumps expression to be evaluated on the data set: ",exp,!!

	set $noerr=1  // turns off error messages
	set line="  " set i=@exp  // test trial of the expression
	if $noerr<0 write "Expression error number ",-$noerror,!  got to again

	set M=10000

	set file="ohsu.converted,old"

	open 1:file
	if '$test write "file error",! halt

	set i=0

	for j=1:1:M do
	. use 1
	. read line
	. if '$test break
	. if @exp do
	.. set off=$piece(line," ",1)
	.. set docnbr=$piece(line," ",2)
	.. use 5
	.. write docnbr,?10,$e(^title(docnbr),1,80),!

	use 5
	write !,M," documents searched",!!
	halt
