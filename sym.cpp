/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2004, 2008, 2013, 2015, 2016, 2017, 2024
#+     by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     https://threadsafebooks.com/
#+     https://www.cs.uni.edu/~okane
#+
#+ This program is free software; you can redistribute it and/or modify
#+ it under the terms of the GNU General Public License as published by
#+ the Free Software Foundation; either version 2 of the License, or
#+ (at your option) any later version.
#+
#+ This program is distributed in the hope that it will be useful,
#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#+ GNU General Public License for more details.
#+
#+ You should have received a copy of the GNU General Public License
#+ along with this program; if not, write to the Free Software
#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#+
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//	June 9, 2024

#define NATIVE

// enable debug code
// #define SYMDEBUG

/* sym.c - Mumps Runtime Library
 *
 * Mumps symbol table management.  Variables not handled by global.h
 * and friends should be handled by these routines.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <monetary.h>
#include <locale.h>
#include <math.h>
// #include <wait.h>

#define _INTERP_
#define CSTR (char *)
#define USTR (unsigned char *)

#include <mumpsc/defines.h>
#include <mumpsc/sym.h>
#include <mumpsc/btree.h>
#include <mumpsc/globalOrder.h>
#include <mumpsc/keyfix.h>
#include <mumpsc/inline.h>

#include <unistd.h>

#include <time.h>

#include <sys/types.h>

using namespace std;

#include <mumpsc/fcns.h>

#include <mumpsc/builtin.h>
#include <mumpsc/interp.h>

// floatSize is defines BIGFLOAT if selected in configure


// intLong will be defined is 32 bit wanted - 64 otherwise
#define intLong

int Interpret(const char *, MSV *);
int ScanParse(char *);
int Eval(unsigned char *, unsigned char *, struct MSV *);  // evaluate expression

extern int (*__label_lookup)(char *);
extern char* (*__text_function)(int);  /* fcn */


/*===========================================================================*
 *                                  BuildLocal                               *
 *===========================================================================*/

int BuildLocal(int code, int g, unsigned char * str, unsigned char * rslt, struct MSV * svPtr) {

      static unsigned char tmp[TSTACK][STR_MAX];
      static int stk=0;

      if (code<0) {
            stk=0;
            return 1;
            }

      if (code==0) {

            if (stk >= TSTACK) {
                  printf("*** Build Stack Overflow %d\n\n", svPtr->LineNumber);
                  sigint(100);
                  }

            strcpy( (char *) tmp[stk++],(const char *) str);
// fff printf("4444444 str=%s\n", str);
            return 1;
            }

      if (code==2) {
            if (stk == 0) {
                  rslt[0]  =0;
                  }
            else strcpy( (char *) rslt, (const char *) tmp[--stk]);

            return 1;
            }

      stk--;

// fff printf("555555 stack=%s g=%d ORDERNEXT=%d\n", tmp[stk], g, ORDERNEXT);

      if (g == NEXT) LocalOrder(tmp[stk], rslt, (unsigned char *) "1", svPtr); //GlobalNext(tmp,rslt, svPtr);
      else if (g == ORDERNEXT) LocalOrder(tmp[stk], rslt, (unsigned char *) "1", svPtr);
      else if (g == ORDERPREV) LocalOrder(tmp[stk], rslt, (unsigned char *) "-1", svPtr);
      else return 0;

      return 1;
      }

//===========================================================================
//                                   sym_
//===========================================================================

char * sym_(int symflg, unsigned char *a, unsigned char *b, struct MSV * svPtr) {

      /*#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      **#+
      **#+  Run Time Symbol Table.  All mumps local variables
      **#+  are created and stored here.  First argument is the
      **#+  operation code, second is the variable name (including
      **#+  array marker information) and the third is the incoming
      **#+  value to be stored or a result being sent back.
      **#+
      **#+    symflg= 0 store/create
      **#+              SymStore
      **#+    symflg= 1 retrieve
      **#+              SymRetrieve
      **#+    symflg= 2 delete explicit
      **#+    symflg= 3 $next on last argument
      **#+    symflg= 4 kill all
      **#+    symflg= 5 kill all except...
      **#+    symflg= 6 $data
      **#+    symflg= 7 New except (...)
      **#+    symflg= 8 New inclusive
      **#+    symflg= 9 No copy flag
      **#+    symflg=10 Return variable name
      **#+    symflg=11 Retrieve variable only - no labels
      **#+              SymRetNoLbls
      **#+    symflg=33 Order
      **#+    symflg=99 Push table
      **#+    symflg=100 store at this level only
      **#+    symflg=101 retrieve next
      **#+    symflg=150 mark copy flag
      **#+    symflg=200 this variable is a call by reference
      **#+               original name is in svPtr->reference
      **#+
      **#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define SYMSTORE 0
#define SYMRETRIEVE 1

      unsigned char key[1024];
      struct stab *p1, *p2, *p3;
      int i;
      unsigned char * mkpt(unsigned char * str);

      if (a == NULL) return NULL;

//    strcpy((char *)key, (char *) a);
//    keyfix((unsigned char *) key);

      unsigned char *kp = a;
      unsigned char *ky = key;

      while (*kp) {
            *ky = *kp & 0x80 ? 1 : *kp;
            kp++;
            ky++;
            }

      *ky = 0;


//----------------
// R E T R I E V E
//----------------

      if (symflg == SymRetrieve || symflg == SymRetNoLbls) {

            int isym,cr;

            for (isym = svPtr->_Sym; isym >= 0; isym--) {

                  p1 = svPtr->start[isym];

                  while (p1 != NULL) {
                        if ( (cr = strcmp((char *) p1->name, (char *) key)) == 0 ) {
                              strcpy((char *) b, (*p1->data).c_str());
                              return (char *) b;
                              }

                        p1 = p1->next;
                        }
                  }

            symflg = 0;
            b[0] = 0;
            return NULL;

            }


//---------------------------
// S T O R E
// S T O R E
// S T O R E
// S T O R E
//---------------------------

// 100 means this level only store

      if (    symflg == SymStore ||
                  symflg == 100 ||
                  symflg == 200 ||
                  symflg == SymCPPdata ||
                  symflg == SymMarkCopyFlag) {

            int icmp, isym = svPtr->_Sym;

            if (key[0] == '$' && strcasecmp((const char *) key,"$noerr") == 0 ) {
                  if (b[0] == '1') svPtr->NOERR = 1;
                  else svPtr->NOERR = 0;
                  return (char *) b;
                  }

            for (; isym >= 0; --isym) { // search this and lower levels

                  p2 = NULL;
                  p3 = svPtr->start[isym];

                  while (p3) { // all level scan requires exact match

                        if ( (icmp = strcmp((char *) p3->name, (char *) key)) == 0) {
                              *(p3->data) = (char *) b;	// store to c++ string
                              return (char *) b;	// variable exists. value updated
                              }

                        if ( icmp > 0 ) break; // past place where it should be

                        p2 = p3;
                        p3 = p3->next;
                        }

                  if (symflg == 100 || symflg == 200) break; // store this level only
                  }


            if (symflg == SymStore) { // ***************************

                  isym = 0;	// seached to the bottom

                  p1 = new struct stab;

                  if (p1 == NULL) {
                        printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                        sigint(100);
                        }

                  p1->OrigName = new string;

                  p1->exec = 0;

                  p1->name = (char *) malloc(strlen((char *) key) + 1);

                  if (p1->name == NULL) {
                        printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                        sigint(100);
                        }

                  strcpy((char *) p1->name, (char *) key);

                  if (p2 == NULL) { // no previous. list is empty (p3==NULL) or has no element
                        p1->next = p3;
                        svPtr->start[isym] = p1;
                        }

                  else { // if last, p3 is NULL, next otherwise. add to list
                        p1->next = p3;
                        p2->next = p1;
                        }

                  p1 -> data = new string ((char *) b);

                  if (p1->data == NULL) {
                        printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                        sigint(100);
                        }

                  p1->copy = 0;
                  return (char *) p1;
                  }



            if (symflg == 100 || symflg == 200)
                  isym = svPtr->_Sym;
            else isym = 0;

            p2 = NULL;

            p3 = svPtr->start[isym];

            while (p3) {
                  if (strcmp((char *) p3->name, (char *) key) > 0) break;
                  p2 = p3;
                  p3 = p3->next;
                  }

            p1 = new struct stab;

            if (p1 == NULL) {
                  printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                  sigint(100);
                  }

            p1->OrigName = new string;

//----------------------------------------------------------------------
//	if this is a 'this level only' store (100/200), flag the level
//----------------------------------------------------------------------

            if (symflg == 100 || symflg == 200)
                  p1->exec = 1;
            else p1->exec = 0;

            if (symflg == 200)
                  *p1->OrigName = svPtr->reference; // call by reference var
            else *p1->OrigName="";

            p1->name = (char *) malloc(strlen((char *) key) + 1);

            if (p1->name == NULL) {
                  printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                  sigint(100);
                  }

            strcpy((char *) p1->name, (char *) key);

            if (p2==NULL) { // no previous. list is empty (p3==NULL) or has on member
                  p1->next = p3;
                  svPtr->start[isym] = p1;
                  }

            else { // if last, p3 is NULL, next otherwise.
                  p1->next=p3;
                  p2->next=p1;
                  }

            p1 -> data = new string ((char *) b);

            if (p1->data == NULL) {
                  printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                  sigint(100);
                  }

            p1->copy = 0;
            return (char *) p1;
            }

//-----------------------------------------------------------------------------------------


      if ( symflg == 9 || symflg == 101 ) {   /* retrieve or zero copy flag */

            int isym, cr;

            for (isym = svPtr->_Sym; isym >= 0; isym--) {

                  p1 = svPtr->start[isym];

                  while (p1 != NULL) {
                        if ( (cr = strcmp((char *) p1->name, (char *) key)) == 0 ) goto found;
                        if (symflg == 101 && cr > 0) goto found;
                        p1 = p1->next;
                        }
                  }

            if (p1 == NULL) {
                  if (symflg == 11 ) return NULL;
                  if (symflg == 9) {
                        symflg = 0;
                        return NULL;
                        }

                  i = __label_lookup((char *) key);

                  if (i != -1) {
                        sprintf((char *) b, "%d", i);
                        symflg = 2;
                        return (char *) b;
                        }

                  symflg = 0;
                  b[0] = 0;
                  return NULL;
                  }

found:

            if (symflg == 9) {	// no copy flag
                  symflg = 1;
                  p1->copy = 0;
                  return NULL;
                  }

            if (symflg == 101 ) { /* next absolute */

                  if (p1 == NULL && p1->next == NULL ) {
                        strcpy((char *) b, "");
                        return NULL;
                        }

                  if (cr == 0) {
                        if (p1->next == NULL) {
                              strcpy((char *) b, "");
                              return NULL;
                              }

                        p1 = p1->next; // exact match - get next
                        }

                  for (i=0; key[i] !=1 && key[i] != 0; i++);

                  if (strncmp((char *) key,(char *) p1->name,i) == 0 && p1->name[i] == 1) {
                        unsigned char tmp[STR_MAX];
                        strcpy((char *) tmp, (char *) p1->name);
                        fullUnpad(tmp, svPtr);
                        strcpy((char *) b, (const char *) tmp);
                        return (char *) b;
                        }
                  strcpy((char *) b, "");
                  return NULL;
                  }

//-------------------
//	other cases
//-------------------

            strcpy((char *) b, (*p1->data).c_str());
            return (char *) b;
            }


// ------
// $order
// ------

      if (symflg == 33) { /* order */
            i = strlen((char *) key);
            if (svPtr->order<0) {
                  if (key[i-1]==1 && key[i-2]==1) {
                        key[i-1]='\x80';
                        key[i]=1;
                        key[++i]='\0';
                        }
                  }
//       else {
//            }
            }

//------
// $next
//------

      if (symflg == 3) {          /* next */
            i = strlen((char *) key);
            if (key[i - 3] == '-' &&
                        key[i - 2] == '1' &&
                        key[i - 1] == 1) {
                  key[i - 3] = 1;
                  key[i - 2] = 0;
                  i = i - 2;
                  }
            }

      if (symflg == 3 || symflg == 33) {          /* next or order */

            int isym=svPtr->_Sym;

o_again:

            p1 = svPtr->start[isym];
            p2 = NULL;

            while (p1 != NULL) { /* looking for entry GT or EQ key */

                  if (strncmp((char *) p1->name, (char *) key, i) >= 0) break;
                  p2 = p1;
                  p1 = p1->next;
                  }

            if (symflg==33 && svPtr->order < 0) { //reverse order

                  if (isym > 0 && p2 == NULL) { // not finished with sym table
                        --isym;
                        goto o_again;
                        }

                  if (p2==NULL) {
                        strcpy((char *)b,"");
                        symflg=1;
                        return (char *) b;
                        }

                  for (i=i-2; key[i] != 1; i--);
                  p1=p2;
                  goto end;
                  goto orderbackexit;
                  }

            if (p1 == NULL) {  /* none found - search key GT all stored names */
                  if (isym > 0) {  // more sym table levels th check
                        --isym;
                        goto o_again;
                        }
                  if (symflg == 3) strcpy((char *) b, "-1");
                  else strcpy ((char *) b, "");
                  symflg = 1;
                  return (char *) b;
                  }

            if (strncmp((char *) p1->name, (char *) key, i) > 0) goto end;  /* stored name GT key */

            p1 = p1->next; /* found EQ key fragment */
            while (p1 != NULL) { /* advance to 1st NE to key */
                  if (strncmp((char *) p1->name, (char *) key, i) != 0) break;
                  p1 = p1->next;
                  }

            if (p1 == NULL) {  /* none greater found */
                  if (symflg == 3) strcpy((char *) b, "-1");
                  else strcpy ((char *) b, "");
                  symflg = 1;
                  return (char *) b;
                  }

end:

            if (symflg == 3 ) i = strlen((char *) key) - 2; /* back off to prior code lev */
            else if (symflg == 33 ) i = strlen((char *) key) - 2; /* back off to prior code lev */

            for (; key[i] != 1; i--);

            if (strncmp((char *) key, (char *) p1->name, i) != 0) {
                  if (symflg == 3) strcpy((char *) b, "-1");
                  else strcpy ((char *) b, "");
                  symflg=1;
                  return (char *) b;
                  }

orderbackexit:

            if ( *(p1->name+i) == 0) {
                  b[0]=0;    // case when there is a scalar var with same name
                  return (char *) b;
                  }
            strcpy((char *) b, (char *) p1->name + i + 1 );

            for (i = 0; b[i] != 1 && b[i] != 0; i++);
            b[i] = 0;

            while (b[0] == ' ') for (unsigned char * x=b; *x != 0; x++) *x = *(x+1); //STRCPY

exit_processing:

            symflg = 1;
            if (*b == '\x1f') { /* coded number - kok */
                  unsigned char tmp[64];
                  unsigned char * r = b;
                  double x;
                  r++;
                  if (*r=='0') *r='-';
                  else *r='+';
                  memcpy(tmp,r,40);
                  tmp[40]='\0';
                  sscanf((char *) tmp,"%lf",&x);
                  sprintf((char *) b,"%lg", x);
                  }

            return (char *) b;
            }

//--------------
// kill selected
//--------------

      if (symflg == 2) {          /* KILL selected */

            int isym,cr;

            for (isym=svPtr->_Sym; isym>=0; isym--) {

repeat:
                  p1 = svPtr->start[isym];
                  p2 = NULL;
                  i = 0;
                  i = strlen((char *) key);

                  while (p1 != NULL) {

                        if (strncmp((char *) p1->name, (char *) key, i)==0
                                    && p1->name[i]==1 ) break;

                        else if (strcmp((char *) p1->name, (char *) key) == 0) break;

                        p2 = p1;
                        p1 = p1->next;
                        }

                  if (p1 == NULL) continue;

                  if (p2 == NULL) svPtr->start[svPtr->_Sym] = p1->next;
                  else p2->next = p1->next;

                  delete p1->data;
                  free(p1->name);
                  delete p1->OrigName;
                  delete p1;

                  goto repeat;
                  }
            return NULL;
            }

      if (symflg == 4) {          /* kill all */
            p1 = svPtr->start[svPtr->_Sym];
            while (p1 != NULL) {
                  p2 = p1->next;
                  delete p1->data;
                  free( p1->name);
                  delete p1->OrigName;
                  delete p1;
                  p1 = p2;
                  }
            svPtr->start[svPtr->_Sym] = NULL;
            symflg = 1;
            return NULL;
            }

      if (symflg == 5) {          /* kill all except... */

            struct nmes *np1;
            int flg;

            p1 = svPtr->start[svPtr->_Sym];
            p3 = NULL;
            while (p1 != NULL) {
                  np1 = svPtr->nstart;
                  flg = 0;
                  while (np1 != NULL) {

                        i = strlen(np1->name) - 1;

                        if ((strncmp((char *) np1->name, (char *) p1->name, i) == 0
                                    && p1->name[i] == 1)
                                    || (strcmp((char *) np1->name, (char *) p1->name) == 0)) {
                              p3 = p1;
                              p1 = p1->next;
                              flg = 1;    /* don't delete p1 */
                              break;
                              }
                        np1 = np1->next;
                        }

                  if (flg) continue;
                  delete p1->data;
                  free(p1->name);
                  delete p1->OrigName;
                  p2 = p1->next;
                  delete p1;
                  if (p3 == NULL) svPtr->start[svPtr->_Sym] = p2;
                  else p3->next = p2;
                  p1 = p2;
                  }

            np1 = svPtr->nstart;
            while (np1 != NULL) {
                  struct nmes *np2;
                  np2 = np1->next;
                  free(np1->name);    /* cleanup */
                  free(np1);
                  np1 = np2;
                  }

            symflg = 1;
            return NULL;
            }

//------
// $data
//------

      if (symflg == 6) {          /* $data */

            int isym;

            for (isym=svPtr->_Sym; isym>=0; isym--) {
                  p1 = svPtr->start[isym];

                  while (p1 != NULL) {
                        if (strcmp((char *) p1->name, (char *) key) == 0) break;
                        p1 = p1->next;
                        }

                  if (p1 != NULL) break;
                  }

            if (p1 == NULL) {       /* search fail */

                  i = strlen((char *) key);

                  p1 = svPtr->start[svPtr->_Sym];
                  while (p1 != NULL) {
                        if (strncmp((char *) p1->name, (char *) key, i) == 0) break;
                        p1 = p1->next;
                        }

                  if (p1 != NULL) {

                        if (key[i-1]==1) i--;

                        if (p1->name[i] != 1) {


                              b[0] = '0';
                              b[1] = 0;
                              b[2] = 0;
                              return (char *) b;
                              }
                        else {
                              strcpy((char *) b,"10");
                              return (char *) b;
                              }
                        }

                  b[0] = '0';
                  b[1] = 0;
                  return (char *) b;
                  }


            b[0] = '1';             /* exists */

            p1 = p1->next;

            if (p1 == NULL) {       /* no possible descendants */
                  b[1] = 0;
                  return (char *) b;
                  }

            i = strlen((char *) key);

            if (strncmp((char *) key, (char *) p1->name, i) == 0) {
                  b[1] = '1';
                  b[2] = 0;
                  return (char *) b;
                  }

            b[1] = 0;
            return (char *) b;

            }

      if (symflg == 7) {          /* New except (...) */

            struct nmes *begin=NULL,*np1=NULL,*np2=NULL,*np3=NULL;
            struct stab *symtab=NULL;
            int flg;
            int isym;

            for (isym=svPtr->_Sym; isym>=0; isym--) { /* build unique list of variable names */
                  symtab=svPtr->start[isym];
                  while (symtab != NULL) {
                        flg=1;
                        for (np2=begin; np2!=NULL; np2=np2->next)
                              if (strcmp(np2->name,symtab->name)==0) flg=0; /* in list already? */
                        for (np2=svPtr->nstart; np2!=NULL; np2=np2->next)
                              if (strcmp(np2->name,symtab->name)==0) flg=0; /* in except list? */
                        if (flg) {
                              np3=(struct nmes *)malloc(sizeof(struct nmes));
                              if (np3 == NULL) {
                                    printf("*** Out of memory in or near %d\n\n", svPtr->LineNumber);
                                    sigint(100);
                                    }
                              np3->next=begin;
                              np3->name=symtab->name;
                              begin=np3;
                              }
                        symtab=symtab->next;
                        }
                  }
            svPtr->_SymPush++;
            if (svPtr->_Sym>SYM_MAX) {
                  printf("*** Too many symbol table layers\n");
                  sigint(100);
                  }
            for (np2=begin; np2!=NULL; np2=np2->next) {
                  sym_(100,(unsigned char *) np2->name,(unsigned char *) "",svPtr);
                  }
            }

      if (symflg == 99) {
            svPtr->_SymPush++;
            svPtr->_Sym++;
            svPtr->start[svPtr->_Sym]=NULL;
            if (svPtr->_Sym>SYM_MAX) {
                  printf("*** Too many symbol table layers\n");
                  sigint(100);
                  }
            return NULL;
            }

      if (symflg == 8) {          /* New inclusive */

            p1 = svPtr->start[svPtr->_Sym];
            svPtr->_Sym++;
            svPtr->start[svPtr->_Sym] = NULL;

            while (p1 != NULL) {
                  p2 = (struct stab *) sym_(0, (unsigned char *) p1->name,
                                            (unsigned char *) (*p1->data).c_str(), svPtr);
                  if (symflg == 1)
                        p2->copy = 1;
                  p1 = p1->next;
                  }

            symflg = 1;
            return NULL;
            }

      if (symflg == 10) {         /* return variable name */
            strcpy((char *) b, (char *) a);
            symflg = 1;
            return (char *) b;
            }

      return NULL;

      }


//===========================================================================
//                                 _SymFree
//===========================================================================

int _SymFree(int i, struct MSV * svPtr) {

      struct stab *p1, *p2;

      /* if argument is non zero, pop the symbol table.
         if argument is zero, delete variables only.
         argument of zero is used with kill all locals. */

      p1 = svPtr->start[svPtr->_Sym];
      svPtr->start[svPtr->_Sym]=NULL;

      if (svPtr->_Sym == 0 ) {

            while (p1 != NULL) {

                  free(p1->name);
                  delete p1->data;
                  delete p1->OrigName;
                  p2 = p1->next;
                  free(p1);
                  p1 = p2;
                  }
            return 0;
            }

      while (p1 != NULL) {

            if (p1->exec && *p1->OrigName !="" ) {
                  sym_(0, USTR (*p1->OrigName).c_str(),
                       USTR  (*p1->data).c_str(), svPtr); // store call by ref
                  }

            free(p1->name);
            delete p1->data;
            delete p1->OrigName;
            p2 = p1->next;
            free(p1);
            p1 = p2;
            }

      svPtr->_Sym--;

      if (svPtr->_Sym<0) {
            printf("*** SymFree symbol table underflow\n"), sigint(100);
            }

      return 0;
      }

void SysDump(long pid, struct MSV * svPtr) {

      char key[1024];
      struct stab *p1, *p2, *p3;
      int i;
      FILE *dump;
      char p[32],cp1[STR_MAX];

      Mltoa(pid, (unsigned char *) p);
      dump=fopen(p,"w");
      if (dump==NULL) ErrorMessage("Internal sym dum failed",svPtr->LineNumber);

      p1 = svPtr->start[svPtr->_Sym];

      while (p1 != NULL) {
            fprintf (dump, "%s\n",p1->name);
            strcpy(cp1,(*p1->data).c_str());
            fprintf (dump, "%s\n",cp1);
            p1 = p1->next;
            }
      fclose(dump);

      }

int SysLoad(long pid, struct MSV * svPtr)

      {
      /* sym table load */
      FILE *indump;
      unsigned char inbuf[2048],inbuf2[2048];
      Mltoa(pid,inbuf);
      indump=fopen((char *) inbuf,"r");
      if (indump!=NULL) {
            while(fgets((char *) inbuf,2048,indump)!=NULL) {
                  fgets((char *) inbuf2,2048,indump);
                  inbuf[strlen((char *) inbuf)-1]=0; /* new line */
                  inbuf2[strlen((char *) inbuf2)-1]=0; /* new line */
                  sym_(0,(unsigned char *) inbuf,(unsigned char *) inbuf2,svPtr); /* store */
                  }
            fclose(indump);
            return 1;
            }
      return 0;
      }

unsigned char * mkpt(unsigned char * str) {
      int i=0;
      static unsigned char buf[2048];
      for (i=0; str[i]!=0; i++) {
            if (str[i]>=32 && str[i]<=128) {
                  buf[i]=str[i];
                  continue;
                  }
            if (str[i]<32) buf[i]='-';
            if (str[i]=='\x1f') buf[i]='F';
            if (str[i]=='\x1e') buf[i]='E';
            if (str[i]=='\x01') buf[i]='.';
            }
      buf[i]=0;
      return buf;
      }

#include <stdlib.h>
#include <mumpsc/global.h>

extern "C" long _getpid(void);
extern "C" time_t time(time_t *);

/*===========================================================================*
 *                                    FCN                                    *
 *===========================================================================*/

void fcn(MSV * svPtr) {

      unsigned char tmp1[2] = { 0, 0 };
      long int day, fd;
      time_t timex;
      double t1;
      char *ctime();
      char S;
      unsigned char tmp2[STR_MAX];
      unsigned char tmp3[STR_MAX];
      int rslt;
      const unsigned char cod209[2] = { 209, 0 };
      char *ctmp;
      long timezone = 0;
      long daylight = 0;
#if defined(SQLITE)
      int sql(int, struct MSV *, char *, char *, char *, const char *);
#endif

      const static unsigned char opcode[256] = {

            /*0 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*10 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*20 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*30 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*40 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 21,
            /*50 */ 22, 23, 24, 25, 26, 27, 28, 29, 99, 99,
            /*60 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*70 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*80 */ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
            /*90 */ 99, 99, 99, 99, 99, 99, 99, 2,  99, 3,
            /*100 */ 99, 1, 4, 15, 14, 13, 5, 99, 6, 99,
            /*110 */ 30, 32, 7, 31, 10, 8, 9, 99, 99, 99,
            /*120 */ 11, 12, 98, 99, 99, 99, 99, 99
            };

      short int iargs[10], nargs, m, n, k, l;
      long i, j;

//----------------
//	$$FUNCTION
//----------------

      if (svPtr->v1d[1] == '$' && svPtr->v1d[2] == '$' ) {

            int status;

            svPtr->xpx++;
            int org = svPtr->xpx;
            int end=ScanParse( (char *) &svPtr->xd[svPtr->xpx])+svPtr->xpx;
            int hold=svPtr->xd[end];
            svPtr->xd[end]=0;
            char tcmd[128] = "do ";
            strcat(tcmd, (const char *) &svPtr->v1d[3]);

// look for ^ in function call - if not, function is local

            int ca;
            for (ca=svPtr->xpx; svPtr->xd[ca]!=0; ca++)
                  if (svPtr->xd[ca]=='^') break;

            if (svPtr->xd[ca]==0) {
                  char tmp[STR_MAX];
                  char tmp1[STR_MAX];
                  int i,j,k;
                  for (i=3; svPtr->v1d[i]!=0; i++) if (svPtr->v1d[i]==206) break;
                  if (svPtr->v1d[i]==0) j=1;
                  else j=0; // no arg case
                  svPtr->v1d[i]=0;
                  strcpy(tmp,"do ");
                  strcat(tmp, (char *) &svPtr->v1d[3] ); // label
                  strcat(tmp,"^"); // caret
                  strcat( tmp, svPtr->CurrentFile ); // current directory
                  if (j==0) { // attach argument
                        svPtr->v1d[i]='(';
                        strcat( tmp, (char *) &svPtr->v1d[i] );
                        tmp[strlen(tmp)-1]=0;
                        strcat(tmp,")");
                        }
                  strcpy(tcmd,tmp);
                  }

            else {
                  strcat(tcmd, (const char *) &svPtr->xd[org]);
                  svPtr->xd[end]=hold;
                  }

//--------------------------------------
// create environment and launch command
//--------------------------------------

                  {
                  MSV *svPtr1=AllocSV();

                  svPtr1->_Sym=svPtr->_Sym;
                  svPtr1->LineNumber=svPtr->LineNumber;
                  for (int i=0; i<SYM_MAX; i++) svPtr1->start[i]=svPtr->start[i];
                  svPtr1->nstart=svPtr->nstart;

                  svPtr->ierr=Interpret((const char *) tcmd,svPtr1);

                  for (int i=0; i<SYM_MAX; i++) svPtr->start[i]=svPtr1->start[i];

                  strcpy ( (char *) tmp2, (const char *) &svPtr1->args);

#if defined(SQLITE)

#endif

                  free(svPtr1);
                  }

            if (svPtr->ierr != 0) goto err;

            svPtr->xpx=end-1;
            strcpy ( (char *) &svPtr->bd[1], (const char *) tmp2);
            return;
            }




      svPtr->ierr = 0;
      nargs = 0;
      iargs[0] = 1;
      j = svPtr->v1d[2];  // hold function id
      S = svPtr->v1d[3];

      strcpy( CSTR tmp2, CSTR &svPtr->v1d[1]);

      while (svPtr->v1d[1] != 206 && svPtr->v1d[1] != 0)
            strmove(&svPtr->v1d[1], &svPtr->v1d[2]);

      if (svPtr->v1d[1] == CodedOpen) strmove(&svPtr->v1d[1], &svPtr->v1d[2]);

      if (j == CodedOpen ) { // noname form - parms to sub only
            strcpy((char *) svPtr->args,(const char *) &svPtr->v1d[1]);
            goto ex2;
            }

      for (i = 1; svPtr->v1d[i] != 0; i++)

            if (svPtr->v1d[i] == CodedClose || svPtr->v1d[i] == CodedComma) {
                  svPtr->v1d[i] = 0;
                  iargs[++nargs] = i + 1;
                  }

      j=tolower(j);      /*lower case conversion */

      if ((i = opcode[j]) == 99) {
            svPtr->ierr = 30;
            goto unknown_err;
            }

      switch (i) {

//-----------------------------------------------------------------------
//	unknown code
//-----------------------------------------------------------------------

            default:

                  svPtr->ierr=30;
                  goto unknown_err;

//-----------------------------------------------------------------------
//	$z variable and LHS functions
//-----------------------------------------------------------------------

            case 98:

                  if (strcasecmp(CSTR tmp2, "$ztable") == 0 ) { // change RDBMS name

#if defined(SQLITE)
                        sql( CLOSE, svPtr, NULL, NULL, NULL, NULL);
                        strcpy( CSTR svPtr->Table, CSTR &svPtr->pd1[svPtr->sdlim]);
                        sql( OPEN, svPtr, NULL, NULL, NULL, NULL);
#endif
                        goto ex2;
                        }

                  if (strcasecmp(CSTR tmp2, "$ztabsize") == 0 ) { // change RDBMS table size

#if defined(SQLITE)
                        sql( OPEN, svPtr, NULL, NULL, NULL, NULL);
                        svPtr->TabSize = atoi(CSTR &svPtr->pd1[svPtr->sdlim]);
                        if (svPtr->TabSize>22) goto unknown_err;
                        if (svPtr->TabSize<1) goto unknown_err;
#endif
                        goto ex2;
                        }

                  if (strcasecmp(CSTR tmp2, "$zsqloutput") == 0 ) { // change RDBMS table size

                        strcpy( CSTR svPtr->sqloutput, CSTR &svPtr->pd1[svPtr->sdlim]);
                        goto ex2;
                        }

                  printf("\n*** Unrecognized Z-type variable.\n%s\nIn or near line number %d\n\n",
                         tmp2,svPtr->LineNumber);
                  exit(EXIT_FAILURE);

//-----------------------------------------------------------------------
//	$extract()
//-----------------------------------------------------------------------

            case 1:                    /* $extract */

                  if (nargs == 1) { // no 2nd or 3rd operand - return first char

                        if (svPtr->setpiece) { // LHS ref
                              char tmp1[STR_MAX];

                              if (svPtr->setname[0] == '^') {  // global array reference
                                    int f;
                                    char tmp2[STR_MAX];

                                    f = Mglobal(RETRIEVE, svPtr->setname, (unsigned char *)tmp2, svPtr); // fetch gbl

                                    if (f) { // global exists
                                          strcpy(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]);
                                          if (strlen(tmp2)!=0) strcat (tmp1, &tmp2[1]);
                                          f = Mglobal(STORE, svPtr->setname, (unsigned char *)tmp1, svPtr);
                                          goto ex2;
                                          }

                                    else { // global does not exist
                                          strcpy(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]);
                                          f = Mglobal(STORE, svPtr->setname, (unsigned char *)tmp1, svPtr);
                                          goto ex2;
                                          }

                                    }

                              else {  // local variable reference
                                    ctmp = sym_(SYMRETRIEVE, svPtr->setname, &svPtr->bd[1],svPtr);
                                    if (ctmp != NULL) { // variable exists
                                          strcpy(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]);
                                          if (strlen(ctmp)!=0) strcat (tmp1, &ctmp[1]);
                                          ctmp = sym_(SYMSTORE, svPtr->setname, (unsigned char *) tmp1,svPtr);
                                          goto ex2;
                                          }
                                    else { // variable does not exist
                                          strcpy(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]);
                                          ctmp = sym_(SYMSTORE, svPtr->setname, (unsigned char *) tmp1,svPtr);
                                          goto ex2;
                                          }
                                    }
                              }

//---------------------------
// not LHS $e()	- RHS function
//---------------------------

                        svPtr->bd[1] = svPtr->v1d[1]; // return first char
                        svPtr->bd[2] = 0;
                        goto ex2;
                        }

                  if (nargs != 2 && nargs != 3) goto arg_count_err;

                  i=0;
                  sscanf((char*) &svPtr->v1d[iargs[1]], "%ld", &i); // arg 2

                  if (i <= 0 ) i=1;

                  if (nargs != 3) j = i; // 2nd operand only - return one char
                  else { // get 3rd operand
                        j=0;
                        sscanf( (char *) &svPtr->v1d[iargs[2]], "%ld", &j); // arg 3
                        }

                  if (j <= 0) j=0; // return null string

//------------------------------------
// RHS function where end before start
//------------------------------------

                  if (!(svPtr->setpiece) && (i >= iargs[1] || j < i)) { // start as after end of string or end LT start
                        svPtr->bd[1] = 0; // empty string
                        goto ex2;
                        }

//--------------------------
// LHS where end before start
//--------------------------

                  if (svPtr->setpiece && j < i) { // store empty string if end exceeds start
                        char tmp1[16]= "";

                        if (svPtr->setname[0] == '^') {  // global array reference
                              int f;
                              f = Mglobal(STORE, svPtr->setname, (unsigned char *)tmp1, svPtr);
                              goto ex2;
                              }

                        else { // local variable
                              ctmp = sym_(SYMSTORE, svPtr->setname, (unsigned char *) tmp1,svPtr);
                              goto ex2;
                              }
                        }

                  // if j exceeds str length, make it str length - RHS

                  if ( !(svPtr->setpiece) && j > iargs[1]) j = iargs[1];

                  if (i < 1) i = 1; // is start is less than 1, make it 1

// 3 argument forms

// LHS ref ------------------------------------------

                  if (svPtr->setpiece) { // LHS ref
                        char tmp1[STR_MAX];
                        char tmp2[STR_MAX];

                        if (svPtr->setname[0] == '^') {  // global array reference
                              int f;
                              char tmp2[STR_MAX];

                              f = Mglobal(RETRIEVE, svPtr->setname, (unsigned char *)tmp2, svPtr); // fetch gbl

                              if (f) { // global exists

                                    char ctmp[STR_MAX];
                                    strcpy(ctmp,tmp2); // gbl array value

                                    if (strlen(tmp2) < i) while(strlen(tmp2)<j) strcat(tmp2," "); // make it longer
                                    tmp2[i-1]=0; // truncate

                                    strcpy(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]); // RHS

                                    if (strlen(ctmp)!=0) strcat (tmp2, tmp1);  // insert RHS
                                    if (strlen(ctmp)>=j) strcat(tmp2,&ctmp[j]);

                                    f = Mglobal(STORE, svPtr->setname, (unsigned char *)tmp2, svPtr);

                                    goto ex2;
                                    }

                              else { // global does not exist
                                    for (k=0; k<i-1; k++) tmp1[k]=' ';
                                    tmp1[k]=0; // initial string
                                    strcat(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]);
                                    f = Mglobal(STORE, svPtr->setname, (unsigned char *)tmp1, svPtr);
                                    goto ex2;
                                    }
                              }

                        else { // local variable

                              ctmp = sym_(SYMRETRIEVE, svPtr->setname, &svPtr->bd[1],svPtr); // fetch variable value
                              if (ctmp != NULL) { // variable exists
                                    strcpy(tmp2,ctmp);  // value of variable
                                    if (strlen(tmp2) < i) while(strlen(tmp2)<j) strcat(tmp2," "); // make it longer
                                    tmp2[i-1]=0; // truncate
                                    strcpy(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]); // RHS
                                    if (strlen(ctmp)!=0) strcat (tmp2, tmp1);  // insert RHS
                                    if (strlen(ctmp)>=j) strcat(tmp2,&ctmp[j]);
                                    ctmp = sym_(SYMSTORE, svPtr->setname, (unsigned char *) tmp2,svPtr);
                                    goto ex2;
                                    }
                              else { // variable does not exist - create and append
                                    for (k=0; k<i-1; k++) tmp1[k]=' ';
                                    tmp1[k]=0; // initial string
                                    strcat(tmp1, (char *) &svPtr->pd1[svPtr->sdlim]);
                                    ctmp = sym_(SYMSTORE, svPtr->setname, (unsigned char *) tmp1,svPtr);
                                    goto ex2;
                                    }
                              }
                        }

// RHS ref --------------------------------------------

                  k = 1;
                  for (i = i; i <= j; i++) svPtr->bd[k++] = svPtr->v1d[i];

                  svPtr->bd[k] = 0;
                  goto ex2;

//-----------------------------------------------------------------------
//	$ascii()
//-----------------------------------------------------------------------

            case 2:                    /* $ascii */

                  if (nargs > 2) goto arg_count_err;

                  if (nargs == 2) sscanf((char*) &svPtr->v1d[iargs[1]], "%ld", &i);
                  else i = 1;

                  if (i >= iargs[1] || i <= 0) {
                        svPtr->bd[1] = '-';
                        svPtr->bd[2] = '1';
                        svPtr->bd[3] = 0;
                        return;
                        }

                  rslt = svPtr->v1d[i];
                  if (rslt == 0) rslt = -1;
                  goto ex1;

//-----------------------------------------------------------------------
//	$char()
//-----------------------------------------------------------------------

            case 3:                    /* $char        */

                  svPtr->bd[1] = 0;
                  j = 1;
                  for (i = 1; i <= nargs; i++) {
                        strmove(tmp2, &svPtr->v1d[j]);
                        day = atol((const char *)tmp2);
                        if (day <= 0) day = 0;
                        tmp1[0] = day;
                        tmp1[1] = 0;
                        strcat((char*) &svPtr->bd[1], (char*) tmp1);
                        j = iargs[i];
                        }

                  return;

//-----------------------------------------------------------------------
//	$find() $fnumber()
//-----------------------------------------------------------------------

            case 4:                    /* $find $fnumber */

                  if (tolower(S) == 'n') { // $fnumber()
                        double x;
                        int dp=-1,plus=0,minus=0,comma=0,t=0,t1=0,p=0; // expr flags
                        char str[32],rstr[64];

                        if (nargs != 3 && nargs != 2) goto arg_count_err;

                        if (nargs == 3) dp=atoi( (char *) &svPtr->v1d[iargs[2]]);

                        x=atof((char *)&svPtr->v1d[1]); // value to be converted

                        if ( svPtr->v1d[iargs[1]] == '*') { // arg is for strfmon
                              strfmon((char *)&svPtr->bd[1],256,(char *)&svPtr->v1d[iargs[1]+1],x);
                              return;
                              }

                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) '+') != NULL ) plus=1;
                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) '-') != NULL ) minus=1;
                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) ',') != NULL ) comma=1;
                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) 'T') != NULL ) t=1;
                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) 't') != NULL ) t=1;
                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) 'P') != NULL ) p=1;
                        if (strchr((char *) &svPtr->v1d[iargs[1]], (int) 'p') != NULL ) p=1;

                        if (p && ( plus || minus || t ) ) goto fnum_wrong;

                        strcpy(str,"%!");            // format supress currency sym

                        if (t) { // trailing sign
                              if (x<0) t1=1;
                              x=fabs(x);
                              }

                        if (!comma) strcat(str,"^");  // no grouping chars
                        if (p) strcat (str,"(");     // use parens for neg
                        if (minus) x=fabs(x);        // supress minus

                        if (dp != -1) {
                              char dps[16];
                              sprintf(dps,".%d",dp);
                              strcat(str,dps);
                              }
                        else {
                              int i;
                              for (i=1; svPtr->v1d[i] != 0 && svPtr->v1d[i] != '.'; i++);
                              if (svPtr->v1d[i] == 0) strcat(str,".0");
                              else {
                                    char tmp[32];
                                    i= strlen ( (char *) &svPtr->v1d[1] )-i;
                                    sprintf(tmp,".%d",i);
                                    strcat(str,tmp);
                                    }
                              }

                        strcat(str,"i");

//		printf("\nin pattern %s\nval=%s\nstrfmon string %s\n",
//		&svPtr->v1d[iargs[1]],&svPtr->v1d[1],str);

                        strfmon( &rstr[1], 256, str, x);

                        if (plus && x>0 && ! minus && ! t) rstr[0]='+'; //number was made positive by minus/t
                        else rstr[0]=' ';

                        if (t) {
                              if (t1) strcat(rstr,"-");
                              else if (plus) strcat(rstr,"+");
                              }

                        if (rstr[0]==' ') strcpy ( (char *) &svPtr->bd[1], &rstr[1]);
                        else strcpy ( (char *) &svPtr->bd[1], rstr);

                        return;
                        }

//------
// $find
//------
                  if (nargs != 2 && nargs != 3) goto arg_count_err;

                  strmove(tmp2, &svPtr->v1d[1]);
                  strmove(tmp3, &svPtr->v1d[iargs[1]]);

                  if (nargs == 2) i = 1;
                  else sscanf((char*) &svPtr->v1d[iargs[2]], "%ld", &i);

                  if (tmp3[0] != 0) {
                        if ((i = xindex(tmp2, tmp3, (short) i)) > 0) i += strlen((char*) tmp3);
                        }

                  if (i > strlen((char*) tmp2) + 1) i = 0;
                  rslt = i;
                  goto ex1;

//-----------------------------------------------------------------------
//	$justify() $job
//-----------------------------------------------------------------------

            case 5:                    /* $justify $job */

                  if (nargs == 0) {       /* $job */

                        sprintf((char*) &svPtr->bd[1], "%X", getpid());
                        goto ex2;
                        }

                  if (nargs < 2 || nargs > 3) goto arg_count_err;

                  l = atoi((char*) &svPtr->v1d[iargs[1]]);

                  strmove(&svPtr->bd[1], &svPtr->v1d[1]);

                  if (nargs != 3) {
                        k = strlen((char*) &svPtr->bd[1]);
                        if (k >= l) goto ex2;
                        if (l > 255) l = 255;
                        lpad((unsigned char *) &svPtr->bd[1], (unsigned char *) &svPtr->bd[1], (short) l);
                        goto ex2;
                        }

                  k = atoi((char*) &svPtr->v1d[iargs[2]]);       /* arg 3 */

#ifndef MULTI_PRECISION

#ifdef BIGFLOAT
                  sprintf((char*) tmp2, "%c%d%c%dLf", '%', l, '.', k);
#else
                  sprintf((char*) tmp2, "%c%d%c%df", '%', l, '.', k);
#endif

#else
                  sprintf((char*) tmp2, "%c%d%c%dRNf", '%', l, '.', k);
#endif
                  mps_justify( (char *) tmp2, (char *) &svPtr->bd[1], ( char *) &svPtr->v1d[1]);
                  goto ex2;

//-----------------------------------------------------------------------
//	$len()
//-----------------------------------------------------------------------

            case 6:                    /* $len */

                  if (nargs == 0) {
                        svPtr->bd[1] = '0';
                        svPtr->bd[2] = 0;
                        return;
                        }

                  strmove(tmp2, &svPtr->v1d[1]);

                  if (nargs == 2) {
                        i = 1;
                        j = 0;
                        if ((k = strlen((char*) &svPtr->v1d[iargs[1]])) == 0) {
                              rslt = 0;
                              goto ex1;
                              }
                        while ((i = xindex(tmp2, &svPtr->v1d[iargs[1]], (short) i)) != 0) {
                              j++;
                              i += k;
                              }
                        rslt = j + 1;
                        goto ex1;
                        }

                  if (nargs > 1) goto arg_count_err;

                  rslt = strlen((char*) tmp2);

ex1:
                  sprintf((char*) &svPtr->bd[1], "%d", rslt);
                  return;

//-----------------------------------------------------------------------
//	$piece()
//-----------------------------------------------------------------------

            case 7:                    /* $piece */

                  if (nargs == 2 ) {
                        if (svPtr->setpiece) {
                              _piece(svPtr->setname, &svPtr->v1d[1], &svPtr->v1d[iargs[1]], (unsigned char *) "1",
                                     (unsigned char *) "1", 1, (unsigned char *) &svPtr->pd1[svPtr->sdlim], svPtr);
                              }
                        else
                              _piece(&svPtr->bd[1], &svPtr->v1d[1], &svPtr->v1d[iargs[1]], (unsigned char *) "1",
                                     (unsigned char *) "1", 0, (unsigned char *) &svPtr->bd[1], svPtr);
                        goto ex2;
                        }

                  if (nargs == 3 ) {
                        if (svPtr->setpiece)
                              _piece(svPtr->setname, &svPtr->v1d[1], &svPtr->v1d[iargs[1]], &svPtr->v1d[iargs[2]],
                                     &svPtr->v1d[iargs[2]], 1, (unsigned char *) &svPtr->pd1[svPtr->sdlim], svPtr);
                        else
                              _piece(&svPtr->bd[1], &svPtr->v1d[1], &svPtr->v1d[iargs[1]], &svPtr->v1d[iargs[2]],
                                     &svPtr->v1d[iargs[2]], 0, (unsigned char *) &svPtr->bd[1], svPtr);
                        goto ex2;
                        }

                  if (nargs != 4) goto arg_count_err;

                  if (svPtr->setpiece)
                        _piece(svPtr->setname, &svPtr->v1d[1], &svPtr->v1d[iargs[1]], &svPtr->v1d[iargs[2]],
                               &svPtr->v1d[iargs[3]], 1, (unsigned char *) &svPtr->pd1[svPtr->sdlim], svPtr);
                  else
                        _piece(&svPtr->bd[1], &svPtr->v1d[1], &svPtr->v1d[iargs[1]], &svPtr->v1d[iargs[2]],
                               &svPtr->v1d[iargs[3]], 0, (unsigned char *) &svPtr->bd[1], svPtr);

                  goto ex2;

//-----------------------------------------------------------------------
//	$select() $storage
//-----------------------------------------------------------------------

            case 8:          //  $select $storage

                  if (nargs == 0) {       /* $storage */

                        rslt = svPtr->PD1;
                        sprintf((char*) &svPtr->bd[1], "%d", rslt);
                        goto ex2;
                        }

//----------
// $select()
//----------

                  i = 0;
sel1:
                  strmove(tmp2, &svPtr->v1d[iargs[i]]); // scan for a true result expt:rslt
                  j = xindex(tmp2, cod209, (short) 1) - 1;
                  if (j <= 0) goto err;
                  if (tmp2[j - 1] == '0') {
                        i++;
                        if (i >= nargs) goto err;
                        else goto sel1;
                        }
                  strmove(&svPtr->bd[1], &tmp2[j + 1]);
                  goto ex2;

//-----------------------------------------------------------------------
//	$test $text() $translate()
//-----------------------------------------------------------------------

            case 9:                    /* $test $text() $translate() */

                  if (nargs == 0) {       /* $test */
                        if (svPtr->tpx == 1) svPtr->bd[1] = '1';
                        else svPtr->bd[1] = '0';
                        svPtr->bd[2] = 0;
                        goto ex2;
                        }

                  if (toupper(S) == 'R') goto trns;  /* $translate() */

//-----------------------------------------------------------------------
//	$text()
//-----------------------------------------------------------------------

                  if (nargs != 1) goto arg_count_err;

                  i = atoi ( (char *) &svPtr->v1d[1] );
                  if ( i <= 0 ) {
                        svPtr->bd[1]=0;
                        goto ex2;
                        }
                  j=1;
                  k=1;
                  while ( k != i ) {
                        j = j + strlen( (char *) &svPtr->pd1[j] ) + 1;
                        if ( j > svPtr->pd1len ) {
                              svPtr->bd[1] = 0;
                              goto ex2;
                              }
                        k++;
                        }
                  i = 1;
                  while( svPtr->pd1[j] != 0 )
                        if ( svPtr->pd1[j] != TAB &&
                                    svPtr->pd1[j] != ' ' ) svPtr->bd[i++] = svPtr->pd1[j++];
                        else {
                              svPtr->bd[i++] = ' ';
                              j++;
                              }
                  svPtr->bd[i] = 0;
                  goto ex2;

//-----------------------------------------------------------------------
//	$translate()
//-----------------------------------------------------------------------

trns:                    /* $translate() function */

                  if (nargs < 2 || nargs > 3) goto arg_count_err;

                  if (nargs == 2) {       /* 2 argument form */
                        char r[256];
                        int i, j;

                        strmove((unsigned char*) r, &svPtr->v1d[iargs[1]]);

                        for (j = 1, i = 1; svPtr->v1d[i] != 0; i++) {
                              if (strchr(r, svPtr->v1d[i]) == NULL) {
                                    svPtr->bd[j++] = svPtr->v1d[i];
                                    }
                              }
                        svPtr->bd[j] = 0;
                        goto ex2;
                        }

                        {
                        /* 3 argument form */

                        char r[256], s[256], t[256] = "";
                        int i, j;

                        strmove((unsigned char*) r, &svPtr->v1d[iargs[1]]);
                        strmove((unsigned char*) s, &svPtr->v1d[iargs[2]]);

                        if (strlen(r) > strlen(s)) {
                              i = strlen(s);
                              strmove((unsigned char*) t, (unsigned char*) &r[i]);
                              }

                        for (i = 1, j = 1; svPtr->v1d[i] != 0; i++) {
                              if (strchr(t, svPtr->v1d[i]) == NULL) {
                                    if (strchr(r, svPtr->v1d[i]) == NULL)
                                          svPtr->bd[j++] = svPtr->v1d[i];
                                    else
                                          svPtr->bd[j++] = s[(long) strchr(r, svPtr->v1d[i]) - (long) r];
                                    }
                              }
                        svPtr->bd[j] = 0;
                        goto ex2;
                        }

//-----------------------------------------------------------------------
//	$random() $reverse()
//-----------------------------------------------------------------------

            case 10:                   /* $random() and $reverse() */

                  if (nargs != 1) goto arg_count_err;

                  if (tolower(S)=='e') { /* $reverse() */
                        j=strlen((const char *) &svPtr->v1d[1])+1;
                        svPtr->bd[j]='\0';
                        for (i=1; svPtr->v1d[i]!='\0'; i++) svPtr->bd[--j]=svPtr->v1d[i];
                        return;
                        }

#ifdef MULTI_PRECISION
//	multiple precision integer random number
                  static gmp_randstate_t state;
                  static mpz_t  na;
                  static mpz_t  nb;
                  static mpz_t  nc;
                  static mpz_t  zero;
                  static mp_bitcnt_t bc;
                  static int flg=1;
                  bc=64;
                  static unsigned long seed;
                  seed=time(NULL);

                  if (flg) {
                        flg=0;
                        mpz_init(na);
                        mpz_init(nb);
                        mpz_init(nc);
                        mpz_init(zero);
                        gmp_randinit_default (state);
                        mpz_set_ui ( na, seed );
                        gmp_randseed_ui ( state, seed );
                        }

                  mpz_urandomb (na, state, 64);

                  gmp_sscanf ((char *)&svPtr->v1d[1], "%Zd", nb );

                  if (mpz_cmp(nb,zero)==0) {
                        printf("*** Zero divisor error in modulo\n");
                        sigint(100);
                        }
                  mpz_mod(nc,na,nb);
                  gmp_sprintf((char *)&svPtr->bd[1],"%Zd",nc);
                  return;
#endif

                  j = atoi((char*) &svPtr->v1d[1]);

                  if (j < 2) {
                        svPtr->bd[1] = '0';
                        svPtr->bd[2] = 0;
                        return;
                        }
                  sprintf((char*) &svPtr->bd[1], "%d", rand()%j );

                  return;

            case 11:                   /* $x */

                  sprintf((char*) &svPtr->bd[1], "%d", svPtr->hor[svPtr->io]);
                  goto ex2;

            case 12:                   /* $y */

                  sprintf((char*) &svPtr->bd[1], "%d", svPtr->ver[svPtr->io]);
                  goto ex2;

            case 13:                   /* $io */

                  sprintf((char*) &svPtr->bd[1], "%ld", svPtr->io);
                  goto ex2;

            case 14:                   /* $horolog */

                  timex = time(&timex);
                  day = timex / 86400;
                  timex = timex - (day * 86400);
                  day = 47116 + day;
                  fd = day+1;
                  sprintf((char*) &svPtr->bd[1], "%ld", fd);
                  strcat((char*) &svPtr->bd[1], ",");
                  fd = timex;
                  sprintf((char*) tmp2, "%ld", fd);
                  strcat((char*) &svPtr->bd[1], (char*) tmp2);
                  return;

//------------------------
//	$get()
//------------------------

            case 15:                   /* $get */

                  if (nargs < 1 || nargs > 3) goto arg_count_err;

// the following lines are unknown

                  if (svPtr->v1d[1]=='\x03') {
                        if (svPtr->v1d[2]=='\x02') {
                              if (nargs == 1) {
                                    strmove(&svPtr->bd[1], (unsigned char *) "");
                                    goto ex2;
                                    }
                              strmove(&svPtr->bd[1], &svPtr->v1d[iargs[1]]);
                              goto ex2;
                              }
                        strcpy((char *) &svPtr->bd[1],(const char *) &svPtr->v1d[2]);
                        goto ex2;
                        }

// actual $get code

                  ctmp = sym_(1, &svPtr->v1d[1], &svPtr->bd[1],svPtr);

                  if (ctmp != NULL) goto ex2; // value returned and in bd

                  if (nargs == 1) {
                        if (ctmp == NULL) strmove(&svPtr->bd[1], (unsigned char *) "");
                        goto ex2;
                        }

                  if (nargs != 2) goto arg_count_err;

                  strmove(&svPtr->bd[1], &svPtr->v1d[iargs[1]]);
                  goto ex2;

//---------------------------
// 	Pattern match codes
//---------------------------

            case 21:  /* $1 */
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29: /* $9 */

                  {
                  char x[10];
                  x[0]='_';
                  x[1]=j;
                  x[2]=S;
                  x[3]='\0';
                  svPtr->ierr = 0;
                  if (sym_(1,(unsigned char *) x,(unsigned char *) &svPtr->bd[1],svPtr)==NULL) {
                        printf("*** %s: No such Perl back reference in or near line %d\n\n",
                               x,svPtr->LineNumber);
                        sigint(100);
                        }
                  return;
                  }

//----------------------------
//	$name()
//	$next()
//	$noerr
//----------------------------

            case 30: /* $name() $next() $noerr */

                  if (toupper(S) == 'O') { /* $noerr */
                        sprintf((char *) &svPtr->bd[1], "%d", svPtr->NOERR);
                        svPtr->ierr=0;
                        return;
                        }

                  if (toupper(S)=='A') { /* $name() section */

                        if (strcmp((const char *) &svPtr->v1d[1],(const char *) "<lcl>")==0) {
                              BuildLocal(2,0,(unsigned char *) "",(unsigned char *) tmp2,svPtr);
                              if (nargs==1) LocalName(tmp2,&svPtr->bd[1],NULL,svPtr);
                              else LocalName(tmp2,&svPtr->bd[1],&svPtr->v1d[iargs[1]],svPtr);
                              }

                        else if (strcmp((const char *) &svPtr->v1d[1],(const char *) "<gbl>")==0) {

                              /* globals processed by compiler have x01 codes embedded.
                              	use LocalName() here because interpreter does not
                              	embed x01's at this point.
                              	*/

                              BuildGlobal(2,0,(unsigned char *) "",(unsigned char *) tmp2,svPtr);
                              if (nargs==1) LocalName(tmp2,&svPtr->bd[1],NULL,svPtr);
                              else          LocalName(tmp2,&svPtr->bd[1],&svPtr->v1d[iargs[1]],svPtr);
                              }
                        svPtr->ierr = 0;
                        return;
                        }

                  /* $next() section */

                  if (toupper(S)!='E' && isalpha(S)) goto err; /* second char not correct */

                  if (strcmp((const char *) &svPtr->v1d[1],(const char *) "<gbl>")==0) {
                        if (BuildGlobal(1,NEXT,&svPtr->v1d[1],&svPtr->bd[1],svPtr)==0) goto err;
                        if (strlen((char *) &svPtr->bd[1]) == 0 ) strcpy((char *) &svPtr->bd[1], "-1");
                        }
                  else {
                        if (BuildLocal(1,NEXT,&svPtr->v1d[1],&svPtr->bd[1],svPtr)==0) goto err;
                        }
                  svPtr->ierr = 0;
                  return;

            case 32: /* $order() section */

                  if (nargs==1) {

                        if (svPtr->v1d[1] == '^') {
                              GlobalOrder(&svPtr->v1d[1], &svPtr->bd[1], (unsigned char *) "1", svPtr);
                              goto ex2;
                              }

                        if (BuildLocal(1, ORDERNEXT, &svPtr->v1d[1], &svPtr->bd[1], svPtr) == 0) goto err;
// fff printf("333333 here it is 2 args  sym \n");
                        goto ex2;
                        }

                  if (nargs!=2) goto err;
// fff printf("333333 3 args  sym \n");

                  if (strcmp((const char *) &svPtr->v1d[iargs[1]], "1") == 0) {

                        if (svPtr->v1d[1] == '^') {
                              GlobalOrder(&svPtr->v1d[1], &svPtr->bd[1], (unsigned char *) "1",svPtr);
                              goto ex2;
                              }

                        if (BuildLocal(1, ORDERNEXT, &svPtr->v1d[1], &svPtr->bd[1], svPtr) == 0) goto err;
                        else goto ex2;
                        }

                  if (strcmp((const char *) &svPtr->v1d[iargs[1]],"-1")==0) {

                        if (svPtr->v1d[1] == '^') {
                              GlobalOrder(&svPtr->v1d[1], &svPtr->bd[1], (unsigned char *) "-1",svPtr);
                              goto ex2;
                              }

                        if (BuildLocal(1, ORDERPREV, &svPtr->v1d[1], &svPtr->bd[1], svPtr) == 0) goto err;
                        else goto ex2;
                        }
                  goto err;


            case 31: /* $query(x)  $qlength(x) $qsubscript(x,y) */

                  /*    $query(arg) */
                  /*    receives arguments unevaluated */


//-------------------------
//	$query()
//-------------------------

                  if (tolower(S)=='u' || !isalpha(S)) { /* $query() */
                        if ( nargs > 1 || nargs ==0 ) goto arg_count_err;

                        if (strcmp((const char *) &svPtr->v1d[1],(const char *) "<lcl>")==0) {
                              BuildLocal(2,0,(unsigned char *) "",(unsigned char *) tmp2,svPtr);
                              LocalName(tmp2,&svPtr->bd[1],NULL,svPtr);
                              strcpy((char *)&svPtr->v1d[1],(char *)tmp2);
                              Qsub((unsigned char *) &svPtr->v1d[1],(unsigned char *) tmp2,
                                   (unsigned char *) "-2",svPtr);
                              for (i=1; svPtr->v1d[i]!=1 && svPtr->v1d[i] != 0; i++);
                              if (svPtr->v1d[i] == 0) {
                                    svPtr->v1d[i++]=1;
                                    svPtr->v1d[i]=0;
                                    }
                              sym_((int)101,&svPtr->v1d[1],&svPtr->bd[1],svPtr);
                              GlobalName(&svPtr->bd[1],0,svPtr); /* name fixer */
                              goto ex2;
                              }

                        else if (strcmp((const char *) &svPtr->v1d[1],(const char *) "<gbl>")==0) {

                              /* globals processed by compiler have x01 codes embedded.
                                          use LocalName() here because interpreter does not
                                          embed x01's at this point.
                                          */

                              BuildGlobal(2,0,(unsigned char *) "",(unsigned char *) tmp2,svPtr);
                              LocalName(tmp2,&svPtr->bd[1],NULL,svPtr);
                              }

                        GlobalQuery(&svPtr->bd[1],&svPtr->bd[1],0,svPtr);
                        goto ex2;

                        }

//-----------------------
//	$qlength()
//-----------------------

                  if (tolower(S)=='l') { /* $qlength() */

                        unsigned char op1[512];

                        if (nargs !=1) goto arg_count_err;


                        if ( strcmp( (char *) &svPtr->v1d[1], "<lcl>") == 0 ) {
                              BuildLocal(2, 0, (unsigned char *) "", op1, svPtr); // extract lcl stack
                              if (Eval( op1, op1, svPtr)) goto qlenerr;
                              }
                        else if ( strcmp( (char *) &svPtr->v1d[1], "<gbl>") == 0 ) {
                              BuildGlobal(2,0,(unsigned char *) "", op1, svPtr); // get global indx nbr
//                        if (Eval( op1, op1, svPtr)) goto qlenerr;
                              }
                        else strcpy ( (char *) op1, (char *) &svPtr->v1d[1]);

                        Qlength(op1, (unsigned char *) &svPtr->bd[1],svPtr);
                        goto ex2;
                        }

//------------------------
//	$qsubscript()
//------------------------

                  if (tolower(S)=='s') { // $qsubscript()
                        char tmp[512];

                        unsigned char op1[512], op2[512];

                        if (nargs !=2) goto arg_count_err;

                        if (strcmp((const char *) &svPtr->v1d[iargs[1]],"-1")==0) {
                              // return current working directory
                              if (getcwd((char *) &svPtr->bd[1],512)==NULL) goto err;
                              goto ex2;
                              }

// order of these operations is important because of the stack
// do not change.


                        if ( strcmp( (char *) &svPtr->v1d[iargs[1]], "<lcl>") == 0 ) {
                              BuildLocal(2, 0, (unsigned char *) "", op2, svPtr); // extract lcl stack
                              for (int i=0; op2[i] != 0; i++)
                                    if (op2[i] == 206) op2[i] = '(';
                                    else if (op2[i] == 207) op2[i] = ')';
                                    else if (op2[i] == 208) op2[i] = ',';

                              if (Eval( op2, op2, svPtr)) goto qsuberr1;
                              }
                        else if ( strcmp( (char *) &svPtr->v1d[iargs[1]], "<gbl>") == 0 ) {
                              BuildGlobal(2,0,(unsigned char *) "", op2, svPtr); // get global indx nbr
                              if (Eval( op2, op2, svPtr)) goto qsuberr1;
                              }
                        else strcpy ( (char *) op2, (char *) &svPtr->v1d[iargs[1]]);

                        if ( strcmp( (char *) &svPtr->v1d[1], "<lcl>") == 0 ) {
                              BuildLocal(2, 0, (unsigned char *) "", op1, svPtr); // extract lcl stack
                              if (Eval( op1, op1, svPtr)) goto qsuberr1;
                              }
                        else if ( strcmp( (char *) &svPtr->v1d[1], "<gbl>") == 0 ) {
                              BuildGlobal(2,0,(unsigned char *) "", op1, svPtr); // get global indx nbr
//                        if (Eval( op1, op1, svPtr)) goto qsuberr1;
                              }
                        else strcpy ( (char *) op1, (char *) &svPtr->v1d[1]);

                        Qsub(op1, &svPtr->bd[1], op2, svPtr);

                        goto ex2;

                        }

                  goto unknown_err; // function not recognized

            }

ex2:

      svPtr->ierr = 0;
      return;

fnum_wrong:

      printf("\n*** Function error: $fnumber pattern error. In or near line %d\n\n", svPtr->LineNumber);
      sigint(100);

qlenerr:

      printf("\n*** Qlength Function argument error. In or near line %d\n\n", svPtr->LineNumber);
      sigint(100);

qsuberr:

      printf("\n*** Qsubscript Function error: 2nd argument. In or near line %d\n\n", svPtr->LineNumber);
      sigint(100);

qsuberr1:

      printf("\n*** Qsubscript Function error: 1nd argument. In or near line %d\n\n", svPtr->LineNumber);
      sigint(100);

arg_count_err:

      printf("\n*** Function error: wrong number of arguments. In or near line %d\n\n", svPtr->LineNumber);
      sigint(100);

unknown_err:

      printf("\n*** Unknown function. In or near line %d\n\n", svPtr->LineNumber);
      sigint(100);

err:

      printf("\n*** Function error in or near line %d\n\n", svPtr->LineNumber);
      sigint(100);
      }

void GlobalQuery(unsigned char * tmp0, unsigned char * tmp1, char tok, struct MSV * svPtrX) {

      int i,j,k=0,oflg=0;
      struct MSV * svPtr;

      i=svPtrX->LineNumber;
      svPtr=AllocSV(); /* push statevector */
      svPtr->LineNumber=i;

      memcpy(svPtr,svPtrX,sizeof(struct MSV)); /* copy old state vector - inefficient */

      svPtr->t0px=1;
      svPtr->t2=1;

      strcpy( (char *) svPtr->xd, (const char *) tmp0);

      svPtr->xpx=0;
      i=parse_(svPtr);

      if (i) {
            printf("** $Query error\n");
            sigint(100);
            }


      keyfix(&svPtr->v1d[1]);

      if ( svPtr->v1d[i=strlen( (const char *) &svPtr->v1d[1])] != 1) { // ^a case - no parens
            svPtr->v1d[++i]=1;
            svPtr->v1d[++i]=1;
            svPtr->v1d[++i]=0;
            }

      char *p1= (char *)malloc(strlen( (const char *)&svPtr->v1d[1])+1);
      strcpy(p1,(const char *)&svPtr->v1d[1]);

      for (i=1; svPtr->v1d[i] != 1 && svPtr->v1d[i] != 0; i++);

      if ((k=Mglobal(XNEXT,&svPtr->v1d[1],tmp1,svPtr))==0) {
            strcpy( (char *) tmp1,"");
            free (svPtr);
            free (p1);
            return;
            }

#if defined(SQLITE)

      free (svPtr);
      free (p1);
      return;

#endif

      if (strncmp(p1,(const char *) tmp1,i)!=0) {
            strcpy( (char *) tmp1,"");
            free (svPtr);
            free (p1);
            return;
            }

      free (p1);

      char tmp3[STR_MAX];
      int tx=1;
      tmp3[0]='^';

      if (tok == 0) {
            for (i=0; tmp1[i]!='\0'; i++) {
                  if (tmp1[i]=='\x01' && !oflg) {
                        tmp3[tx++]='(';
                        tmp3[tx++]='\"';
                        k++;
                        oflg=1;
                        }
                  else if (tmp1[i]=='\x01' && oflg && tmp1[i+1]!='\0') {
                        tmp3[tx++]='\"';
                        tmp3[tx++]=',';
                        tmp3[tx++]='\"';
                        k++;
                        }
                  else if (tmp1[i]=='\x01') {
                        tmp3[tx++]='\"';
                        tmp3[tx++]=')';
                        k++;
                        }
                  else tmp3[tx++]=tmp1[i];
                  }
            }
      else {
            for (i=0; tmp1[i]!='\0'; i++) {
                  if (tmp1[i]=='\x01' && !oflg) {
                        tmp3[tx++]=tok;
                        k++;
                        oflg=1;
                        }
                  else if (tmp1[i]=='\x01' && oflg && tmp1[i+1]!='\0') {
                        tmp3[tx++]=tok;
                        k++;
                        }
                  else if (tmp1[i]=='\x01') {
                        tmp3[tx++]=tok;
                        k++;
                        }
                  else tmp3[tx++]=tmp1[i];
                  }
            }
      tmp3[tx]=0;
      strcpy((char *) tmp1,tmp3);
      free (svPtr);
      return;
      }

void ServiceInterface(unsigned char * tmp0, unsigned char * tmp1, unsigned char * tmp4, unsigned char * tmp5,
                      unsigned char * tmp3, char tok, struct MSV * svPtrX) {

      int i,j,k=0,oflg=0;
      struct MSV * svPtr;
      unsigned char tmpA[STR_MAX],tmpB[STR_MAX],tmpC[STR_MAX];

      tmpA[0]=0;
      tmpB[0]=0;

      if (tok==GETENV) {

            Getenv( (unsigned char *) tmp3, (unsigned char *) tmp0);
            return;
            }

      if (tok==SCANALNUM) {

            strmove( (unsigned char *) tmp3, (unsigned char *) ScanAlnum(svPtrX, (char *) ""));
            return;
            }

      if (tok==SCAN) {

            strmove( (unsigned char *) tmp3, (unsigned char *) Scan(svPtrX));
            return;
            }

      if (tok==INPUT) {
            strmove( (unsigned char *) tmp3, (unsigned char *) Input(svPtrX, (char *) "", (char *) tmp0));
            return;
            }

      i=svPtrX->LineNumber;
      svPtr=AllocSV(); /* push statevector */
      svPtr->LineNumber=i;

      memcpy(svPtr,svPtrX,sizeof(struct MSV)); /* copy old state vector - inefficient */

      if (tok==COSINE) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Cosine(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));
            }

      else if (tok==SIM1) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Sim1(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));
            }

      else if (tok==JACCARD) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Jaccard(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));
            }

      else if (tok==TRANSPOSE) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Transpose(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));
            }

      else if (tok==MMULTIPLY) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp4);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpC,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Multiply(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB, (char *) tmpC));
            }

      else if (tok==CENTROID) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Centroid(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));
            }

      else if (tok==TERMCORRELATE) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) TermCorrelate(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));
            }

      else if (tok==DOCCORRELATE) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpB,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) DocCorrelate(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB, (char *) tmp4, (char *) tmp5) );
            }

      else if (tok==INVDF) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);
            tmpA[strlen((char *)tmpA)+1]=0;

            strcpy(  (char *) tmpB,  (char *) tmp1);

            strmove( (unsigned char *) tmp3, (unsigned char *) IDF(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));

            }

      else if (tok==AVG) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Avg(svPtr,
                        (char *) "", (char *) tmpA ));

            }

      else if (tok==COUNT) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Count(svPtr,
                        (char *) "", (char *) tmpA ));

            }

      else if (tok==MAX) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Max(svPtr,
                        (char *) "", (char *) tmpA ));

            }

      else if (tok==MIN) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Min(svPtr,
                        (char *) "", (char *) tmpA ));

            }

      else if (tok==SUM) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy(  (char *) tmpA,  (char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Sum(svPtr,
                        (char *) "", (char *) tmpA ));

            }

      else if (tok==DICE) {

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp0);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy( (char *) tmpA, (const char *) &svPtr->v1d[1]);

            svPtr->t0px=1;
            svPtr->t2=1;
            strcpy( (char *) svPtr->xd, (const char *) tmp1);
            svPtr->xpx=0;
            i=parse_(svPtr);
            keyfix(&svPtr->v1d[2]);
            strcpy( (char *) tmpB, (const char *) &svPtr->v1d[1]);

            strmove( (unsigned char *) tmp3, (unsigned char *) Dice(svPtr,
                        (char *) "", (char *) tmpA, (char *) tmpB));

            }

      svPtrX->tpx = svPtr->tpx;

      free (svPtr);
      return;
      }

void Qlength(unsigned char * tmp0, unsigned char * tmp2, struct MSV * svPtrX) {

      int i,j=0;
      struct MSV * svPtr;

//if (tmp0[0]!='^') ErrorMessage("$qlength() must have global array name as argument",
//                               svPtr->LineNumber);

      i=svPtrX->LineNumber;
      svPtr=AllocSV(); /* push statevector */
      svPtr->LineNumber=i;

      memcpy(svPtr,svPtrX,sizeof(struct MSV)); /* copy old state vector - inefficient */
      svPtr->t0px=1;
      svPtr->t2=1;
      strcpy( (char *) svPtr->xd, (const char *) tmp0);
      svPtr->xpx=0;
      i=parse_(svPtr);
      keyfix(&svPtr->v1d[1]);
      for (i=1; svPtr->v1d[i]!=0; i++) if (svPtr->v1d[i]==1 || svPtr->v1d[i]>128) j++;
      if (j==0) strcpy( (char *) tmp2,"0");
      else sprintf( (char *) tmp2,"%d",j-1);

      free (svPtr);
      }

void Qsub(unsigned char * tmp0, unsigned char * tmp1, unsigned char * tmp2, struct MSV * svPtrX) {
      int i,j=0,k=0,m=0,n,gblflg=0;
      struct MSV * svPtr;

      if (tmp0[0]=='^') gblflg=1;

      i=svPtrX->LineNumber;
      svPtr=AllocSV(); /* push statevector */
      svPtr->LineNumber=i;

      memcpy(svPtr,svPtrX,sizeof(struct MSV)); /* copy old state vector - inefficient */
      svPtr->t0px=0;
      svPtr->t2=1;
      strcpy( (char *) svPtr->xd, (const char *) tmp0);
      svPtr->xpx=0;
      j=svPtr->gpad;
      svPtr->gpad=0;
      i=parse_(svPtr);
      keyfix(&svPtr->v1d[1]);
      svPtr->gpad=j;
      if (gblflg)
            strcpy( (char *) &tmp0[1], (const char *) &svPtr->v1d[1]);
      else  strcpy( (char *) tmp0, (const char *) &svPtr->v1d[1]);
      if (tmp2 != NULL) j=atoi( (const char *) tmp2);

      if (j==-1) {
            strcpy( (char *) tmp1,"");
            free (svPtr);
            return;
            }

      if (j==-2) {
            strcpy( (char *) tmp1, (const char *) tmp0);
            free (svPtr);
            return;
            }

      n=strlen( (const char *) tmp0);
      i=0;
      for (k=0; k<=j; k++) {
            m=i;
            if (m>=n) {
                  strcpy( (char *) tmp1,"");
                  free (svPtr);
                  return;
                  }
            for (; tmp0[i]!='\0'; i++) {
                  if (tmp0[i]=='\x01'||tmp0[i]>128) {
                        tmp0[i]='\0';
                        break;
                        }
                  }
            i++;
            }
      strcpy( (char *) tmp1, (const char *) &tmp0[m]);

      free (svPtr);
      return;
      }

//======================================================
//
// evaluate an expression - non zero return is error
//
//======================================================

int Eval(unsigned char * tmp0, unsigned char * tmp2, struct MSV * svPtrX) {

      int i;
      struct MSV * svPtr;

      i=svPtrX->LineNumber;
      svPtr=AllocSV(); /* push statevector */
      svPtr->LineNumber=i;

      memcpy(svPtr,svPtrX,sizeof(struct MSV)); /* copy old state vector - inefficient */
      svPtr->t2=1;
      strcpy( (char *) svPtr->xd, (const char *) tmp0);
      svPtr->xpx=0;
      i=parse_(svPtr);
      strcpy( (char *) tmp2,  (const char *) &svPtr->pd1[svPtr->sdlim]);
      free (svPtr);
      return i;
      }


/*===========================================================================*
 *                                  _ascii                                   *
 *===========================================================================*/
void _ascii(unsigned char out[], unsigned char in[], unsigned char begin[]) {

      int i;
      i = atoi((char *) begin);
      if (i < 1)
            i = 1;

      if (i > strlen((char *) in)) {
            strmove(out, (unsigned char *) "-1");
            return;
            }

      i = in[i - 1];
      sprintf((char *) out, "%d", i);
      return;
      }

/*===========================================================================*
 *                                 _extract                                  *
 *===========================================================================*/
void _extract(unsigned char * out, unsigned char * in,
              unsigned char * start, unsigned char * len) {

      int i, j, k, n;

      i = atoi((char *) start);
      j = atoi((char *) len);

      if (i < 0)
            i = 0;
      else
            i--;

      if (j < 0)
            j = i;
      else
            j--;

      if (j < i) {
            strmove(out, (unsigned char *) "");
            return;
            }

      if (i == 0 && j == 0) {
            out[0] = in[0];
            out[1] = '\0';
            return;
            }

      n = strlen((char *) in);

      if (i > n) {
            strmove(out, (unsigned char *) "");
            return;
            }

      if (j > n)
            j = n;
      for (k = 0; i <= j; i++)
            out[k++] = in[i];
      out[k] = 0;
      return;
      }

/*===========================================================================*
 *                                   _find                                   *
 *===========================================================================*/
void _find(unsigned char * out, unsigned char * in,
           unsigned char * key, unsigned char * begin) {

      int i;
      i = atoi((char *) begin);
      if (i < 0)
            i = 0;

      if (key[0] != 0) {
            if ((i = xindex(in, key, (short) i)) > 0)
                  i += strlen((char *) key);
            }

      if (i > strlen((char *) in) + 1)
            i = 0;

      sprintf((char *) out, "%d", i);
      return;
      }

/*===========================================================================*
 *                                 _horolog                                  *
 *===========================================================================*/
void _horolog(unsigned char in[]) {

      long day, fd;
      time_t timex;
      char tmp2[64];

      timex = time(&timex);
      day = timex / 86400;
      timex = timex - (day * 86400);
      day = 47122 + day;
      fd = day;
      sprintf((char*) in, "%ld", fd);
      strcat((char*) in, ",");
      fd = timex;
      sprintf(tmp2, "%ld", fd);
      strcat((char*) in, tmp2);
      return;
      }

/*===========================================================================*
 *                                 _justify                                  *
// may be dead
 *===========================================================================*/
void _justify(unsigned char out[], unsigned char in[],
              unsigned char w[], unsigned char p[]) {

      int l, k;
      char bd[STR_MAX], tmp2[32];
      double t1;
      l = atoi((char *) w);
      strncpy((char *) bd, (char *) in, STR_MAX);

      if (strcmp((char *) p, "-1") == 0) {
            k = strlen(bd);

            if (k >= l) {
                  strncpy((char *) out, (char *) in, STR_MAX);
                  return;
                  }

            if (l > 255)
                  l = 255;
            lpad((unsigned char *) bd, (unsigned char *) bd, (short) l);
            strncpy((char *) out, bd, STR_MAX);
            return;
            }

      k = atoi((char *) p);       /* arg 3 */
      sprintf(tmp2, "%c%d%c%dlf", '%', l, '.', k);
      t1 = atof((char *) in);
      sprintf(bd, tmp2, t1);
      strncpy((char *) out, bd, STR_MAX);
      return;
      }

/*===========================================================================*
 *                                  _length                                  *
 *===========================================================================*/
void _length(unsigned char out[], unsigned char in[], unsigned char key[]) {

      int i, j, k;
      if (strlen((char *) key)) {
            i = 1;
            j = 0;
            k = strlen((char *) key);

            while ((i = xindex(in, key, (short) i)) != 0) {
                  j++;
                  i += k;
                  }

            sprintf((char *) out, "%d", j + 1);
            return;
            }

      sprintf((char *) out, "%d", strlen((char *) in));
      return;
      }

//===========================================================================
//                                  _piece
//===========================================================================

void _piece(unsigned char out[],
            unsigned char in[],
            unsigned char key[],
            unsigned char start[],
            unsigned char end[],
            int setpiece,
            unsigned char lhs[],
            struct MSV * svPtr) {

      /* for setpiece=1:
         out is name of variable into which to store result
         in is input value of variable given in out
         key is the pattern code
         start and end are location counters
         lhs is the value to be inserted into in
      */


      unsigned char tmp2[STR_MAX], tmp3[STR_MAX];
      int k, l, m, n;

      if (strcmp((char *) start, "-1") == 0) {    /* no start or end */
            k = 1;
            l = 1;
            goto p1;
            }

      strmove(tmp2, start);
      k = atoi((char *) tmp2);    /*start */

      l = k;                      /* default end is same as start */


      if (strcmp((char *) end, "-1") != 0) {      /* end? */
            strmove(tmp2, end);
            l = atoi((char *) tmp2);
            }

      if (k < 0 || l < k) {       /* check */
            printf("** $Piece error start=%d end=%d\n",k,l);
            sigint(100);
            }

p1:

      strmove(tmp2, in);
      strmove(tmp3, key);

      if (tmp2[0] == 0 || tmp3[0] == 0 || k > l || l <= 0) {
            if (setpiece == 0) strmove(out, (unsigned char *) "");
            return;
            }

      m = 0;
      n = 0;

      while (n < k - 1) {
            m = xindex(tmp2, tmp3, (short) m) + 1;

            if (m == 1) {
                  if (setpiece == 0) strmove(out, (unsigned char *) "");
                  return;
                  }

            n++;
            }


      if (k != 1) k = m + strlen((char *) tmp3) - 1;

      m=0;
      n=0;
      while (n != l) {
            m = xindex(tmp2, tmp3, (short) (m + 1));

            if (m <= 0) {
                  m = strlen((char *) tmp2) + 1;
                  goto piece1;
                  }

            else n++;
            }

      m = m - k;

piece1:

      if (m == 0 && setpiece == 0) {
            if (setpiece == 0)
                  strmove(out, (unsigned char *) "");
            return;
            }

      if (setpiece == 1) {
            char tmp[STR_MAX];
            strmove((unsigned char *) tmp, in);
            substr(tmp2, (unsigned char *) tmp, 1, k - 1);
            tmp[k] = 1;
            tmp[k + 1] = 0;
            strcat(tmp, (char *) lhs);
            if (k + m - 1 < strlen((char *) tmp2))
                  strcat(tmp, (char *) &tmp2[k + m - 1]);
            if (out[0] != '^') sym_(0, out, (unsigned char*) tmp,svPtr);
            else Mglobal (STORE, out, (unsigned char *) tmp, svPtr);
            return;
            }

      substr(tmp2, out, k, m);
      return;
      }

/*===========================================================================*
 *                                  _random                                  *
 *===========================================================================*/
void _random(unsigned char out[], unsigned char in[]) {

      int rslt, j;
      static int first = 1;
      time_t timex;

      if (first) {
            j = (int) time(&timex);
            srand(j);
            first = 0;
            }

      j = atoi((char *) in);
      if (j < 2) {
            strmove(out, (unsigned char *) "0");
            return;
            }

      while ((rslt = (long) ((double) j * rand() / (RAND_MAX + 1.0))) >= j);
      sprintf((char *) out, "%d", rslt);
      return;
      }

/*===========================================================================*
 *                                  _select                                  *
 *===========================================================================*/
void _select(unsigned char * out,
             unsigned char *in1,
             unsigned char *in2,
             unsigned char *in3,
             unsigned char *in4,
             unsigned char *in5,
             unsigned char *in6,
             unsigned char *in7,
             unsigned char *in8,
             unsigned char *in9,
             unsigned char *in10,
             unsigned char *in11,
             unsigned char *in12,
             unsigned char *in13,
             unsigned char *in14,
             unsigned char *in15,
             unsigned char *in16,
             unsigned char *in17,
             unsigned char *in18,
             unsigned char *in19,
             unsigned char *in20,
             struct MSV * svPtr) {

      if (atol((char *) in1) != 0) {
            strmove(out, in2);
            return;
            }

      if (in3 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in3) != 0) {
            strmove(out, in4);
            return;
            }

      if (in5 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in5) != 0) {
            strmove(out, in6);
            return;
            }

      if (in7 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in7) != 0) {
            strmove(out, in8);
            return;
            }

      if (in9 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in9) != 0) {
            strmove(out, in10);
            return;
            }

      if (in11 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in11) != 0) {
            strmove(out, in12);
            return;
            }

      if (in13 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in13) != 0) {
            strmove(out, in14);
            return;
            }

      if (in15 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in15) != 0) {
            strmove(out, in16);
            return;
            }

      if (in17 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in17) != 0) {
            strmove(out, in18);
            return;
            }

      if (in19 == NULL) {
            printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
            sigint(100);
            }

      if (atol((char *) in19) != 0) {
            strmove(out, in20);
            return;
            }

      printf("*** Error M4 - Select in or near line %d\n", svPtr->LineNumber);
      sigint(100);
      }

int ScanParse(char * tmp1) {

      int i=0,p=0;

      while (1) {

            if (tmp1[i] == 0) break;
            if (tmp1[i] == ' ') break;
            if (tmp1[i] == ',' && p == 0) break;
            if (tmp1[i]=='(') p++;
            if (tmp1[i]==')') p--;
            if (tmp1[i] != '\"') {
                  ++i;
                  continue;
                  }
            ++i;
            while (1) {
                  if (tmp1[i] == 0 ) break;
                  if (tmp1[i] != '\"' ) {
                        ++i;
                        continue;
                        }
                  ++i;
                  break;
                  }
            }
      return i;
      }

//-----------------------------------------------------------------------
//	$fnumber() wrapper for compiler
//-----------------------------------------------------------------------

void _fnumber(unsigned char * tmp0, unsigned char * number, unsigned char * arg1, unsigned char * arg2, MSV * svPtr) {

      char open[]  = {CodedOpen,0};
      char comma[] = {CodedComma,0};
      char close[] = {CodedClose,0};

      strcpy((char *) &svPtr->v1d[1], "$fn");
      strcat((char *) &svPtr->v1d[1], open);

      strcat((char *) &svPtr->v1d[1], (char *) number);
      strcat((char *) &svPtr->v1d[1], comma);

      strcat((char *) &svPtr->v1d[1], (char *) arg1);
      strcat((char *) &svPtr->v1d[1], comma);

      strcat((char *) &svPtr->v1d[1], (char *) arg2);
      strcat((char *) &svPtr->v1d[1], close);

      fcn (svPtr);

      strcpy((char *) tmp0, (char *) &svPtr->bd[1]);

      return;
      }

/*
char * _text_function(int i) { return 0; }
int _label_lookup(char * x) { return -1; }

int (*__label_lookup)(char *) = _label_lookup;
char* (*__text_function)(int) = _text_function;
*/


