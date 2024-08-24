//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                                  
//#+    #     # #     # #     # ######   #####
//#+    ##   ## #     # ##   ## #     # #     #
//#+    # # # # #     # # # # # #     # #
//#+    #  #  # #     # #  #  # ######   #####
//#+    #     # #     # #     # #             #
//#+    #     # #     # #     # #       #     #
//#+    #     #  #####  #     # #        #####
//#+    
//#+    Mumps Interpreter                            
//#+
//#+    Copyright (C) MM  Kevin C. O'Kane
//#+
//#+    Kevin C. O'Kane
//#+    Computer Science Department
//#+    University of Northern Iowa
//#+    Cedar Falls, IA 50614-0507
//#+    Tel 319 273 7322
//#+    okane@cs.uni.edu
//#+    http://www.cs.uni.edu/~okane
//#+
//#+    This program is free software; you can redistribute it and/or modify
//#+    it under the terms of the GNU General Public License as published by
//#+    the Free Software Foundation; either version 2 of the License, or
//#+    (at your option) any later version.
//#+
//#+    This program is distributed in the hope that it will be useful,
//#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#+    GNU General Public License for more details.
//#+
//#+    You should have received a copy of the GNU General Public License
//#+    along with this program; if not, write to the Free Software
//#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+
//#+                                                              
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "tmplts.h"

	extern short setpiece;
	extern  unsigned char setname[64];
	extern  short zerr;
	extern  unsigned char bd[2048];
	extern  short int io,hor[7],ver[7];
	extern  short int tpx;
	extern  short t2;
	extern  short pd1len;
	extern  short iorg;
	extern  short xpx;
	extern  short t0px;
	extern  short ierr;
	extern  short symlen;
	extern  short symadr;
	extern char *symaddr;
	extern  short sdlim;
	extern  unsigned char symflg;
	extern  unsigned char pd1[PS1];
	extern  unsigned char xd[2048];
	extern  unsigned char v1d[2048];
	extern  short int ctrlc;
	extern  int gpad;
	extern  short ierr1;
	extern  int kflg;
	extern  int Tab;
	extern int mflag;
	extern struct nmes *nstart;
	extern short Jflg;
	extern FILE *cf;
