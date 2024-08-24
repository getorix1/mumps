#!/usr/bin/mumps
# ~/Medline2012/medlineRetrieve.mps 
# Copyright 2014, 2022 Kevin C. O'Kane
# Feb 28, 2022

	zmain

	kill ^query
	kill ^ans
	kill ^tmp

	for  do                    // extract query words to query vector
	. set w=$zzscanalnum
	. if w="" break
# terms
	. set w1=$zstem(w)
# unknown ?
	. if '$data(^dict(w1)) write ?4,"unknown word: ",w,! quit
	. write !,"query term: ",w,!
# stems
	. if $data(^stem(w1)) do
	.. write !,?4,"Words in this stem group are: "
	.. do stems(w1)
	. else  write ?4,w," has no stem related words"
# related
	. write !,?4,"adding related phrase words: "
	. if '$data(^phrase(w1)) do
	.. write "none",!
	. else  do
	.. set f=1
	.. for prs=$order(^phrase(w1,prs)) do
	... write prs," " set f=0
	... if $x>60 w !,?26
	... set ^query(prs)=1
	.. if f write "none"
	.. write !
# soundex
	. set s=$zzsoundex(w1)
	. write ?4,"words that may sound similar to ",w,": "
	. if '$data(^sndx(s)) write "none."
	. else  do
	.. set f=1
	.. for ws=$order(^sndx(s,ws)) do
	... if $e(w1,1,5)=$e(ws,1,5)&(w1'=ws) do
	.... set f=0
	.... do stems(ws)
	.. if f write "none"
	. if $x'=26 write !
# query word
	. set ^query(w)=3

# Find documents containing one or more query terms.

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
      write f," documents found.",!!
      write "   Doc Rel   Title",!

      for i=1:1:10 do   // display loop
      . use 1 read a
      . if '$test break
      . set d=$piece(a," ",2)
      . set c=$piece(a," ",1)
      . use 5 write $justify(d,6)," ",c," ",$e(^title(d),1,70)," ...",!
      . write ?14,"matched keys: "
      . for w=$order(^doc(d,w)) do
      .. if $data(^query(w)) do
      ... if $data(^stem(w)) do
      .... for w1=$order(^stem(w,w1)) write w1," "
      ... else  write "*",w," " write:$x>60 ?26
      . write !

      use 5
      close 1

      write !,"*Time used: ",$zd1-time0," seconds",!
      shell rm *.tmp
	halt

stems(ws)
	for w3=$order(^stem(ws,w3)) write:$x>60 ?26 write w3," "
	quit
