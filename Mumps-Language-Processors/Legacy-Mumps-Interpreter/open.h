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
//#+    Copyright (C) 2022  Kevin C. O'Kane
//#+
//#+    Kevin C. O'Kane, Ph.D.
//#+    Computer Science Department
//#+    University of Northern Iowa
//#+    Cedar Falls, IA 50614-0507
//#+    kc.okane@gmail.com
//#+    https://threadsafebooks.com/
//#+    https://www.cs.uni.edu/~okane
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

openf:  t2=1;
        if ((ierr=parse_())) goto parse_err;
        k=atoi(&pd1[sdlim]);
        if (k<=0||k>=5) goto file_error;
        if (opnflg[k]) goto file_error;
        if (xd[xpx]!=':') goto arg_list_error;
        xpx++; 
	t2=1;
        if ((ierr=parse_())) goto parse_err;

        for	(i=0; 
		pd1[sdlim+i]!=0 && pd1[sdlim+i]!=','; 
		i++)
                	tmp2[i]=pd1[sdlim+i]; 

	if (pd1[sdlim+i]!=',') {

        	for	(i=0; 
			pd1[sdlim+i]!=0 && pd1[sdlim+i]!='/'; 
			i++)
                		tmp2[i]=pd1[sdlim+i]; 
		}


	tmp2[i++]=0;

        if (strcmp(&pd1[sdlim+i],"old")==0||
                strcmp(&pd1[sdlim+i],"OLD")==0) new=0;
        else if (strcmp(&pd1[sdlim+i],"new")==0||
                strcmp(&pd1[sdlim+i],"NEW")==0) new=1;
	else if (strcmp(&pd1[sdlim+i],"APPEND")==0) new=2;
	else if (strcmp(&pd1[sdlim+i],"append")==0) new=2;
       	else goto arg_list_error;

        if (new==1) { /* output */
                if ((opnfile[k]=fopen(tmp2,"w"))==NULL) {
			tpx=0; 
                	opnflg[k]=0; 
			}
		else {
			tpx=1;
			opnflg[k]=2;
			}
		goto opn1; 
		}

        if (new==2) { /* output */
                if ((opnfile[k]=fopen(tmp2,"a"))==NULL) {
			tpx=0; 
                	opnflg[k]=0; 
			}
		else {
			opnflg[k]=2;
			tpx=1;
			}
		goto opn1; 
		}

        if ((opnfile[k]=fopen(tmp2,"r"))==NULL) {
		tpx=0; 
	        opnflg[k]=0;
		}
	else {
		opnflg[k]=1;
		tpx=1;
		}

opn1:   if (xd[++xpx]!=',') goto next_cmnd;
        xpx++; 
	goto openf;
