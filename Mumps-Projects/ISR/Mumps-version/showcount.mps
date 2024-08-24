#!/usr/bin/mumps

 zmain

 shell/p wc &~%1~

 use 6 read line
 set line=$zblanks(line)
 use 5 write %1," (",$p(line," ",1),")",!
