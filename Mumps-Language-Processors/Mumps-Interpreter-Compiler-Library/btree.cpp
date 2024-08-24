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
 +#+	2017, 2022, 2024 by Kevin C. O'Kane
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
 *#+	June 2, 2024
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



struct buffers *bfr[GBLBUF]= {0};

#define DumpBlocks

#ifdef DumpBlocks
void pb(int, unsigned char * );
#endif

void sigint(int);

inline void memxmove(char *to, char *from, char * limit, char *end) {
      while (from <= limit) {
            *(to++) = *(from++);
            }
      while (to < end) {
            *to = 0;
            to++;
            }
      }

inline char * strxxcpy(char * a, char *b) {
      while( (*(a) = *(b)) ) {
            a++;
            b++;
            }
      return b;
      };

unsigned char BigKey[STR_MAX];
unsigned int BigKeyBlock;
unsigned int FoundBlock;
unsigned int Reinsert;
unsigned int BigKeyParent;
unsigned int BigKeyParentOffset;
int BigKeyOffset;
int BigKeyLower;
int FoundOffset;
OFF_T BigKeyData;

extern int DBSem;

/*===========================================================================*
 *				    freadx				     *
 *===========================================================================*/

//inline void freadx(unsigned char **buf, int r, int flag, struct MSV * svPtr) {

inline void freadx(unsigned char **buf, int r, int flag) {

      static int indx;

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

      indx = (r & AMASK) + 1;

      if ( bfr[indx]->gblbfx == r ) {
//            if (flag) bfr[indx]->gblmod = 1;
            bfr[indx]->gblmod |= flag;
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
      int is,*root;

      for (is = 0; is < GBLBUF; is++) {	/* dump global buffers */
            if (bfr[is]->gblmod > 0) {
                  LSEEK(gblfd, (OFF_T)(bfr[is]->gblbfx)<<PAGE_SHIFT, SEEK_SET);
                  write(gblfd, bfr[is]->gblbf, MBLOCK);
                  bfr[is]->gblmod = 0;
                  }
            }

      freadx((unsigned char **) &root, 12L, MODIFIED);
      *root = 777L;		/* mark as proper close */

      LSEEK(gblfd, (OFF_T)0, SEEK_SET);
      write(gblfd, bfr[0]->gblbf, MBLOCK);	/* dump super block */
      bfr[0]->gblmod = 0;
      }

/*===========================================================================*
 *				    strxcmp				     *
 *===========================================================================*/

//----------------------------------------------------------------------
//	modified string comparison function that scans to
//	end of second argument if first argument is less than second
//----------------------------------------------------------------------

inline int strxcmp(unsigned char *p1, unsigned char *p2, unsigned char **r) {

      while (*p1 == *p2 && *p1 ) {
            p1++;
            p2++;
            }

      if (*p1 < *p2) return - 1;

//    if (*p1 == *p2) return 0;

      *r = (unsigned char *) index((char *) p2, 0) + 1;;

      if (*p1 == *p2) return 0;

      return 1 ;
      }

/*===========================================================================*
 *				    scanblocks				     *
 *===========================================================================*/

/* ************************************************
   trace descendant blocks for highest key value
   *************************************************/

inline void scanblocks (int i, struct MSV * svPtr) {

      unsigned char *block;
      static unsigned char *Key;  // do not recreate during recursion
      static unsigned char mark[2] = {MMARK, 0}; // do not recreate during recursion 0xff
      int lower, is, ptr1 = 4;

      freadx(&block, i, UNMODIFIED);

      while (1) {

            Key =  block + ptr1;

            if ( mark[0] == Key[0] && mark[1] == Key[1]) break; // block end

            if (strcmp( (const char *) Key, (const char *) BigKey) > 0) {

                  strcpy( (char * ) BigKey, (const char *) Key);
                  BigKeyBlock = i;
                  BigKeyOffset = ptr1;
                  memcpy(&lower, Key - 4, 4);
                  BigKeyLower = lower;
//            memcpy(&BigKeyData, Key + strlen( (const char *) Key) + 1, ADSIZE);
                  memcpy(&BigKeyData, index((char *) Key, 0) + 1, ADSIZE);
                  }

            memcpy(&lower, Key - 4, 4);

            ptr1 += strlen( (const char *) Key) + ADSIZE + 6;      /*key & data */

            }


//--------------------------------------------------
//	trace high end descendents of mark if any
//--------------------------------------------------

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

inline int allocblk(struct MSV * svPtr) {

      int *lastrec;
      OFF_T rec;
      unsigned char *block;

      freadx((unsigned char **) &lastrec, ADSIZE, MODIFIED);
      rec = *lastrec;
      rec=rec << PAGE_SHIFT;
      rec=rec + MBLOCK;
      *lastrec = (int) (rec >> PAGE_SHIFT);
      freadx(&block, (*lastrec), MODIFIED);
      return (int) (*lastrec);
      }

/*===========================================================================*
 *				    btree                                            *
 *===========================================================================*/

int btreeX (int g, unsigned char key[], unsigned char bd[], struct MSV * svPtr);

//-----------------
//	wrapper
//-----------------

int btree (int g, unsigned char key[], unsigned char bd[], struct MSV * svPtr) {

      int i = btreeX (g, key, bd, svPtr) ;

#ifdef NATIVE_SHARED

      if (! 0 ) { // no slice - lock the DB
            unsigned char t1[32]="", t2[32]="";
            btreeX (CLOSE, t1, t2, svPtr) ;
            usleep(1);
            return i;
            }

      if (DBSem) { // lock when alarm triggers
            unsigned char t1[32] = "", t2[32] = "";
            btreeX (CLOSE, t1, t2, svPtr) ;
            DBSem = 0;
            usleep(0);
            }
#endif

      return i;

      }

int btreeX (int g, unsigned char key[], unsigned char bd[], struct MSV * svPtr) {

      static int trxmax = 0,trace[TRXLIMIT], ilong, zero = 0, i, blkaddr1, m ;
      static OFF_T datptr;
      static int *root, *lastrec, *RDFlg;
      static int downblocknbr, rtf, datlen, nxtln, tmp1, blkoff, is, ks, ls, kk, ii;
      static int opnflg = 0;
      static int tracep[TRXLIMIT];
      static unsigned char *block, *block1, *ptr1,*ptr2, *ptr3;
      static unsigned char bufblk[DBL_STR+2]= {0};
      static unsigned char *rtPtr;
      static unsigned char *prev;
      unsigned char vv[2];
      int upper,upperoffset;
      int restoreFlag=0;

      static unsigned char * to, *tox;
      static unsigned char * from;
      static int length;


      if (g == RESTORE) {
            restoreFlag = 1;    // special re-insertion case during deletes
            g = STORE;
            }

      if (opnflg == 0 && g==CLOSE) return OK; // files not open - no need to close

      rtf = 1;   /* retry flag - prevents nxtln from being reset */

      if (g == XNEXT || g == PREVIOUS) {     /* prepare NEXT cases */

            prev = (unsigned char *) "";
            blkoff = strlen( (const char *) key);

            if (rtf) {
                  for (nxtln = blkoff - 2; nxtln > 0 && key[nxtln] != NEXTMARK; nxtln--);
                  if (nxtln > 0) nxtln++;
                  nxtln=blkoff - 1;
                  nxtln = 0;
                  }

            rtf = 0;
            }

      /* *****************************************
         section to open main data and key files
         *****************************************/

      if (opnflg == 0) {		/* auto open */

            for (i = 0; i < GBLBUF; i++) {	/* allocate buffers */

                  if (bfr[i]==0) {

                        bfr[i] = (buffers *) malloc(sizeof(struct buffers));
                        if (bfr[i]==NULL) {
                              printf("Insufficient memory for global array buffers %d\n", GBLBUF);
                              sigint(100);
                              }
                        }

                  bfr[i]->gblbfx = -1L;
                  bfr[i]->gblmod = 0;
                  }

            opnflg = 1;

#ifdef NEW_TREE
            sprintf(cfgkey, "key%d.tmp", getpid());
            sprintf(cfgdata, "dat%d.tmp", getpid());
#endif

reopen:

// -----------------------------------------
// open fails (O_EXCL) if file already exists
// -----------------------------------------

#ifndef READ_ONLY

            gblfd = open( (const char *) svPtr->UKEY,  O_RDWR | O_EXCL); // try to open existing file

            if (gblfd < 0 && errno == EACCES) {
                  printf("\n*** key.dat file access (EACCES) error in or near line %ld\n", svPtr->LineNumber);
                  sigint(100);
                  }

            gbldat = open((const char *) svPtr->UDAT,  O_RDWR | O_EXCL); // try to open existing file

            if (gbldat < 0 && errno == EACCES) {
                  printf("\n*** data.dat file access (EACCES) error in or near line %ld\n", svPtr->LineNumber);
                  sigint(100);
                  }

#else
            gblfd = open( (const char *) svPtr->UKEY,  O_RDONLY | O_EXCL); // try to open existing file
            gbldat = open((const char *) svPtr->UDAT,  O_RDONLY | O_EXCL); // try to open existing file
#endif


// ******************************
//   unix file locking
//   LOCK_EX is a system symbol
// ******************************

#if defined(LOCK_EX) && ! defined(READ_ONLY)

// will wait for file if already locked

            if(gblfd!=-1) flock(gblfd, LOCK_EX);	// do not proceed until locked
            // do not lock if READ_ONLY
#endif

            if (gblfd == -1) {	/* Does not exist - create and initialize */

#ifdef READ_ONLY
                  printf("Attempt to create read only native database\n");
                  sigint(100);
#endif

                  gblfd = open( (const char *) svPtr->UKEY, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                  gbldat = open((const char *) svPtr->UDAT, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

                  freadx(&block, 0L, MODIFIED);	/* get a buffer address */

                        {
                        int i;

                        for (i = 0; i < MBLOCK; i++) block[i] = 0;
                        }

                  for (is = 0; is <= 4; is++) {	/* init to zeros */
                        LSEEK(gblfd, (OFF_T)(is * MBLOCK), SEEK_SET);
                        write(gblfd, block, MBLOCK);
                        }

                  LSEEK(gbldat, (OFF_T)0, SEEK_SET);	      /* move to origin */
                  write(gbldat, block, 2);	/* init data file */

                  freadx((unsigned char **) &root, 12L, MODIFIED);
                  *root = 777L;	/* integrity flg */

                  freadx((unsigned char **) &root, 0L, MODIFIED);
                  *root = 0L;		/* root */

                  freadx((unsigned char **) &root, 4L, MODIFIED);
                  *root = 0L;		/* no meaning */

                  freadx((unsigned char **) &lastrec, 8, MODIFIED);
                  *lastrec = 16;   /* extent */

                  LSEEK(gblfd, (OFF_T)0, SEEK_SET);
                  write(gblfd, bfr[0]->gblbf, MBLOCK);	/* root block */

                  close(gblfd);
                  close(gbldat);
                  goto reopen;
                  }

            freadx((unsigned char **) &root, 12L, MODIFIED);	/* integrity flag */

            if (*root != 777L) {
                  printf("\nFile system error in program %s\n",svPtr->program);
                  abort();
                  }

            /*..........................................
               now done only when file system altered .....
               *root=0L;
               LSEEK(gblfd,0L,SEEK_SET);
               write(gblfd,bfr[0]->gblbf,MBLOCK);
               ............................................ */

            }

      if (g == FLUSH) {
            flush(svPtr);
            return OK;
            }

      if (g == CLOSE ) {	 /* CLOSE SECTION */

#ifdef NEW_TREE
            remove((const char *) svPtr->UDAT);
            remove( (const char *) svPtr->UKEY);
            return OK;
#endif

            if (opnflg == 0) return OK;

            flush(svPtr);

            close(gblfd);
            close(gbldat);
            opnflg = 0;
            return OK;
            }

      freadx((unsigned char **) &root, 0L, UNMODIFIED);

// -----------------------------------------------------------------------------
//                            new tree
// -----------------------------------------------------------------------------

      if (*root == 0) {		/*no tree */

            if (g == PREVIOUS || g == RETRIEVE) {
                  bd[0]=0;
                  return BAD;
                  }

            if (g != STORE) { // no other op permitted on new tree
                  bd[0] = 0;
                  return BAD;
                  }

            if (g == STORE) {	/*insert case */

                  if (ReadOnly) {

#ifdef READ_ONLY
                        printf("Native database read only violation\n");
                        sigint(100);
#endif
                        freadx((unsigned char **) &RDFlg, 12L, MODIFIED);	/* integrity flag */
                        *RDFlg = 0L;	/* mark as open - reset at close */
                        LSEEK(gblfd, (OFF_T)0, SEEK_SET);
                        write(gblfd, bfr[0]->gblbf, MBLOCK);	/* dump super block */
                        ReadOnly = 0;
                        }

                  blkaddr1 = allocblk(svPtr);
                  freadx(&block, 0L, MODIFIED);
                  memcpy(block, &blkaddr1, 4);
                  freadx(&block, blkaddr1, MODIFIED);	/* k block */

                  memset (block, 0, MBLOCK);

                  blkoff = 4;
                  strcpy( (char * ) &block[blkoff],  (const char *) key);	/*key */
                  blkoff += strlen( (const char *) key) + 1;

                  if (restoreFlag) datptr=svPtr->restoredata;

                  else if (strlen( (const char *) bd) == 0) {
                        datptr = 0L;
                        }

                  else {
                        datptr = LSEEK(gbldat, (OFF_T)0, SEEK_END);
                        datlen = strlen( (const char *) bd) + 1;
                        write(gbldat, &datlen, 2);	/* length code */
                        write(gbldat, bd, datlen);	/* data */
                        }

                  memcpy(&block[blkoff], &datptr, ADSIZE);	     /* data address */
                  blkoff += ADSIZE;
                  block[blkoff++] = 0;
                  memcpy(&block[blkoff], &zero, 4);
                  block[blkoff + 4] = MMARK;
                  block[blkoff + 5] = 0;
                  // flush(svPtr);
                  return OK;
                  }
            }

// ----------------------------- end new tree section --------------------------


      /*****************************************************************
                              search section
      ******************************************************************/

	if (strlen( (char *) key) > 256) {
            printf("\n*** Global array reference too long:\n%s\n", key);
            sigint(SIGINT);
            }

      i = *root; // disk address of tree root block
      trx = -1;

nxtblk:

      trace[++trx] = i;

      if (trx > trxmax) trxmax = trx;

      if (trx >= TRXLIMIT) {
            printf("Btree trace stack limit reached\n%s\n%d - increase TRXLIMIT\n", key, trx);
            sigint(SIGINT);
            }

      freadx(&block, i, UNMODIFIED);	// fetch tree root block
      ptr1 = block;  				// address of block in memory


// ------------------------ block scan ------------------------------------------

nxtkey:

      ptr1 += 4;					// skip child pointer

//----------------------------------------------
//	scan keys in block
//	is is len of key scanned if positive
//----------------------------------------------

      while ((is = strxcmp(key, ptr1, &rtPtr)) > 0) {

//-------------------------------
//	remember previous key
//	data file address
//-------------------------------

            if (g == PREVIOUS) {
                  memcpy(&datptr, rtPtr + 1, ADSIZE);
                  if (datptr >= 0 ) prev =  ptr1;
                  }

//-------------------------------
//	advance past key & data
//-------------------------------

            ptr1 = rtPtr + ADSIZE + 5;
            }

// ---------------------end block scan ------------------------------------------

      tracep[trx] = (int) (ptr1 - block) - 4;

      if (is < 0) {

            memcpy(&ilong, ptr1 - 4, 4);		// extract child block addr

            if (ilong <= 0L) {			// no child - we're done
                  blkoff = (int) (ptr1 - block);	// integer offset in block
                  goto fail;
                  }

            i = ilong;
            goto nxtblk;
            }

//---------------------
//	key found
//---------------------

      FoundOffset = ptr1 - block;
      FoundBlock = i;

p000:

      if (g == PREVIOUS) {

            memcpy(&ilong, ptr1 - 4, 4);  /* block address */

            if (ilong <= 0L) {
                  blkoff = (int) (ptr1 - block);
                  strcpy( (char * ) bd, (const char *) prev);
                  return 1;
                  }

            i = ilong;
            goto nxtblk;

            nxtln = strlen( (const char *) key) - 2;
            blkoff = nxtln;

            for (; key[nxtln] != 0 && key[nxtln] != NEXTMARK; nxtln--);

            if (strncmp( (const char *) prev,  (const char *) key, nxtln) == 0) {
                  is = 0;
                  ls = nxtln + 1;

                  while (prev[ls] != NEXTMARK) bd[is++] = prev[ls++];

                  bd[is] = 0;
                  key[blkoff + 1] = 0;

                  if (strcmp( (const char *) bd,  (const char *) &key[nxtln + 1]) == 0) {
                        strcpy( (char * ) bd, "");
                        return BAD;
                        }

                  while(bd[0]==' ') strcpy( (char * ) bd, (const char *) &bd[1]);

                  if (bd[0]==0) {
                        strcpy( (char * ) bd,"");
                        return 0;
                        }

                  return OK;
                  }

            else {
                  strcpy( (char * ) bd, "");
                  return BAD;
                  }
            }

// -----------------------------------------------------------------------------------------------

      if (g == RETRIEVE) {	// search operation

            memcpy(&datptr, rtPtr, ADSIZE);

            if (datptr <= 0L) {
                  bd[0] = 0;	// wmpty string
                  return OK;
                  }

            else {
                  LSEEK(gbldat, (OFF_T)datptr, SEEK_SET);
                  read(gbldat, &datlen, 2);	// length of data
                  read(gbldat, bd, datlen);	// data
                  return OK;
                  }

            return OK;
            }

// -----------------------------------------------------------------------------------------------

      if (g == STORE) {		/* store */

	if (strlen( (char *) key) > 256) {
            printf("\n*** Global array reference too long:\n%s\n", key);
            sigint(SIGINT);
            }

            if (ReadOnly) {

#ifdef READ_ONLY
                  printf("Native database read only violation\n");
                  sigint(100);
#endif

                  freadx((unsigned char **) &RDFlg, 12L, MODIFIED);	/* integrity flag */
                  *RDFlg = 0L;	/* mark as open - reset at close */
                  LSEEK(gblfd, (OFF_T)0, SEEK_SET);
                  write(gblfd, bfr[0]->gblbf, MBLOCK);	/* dump super block */
                  ReadOnly = 0;
                  }


            freadx(&block, i, MODIFIED);

            /*------------------------------*/
            /* add new record - reuse old	*/
            /*------------------------------*/

            ptr1 = rtPtr;
            memcpy(&datptr, ptr1, ADSIZE);

            is = strlen( (const char *) bd); // size of data

            if (datptr <= 0L) goto newrec;

            LSEEK(gbldat, (OFF_T)datptr, SEEK_SET);
            read(gbldat, &datlen, 2);	/* length code */

            if (datlen > is) { // big enough
                  LSEEK(gbldat, (OFF_T)datptr, SEEK_SET);
                  goto save;
                  }

            /*------------------------*/
            /* delete old data record */
            /* add new record at EOF  */
            /*------------------------*/

newrec:

            if (is == 0) {
                  datptr = 0L;
                  goto save1;
                  }

            datptr = LSEEK(gbldat, (OFF_T)0, SEEK_END);
            datlen = is + 1;

save:

            write(gbldat, &datlen, 2);	/* length code */
            write(gbldat, bd, datlen);	/* data */

save1:

            memcpy(ptr1, &datptr, ADSIZE);	     /* data address */
            return OK;
            }


      blkoff = (int) (ptr1 - block);

//-------------------------------------------------------------------------------------------------------
// KILLER
//-------------------------------------------------------------------------------------------------------

      if (g == KILL) {		/* delete */

            if (ReadOnly) {

#ifdef READ_ONLY
                  printf("Native database read only violation\n");
                  sigint(100);
#endif

                  freadx((unsigned char **) &RDFlg, 12L, MODIFIED);	/* integrity flag */
                  *RDFlg = 0L;	/* mark as open - reset at close */
                  LSEEK(gblfd, (OFF_T)0, SEEK_SET);
                  write(gblfd, bfr[0]->gblbf, MBLOCK);	/* dump super block */
                  ReadOnly = 0;
                  }

            freadx(&block, i, MODIFIED);
            memcpy(&ilong, ptr1-4, 4);

            for (i = MBLOCK - 1; block[i] == 0; i--);
            i++;

            memxmove( (char *) ptr1 - 4,  (char *) ptr1 + strlen( (const char *) ptr1) + ADSIZE + 2,
                      (char *) block + i,  (char *) block + MBLOCK); // remove the key

            if (ilong == 0) {
                  return OK;
                  }

            strcpy( (char * ) BigKey, "");
            BigKeyOffset = 0;
            BigKeyBlock = 0;
            Reinsert=ilong;

            scanblocks(ilong,svPtr); // pass the lower

            if (strcmp( (const char *) BigKey,"")==0) return OK;

            freadx(&block,Reinsert,MODIFIED);


            // ilong points to descendent chain of key to be deleted.
            // Big... is the highest key in that chain
            // reload the lower block, remove the key

            freadx(&block, BigKeyBlock, MODIFIED);
            ptr1=block+BigKeyOffset;

            for (i=MBLOCK-1; block[i]==0; i--);
            i++;

            memxmove( (char *) ptr1-4,  (char *) ptr1+strlen( (const char *) ptr1)+ADSIZE+2,
                      (char *) block+i,  (char *) block+MBLOCK); // remove the key

            for (i=MBLOCK-1; block[i]==0; i--);
            memcpy(block+i-4,&BigKeyLower,4);  // put stray descendants at end

            if (*(block+4)==MMARK && BigKeyBlock==Reinsert) { // collapse empty immediate lower block
                  memcpy(&ilong,block,4); // its descendent, if any

                  if (ilong>=0) Reinsert=ilong;
                  }
            else if (*(block+4)==MMARK) {  // collapse non-immediate empty lower block
                  memcpy(&ilong,block,4); // its descendent, if any
                  freadx(&block, BigKeyParent, MODIFIED);
                  memcpy(block+BigKeyParentOffset,&ilong,4);
                  }

            // reinsert key with descendents

            svPtr->restoredata = BigKeyData;
            strcpy( (char * ) bd,"xxx");
                  {
                  unsigned char tmp[STR_MAX];
                  strcpy( (char * ) tmp, (const char *) BigKey);
                  btreeX (RESTORE, tmp, bd, svPtr);
                  }
            btreeX (RETRIEVE, BigKey, bd, svPtr);

            memcpy(&ilong,block+FoundOffset-4,4);

            if (ilong==0) {
                  freadx(&block,FoundBlock,MODIFIED);
                  memcpy(block+FoundOffset-4,&Reinsert,4);
                  }
            else {
                  printf("open not slot found BigKey=%s\n",BigKey);

#ifdef DumpBlocks
                  pb(FoundBlock,block);
#endif

                  sigint(100);
                  }

            // flush(svPtr);
            return OK;
            }

//----------------------------------------------------------------------------------------------------------

      if (g == XNEXT) {	   /* found next */

            blkoff += strlen( (const char *) &block[blkoff]) + ADSIZE + 6;    /* next key */
            memcpy(&ilong, &block[blkoff - 4], 4);	/* look for less-than childrn */

            if (ilong <= 0L) {
                  goto fail;
                  }

            i = ilong;
            tracep[trx] = blkoff - 4;	/* new parent */
            goto nxtblk;
            }

      /*end found */

//----------------------------------------------------------------------------------------------------------

fail:

      if (g == PREVIOUS) goto p000;

      if (g == XNEXT) {

            if (block[blkoff] != MMARK) { // not at end of block

                  if (g == XNEXT) {	/* modify key */

                        to = key;
                        tox =  bd;

                        from = block + blkoff;

//--------------------------------------------------
//	copy global array ref to both key and bd
//--------------------------------------------------

                        while ( *(tox++) = (*(to++) = *(from++)) );

                        *to = 0;
                        *tox = 0;

                        from++;

                        memcpy(&datptr, from, ADSIZE);  /* data ptr */

                        if (datptr < 0L) {
                              ptr1 = block + blkoff - 4;
                              goto nxtkey;	/* dltd key */
                              }

                        return OK;
                        }
                  }

// ----------------------------------------------------------------------------------------------
//	end of block mark found
// ----------------------------------------------------------------------------------------------

upblock:

            trx--;

            if (trx < 0) {
                  strcpy( (char * ) bd, "");
                  return BAD;
                  }

            i = trace[trx];
            blkoff = tracep[trx] + 4;	/* key */
            freadx(&block, i, UNMODIFIED);

            if (block[blkoff] == MMARK) goto upblock;

            goto fail;		/* must be parent */
            }


//-------------------------------------------------------------
//	STORE section
//-------------------------------------------------------------

      if (g == STORE) {	// store new key

            blkaddr1 = 0L;

            if (restoreFlag) datptr = svPtr->restoredata;
            else if (!bd[0] ) datptr = 0L;	// zero length data field

            else {
                  datptr = LSEEK(gbldat, (OFF_T)0, SEEK_END);
                  datlen = strlen( (const char *) bd) + 1;
                  write(gbldat, &datlen, 2);	/* length code */
                  write(gbldat, bd, datlen);	/* data */
                  }

f1:

            freadx(&block, i, MODIFIED);	/* marked for write */

            unsigned char *lsp = block + blkoff;

            while (*lsp != MMARK) { // find block end
                  while( *(lsp++) );
                  lsp += ADSIZE + 5;
                  }

            ls = lsp - block; // occupied length

            ks = strlen( (const char *) key); // key to be added length

            if (ks + ls > BLOCK_SPLIT) goto split;	       /*no room */

//-------------------------------------------------------------
//	Simple store insert
//-------------------------------------------------------------

//----------------------------------------------------------------------------
//	The next block of code substitutes for memmove
//	It copies beginning at the right (high) end of the strings to avoid
//	the overlap problem.
//----------------------------------------------------------------------------

            length = ls - (blkoff - 4);

            to =  block + (blkoff - 4) + ks + ADSIZE + 6 + length;
            from = block + blkoff - 4 + length;

            length++;
            while (length--) *(to--) = *(from--);

            to = block + blkoff - 4;

            from = (unsigned char *) &blkaddr1;
            *(to++) = *(from++);
            *(to++) = *(from++);
            *(to++) = *(from++);
            *(to++) = *(from);

            to = (unsigned char *) memccpy((void *) to, (void *) key, 0, MBLOCK);

            from = (unsigned char *) &datptr;

            *(to++) = *(from++);
            *(to++) = *(from++);
            *(to++) = *(from++);
            *(to++) = *(from++);

            if (ADSIZE == 8) {
                  *(to++) = *(from++);
                  *(to++) = *(from++);
                  *(to++) = *(from++);
                  *(to++) = *(from);
                  }

            *(to) = 0;

            return OK;

//------------- end simple insert section ---------------------

//-------------------------------------------------------------
//	split insert section - split required
//	insert prior to blkoff-4
//-------------------------------------------------------------

split:

            length = ii = blkoff - 4;

            to = bufblk;	// double length block
            from = block;

            while (length--) *(to++) = *(from++);

            memcpy(&bufblk[blkoff - 4], &blkaddr1, 4);	// child

            strcpy( (char * ) &bufblk[blkoff],  (const char *) key);	// key

            blkoff += ks + 1;

            memcpy(&bufblk[blkoff], &datptr, ADSIZE); // data address

            bufblk[blkoff + ADSIZE] = 0;

//        to = &bufblk[blkoff+ADSIZE+1];
//        from = &block[ii];

            to = bufblk + blkoff + ADSIZE + 1;
            from = block + ii;

//        while ( (ii++) < MBLOCK ) *(to++) = *(from++);
//        while ( (ii++) < DBL_STR ) *to++ = 0;

            memcpy(to, from, MBLOCK - ii);
            to += MBLOCK - ii;
            memset (to, 0, MBLOCK);	// zero 2nd half of block - same len as 1st half

//     make blkoff forward new block and other things

//	find a mid-block key boundary

            from = bufblk;

            to = bufblk + MBLOCK / 2;

            while ( from < to ) {	// find middle key
                  from += 4;
                  while ( *(from++) );
                  from += ADSIZE + 1;
                  }

            blkoff = from - bufblk;

            if ( ! strcmp( (const char *) from + 4, (const char *) key) ) {
                  from += 4;
                  from += strlen( (const char *) from ) + ADSIZE + 2;
                  }

            from += 4;

            to = from;

//---------------------------------------------
//	extract key & advance to data pointer
//---------------------------------------------

            from = (unsigned char *) strxxcpy((char *) key, (char *) from) + 1;

            memcpy(&datptr, from, ADSIZE); /* extract data to be sent up */

            blkoff = from - bufblk;

//--------------------------------
//	advance past data pointer
//--------------------------------

            from += ADSIZE +1;

            freadx(&block, i, MODIFIED);	/* make memory resident */

            memcpy(block, from, MBLOCK);	// right block - zeroed?

            memset ( to, 0, DBL_STR - (to - bufblk) );

            *(to++) = MMARK;
            *to = 0;

            blkaddr1 = allocblk(svPtr);		/* allocate new left block */
            freadx(&block1, blkaddr1, MODIFIED);	/* make memory resident */
            memcpy(block1, bufblk, MBLOCK);	/* left part of block */

            /* fix parent */

            trx--;

            if (trx < 0) goto rootfix;

            i = trace[trx];
            blkoff = tracep[trx] + 4;
            goto f1;

rootfix:

            m = allocblk(svPtr);
            freadx(&block, 0L, MODIFIED);
            memcpy(block, &m, 4);
            freadx(&block, m, MODIFIED);

            memcpy(block, &blkaddr1, 4);
            strcpy( (char * ) &block[4],  (const char *) key);
            blkoff = 5 + strlen( (const char *) key);
            memcpy(&block[blkoff], &datptr, ADSIZE);
            block[blkoff + ADSIZE] = 0;
            blkoff += ADSIZE+1;
            memcpy(&block[blkoff], &trace[0], 4);
            block[blkoff + 4] = MMARK;
            block[blkoff + 5] = 0;
            // flush(svPtr);
            return OK;

            }

      bd[0] = 0;
      return (0);			/* default failure */

      }

#ifdef DumpBlocks

/* btree block dumper used for debugging */

void pb(int bn, unsigned char * block) {
      int s,ss;
      int x;
      OFF_T z;

      for(ss=MBLOCK-1; block[ss]==0&&ss>0; ss--);

      printf("\n=== block=%d size=%d =====>\n",bn,ss);

      for (s=0; s<=ss; s++) {
            memcpy(&x,block,4);
            printf(" [%d",x);
            block=block+4;

            if (*block==MMARK) {
                  printf (" MBLOCK END MMARK] ");
                  break;
                  }

            printf(" %s]", block);
            // memcpy(&z,block+strlen( (const char *) block)+1,ADSIZE);
            // printf(" %x]",z);
            block=block+strlen( (const char *) block)+ADSIZE+2;
            }

      printf("\n===========================================================\n");
      }

#endif

#endif

#ifdef SQLITE

//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//                           SQL SECTION
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mumpsc/defines.h>
#include <mumpsc/btree.h>
#include <mumpsc/global.h>
#include <mumpsc/libmpscpp.h>

char * command_print (char * command) {
      static char xcmd[1024];
      int i,j;
      for (i=0; command[i]; i++ ) {
            if (command[i] == 1 )
                  xcmd[i] = '#';
            else xcmd[i] = command[i];
            }
      xcmd[i]=0;
      return xcmd;
      }

void sigint(int);

int	sql(int, struct MSV * svPtr, char *DataSourceName, char *Command, char *, const char *);

// global array index length check

void	lcheck(char * p, MSV * svPtr) {

      if (strlen(p) > 64) { // index_size filled in by configure

            fprintf(stderr, "*** Length of global array index exceeds configuration max.\n");
            fprintf(stderr, "*** %s\n", p);
            fprintf(stderr, "*** In or near line %d\n\n",
                    svPtr->LineNumber);
            abort();
            }

      return; // ok
      }

// global array data length check

void	dcheck(char * p, MSV * svPtr) {

      if (strlen(p) > 128) { // data_size filled in by configure

            fprintf(stderr, "*** Length of global array data value exceeds configuration max.\n");
            fprintf(stderr, "*** In or near line %d\n\n",
                    svPtr->LineNumber);
            abort();
            }

      return; // ok
      }


// fix embedded single quote marks

char*	embed(char *in) {

      static char tmp[STR_MAX];
      int i,j=0,k=1;
      char *p1, *p2 = tmp;

      tmp[0]=0;

      if (in==NULL) return tmp;

      for (p1=in; *p1!=0; p1++)
            if (*p1=='\'') {
                  k=0;
                  break;
                  }

      if (k) return in; // contains no quotes

      for (p1=in; *p1; p1++) {
            if (*p1!='\'') *(p2++)=*p1;
            else {
                  *(p2++) = '\'';
                  *(p2++) = '\'';
                  }
            }

      *p2 = 0;
      return tmp;
      }

//===========================================================================
//			sql btree common section
//===========================================================================


unsigned char	rsltSpec[STR_MAX];	// MDH result interface

int	btree (int g, unsigned char key[], unsigned char bd[], struct MSV * svPtr) {

      static char X01[2] = {1,0};
      static char XNULL[2] = {0,0};
      static char XTAB[2] = {'\t',0};
      static char XCMD[2] = {10,0};
      int IMAX = 10;;
      char cmd[STR_MAX], *p1, *p2, *p3, *p4;
      char delim[2] = {1,0};
      int i;

// The following are the column names for PostgreSQL access.
// The number of columns is limited to 32. The first one is a dummy
// as the column names begin with a1

      char *nums[33] = {
            " ", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "a10",
            "a11", "a12", "a13", "a14", "a15", "a16", "a17", "a18", "a19",
            "a20", "a21", "a22", "a23", "a24", "a25", "a26", "a27", "a28",
            "a29", "a30", "a31", "a32"
            };

//-----------------------------
// make sure connection is open
//-----------------------------

      sql(OPEN, svPtr, (char *) "mumps", (char *) key, (char *) bd, XCMD);

      if (g == NEXT) g = XNEXT;

      if (g == SQLCMD) {
            return sql(g, svPtr, (char *) "mumps", (char *) key, (char *) bd, XCMD);
            }

//------------------------------------------------------------------------
//
//			RETRIEVE
//
//------------------------------------------------------------------------

      if (g==RETRIEVE) {
            char col[64];
            char delim[2] = {1,0};
            char save[STR_MAX];
            int i,tok=0;

            for (p1 = (char *) key; *p1; p1++) if (*p1 == 1) tok++; // count indices

            if (tok>IMAX) {
                  fprintf(stderr, "*** 001 Maximum number if global array indices exceeded\n"
                          "in or near line %d key=%s\n",svPtr->LineNumber,key);
                  sigint(100);
                  }

            strcpy(save,(const char *)key);

            p1=strtok( (char *) key, delim);

            strcpy(cmd, "select ");
            strcat(cmd,nums[IMAX]);
            strcat(cmd," from mumps ");

            strcat(cmd," where a1=\'");

            if (p1 != NULL ) {
                  lcheck(p1,svPtr);
                  strcat(cmd,embed(p1));
                  strcat(cmd,"\' ");
                  }
            else strcat( cmd, "\' ");  // no indices to global provided

            for (i=2; i<IMAX; i++) {

                  if ( (p1 = strtok(NULL,delim)) == NULL) break;

                  lcheck(p1,svPtr);
                  strcat(cmd," and ");
                  strcat(cmd,nums[i]);
                  strcat(cmd,"=\'");
                  strcat(cmd,embed(p1));
                  strcat(cmd,"\'");
                  }

            for (; i<IMAX; i++) {

                  strcat(cmd," and ");
                  strcat(cmd,nums[i]);
                  strcat(cmd,"=\'\'");
                  }

            strcat(cmd," limit 1;");
            bd[0] = 0; // empty string
            i=sql(g, svPtr, (char *) "mumps",cmd, (char *) bd, XTAB );
            strcpy((char *)key, save);
            return i;
            }

//------------------------------------------------------------------------
//
// 		STORE
//
//------------------------------------------------------------------------

      if (g==STORE) {

            char delim[2] = {1,0};
            int i,j,tok=0;
            char save[STR_MAX];
            char tmp[32];
            char cmd1[STR_MAX*2];

            for (p1 = (char *)key; *p1; p1++) if (*p1 == 1) tok++;

            if (tok>IMAX) {
                  fprintf(stderr, "*** 002 Maximum number if global array indices exceeded\n"
                          "in or near line %d key=%s count=%d max=%d program=%s\n",
                          svPtr->LineNumber,key,tok,IMAX,svPtr->program);
                  sigint(100);
                  }

            strcpy(save,(const char *) key);

// delete any previous tuple

            strcpy(cmd1,"delete from mumps ");
            strcat(cmd1," where a1=\'");

            p1=strtok( (char *) key, delim);

            lcheck(p1,svPtr);

            strcat(cmd1,embed(p1));
            strcat(cmd1,"\'");

            for (i=1; i<IMAX; i++) {
                  p1=strtok(NULL,delim);

                  if (p1==NULL) break;

                  strcat(cmd1," and ");
                  strcat(cmd1,nums[i+1]);
                  strcat(cmd1,"=\'");
                  lcheck(p1,svPtr);
                  strcat(cmd1,embed(p1));
                  strcat(cmd1,"\'");
                  }

            for (; i<IMAX-1; i++) {
                  strcat(cmd1," and ");
                  strcat(cmd1,nums[i+1]);

                  if (i<IMAX-2) strcat(cmd1,"=\'\'");
                  else strcat(cmd1,"=\'\';");
                  }

//            sql(g, svPtr, (char *) "mumps",cmd1, (char *) tmp, " "); // delete

            strcpy(( char *) key,save);

            p1=strtok( (char *) key,delim);

            sprintf(cmd, "replace into mumps values (\'%s\', ", embed(p1));

            j = strlen(cmd);
            p3 = &cmd[j];

            for (i=2; i<IMAX; i++) {

                  p1=strtok(NULL,delim);

                  if (p1==NULL) break;

                  lcheck(p1,svPtr);
                  p2 = embed(p1);
                  *(p3++)='\'';

                  while ( *p2 != 0 ) *(p3++) = *(p2++);

                  *(p3++) = '\'';
                  *(p3++) = ',';
                  *(p3++) = ' ';
                  *p3 = 0;

                  }

            for (; i<IMAX; i++) { // off count but final val in bd

                  *(p3++)='\'';
                  *(p3++)='\'';
                  *(p3++)=',';
                  *(p3++)=' ';

                  }

            *(p3)=0;

            dcheck((char *) bd,svPtr);
            p2 = embed((char *)bd);
            *(p3++)='\'';

            while ( *p2 != 0 ) *(p3++) = *(p2++);

            *(p3++) = '\'';
            *(p3++) = ' ';
            *(p3++) = ')';
            *(p3++) = ';';
            *p3 = 0;

            strcat(cmd1, cmd);
            i=sql(g, svPtr, (char *) "mumps",cmd1, (char *) bd, " ");

            if (i == 0) {
                  fprintf(stderr, "\n*** SQL error reported during assignment "
                          "to global array:\n*** %s\nCommand: %s\n",
                          svPtr->pgsql_msg, cmd);
                  abort();
                  }

            return 1;
            }

//------------------------------------------------------------------------
//
// 			NEXT used by $query
//
//------------------------------------------------------------------------

      if ( g==XNEXT ) {
            char cmd[STR_MAX];
            char col[64];
            int i;
            char keybak[STR_MAX];
            int tokens=0;
            char delim[2] = {1,0};
            char tmp[STR_MAX],tmp1[STR_MAX];

            for (p1 = (char *)key; *p1; p1++) if (*p1 == 1) {
                        tokens++;
                        }

            if (tokens>IMAX) {
                  fprintf(stderr, "*** 003 Maximum number if global array indices exceeded\n"
                          "in or near line %d key=%s\n",svPtr->LineNumber,key);
                  sigint(100);
                  }

            p1=strtok((char *)key, delim);

            strcpy(cmd,"select a1,a2,a3,a4,a5,a6,a7,a8,a9 from mumps where a1=\"");
            strcat(cmd, p1);
            strcat(cmd,"\" and (a2,a3,a4,a5,a6,a7,a8,a9) > (");

            p1=strtok(NULL, delim); // get another

            for (i=2; i<tokens; i++) {

                  if (p1==NULL) strcat(cmd, "\'\',");
                  else {
                        strcat(cmd, "\'");
                        lcheck(p1, svPtr);
                        strcat(cmd, embed(p1));
                        strcat(cmd, "\',");
                        p1=strtok(NULL, delim); // get another
                        }
                  }

            if (p1==NULL) {
                  strcat(cmd, "\'\'");
                  }

            else {
                  strcat(cmd, "\'");
                  lcheck(p1, svPtr);
                  strcat(cmd, embed(p1));
                  strcat(cmd, "\'");
                  }

            for (; i<9; i++) strcat(cmd, ",\'\'");

            strcat(cmd,") order by a2,a3,a4,a5,a6,a7,a8,a9");

            for (i=2; i<tokens-1; i++) {
                  sprintf(tmp,",a%d",i+1);
                  strcat(cmd,tmp);
                  }

            strcat(cmd," limit 1;");

            strcpy((char *) key, keybak);

            i=sql(g, svPtr, (char *) "mumps", cmd, (char *) bd, X01 );

            strcpy((char *) key, (char *) rsltSpec);

            return i;

            }

//------------------------------------------------------------------------
//
// 			GKILL
//
//------------------------------------------------------------------------

      if ( g==GKILL ) {
            char cmd[STR_MAX];
            char col[64];
            int i;
            char keybak[STR_MAX];
            char tmp[64];
            int tokens=0;

            for (p1 = (char *)key; *p1; p1++) if (*p1 == 1) tokens++;

            if (tokens>IMAX) {
                  fprintf(stderr, "*** 003 Maximum number if global array indices exceeded\n"
                          "in or near line %d key=%s\n",svPtr->LineNumber,key);
                  sigint(100);
                  }

            strcpy(keybak,(char *) key);
            sprintf(cmd, "delete from mumps ", svPtr->Table);

            if ( tokens == 1) {
                  strcat(cmd,"where a1=\'");
                  p3=strtok( (char *) key, delim); // array name
                  strcat(cmd, (char *) p3);
                  strcat(cmd, "\';");
                  }

            else {

                  strcat(cmd,"where ");
                  p3=strtok( (char *) key,delim); // array name

                  for (i=1; i<IMAX; i++) {

                        if (p3==NULL) break;

                        if (i !=1) strcat(cmd," and ");

                        strcat(cmd,nums[i]);
                        strcat(cmd,"=\'");
                        lcheck(p3,svPtr);
                        strcat(cmd,embed(p3));
                        strcat(cmd,"\' ");
                        p3=strtok(NULL,delim);
                        }

                  strcat(cmd,";");
                  }

            i=sql(g, svPtr, (char *) "mumps",cmd, (char *) bd, X01 );

            return i;
            }

// ..................... ORDER
// ..................... ORDER
// ..................... ORDER

      if (g==ORDERNEXT || g==ORDERPREV) {

            char cmd[STR_MAX];
            char col[64];
            char delim[2] = {1,0};
            int i,j,w=0;

            j=0;

            for (unsigned char *p=key; *p; p++) if ( *p == 1 ) j++;

// arg count of 1 impossible

            if (j>IMAX) {
                  return 0; // permits $data to work at this level
                  fprintf(stderr, "*** 004 Maximum number if global array indices exceeded\n"
                          "in or near line %d key=%s\n",svPtr->LineNumber,key);
                  sigint(100);
                  }

            p3=strtok( (char *) key,delim); // array name
            sprintf(cmd,"select %s from mumps where a1=\'%s\' ", nums[j], p3 );

            p3=strtok(NULL,delim);

            for (i=2; i<j; i++) { // specified indices

                  if (p3==NULL) {
                        break;
                        }
                  if (strlen(p3) == 0) break;

                  strcat(cmd," and ");

                  strcat(cmd,nums[i]);
                  strcat(cmd,"=\'");
                  lcheck(p3,svPtr);
                  strcat(cmd,embed(p3));
                  strcat(cmd,"\' ");
                  p3=strtok(NULL,delim);
                  }

            strcat(cmd," and ");
            strcat(cmd,nums[j]);

            if (g==ORDERNEXT)
                  strcat(cmd,">\'");
            else
                  strcat(cmd,"<\'");

            if (p3 != NULL) {
                  lcheck(p3,svPtr);
                  strcat(cmd,embed(p3));
                  }

            strcat(cmd,"\' ");

            if (g==ORDERNEXT) {
                  strcat(cmd," order by ");
                  strcat(cmd,nums[j]);
                  strcat(cmd," asc limit 1;");
                  }

            else {
                  strcat(cmd," order by ");
                  strcat(cmd,nums[j]);
                  strcat(cmd," desc limit 1;");
                  }

            i = sql(g, svPtr, (char *) "mumps",cmd, (char *) bd, XNULL );

            if (i==0) bd[0]=0;

            return i;
            }

// ..................... CLOSE
// ..................... CLOSE
// ..................... CLOSE

      if (g==CLOSE) {
            sql(CLOSE, svPtr, (char *) "",(char *) "",(char *) "", XTAB);
            return OK;
            }

      return OK;
      }

//ppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp
//                        SQLITE INTERFACE
//ppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp

#include <sqlite3.h>

static int call_back = 0;

int rslt_callback(void *rslt1, int num_cols, char *col_vals[], char **err_msgs) {

      char *rslt = (char *) rslt1;
      static char mark[2] = {1,0};

      call_back = 1; // we've been called

      if (num_cols == 1) strcpy(rslt, col_vals[0]); // one column answer

      else {
            int i;
            strcpy(rslt,"^");
            strcat(rslt, col_vals[0]); // array name
            strcpy((char *) rsltSpec, col_vals[0]);	// for MDH result reporting
            strcat(rslt, "(\"");

            for (i = 1; i < num_cols; i++) { // 0 is array name

                  if (! strlen(col_vals[i])) break; // empty string

                  strcat(rslt, col_vals[i]);
                  strcat((char *) rsltSpec, mark);	// for MDH result reporting
                  strcat((char *) rsltSpec, col_vals[i]);	// for MDH result reporting
                  strcat(rslt, "\",\"");
                  }

            i = strlen((char *) rslt);
            strcat((char *) rsltSpec, mark);	// for MDH result reporting
            rslt[i - 2] = ')';
            rslt[i - 1]  = 0;
            }


      return 0;
      }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SQLITE Interface Section
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int sql(int CODE, struct MSV *svPtr, char *DataSourceName, char *Command, char *rslt, const char *TOK) {

      static int open=0;

      static long long access = 0;

      int max;

      static sqlite3 *ppDb;
      int i, j, nt = 0, nf = 0;
      char tmp[256];

      strcpy(svPtr->Col_Names,"");

      if (CODE == CLOSE) {
            if (!open) return 1;
            svPtr->ConOpen=0;
            open=0;
            if (sqlite3_close(ppDb) == SQLITE_OK)  return OK;
            fprintf(stderr, "*** Sql database close error\n");
            }

      if (CODE == OPEN ) {
            if (open) return OK;
            }

      if (!open) {  // default open

            FILE *f1;
            f1 = fopen("mumps.sqlite", "r");

            if (f1 == NULL) {
                  fprintf(stderr, "\n\n*** Sqlite3 database not found.\n");
                  fprintf(stderr, "*** Database operations not available.\n");
                  fprintf(stderr, "*** Please run build script.\n");
                  return BAD;
                  }

            fclose(f1);

            int sql3 = sqlite3_open("mumps.sqlite", &ppDb);

            if (sql3 != SQLITE_OK) {
                  fprintf(stderr, "\n\n*** Sqlite3 database open returned an error..\n");
                  fprintf(stderr, "*** Database operations not available.\n");
                  fprintf(stderr, "*** Please run build script.\n");
                  return BAD;
                  }

            sqlite3_busy_timeout(ppDb, 20000); // busy handler

            open = 1;
            svPtr->tpx=open;
            if (CODE == OPEN ) return OK;
            } //default open

      if (strlen(Command)==0) return open;

      call_back = 0;

      int sql3;

      access++;

//----------------------------------------------------------------
//	If the connection is busy, keep retrying for 10 seconds
//----------------------------------------------------------------

      if (access % 1000 == 0) usleep(100000); // sync other users - tenth of a second

      char *err_msg = NULL;

      while ((sql3 = sqlite3_exec(ppDb, Command, rslt_callback, (void *)rslt, &err_msg)) == SQLITE_BUSY);

//---------------------------------------------------------------------------------------
//	how to detect a busy error message
//	if (sql3 == SQLITE_BUSY) printf("*** Database busy - adjust busy_timeout()\n");
//---------------------------------------------------------------------------------------

      if (err_msg) {
            fprintf(stderr, "*** Sqlite Error msg: %d %s\n", sql3, err_msg);
            sqlite3_free(err_msg);
            *rslt = 0;
            return BAD;
            }

      if (sql3 == 0 && (CODE == ORDERNEXT || CODE == ORDERPREV) && call_back && *rslt == 0) return BAD;

      if (sql3 == 0 && CODE == STORE) return OK;

      if (CODE == BEGIN_TRANSACTION) {
            if (sql3 == 0) return OK;
            else return BAD;
            }
      if (CODE == COMMIT_TRANSACTION) {
            if(sql3 == 0) return OK;
            else return BAD;
            }
      if (CODE == SAVEPOINT) {
            if(sql3 == 0) return OK;
            else return BAD;
            }
      if (CODE == ROLLBACK_TRANSACTION) {
            if(sql3 == 0) return OK;
            else return BAD;
            }
      if (CODE == PRAGMA) {
            if(sql3 == 0) return OK;
            else return BAD;
            }

      if (! call_back) return BAD;

      if (sql3 == 0) return OK;

      return BAD;

      }
#endif
