//*#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*#+    Mumps Compiler Run-Time Support Functions
//*#+    Copyright (c) 2001, 2002, 2003, 2004, 2013, 2015, 2016,
//+#+    2017, 2022, 2024 by Kevin C. O'Kane
//*#+    kc.okane@gmail.com
//*#+    https://threadsafebooks.com/
//*#+    https://www.cs.uni.edu/~okane
//*#+
//*#+    This library is free software; you can redistribute it and/or
//*#+    modify it under the terms of the GNU Lesser General Public
//*#+    License as published by the Free Software Foundation; either
//*#+    version 2.1 of the License, or (at your option) any later version.
//*#+
//*#+    This library is distributed in the hope that it will be useful,
//*#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
//*#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//*#+    Lesser General Public License for more details.
//*#+
//*#+   You should have received a copy of the GNU Lesser General Public
//*#+    License along with this library; if not, write to the Free Software
//*#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//*#+
//*#+
//*#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*#+
//*#+ May 3, 2024
//*#+

#define STRMOVE(a,b) \
	strcpy(a, b);

//	{ unsigned char *c = b, *d = a; while ( *(d++) = *(c++) ); }

/*===========================================================================*
 *                                  strmove                                  *
 *===========================================================================*/

inline void strmove(unsigned char *a, unsigned char *b) {
    while ( *(a++) = *(b++) );
    }

/*===========================================================================*
 *                                  strcatx                                  *
 *===========================================================================*/

inline void strcatx(unsigned char * out, unsigned char * in) {

	for (; *out; out++); 

	while(*in) *(out++) = *(in++);

	*(out++)='\x01';
	*out='\0';
	}

