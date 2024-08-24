#define NATIVE

#define CSTR (char *)

#define OK 1
#define BAD 0

#if !(defined SQLITE)

//************************************************************************************
//************************************************************************************
//************************************************************************************
//************************************************************************************
// NATIVE SHARED
//************************************************************************************
//************************************************************************************
//************************************************************************************
//************************************************************************************

/*#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *#+	Mumps Compiler Run-Time Support Functions
 *#+	Copyright (c) 2001, 2002, 2003, 2004, 2013, 2015, 2016, 
 +#+	2017, 2022 by Kevin C. O'Kane
 *#+    kc.okane@gmail.com
 *#+    https://threadsafebooks.com/
 *#+	https://www.cs.uni.edu/~okane
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

/*#+...............................................................
 *#+	Modify this file depending on your OS and desired features
 *#+	March 30, 2024
 *#+...............................................................*/

#include <mumpsc/defines.h>
#include <mumpsc/btree.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#define MODIFIED 1
#define UNMODIFIED 0

#include <unistd.h>
#define LSEEK lseek
#define OFF_T off_t




#define DumpBlocks

#ifdef DumpBlocks
void pb(int, unsigned char * );
#endif

void sigint(int);

// unsigned char BigKey[STR_MAX];
// unsigned int BigKeyBlock;
// unsigned int FoundBlock;
// unsigned int Reinsert;
// unsigned int BigKeyParent;
// unsigned int BigKeyParentOffset;
// int BigKeyOffset;
// int BigKeyLower;
// int FoundOffset;
// OFF_T BigKeyData;

/*
extern struct buffers *bfr[GBLBUF];
extern unsigned char BigKey[STR_MAX];
extern unsigned int BigKeyBlock;
extern unsigned int FoundBlock;
extern unsigned int Reinsert;
extern unsigned int BigKeyParent;
extern unsigned int BigKeyParentOffset;
extern int BigKeyOffset;
extern int BigKeyLower;
extern int FoundOffset;
extern OFF_T BigKeyData;
*/


 void memxmove(char *to, char *from, char * limit, char *end) {
    while (from <= limit) {
        *(to++) = *(from++);
        }
    while (to < end) {
        *to = 0;
        to++;
        }
    }


/*===========================================================================*
 *				    freadx				     *
 *===========================================================================*/

 void freadx(unsigned char **buf, int r, int flag, struct MSV * svPtr) {

extern struct buffers *bfr[GBLBUF];
extern unsigned char BigKey[STR_MAX];
extern unsigned int BigKeyBlock;
extern unsigned int FoundBlock;
extern unsigned int Reinsert;
extern unsigned int BigKeyParent;
extern unsigned int BigKeyParentOffset;
extern int BigKeyOffset;
extern int BigKeyLower;
extern int FoundOffset;
extern OFF_T BigKeyData;
//    static int R;
//    R  = r << PAGE_SHIFT;
    static int indx;
//    indx = (r & AMASK) + 1;

    if (r < 16) {	     /* root block */

        bfr[0]->gblmod=1;

        if ( bfr[0]->gblbfx < 0L ) {	/* load root */
            LSEEK(gblfd, (OFF_T)0, SEEK_SET);
            read(gblfd, bfr[0]->gblbf, MBLOCK);
            bfr[0]->gblbfx = 0L;
            *buf = (unsigned char *) bfr[0]->gblbf + r;
            return;
            }

        *buf = (unsigned char *) bfr[0]->gblbf + r;
        return;
        }

//    svPtr->count++; // statistics

    indx = (r & AMASK) + 1;

    if ( bfr[indx]->gblbfx == r ) {
        if (flag) bfr[indx]->gblmod = 1;
        *buf = (unsigned char *) bfr[indx]->gblbf;
	// svPtr->hit++; // statistics
        return;
        }

    if ( bfr[indx]->gblmod ) {		/* write modified block */
        LSEEK(gblfd, (OFF_T)(bfr[indx]->gblbfx) << PAGE_SHIFT, SEEK_SET);
        write(gblfd, bfr[indx]->gblbf, MBLOCK);
        }

    LSEEK(gblfd, (OFF_T)(r) << PAGE_SHIFT, SEEK_SET);
    read(gblfd, bfr[indx]->gblbf, MBLOCK);
    *buf = (unsigned char *) bfr[indx]->gblbf;
    bfr[indx]->gblbfx = r;
    bfr[indx]->gblmod = flag;
    return;
    }

/*===========================================================================*
 *				    flush				     *
 *===========================================================================*/

/* *****************************
   flush all internal buffers
   *****************************/

void flush(struct MSV *svPtr) {
extern struct buffers *bfr[GBLBUF];
extern unsigned char BigKey[STR_MAX];
extern unsigned int BigKeyBlock;
extern unsigned int FoundBlock;
extern unsigned int Reinsert;
extern unsigned int BigKeyParent;
extern unsigned int BigKeyParentOffset;
extern int BigKeyOffset;
extern int BigKeyLower;
extern int FoundOffset;
extern OFF_T BigKeyData;
    int is,*root;

    for (is = 0; is < GBLBUF; is++) {	/* dump global buffers */
        if (bfr[is]->gblmod > 0) {
            LSEEK(gblfd, (OFF_T)(bfr[is]->gblbfx)<<PAGE_SHIFT, SEEK_SET);
            write(gblfd, bfr[is]->gblbf, MBLOCK);
            bfr[is]->gblmod = 0;
            }
        }

    freadx((unsigned char **) &root, 12L, MODIFIED, svPtr);
    *root = 777L;		/* mark as proper close */

    LSEEK(gblfd, (OFF_T)0, SEEK_SET);
    write(gblfd, bfr[0]->gblbf, MBLOCK);	/* dump super block */
    bfr[0]->gblmod = 0;
    }

/*===========================================================================*
 *				    strxcmp				     *
 *===========================================================================*/

/* ***************************************************************
   modified string comparison function that scans to
   end of second argument if first argument is less than second
   ***************************************************************/

 int strxcmp(unsigned char *p1, unsigned char *p2) {

    unsigned char *p3 = p2;

//    while (*p1 && *p2 && *p1==*p2) {

//    while (*p1 == *p2 && *p1 && *p2 ) {

    while (*p1 == *p2 && *p1 ) {
        p1++;
        p2++;
        }

    if (*p1 < *p2) return -1;

    if (*p1 == *p2) return 0;

//    if (*p1 < *p2) return -1;

    while (*(p2++));

    return p2 - p3 - 1;
    }

/*===========================================================================*
 *				    scanblocks				     *
 *===========================================================================*/

/* ************************************************
   trace descendant blocks for highest key value
   *************************************************/

 void scanblocks (int i, struct MSV * svPtr) {
extern struct buffers *bfr[GBLBUF];
extern unsigned char BigKey[STR_MAX];
extern unsigned int BigKeyBlock;
extern unsigned int FoundBlock;
extern unsigned int Reinsert;
extern unsigned int BigKeyParent;
extern unsigned int BigKeyParentOffset;
extern int BigKeyOffset;
extern int BigKeyLower;
extern int FoundOffset;
extern OFF_T BigKeyData;

    unsigned char *block;
//    static unsigned char key[STR_MAX];  // do not recreate during recursion
    static unsigned char *Key;  // do not recreate during recursion
    static unsigned char mark[2] = {MMARK, 0}; // do not recreate during recursion 0xff
    int lower, is, ptr1 = 4;

    freadx(&block, i, UNMODIFIED, svPtr);

    while (1) {

        if (strcmp( (const char *) mark,  (const char *) block + ptr1) == 0) break;

//        strcpy( (char * ) key, (const char *) block + ptr1);
	Key =  block + ptr1;

        if (strcmp( (const char *) Key, (const char *) BigKey) > 0) {
            strcpy( (char * ) BigKey, (const char *) Key);
            BigKeyBlock = i;
            BigKeyOffset = ptr1;
            memcpy(&lower, block+ptr1 - 4, 4);
            BigKeyLower = lower;
            memcpy(&BigKeyData, block + ptr1 + strlen( (const char *) block + ptr1) + 1, ADSIZE);
            }

        memcpy(&lower, block + ptr1 - 4, 4);
        ptr1 += strlen( (const char *) block + ptr1) + ADSIZE + 6;      /*key & data */
        }

// trace high end descendents of mark if any

    memcpy(&lower, block + ptr1 - 4, 4);

    if (lower > 0) {
        BigKeyParent = i;
        BigKeyParentOffset = ptr1 - 4;
        scanblocks(lower, svPtr);
        }
    }

/*===========================================================================*
 *				   allocblk				     *
 *===========================================================================*/

 int allocblk(struct MSV * svPtr) {

    int *lastrec;
    OFF_T rec;
    unsigned char *block;

    freadx((unsigned char **) &lastrec, ADSIZE, MODIFIED, svPtr);
    rec = *lastrec;
    rec=rec << PAGE_SHIFT;
    rec=rec + MBLOCK;
    *lastrec = (int) (rec >> PAGE_SHIFT);
    freadx(&block, (*lastrec), MODIFIED, svPtr);
//    return (int) (rec >> PAGE_SHIFT);
    return (int) (*lastrec);
    }

#endif
