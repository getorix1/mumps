#!/usr/bin/mumps

	zmain

	for h=$o(^h(h)) do
	. for c=$o(^h(h,c)) do
	.. for d=$o(^h(h,c,d)) do
	... write h," ",c," ",d,!
