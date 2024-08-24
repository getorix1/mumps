#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+                                                                  
#+    #     # #     # #     # ######   #####
#+    ##   ## #     # ##   ## #     # #     #
#+    # # # # #     # # # # # #     # #
#+    #  #  # #     # #  #  # ######   #####
#+    #     # #     # #     # #             #
#+    #     # #     # #     # #       #     #
#+    #     #  #####  #     # #        #####
#+    
#+    Mumps Interpreter                            
#+
#+    Copyright (C) 2022  Kevin C. O'Kane
#+
#+    Kevin C. O'Kane, Ph.D.
#+    Computer Science Department
#+    University of Northern Iowa
#+    Cedar Falls, IA 50614-0507
#+    kc.okane@gmail.com
#+    https://threadsafebooks.com/
#+    https://www.cs.uni.edu/~okane
#+
#+    This program is free software; you can redistribute it and/or modify
#+    it under the terms of the GNU General Public License as published by
#+    the Free Software Foundation; either version 2 of the License, or
#+    (at your option) any later version.
#+
#+    This program is distributed in the hope that it will be useful,
#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+    GNU General Public License for more details.
#+
#+    You should have received a copy of the GNU General Public License
#+    along with this program; if not, write to the Free Software
#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#+                                                              
#+ ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#...............................................
# Mumps interpreter checkout code - not exhaustive.
# Examples of code that works (mostly - the pattern
# match has problems).
#
# BE SURE to delete key.dat and data.dat before running or,
# if you are using Sqlite3, rebuild the Sqlite database.
#...............................................

	; comments may either begin with a semi-colon like
	; this PROVIDED that the semi-colon is placed where
	; a command would go.  Semi-colons may not be used
	; prior to the required TAB character at the beginning
	; of a line.

# This is also a comment - it must be the very 1st thing on
# a line and have NO TAB characters.

	write "Builtin variables ",!

	write "$fnumber(42000,""P"",2) ",$fn(42000,"P",2)," (wrong)",!
	write "$io 5 ",5,!
	write "$zd current date ",$zd,!
	if 1=1 set i=1
	write "$test 1 ",$test,!
	write "$TEST 1 ",$TEST,!
	write "$T 1 ",$T,!
	write "$t 1 ",$t,!
	if 1=0 set i=1
	write "$test 0 ",$test,!
	write "$job ",$job,!
	write "$storage (always 999) ",$s,!
	write "$x $y ",$x," ",$y,!

	write "Functions (expected actual)... ",!

	write "$ascii 66 ",$ascii("Beethoven"),!
	write "$ascii 89 ",$ascii("HAYDEN",3),!
	write "$CHAR BACH ",$CHAR(66,-1,65,67,72),!
	write "$char BACH ",$char(66,-1,65,67,72),!
	set a="66"
	write "$char B ",$c(a),!

	set ^a(1)=1
	set ^a(2)=1

	set ^a(1,1)=11
	set ^a(3,1)=1
	write "$data(^a(""none"")) 0 ",$data(^a("none")),!
	write "$data(^a(1)) 11 ",$data(^a(1)),!
	write "$data(^a(1,1)) 1 ",$data(^a(1,1)),!
	write "$data(^a(2)) 1 ",$data(^a(2)),!
	write "$data(^a(3)) 10 ",$data(^a(3)),!

	write "$extract B ",$extract("Brahms"),!
	write "$extract e ",$extract("Handel",5),!
	write "$extract art ",$extract("Mozart",4,6),!
	set i=$extract("Mozart",4,6)_"test"

	write "$find 3 ",$find("SIBELIUS","I"),!
	set i=3
	write "$find 7 ",$find("SIBELIUS","I",i),!
	write "$find 0 ",$find("SIBELIUS","I",7),!

	write "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",!
	write "$justify xx3.14159",$justify("3.14159",9),!
	write "$justify xxxxx3.14",$justify("3.14159",9,2),!

	write "$length 14 ",$length("Verdi & Wagner"),!
	write "$length 2 ",$length("Verdi & Wagner","&"),!
	write "$length 3 ",$length("Verdi & Wagner"," "),!

	set B3="Beethoven,Bach,Brahms"
	write "$piece Beethoven ",$p(B3,","),!
	write "$piece Brahms ",$p(B3,",",3),!
	write "$piece Beethoven,Bach ",$p(B3,",",1,2),!

	write "$random {0,8} ",$random(9),!

	write !,"Operators ...",!!

	write "Unary + 27.3 ",+"27.3 days",!
	write "Unary - -3.14 ",-"3.14 radians",!
	write "Sum 4.718 ",2.718+"2 above e",!
	write "Difference -4.18 ",2.12-"6.3 eV",!
	write "Product 2.01588 ",1.00794*"2 atoms/H2",!
	write "Division 12 ",144.132/12.011,!
	write "Integer Division 2 ",82.8\"29.5 years/orbit",!

	write "modulo works lik C modulo - not to mumps standard",!
	write "Modulo 2 ",42#5,!
	write "Modulo (see note) 3 ",-42#5,!
	write "Modulo (see note) -3 ",42#-5,!
	write "Modulo -2 ",-42#-5,!

	write "< 1 ",1642<1879,!
	write "> 0 ",1452>1564,!

	write "concat Feynman: 1918 ","Feynman: "_1918,!

	write "equals 1 ",1969-5=1964,!
	write "equals 0 ",1967="1967: M",!
	write "equals 1 ",1966=01966,!
	write "equals 0 ",1966="01966",!
	write "equals 2 ","Lovelace"="Hopper"+2,!
	write "equals 3 ","Lovelace"="Lovelace"+2,!

	write "not equals 0 ",1969-5'=1964,!
	write "not equals 1 ",1967'="1967: M",!
	write "not equals 0 ",1966'=01966,!
	write "not equals 1 ",1966'="01966",!
	write "not equals 3 ","Lovelace"'="Hopper"+2,!
	write "not equals 2 ","Lovelace"'="Lovelace"+2,!

	write "Contains 1 ","Darwin"["win",!
	write "Contains 1 ","Linnaeus"["",!

	write "Follows 0 ","COPERNICUS"]"KEPLER",!

	write "Pattern 0 ","Leakey"?1a,!
	write "Pattern 1 ","Boaz"?1.a,!
	write "Pattern 1 ","Fossey"?1u.5l,!
	write "Pattern 0 ","Goodall"?.4l.p6c.e,!

	write "And 0 ","Watson"&"Crick",!
	write "And 0 ","Morgan"&1735,!
	write "And 1 ",1838&1839,!
	write "And 1 ",-12000&1996,!
	write "And 0 ",1859&0,!

	write "Or 0 ","Jennifer"!"Pasteur",!
	write "Or 1 ","Hoffman"!1928,!
	write "Or 1 ",1898!-400,!
	write "Or 1 ",1867!0,!

	set a="aaa$bbb$ccc"
	set b="$"
	set $p(a,b,2)="test"
	write "setpiece: ",a,!

	write "exponention doesn't work",!
	; set a=3
	; set a=a**2
	; write "exponentiation: ",a,!
	; set a=9
	; set a=a**.5
	; write "exponentiation: ",a,!

	write "Not 1 ",'"Turing",!
	write "problem with multiple not signs",!
	; write "Not 0 ",''"Babbage",!
	write "Not 1 ",'"Backus"&1957,!
	write "Not 1 ",'("Wirth"&"Codd"),!

	write !!,"Command examples...",!!
	write "Open a file for output and write to it...",!
	open 1:"tmp.tmp,new"
	use 1
	for i=1:1:10 write "test ",i,!
	close 1
	use 5
	write "Reopen the same file and read from it ...",!
	open 1:"tmp.tmp,old"
	for i=1:1 use 1 read a quit:'$t  use 5 write $t,"***",a,!
	use 5
	close 1

	write "Set command ...",!
	set a=1+(2+(3+(4+5)))
	set:1=1 a=1+(2+(3+(4+5))) set a=0 write "0 ",a,!

	write "for command ...",!
	for i=1:1:10 write " ",i
	write !
	for i=1:1 quit:i>10  write " ",i
	write !

	write "Goto based loops...",!
	set i=1
lab1	write i," "
	set i=i+1
	if i<11 goto lab1
	write !

	write "Init a global and Next thru it ...",!
	for i=1:1:10 for j=1:1:10 set ^a(i,j)=i_","_j
	for i=1:1:10 write ! for j=1:1:10 w ^a(i,j)," "
	write !

	set i=-1
lab2	set i=$next(^a(i))
	if i<0 goto lab3
	write !,i,":"
	set j=-1
lab2a	set j=$next(^a(i,j))
	if j<0 goto lab2
	write ^a(i,j)," "
	goto lab2a
lab3	write !

# This version of mumps normally stores numeric subscripts as strings.
# this means that, on retrieval, the order will be according to a
# character collating sequence.  The ZNUMBER command causes numeric
# subscripts to be stored padded to the left with blanks to a fixed
# length field of width 8.  Thus, positive integers will appear
# in numeric order when retrieved.  ZNUMBER must be ineffect when
# the array is created.

	znumber
	write !,"Init a global and Next thru it using cannonical numbers...",!
	for i=1:1:10 for j=1:1:10 set ^b(i,j)=i_","_j
	for i=1:1:10 write ! for j=1:1:10 w ^b(i,j)," "
	write !
	set i=-1
xlab2	set i=$next(^b(i))
	if i<0 goto xlab3
	write !,i,":"
	set j=-1
xlab2a	set j=$next(^b(i,j))
	if j<0 goto xlab2
	write ^b(i,j)," "
	goto xlab2a
xlab3	write !

	read "read with prompt - enter something: ",a
	write "you wrote ",a,!

	write "Kill test ...",!
	kill ^a(5)
	write "6 ",$n(^a(4)),!

	write "Hang test for 1 second ...",!
	hang 1
	write "Was that 1 second?",!

# I'm not sure about all these.  $zd is safe, though.

	write "Horolog (wrong) ",$zd," ",$h,!
	write "$zd ",$zd,!
	write "$zd1 - seconds since Jan 1, 1970: ",$zd1,!
	write "$zd2($zd1): ",$zd2($zd1),!
	write "$zd3(1998,11,25) (day of year) ",$zd3(1998,11,25),!
	write "$zd4(1998,329) (Nov 25 1998) ",$zd4(1998,329),!
	write "$zd5(1998,11,25) ",$zd5(1998,11,25),!
	write "$zd6 ",$zd6,!
	write "$zd7 ",$zd7,!
	write "$zd8 ",$zd8,!


	set i="test"
	set j=9
	write "select (test): ",$select(j+1:i,1:2),!

##################
# ARRAY test
##################

	set b=1
	set c=2
	set aa(1,2)="test array"
	set a=aa(b,c)
	write "array test ... ",a," ",aa(b,c),!

	write "block test",!
	for j=1:1:5 do
	. write "I am an outer block ",j,!
	. for i=1:1:5 do
	.. write "I am an inner block ",i,!
	.. quit
	. quit
	write "I am not a block",!

	halt

