#!/usr/bin/mumps
# Copyright 2014, 2022, 2023 Kevin C. O'Kane
# Feb 28, 2022

	zmain

	kill ^query
	kill ^ans
	kill ^tmp

	read raw
	set tmp=""
	set query=""

	for i=1:1:$l(raw) do
	. set x=$e(raw,i)
	. if x=" " quit
	. if x?.n quit
	. set tmp=tmp_x

	set j=0
	for  do
	. set j=$zShredQuery(tmp,11)
	. if j="" break
	. set query=query_" "_j

	open 1:"full.seq,new"
	use 1
	write tmp,!
	close 1
	use 5

	write !
	write "Query is: ",tmp,!!

	for i=2:1 do  // leading blank - start at 2
	. set w=$p(query," ",i)
	. if w="" break

#	. if '$data(^dict(w)) quit

	. write "Known query ngram: ",w,!

	. set ^query(w)=3

# Find sequences containing one or more query terms.

	. for d=$order(^index(w,d)) set ^tmp(d)=""  // retain doc id

      set time0=$zd1
      write !

      set file=$j_".tmp"
      open 1:file_",new"

      set f=0

      for i=$order(^tmp(i)) do  // calculate cosine between query and each doc
      . set c=$zzcosine(^doc(i),^query)   // MDH cosine calculation
      . if c>0 use 1 write $justify(c,5,3)," ",i,! set f=f+1

      close 1
      use 5
      if 'f halt

      set outfile="sorted"_file
      shell sort -nr < &~file~ > &~outfile~

      open 1:"sorted"_file_",old"

      use 5

	open 2:"full.seq,old"
	use 2 read full close 2
	use 5

	open 3:"db.stemmed,old"

      write f," sequences found.",!!
      write "   Doc Rel   Title",!

      for i=1:1:10 do   // display loop
      . use 1 read a
      . if '$test break
      . set d=$piece(a," ",2)
	. set off=^doc(d)
      . set c=$piece(a," ",1)
      . open 4:"tmp,new" use 4 write "###",!,$justify(d,6)," ",c," ",$e(^title(d),1,70)," ...",!
      . write ?14,"matched keys: "
      . for w=$order(^doc(d,w)) do
      .. if $data(^query(w)) do
      ... write "*",w," "
	. set gen=""
	. write !
	. use 3
	. set %=$zseek(off)
	. read seq
	. for ii=1:1:$l(seq) do
	.. set x=$e(seq,ii)
	.. if x=" " quit
	.. set gen=gen_x
	. use 4 write !
	. set %=$zSmithWaterman(full,gen,1,0,-1,-1,2)
	. write "	Score = ",%,!!
	. close 4
	. set %%=$zsystem("mv tmp "_(1000+%)_".rslt")

      use 5
      close 1

#      write !,"*Time used: ",$zd1-time0," seconds",!
#      shell rm *.tmp
	halt

stems(ws)
	for w3=$order(^stem(ws,w3)) write:$x>60 ?26 write w3," "
	quit
