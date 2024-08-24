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

/*---------------------------------------------
	decode arguments passed to processor
	from command line or environment
----------------------------------------------*/

/****************************************************************************
*******
******* install default program to execute init.mps or decode shell variable
*******
****************************************************************************/

if (getenv("QUERY_STRING")!=NULL)  {

		  char t[1024],t1[256];
		  int i,j,k,m,n,p,dup=0;

		  strcpy(t,getenv("QUERY_STRING"));
		  cgi_flag=1;
		  symflg=0; 
		  sym_("%QS",t); /* store */

	/***************************
	convert plus signs to blanks
	***************************/

		  for (i=0; t[i]!=0; i++) if (t[i]=='+') t[i]=' ';

nxtarg: for (i=0; t[i]!='&'&&t[i]!=0; i++); /* find end */

        if (t[i]==0) k=0; 
		  else k=1;

		  t[i]=0;
		  for (j=0; t[j]!='='; j++);
		  t[j]=0;

	/***************************
	copy right hand side to ftmp
	***************************/

		  for (n=0,m=j+1; t[m]!=0; m++) {

					 if (t[m]!='%') {
			ftmp[n]=t[m];
			n++;
			continue;
			}

		/**************
		first hex digit
		**************/

		if (t[m+1]>='a'&&t[m+1]<='z')t[m+1]=t[m+1]-32;
		if (t[m+1]>='0'&&t[m+1]<='9') p=(t[m+1]-'0')*16;
			else p=(t[m+1]-'A'+10)*16;

		/***************
		second hex digit
		***************/
		if (t[m+2]>='a'&&t[m+2]<='z')t[m+2]=t[m+2]-32;
                if (t[m+2]>='0'&&t[m+2]<='9') p=p+(t[m+2]-'0');
	                else p=p+(t[m+2]-'A'+10);

                ftmp[n]=p;  /* insert value into output string */
		n++; 
		m=m+2;
                }

        ftmp[n]=0; 
	symflg=1;  /* retrieve */
	sym_(t,t1);

	/*********************************************************************
	multiple instance of the same variable name have nbrs appended to name
	*********************************************************************/

		  if (symflg!=0) {
                dup++;
                sprintf(t1,"%d",dup);
                strcat(t,t1);
                }

        symflg=0; 
        sym_(t,ftmp); /* store */

	/**********************************
	not at end yet - shift input string
	**********************************/

        if (k) { 
					 for (k=i+1; t[k]!=0; k++) t[k-(i+1)]=t[k];
                t[k-(i+1)]=0;
                goto nxtarg;
                }
}

if (argc>1) { /* file name specified */
	readf_(argv[1],1,opnfile); /* load script file */
	}

else { /* no file name specified */
	if (getenv("QUERY_STRING")!=NULL) {
		strcpy(&pd1[1],"\tg @prog");
		}
	else {

		  printf("\nMumps (c) 2022 by Kevin C. O'Kane (kc.okane@gmail.com)"
			"\nGlobals are %s and %s Version %s\n",
			svPtr->UKEY, svPtr->UKEY, VERSION);

		  strcpy(&pd1[1],"\tg ^init.mps");
		  }
	pd1len=strlen(&pd1[1])+1;
	}
