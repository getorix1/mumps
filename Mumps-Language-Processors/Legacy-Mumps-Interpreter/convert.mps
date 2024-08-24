
#//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#//#+                                                                  
#//#+    #     # #     # #     # ######   #####
#//#+    ##   ## #     # ##   ## #     # #     #
#//#+    # # # # #     # # # # # #     # #
#//#+    #  #  # #     # #  #  # ######   #####
#//#+    #     # #     # #     # #             #
#//#+    #     # #     # #     # #       #     #
#//#+    #     #  #####  #     # #        #####
#//#+    
#//#+    Mumps Interpreter                            
#//#+
#//#+    Copyright (C) MM  Kevin C. O'Kane
#//#+
#//#+    This program is free software; you can redistribute it and/or modify
#//#+    it under the terms of the GNU General Public License as published by
#//#+    the Free Software Foundation; either version 2 of the License, or
#//#+    (at your option) any later version.
#//#+
#//#+    This program is distributed in the hope that it will be useful,
#//#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
#//#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#//#+    GNU General Public License for more details.
#//#+
#//#+    You should have received a copy of the GNU General Public License
#//#+    along with this program; if not, write to the Free Software
#//#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#//#+
#//#+                                                              
#//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	W "static unsigned char preloadPgm[]=",!
a	r a
	i '$t w ";",! H
	i $e(a,1,1)="#" g a
	i $l(a)=0 g a
	s b=""
	f i=1:1:$l(a) s b=b_$s($e(a,i,i)=$c(9):"\t",1:$e(a,i,i))
	s c=""
	f i=1:1:$l(b) s:$e(b,i,i)="""" c=c_"\" s c=c_$e(b,i,i)
	w """",c,"\n""",!
	g a
