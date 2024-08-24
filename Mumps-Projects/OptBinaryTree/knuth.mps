#!/usr/bin/mumps
#===============================================================
#+
#+ Kevin C. O'Kane, Ph.D.
#+ Professor Emeritus
#+ Computer Science Department
#+ University of Northern Iowa
#+ Cedar Falls, IA 50614-0507
#+ kc.okane@gmail.com
#+ okane@uni.edu
#+ https://threadsafebooks.com
#+ https://www.cs.uni.edu/~okane
#+
#+==============================================================

	zmain

	read "n ",n

	for i=1:1:n do
	. write "p",i," "
	. read p(i)

	for i=0:1:n do
	. write "q",i," "
	. read q(i)

	for i=0:1:n do
	. for j=0:1:n do
	.. set r(i,j)=0

	for i=0:1:n do
	. set c(i,i)=0,w(i,i)=q(i)
	. for j=i+1:1:n do
	.. set:j'>n w(i,j)=w(i,j-1)+p(j)+q(j)

	for j=1:1:n do
	. set c(j-1,j)=w(j-1,j),r(j-1,j)=j

	for d=2:1:n do
	. for j=d:1:n do
	.. set i=j-d,y=r(i,j-1),x=c(i,y-1)+c(y,j)
	.. do xx
	.. set c(i,j)=w(i,j)+x,r(i,j)=y

	write !,"matrix",!
	for m=0:1:n-1 do
	. write !
	. for l=1:1:n do
	.. write r(m,l)," "

	write !!
	set s=1
	set s(s)=0_","_n
	set c=1
	set nx=2
	set a(1)="b(0"
	write 1

y	if $p(s(c),",",1)-$p(s(c),",",2)=0 set c=c+1 goto:c<nx y goto z

	write 2," ",a(c)," "
	set s(nx)=$p(s(c),",",1)_","_(r(@s(c))-1)
	set a(nx)=a(c)_",1"
	write 3
	set nx=nx+1
	set s(nx)=r(@s(c))_","_$p(s(c),",",2)
	set a(nx)=a(c)_",2"
	set nx=nx+1
	write 4
	set c=c+1
	goto y

z	for i=1:1:c-1 set a(i)=a(i)_")"
	for i=1:1:c-1 set @a(i)=r(@s(i))
	for i=1:1:c-1 write !,a(i),"->",@a(i)
	halt

xx	for k=r(i,j-1):1:r(i+1,j) i c(i,k-1)+c(k,j)<x set x=c(i,k-1)+c(k,j),y=k
	quit
