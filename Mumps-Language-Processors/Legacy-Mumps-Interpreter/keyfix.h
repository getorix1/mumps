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

/*===========================================================================*
 *                                  keyfix                                   *
 *===========================================================================*/

static inline void keyfix (unsigned char *vxd) { 

    /************************************************************************
    *                                                                       *
    *       remove leading ^.  convert high codes to 1 (comma, paren)       *
    *                                                                       *
    ************************************************************************/

    if (*vxd == '^') {
        while (*vxd) { 
		*vxd = *(vxd+1) & 0x80 ? 1 : *(vxd+1); 
		vxd++; 
		}
	return;
	}

    /************************************************************************
    *                                                                       *
    *       convert markers to 1's                                          *
    *                                                                       *
    ************************************************************************/

    while (*vxd != 0) {
        if (*vxd & 0x80 ) *vxd = 1 ;
        vxd++;
	}

    return;

    }

