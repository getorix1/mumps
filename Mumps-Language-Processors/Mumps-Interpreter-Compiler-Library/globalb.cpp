/*#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *#+	Mumps Compiler Run-Time Support Functions
 *#+	Copyright (c) A.D. 2001, 2002, 2003, 2010, 2012, 2017, 2024
 +#     by Kevin C. O'Kane
 *#+	okane@cs.uni.edu
 *#+
 *#+	This library is free software; you can redistribute it and/or
 *#+	modify it under the terms of the GNU Lesser General Public
 *#+	License as published by the Free Software Foundation; either
 *#+	version 2.1 of the License, or (at your option) any later version.
 *#+
 *#+	This library is distributed in the hope that it will be useful,
 *#+	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *#+	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *#+	Lesser General Public License for more details.
 *#+
 *#+   You should have received a copy of the GNU Lesser General Public
 *#+	License along with this library; if not, write to the Free Software
 *#+	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *#+
 *#+	http://www.cs.uni.edu/~okane
 *#+
 *#+    The sockets code was done by Ben Fain
 *#+
 *#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *#+
 *#+	Some of this code was originally written in Fortran
 *#+	which will explain the odd array and label usage,
 *#+	especially arrays beginning at index 1.
 *#+
 *#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *#+
 *#+	This is the native btree global array handler from the
 *#+	Mumps Compiler.
 *#+
 *#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

// April 5, 2024

//--------------------------------------------------------------------------
// the next item becomes #define NATIVE or SQLITE depending on the DB in use
//--------------------------------------------------------------------------

#define NATIVE

#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

#include <mumpsc/defines.h>
#include <mumpsc/stateVector.h>
#include <mumpsc/global.h>
#include <mumpsc/defines.h>
#include <mumpsc/btree.h>
#include <mumpsc/keyfix.h>

int Mglobal (const int g, unsigned char key[], unsigned char bd[], struct MSV * svPtr) {

      void unpad(unsigned char *key, struct MSV *svPtr);

      void sigint(int);
      static int CloseFlg=0;
      void flush(MSV *);

#if defined(SQLITE)
      keyfix(key);
      return btree(g,key,bd,svPtr);
      }

#else

      unsigned char t1[2],t2[2];

      if (g==RETRIEVE) {
            int i;
            keyfix(key);
            return btree(g,key,bd,svPtr);
            }

      if (g==STORE) {
            int i;
            keyfix(key);
            return btree(g,key,bd,svPtr);
            }

      if (g==ORDERNEXT || g==ORDERPREV ||
                  g==NEXT || g==PREVIOUS) {     /* prepare NEXT/ORDER cases */

            int b,j,gx=g;
            unsigned char hold[STR_MAX];
            j=strlen( (const char *) key);

            if (g!=ORDERNEXT && g!=ORDERPREV &&
                        key[j-1]==1 && key[j-2]=='1' &&
                        key[j-3]=='-' && key[j-4]==1) { /* $next -1 case */
                  key[j-3]=2;
                  key[j-2]=1;
                  key[j-2]=0;
                  key[j-1]=0;
                  strcpy( (char *) bd,"");
                  strcpy( (char *) svPtr->naked,"");
                  j--;
                  }
            else if (g==NEXT || g==ORDERNEXT) {
                  key[j-1]=2;
                  key[j]=0;
                  key[j+1]=0;
                  key[j+2]=0;
                  key[j+3]=0;
                  strcpy( (char *) bd,"");
                  strcpy( (char *) svPtr->naked,"");
                  }
            else if (g==ORDERPREV && key[j-1]==1 && key[j-2]==1) {
                  key[j-1]='~';
                  }

            gx=g;

            if (gx==NEXT) gx=XNEXT;
            else if (gx==ORDERNEXT) gx=XNEXT;
            else if (gx==ORDERPREV) gx=PREVIOUS;

            keyfix(key);
            strcpy( (char *) hold, (const char * ) key);
            b=btree(gx,key,bd,svPtr);

            if (g==NEXT || g==ORDERNEXT || g==ORDERPREV) {
                  j=strlen( (const char *) hold)-1;

                  if (hold[j]==1) j--;

                  for (; j>0 && hold[j]!=1; j--);

                  if (strncmp(( const char  * ) hold,( const char  * ) bd,j)!=0 ||
                              hold[j]!=1 ||
                              bd[j]!=1) {
                        if (g==NEXT) strcpy( (char *) bd,"-1");
                        else strcpy( (char *) bd,"");

                        return 0;
                        }

                  if (j>0) {
                        unsigned char * x=bd;
                        unsigned char * y= &bd[j+1];

                        for (; *y != 0; x++,y++) *x = *y; // STRCPY

                        *x=0;
                        }

                  for (j=0; bd[j]!=1&&bd[j]!=0; j++);

                  bd[j]=0;
                  unpad(bd,svPtr);

                  if (strlen( (const char *) bd)==0) {
                        if (g==NEXT) strcpy( (char *) bd,"-1");
                        else strcpy( (char *) bd,"");

                        return 0;
                        }

                  return 1;
                  }
            }

      if (g==GKILL) {
            int i= btree(g,key,bd,svPtr);

            return i;
            }

      if (g==XNEXT) {
            int i;
            keyfix(key);
            i= btree(g,key,bd,svPtr);

            return i;
            }

      if (g==CLOSE) {
            btree(g,key,bd,svPtr);
            return EXIT_SUCCESS;
            }

      return 0;
      }

#endif

