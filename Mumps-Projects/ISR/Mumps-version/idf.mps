#!/usr/bin/mumps
# idf.mps 
# Copyright 2014, 2022, 2023 Kevin C. O'Kane
# May 12, 2023

 zmain

 set doc=^DocCount(1)

# if '$data(%1) write "missing idfmin - using 4.0" set min=4.0
# else  set min=%1

 set av=0
 set i=0

 for w=$order(^df(w)) do 
 . set x=$zlog2(doc/^df(w))
 . set av=av+x
 . set i=i+1
 . set ^idf(w)=$justify(x,1,3)

 set av=av/i/2

 write ?8,"Calculated minimum IDF = ",av,!

 open 1:"idf-deleted.txt,new"
 use 1

 for w=$o(^idf(w)) if ^idf(w)<av write w," ",^idf(w),! kill ^idf(w)

 use 5
 close 1
 
 halt
