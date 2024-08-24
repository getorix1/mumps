/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+   
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2015, 2024 by Kevin C. O'Kane  
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     http://threadsafebooks.com
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

//	April 6, 2024

#ifndef MUMPSLIB_H
#define MUMPSLIB_H
#define MPSCPP
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <cctype>
#include <set>
#include <algorithm>
#include <assert.h>
#include <stdexcept>
#include <fstream>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include <mumpsc/btree.h>
#include <mumpsc/global.h>
#include <mumpsc/arith.h>

#ifdef _MDH_

char * _text_function(int i) { return 0; }
int _label_lookup(char * x) { return -1; }

int (*__label_lookup)(char *) = _label_lookup;
char* (*__text_function)(int) = _text_function;

#endif

using namespace std;

extern int GlobalExceptionCode;

#define FTELL ftello
#define FSEEK fseeko

// GlobalExceptionCode values

#define UNSPECIFIED 0
#define INVALID_CONVERSION 1
#define GLOBAL_NOT_FOUND 2
#define VARIABLE_NOT_FOUND 3
#define DATA_RANGE_ERROR 4
#define NUMERIC_RANGE 5
#define INPUT_ERROR 6
#define PATTERN_ERROR 7
#define INTERPRETER_ERROR 8
#define SYMTAB_ERROR 9
#define INPUT_LENGTH 10

#define BIGLONG long

// Some older compilers do not have log2() function.
// Use log() instead.

#define LOG2 log2

char*   embed1(const char *in); // embed "" for "

class mstring;
class global;
class indirect;
class MumpsGlobalException;

#define $test __test()
#define $perl __perl
#define $SymGet __symget
#define $SymPut __symput
#define $lock __lock
#define $name __name
#define $pattern __pattern
#define $query __query
#define $queryX __queryX

#if defined(__STDC_HOSTED__) 
#define char_global(name,...) (name.MakeGlobal(0,#name,#__VA_ARGS__,__VA_ARGS__ ))
#define int_global(name,...) (name.MakeGlobal(1,#name,#__VA_ARGS__,__VA_ARGS__ ))
#define Xecute(exp) (xecute(__command__(#exp)))
#define write(...) (svPtr->tpx=WriteMumps1(svPtr,#__VA_ARGS__))
#endif

#if !defined(__STDC_HOSTED__) 
#define write(x) (svPtr->tpx=WriteMumps1(svPtr,#x))
#endif

void relationalXnext ( unsigned char * tmp1, unsigned char * right, struct MSV * svPtr);

#define BTREE(code,key,data) (Mglobal(code,key,data,svPtr))
#define get_global(ref,rslt) (svPtr->tpx=Mglobal(RETRIEVE,(unsigned char *)ref,(unsigned char *)rslt,svPtr),rslt)
#define set_global(ref,rslt) (svPtr->tpx=Mglobal(STORE,(unsigned char *)ref,(unsigned char *)rslt,svPtr),rslt);
#define GlobalClose Mglobal(CLOSE,(unsigned char *)"",(unsigned char *)"",svPtr)
#define ltoa(out,in) (sprintf(out,"%ld",in),out)
#define ftoa(out,in) (sprintf(out,"%f",in),out)
#define string(x) char x[STR_MAX]
#define command(ref) __command__(#ref)

void    mdh_tree_level_add(GtkTreeStore *tree, int depth, char * col1);
void    mdh_tree_level_add(GtkTreeStore *tree, int depth, char * col1, char * col2);
void    mdh_tree_level_add(GtkTreeStore *tree, int depth, char * col1, char * col2, char * col3);
void    mdh_tree_level_add(GtkTreeStore *tree, int depth, char * col1, char * col2, 
	char * col3, char * col4);
void    mdh_tree_level_add(GtkTreeStore *tree, int depth, char * col1, char * col2, 
	char * col3, char * col4, char *col5);
int     mdh_dialog_new_with_buttons(GtkWindow *win, char * text);
double  mdh_range_get_adjustment(GtkRange *e);
char *  mdh_calendar_get_date(GtkCalendar *e);
int	mdh_toggle_button_get_active(GtkToggleButton *b) ;
char *	mdh_color_chooser_get_rgba(GtkColorChooser *b) ;
char *	mdh_font_chooser_get_font(GtkFontChooser *b) ;
int	mdh_switch_get_active(GtkSwitch *b) ;
void	mdh_toggle_button_set_active(GtkToggleButton *b, int v) ;
char *	mdh_entry_get_text(GtkEntry *e, char *) ;
void	mdh_entry_set_text(GtkEntry *e, char * txt) ;
void	mdh_text_buffer_set_text(GtkTextBuffer *t, char * txt) ;
void	mdh_label_set_text(GtkLabel *l, char * txt) ;
char *	mdh_tree_selection_get_selected(GtkTreeSelection *t, int col, char *) ;
void	mdh_tree_store_clear(GtkTreeStore *t) ;
double	mdh_spin_button_get_value(GtkSpinButton *s) ;
void	mdh_spin_button_set_value(GtkSpinButton *s, double v) ;
void    mdh_widget_hide(GtkWidget *w) ;
void    mdh_widget_show(GtkWidget *w) ;

char * Piece(const char * source, char * rslt, const char *pat, int ii, int jj);

int SQL_Disconnect();
int SQL_Connect(char *);
int SQL_Connect(string);
int SQL_Connect(mstring);

int SQL_Format(mstring);
int SQL_Format(string);
int SQL_Format(char *);
int SQL_Format();

int SQL_Command(mstring);
int SQL_Command(string);
int SQL_Command(char *);
mstring SQL_Table();
mstring SQL_Table(mstring, int=10);
mstring SQL_Table(string, int=10);
mstring SQL_Table(char *, int=10);
mstring SQL_Message();
bool SQL_Open();
bool SQL_Native();

bool Ztell(FILE *, mstring&);
bool Ztell(FILE *, global&);
bool Zseek(FILE *, mstring &);
bool Zseek(FILE *, global &);

mstring SymGet(mstring);
mstring SymGet(char *);
mstring SymGet(global);

bool SymPut(mstring &, mstring &);
bool SymPut(mstring &, global &);
bool SymPut (mstring &, char *);

bool SymPut (char *, mstring &);
bool SymPut (char *, global &);
bool SymPut (char *, char *);

bool SymPut (global &, char *);
bool SymPut (global &, mstring &);
bool SymPut (global &, global &);

mstring TokenInit(mstring &);
mstring Token();

void __symput(const char *,const char *);

#define __query(str) (__Query(str,0,svPtr))
#define __queryX(str,pat) (__Query(str,pat,svPtr))
#define __name(ref) ref.Name()

int Perl(mstring &, mstring &);
int Perl(mstring &, char *);

int perl_pm(const char *, const char *, const int, struct MSV *);

int wstem(char *, int);
extern struct MSV * svPtr; 
void GlobalQuery(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
int Interpret(const char *,struct MSV *);
void _piece(unsigned char *, unsigned char *, unsigned char *, unsigned char *,
	unsigned char *, int, unsigned char *, struct MSV *);
char * cleanLocks(char *);
char * makeLock(char *,char *, char *, char, char *);


void _extract(unsigned char *, unsigned char *, unsigned char *, unsigned char *);

extern "C" void bmg_setup(char *, int);
extern "C" int bmg_search(char *, int, int (*)(char*,int));
extern "C" int bmg_fullsearch(char *, char *);
extern "C" int bmg_fsearch(int, int (*)(char*,int));

int bmg_fullsearch(string, string);
int bmg_fullsearch(mstring, mstring);
int bmg_fullsearch(mstring, global);
int bmg_fullsearch(string, global);
int bmg_fullsearch(char *, global);

int pmw(char *, char *, struct MSV *);
int pmw(string, char *, struct MSV *);
int pmw(char *, string, struct MSV *);
int pmw(string, string, struct MSV *);
int pmw(mstring, mstring, struct MSV *);
int pm(const unsigned char *, const unsigned char *, struct MSV *);
void _find(unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void _horolog(unsigned char *);
int stem(char *, int); // stem(word,word_length) returns new length, mods word.
int perlcpp(char *, const char *, const char *, struct MSV *);
void LocalName(unsigned char *, unsigned char *, unsigned char *, struct MSV *);

int nodash(char *);
int Score(char, char);
int prnt(int, char *, char *, int, int, int); 
void init();
int Cost(char *, char *, int , int );
int sw(char *, char *, int=0, int=0, int=-1, int=-1, int=-2);
int sw(mstring, mstring, int=0, int=0, int=-1, int=-1, int=-2);
int sw(string, string, int=0, int=0, int=-1, int=-1, int=-2);

mstring Stem(mstring &);

void Dump(mstring &);
void Dump(string &);
void Dump(char *);

void Restore (mstring &);
void Restore(string &);
void Restore(char *);

bool ZSeek(FILE *, mstring &);
bool ZTell(FILE *, mstring &);

bool ZSeek(FILE *, global &);
bool ZTell(FILE *, global &);

string stem(string &);
mstring stem(mstring &);
char * stem(char *);
char * chash(const char *);
unsigned long lhash(const char *);
char * __Query(char *,char, struct MSV *);
void KiLL(char *, struct MSV *);
int RunMumps(struct MSV *, const char *, char *, ...);
int xecute(char *);
int xecute(string);
int xecute(mstring);
int __test();
char * Shred(char *, int);
mstring Shred(mstring, int);
string Shred(string, int);
char * ShredQuery(char *, int);
mstring ShredQuery(mstring, int);
string ShredQuery(string, int);

char * Justify(const char *, struct MSV *, char *, ...);
char * Justify(global, struct MSV *, char *, ...);
char * Justify(string, struct MSV *, char *, ...);
mstring Justify(mstring, struct MSV *, char *, ...);
int WriteMumps1(struct MSV *,char *);
void CleanLocks(void);
void CleanAllLocks(void);
double HitRatio(void);
mstring EncodeHTML(char *);
mstring EncodeHTML(mstring &);
char * __command__(char *);

mstring mcvt(char *);
mstring mcvt(long);
mstring mcvt(float);
mstring mcvt(double);
mstring mcvt(int);

char * cvt(long);
char * cvt(float);
char * cvt(double);
char * cvt(int);

int GlobalSet(string,string);
int GlobalSet(mstring,mstring);
int GlobalSet(char *,string);
int GlobalSet(char *,mstring);
int GlobalSet(string,char *);
int GlobalSet(mstring,char *);
int GlobalSet(char*,char*);
char * GlobalGet(char *);
string GlobalGet(string);
mstring GlobalGet(mstring);
char * GlobalOrder(char *, int);
string GlobalOrder(string, int);
mstring GlobalOrder(mstring, int);
int GlobalData(char *);
int GlobalData(string);
int GlobalData(mstring);
void Transpose(char *, char *);
void Transpose(string, string);
void Transpose(mstring, mstring);
void Multiply(string, string, string);

class MumpsGlobalException  {
public:
      MumpsGlobalException() : message ((char *) "Global reference not found") {};
      char *what() { return message; }
private:
      char * message;
      };

class indirect {
      public:
            friend indirect operator*(string);
             indirect operator=(string);
            string reference;
      private:
            string xreference;
      };

int Qlength(mstring &);
int Qlength(char *);
mstring Query(mstring &);
mstring Query(const char *);
mstring Qsubscript(mstring &, mstring &);
mstring Qsubscript(mstring &, int);
mstring Qsubscript(const char *, int);

#include <mumpsc/globalClass.h>

class Hash {
 public:
      Hash(string="hash",long=100000,string="new");
      ~Hash();
      string operator=(string);
      mstring operator=(mstring);
      Hash & operator()(string);
      Hash & operator()(mstring);
      operator const char*();
 private:
      string key;
      string data;
      size_t size;
      FILE * hashkey;
      FILE * hashdata;
      string ref;
      };

typedef set < string, less<string> > StopList;
void StopInit(string);
void StopInit(mstring);
void StopInit(char *);
int StopLookup(mstring);
int StopLookup(string);
int StopLookup(char *);

#define SYNMAX 20000

struct Syn {
      char *word;
      char *alias;
      };

char * SYN(char *);
mstring SYN(mstring);
string SYN(string);
int SynInit(string);
int SynInit(mstring);
int SynInit(char *);

mstring Horolog();
mstring ScanAlnum(FILE *, int min=3, int max=25);
mstring ScanAlnum(istream, int min=3, int max=25);

#include <mumpsc/mstringClass.h>

unsigned char * VariableNotFound(long, char *);
inline void strcatx(unsigned char *, unsigned char *);
char * strcatz(unsigned char *, unsigned char *);
void ErrorMessage(const char *, long);
char * sym_(int, unsigned char *, unsigned char *, struct MSV *);
void zfcn(unsigned char *, unsigned char *, struct MSV *);
int numcomp(unsigned char *, unsigned char *);
int numcomp1(unsigned char *, long);
void mult1(const char *, long, char *);
char * add1(char *, long, char *);
// void GlobalOrder(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
void GlobalQS(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
int _SymFree(int,  struct MSV * );
void LocalData(unsigned char *, unsigned char *, struct MSV *);
void LocalOrder(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
void LocalQS(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
void LocalNext(unsigned char *, unsigned char *, struct MSV *);
void LocalGet(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
void GlobalNext(unsigned char *, unsigned char *, struct MSV *);
void GlobalGet(unsigned char *, unsigned char *, unsigned char *, struct MSV *);
void GlobalData(unsigned char *, unsigned char *, struct MSV *);
int Merge(unsigned char *, unsigned char *, struct MSV *);
int getstr1(FILE * opnfile, unsigned char []);
void expx(const char *, const char *, char *);
void divi(const char *, const char *, char *);
void _justify(unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void fcn(MSV *);
void _select(unsigned char *, unsigned char *, unsigned char *,
             unsigned char *in3, unsigned char *in4, unsigned char *in5,
             unsigned char *in6, unsigned char *in7, unsigned char *in8,
             unsigned char *in9, unsigned char *in10, unsigned char *in11,
             unsigned char *in12, unsigned char *in13, unsigned char *in14,
             unsigned char *in15, unsigned char *in16, unsigned char *in17,
             unsigned char *in18, unsigned char *in19,
             unsigned char *in20, struct MSV * svPtr);

#endif
