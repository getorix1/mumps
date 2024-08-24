#!/usr/bin/mumps
# simpleRetrieval.mps Copyright 2014 Kevin C. O'Kane

 kill ^query

 write "Enter search terms:",!

 set i=$zzInput("w")-1
 for j=0:1:i do
 . set a=$zlower(w(j))
 . set a=$zstem(a)
 . set ^query(a)=1

 if $order(^query(""))="" halt

 write "Query is: "
 for w=$order(^query(w)) write w," "
 write !

 set time0=$zd1

 set fn=$job_",new"
 open 1:fn
 use 1

 set x=0
 for i=$order(^doc(i))  do                    // calculate cosine between query and each doc
 . if i="" break
 . set c=$zzCosine(^doc(i),^query)

# If cosine is > zero, put it and the doc offset (^doc(i)) into an answer vector.
# Make the cosine a right justified string of length 5 with 3 digits to the
# right of the decimal point.  This will force numeric ordering on the first key.

 . if c>0 write $justify(c,5,3)," ",^title(i),! set x=x+1

 use 5
 close 1

 write x," documents found - showing top 10 results:",!!

 set cmd="sort -n <"_$job_" | tail -10 ; rm "_$job
 shell &~cmd~

 write !,"Time used: ",$zd1-time0," seconds",!

 halt
