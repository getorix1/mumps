/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//* Except as specifically noted otherwise in the bmgsubs at the end of
//* this file, the following copyright and license applies:
//*---------------------------------------------------------------------
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2004, 2008, 2013, 2015, 2017, 2024
#+      by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     http://www.cs.uni.edu/~okane
#+     http://threadsafebooks.com/
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

#define MPSCPP
#define _INTERP_
#include <exception>
#include <iostream>
#include <string>
#include <cctype>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include <mumpsc/defines.h>
#include <mumpsc/btree.h>
#include <mumpsc/libmpscpp.h>
#include <pcre.h>
#include <mumpsc/sym.h>
#include <mumpsc/global.h>
#include <mumpsc/globalOrder.h>
#include <mumpsc/keyfix.h>
#include <mumpsc/inline.h>

#define VERSION "@VERSION@"

#define LOG2 log2

#define NO 0
#define YES 1

#ifdef GCVT
#define gcvt_wrapper(x,i,a) sprintf(a, "%g", x)
#define GETPID getpid

#else
#define gcvt_wrapper(x,i,a) gcvt(x, (size_t) i, (char *) a)
#define GETPID getpid
#endif

void sigint(int);

int GlobalExceptionCode = UNSPECIFIED;

void thrownMessage() {
      cout << "Mumps has detetected an uncaught exception: " << endl;

      if (GlobalExceptionCode==INVALID_CONVERSION)
            cout << "Invalid conversion from global to numeric" << endl;

      if (GlobalExceptionCode==GLOBAL_NOT_FOUND)
            cout << "Global not found" << endl;

      if (GlobalExceptionCode==INPUT_LENGTH)
            cout << "Input Line Too Long" << endl;
      }

MSV *svPtr = AllocSV(); // global initial stack frame
int global::Eflg=0;

void __finish() {
      static int flg=0;

      if (flg) _exit(0); // immediate terminate

      GlobalClose;
      flg=1;
      exit(0);
      }

int global::Int() {
      int i;
      (*this) >> i;
      return i;
      }

double global::Double() {
      double i;
      (*this) >> i;
      return i;
      }

mstring global::Mstring() {
      mstring i;
      (*this) >> i;
      return i;
      }

char * global::Char(char * i, int max) {
      char tmp[STR_MAX];
      (*this)>>tmp;
      strncpy(i, tmp, max);
      return i;
      }

mstring Stem(mstring &arg) {
      mstring tmp3;
      char tmp[STR_MAX];
      strcpy(tmp,arg.c_str());
      wstem((char *) tmp, strlen((char *) tmp));
      tmp3 = tmp;
      return tmp3;
      }

mstring TokenInit(mstring &arg) {
      char tmp[STR_MAX] = " $zwi\xce";
      char tmp2[STR_MAX];
      mstring rslt;
      strcat (tmp,(char *) arg.c_str());
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      rslt=(char *) (svPtr->bd);
      return rslt;
      }

mstring Token() {
      char tmp[STR_MAX] = " $zwp";
      char tmp2[STR_MAX];
      mstring rslt;
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      rslt=tmp2;
      return rslt;
      }


//------------------------
//	GTK interface
//------------------------

//---------------------------------------------------------------------
//	$z~mdh~tree~level~add(treeStore,treeDepth,col1,col2,col3,...)
//---------------------------------------------------------------------

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3, char * col4) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3,
                         char * col4, char * col5) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col5);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3,
                         char * col4, char * col5, char *col6) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col5);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col6);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3,
                         char * col4, char * col5, char *col6,
                         char *col7) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col5);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col6);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col7);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3,
                         char * col4, char * col5, char * col6,
                         char * col7, char * col8) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col5);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col6);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col7);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col8);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3,
                         char * col4, char * col5, char * col6,
                         char * col7, char * col8, char * col9) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col5);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col6);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col7);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col8);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col9);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }

void	mdh_tree_level_add(GtkTreeStore *tree, int depth,
                         char * col1, char * col2, char * col3,
                         char * col4, char * col5, char * col6,
                         char * col7, char * col8, char * col9,
                         char * col10) {

//----------------------
//	xce open paren
//	xcf close paren
//	cd0 comma
//----------------------

      char tmp[4096], tmp1[256], tmp2[256];
      strcpy(&tmp[1],"$z~mdh~tree~level~add");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", tree);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      sprintf(tmp1, "%d", depth);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col2);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col3);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col4);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col5);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col6);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col7);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col8);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col9);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1],col10);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      }


//-------------------------------------------------------
//	$z~mdh~dialog~new~with~buttons(window,string)
//-------------------------------------------------------

int	mdh_dialog_new_with_buttons(GtkWindow *win, char * text) {

      char tmp[4096], tmp1[256];
      strcpy(&tmp[1],"$z~mdh~dialog~new~with~buttons");
      strcat(&tmp[1],"\xce");

      sprintf(tmp1, "%p", win);
      strcat(&tmp[1],tmp1);
      strcat(&tmp[1],"\xd0");

      strcat(&tmp[1], text);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp1, svPtr);

      if (tmp1[0] == '0') return 0;

      return 1;

      }

//-------------------------------------------------------------
//	$z~mdh~toggle~button~get~active(ToggleButtonReference)
//-------------------------------------------------------------

int	mdh_toggle_button_get_active(GtkToggleButton *b) {

      return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));

      }

//-------------------------------------------------------------
//	$z~mdh~switch~get~active(ToggleButtonReference)
//-------------------------------------------------------------

int	mdh_switch_get_active(GtkSwitch *b) {

      return gtk_switch_get_active(GTK_SWITCH(b));

      }

//---------------------------------------------------------------------
//	$z~mdh~toggle~button~set~active(ToggleButtonReference,intVal)
//---------------------------------------------------------------------

void	mdh_toggle_button_set_active(GtkToggleButton *b, int v) {

      gboolean x;

      if (v == 0) x = FALSE;
      else x = TRUE;

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b), x);

      }

//------------------------------------------------
//	$z~mdh~entry~get~text(EntryReference)
//------------------------------------------------

char *	mdh_entry_get_text(GtkEntry *e, char * txt) {

      strcpy(txt, gtk_entry_get_text(GTK_ENTRY(e)));

      return txt;
      }

//----------------------------------------------------
//	$z~mdh~range~get~adjustment
//----------------------------------------------------

double	mdh_range_get_adjustment(GtkRange *e) {
      GtkAdjustment *adj = gtk_range_get_adjustment ( (GtkRange*) e);
      return gtk_adjustment_get_value(adj);
      }

//----------------------------------------------------
//	$z~mdh~calendar~get~date
//----------------------------------------------------

char *	mdh_calendar_get_date(GtkCalendar *e) {

      static char tmp[256];
      guint mn, dy, yr;
      gtk_calendar_get_date ((GtkCalendar *) e, &yr, &mn, &dy);
      sprintf((char *) tmp, "%d %d %d", yr, mn, dy);
      return tmp;
      }



//----------------------------------------------------
//	$z~mdh~entry~set~text(EntryReference,value)
//----------------------------------------------------

void	mdh_entry_set_text(GtkEntry *e, char * txt) {

      gtk_entry_set_text(e,txt);

      }

//---------------------------------------------------------------
//	$z~mdh~text~buffer~set~text(TextBufferReference,string)
//---------------------------------------------------------------

void	mdh_text_buffer_set_text(GtkTextBuffer *t, char * txt) {

      gtk_text_buffer_set_text(t, txt, -1);

      }

//-----------------------------------------------------
//	$z~mdh~font~chooser~get~rgba(GtkFontChooser r)
//-----------------------------------------------------

char *  mdh_font_chooser_get_font(GtkFontChooser *b) {
      static char tmp[256];
      sprintf((char *) tmp, "%s",gtk_font_chooser_get_font(b));
      return tmp;
      }

//-----------------------------------------------------
//	$z~mdh~color~chooser~get~rgba(GtkColorChooser r)
//-----------------------------------------------------

char *	mdh_color_chooser_get_rgba(GtkColorChooser *c) {

      static char tmp[256];
      GdkRGBA r;
      gtk_color_chooser_get_rgba (c, &r);
      sprintf((char *) tmp, "%f %f %f %f", r.red, r.green, r.blue, r.alpha );
      return tmp;
      }

//-----------------------------------------------------
//	$z~mdh~label~set~text(LabelReference,string)
//-----------------------------------------------------

void	mdh_label_set_text(GtkLabel *l, char * txt) {

      gtk_label_set_text(l, txt) ;

      }

//------------------------------------
//	$z~mdh~widget~hide(widget)
//------------------------------------

void	mdh_widget_hide(GtkWidget *w) {

      gtk_widget_hide(w);

      }

//-------------------------------------
//	$z~mdh~widget~show(widget)
//-------------------------------------

void	mdh_widget_show(GtkWidget *w) {

      gtk_widget_show(w);

      }


//---------------------------------------------------------------------
//	$z~mdh~tree~selection~get~selected(TreeModelReference,column)
//---------------------------------------------------------------------

char *	mdh_tree_selection_get_selected(GtkTreeSelection *t, int col, char *txt) {

      GtkTreeModel *m;
      gchar *value;
      GtkTreeIter iter;

      strcpy(txt,"");

      if (gtk_tree_selection_get_selected (t, &m, &iter) == FALSE)
            return txt;

      gtk_tree_model_get(m, &iter, col, &value, -1);

      strcpy(txt, value);

      return txt;

      }

//----------------------------------------------------
//	$z~mdh~tree~store~clear(TreeStoreReference)
//----------------------------------------------------

void	mdh_tree_store_clear(GtkTreeStore *t) {
      gtk_tree_store_clear (t);
      }

//-------------------------------------------------------------
//	$z~mdh~spin~button~get~value(SpinButtonReference)
//-------------------------------------------------------------

double	mdh_spin_button_get_value(GtkSpinButton *s) {

      double x;

      x = gtk_spin_button_get_value(s);

      return x;

      }

//----------------------------------------------------------------
//	$z~mdh~spin~button~set~value(SpinButtonReference,number)
//----------------------------------------------------------------

void	mdh_spin_button_set_value(GtkSpinButton *s, double v) {

      gtk_spin_button_set_value(s,v);

      }



void Dump(mstring &arg) {
      char tmp[256] = "$zcd\xce";
      char tmp2[256];
      strcat (tmp,(char *) arg.c_str());
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      return;
      }

void Dump(string &arg) {
      char tmp[256] = "$zcd\xce";
      char tmp2[256];
      strcat (tmp,(char *) arg.c_str());
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      return;
      }

void Dump(char * arg) {
      char tmp[256] = "$zcd\xce";
      char tmp2[256];
      strcat (tmp,(char *) arg);
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      return;
      }

void Restore(mstring &arg) {
      char tmp[256] = "$zcl\xce";
      char tmp2[256];
      strcat (tmp,(char *) arg.c_str());
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      return;
      }

void Restore(string &arg) {
      char tmp[256] = "$zcl\xce";
      char tmp2[256];
      strcat (tmp,(char *) arg.c_str());
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      return;
      }

void Restore(char * arg) {
      char tmp[256] = "$zcl\xce";
      char tmp2[256];
      strcat (tmp,(char *) arg);
      strcat (tmp,"\xcf");
      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);
      return;
      }

void Getenv(unsigned char * out, unsigned char * in) {

      char * p1 = getenv( (char *) in);

      if (p1 == NULL ) {
            strcpy( (char *) out,"");
            return;
            }

      strcpy( (char *) out, p1);
      return;
      }

bool ZSeek(FILE *u, mstring &s) {

      fpos_t offset;
      sscanf(s.c_str(),"%lld",&offset);
      return !fsetpos(u,&offset);
      }

bool Ztell(FILE *u, mstring &s) {

      fpos_t offset;
      int i;
      char tmp[64];
      i=fgetpos(u,&offset);
      sprintf(tmp,"%lld",&offset);
      s = tmp;
      return !i;
      }

bool ZSeek(FILE *u, global &s) {

      fpos_t offset;
      char tmp[64];
      char tmp1[STR_MAX];
      s >> tmp1;
      sscanf(tmp1,"%lld",&offset);
      return !fsetpos(u,&offset);
      }

bool Ztell(FILE *u, global &s) {

      fpos_t offset;
      int i;
      char tmp[64];
      i=fgetpos(u,&offset);
      sprintf(tmp,"%lld",&offset);
      s = tmp;
      return !i;
      }

int Perl (mstring &a, mstring &b) {
      svPtr->tpx=perlcpp((char *) "",(char *) a.c_str(), (char *) b.c_str(),svPtr);
      return svPtr->tpx;
      }

int Perl (mstring &a, char *b) {
      svPtr->tpx=perlcpp((char *) "",(char *) a.c_str(), (char *) b,svPtr);
      return svPtr->tpx;
      }

Hash::Hash(string name, long sz, string disp) { // constructor
      char x='\1';
      char *code;
      char *p=&x;
      key=name+".key";
      size=sz;
      long i;

      if (name.length()==0) {
            cout << "Invalid hash file name" << endl;
            exit(EXIT_FAILURE);
            }

      if (sz < 1000 ) {
            cout << "Invalid hash file size in hash " << name << endl;
            exit(EXIT_FAILURE);
            }

      if (disp=="new") code= (char *) "w+";
      else if (disp=="old") code= (char *) "r+";
      else {
            cout << "Unknown hash file disposition" << endl;
            exit(EXIT_FAILURE);
            }

      hashkey=fopen(key.c_str(),code);

      if (hashkey==NULL) {
            cout << "Hash file open error." << endl;
            exit(EXIT_FAILURE);
            }

      if (disp=="new") {
            for (i=0; i<size; i++) fwrite(p,(size_t)1,1,hashkey);
            }
      }

Hash::~Hash() { // destructor
      if (hashkey!=NULL) fclose(hashkey);
      }

Hash & Hash::operator()(string str) {
      ref=str;
      return (*this);
      }

Hash & Hash::operator()(mstring str) {
      ref=str.s_str();
      return (*this);
      }

mstring Hash::operator=(mstring str) { // assignment of mstring to hash

      str.data = (*this = str.data);
      return str;

      }

string Hash::operator=(string str) { // assignment of string to hash
      off_t i,ii;
      long j,k,m;
      int flg,c;
      char p[STR_MAX];

      if (str.length()==0) {
            cout << "zero length hash reference" << endl;
            exit(EXIT_FAILURE);
            }

      i=lhash(ref.c_str())%size;  // calculate a hash

      FSEEK(hashkey,i,SEEK_SET);  // move to that location

      flg=0;
      int loopflg=0;

      while (1) {

            ii=FTELL(hashkey); // starting point

            j=0;

            while (1) {
                  c=fgetc(hashkey);

                  if (c==EOF) break;

                  if (c==2) continue;

                  if (c==1) break; // not found

                  p[j++]=c;

                  if (c=='\0') break;
                  }

            if (c==1||(loopflg&&c==EOF)) break; // not found exit

            if (c==EOF) {
                  loopflg=1;
                  i=0;
                  FSEEK(hashkey,i,SEEK_SET);  // move to that location
                  ii=FTELL(hashkey); // starting point
                  continue;
                  }

            if (strcmp(p,ref.c_str())==0) { // match?
                  flg=1;
                  break; // matched: exit loop
                  }

            j=0;

            while (1) { // bypass value
                  c=fgetc(hashkey);

                  if (c==EOF) break;

                  if (c==1) break;

                  p[j++]=c;

                  if (c=='\0') break;
                  }
            }

      if (flg) { // instance found
            i=FTELL(hashkey);

            for (j=0; j<STR_MAX; j++) { // get value
                  c=fgetc(hashkey);

                  if (c==EOF) {
                        cout << "EOF in hash file" << endl;
                        exit(EXIT_FAILURE);
                        }

                  p[j]=c;

                  if (p[j]=='\0') break;
                  }

            if (strlen(p)>=str.length()) { // replace
                  FSEEK(hashkey,i,SEEK_SET);
                  k=strlen(p);
                  p[0]=2;

                  for (j=0; j<=k; j++) fwrite(p,(size_t)1,1,hashkey); // zap to 2's

                  FSEEK(hashkey,i,SEEK_SET); // reset
                  fwrite(str.c_str(),str.length()+1,1,hashkey);
                  return str;
                  }

            FSEEK(hashkey,ii,SEEK_SET);
            k=ref.length()+1;
            p[0]=2;

            for (j=0; j<=k; j++) fwrite(p,(size_t)1,1,hashkey); // zap to 2's

            ii=FTELL(hashkey);

            for (j=0; j<STR_MAX; j++) { // get value
                  p[j]=fgetc(hashkey);

                  if (p[j]=='\0') break;
                  }

            k=strlen(p)+1;
            p[0]=2;
            FSEEK(hashkey,ii,SEEK_SET);

            for (j=0; j<=k; j++) fwrite(p,(size_t)1,1,hashkey); // zap to 1's
            }

      loopflg=0;
      j=ref.length()+str.length()+3;
      i=lhash(ref.c_str())%size;
      FSEEK(hashkey,i,SEEK_SET);
      k=0;
      i=FTELL(hashkey);

      while(1) {
            fread(p,(size_t)1,1,hashkey);

            if (feof(hashkey)&&!loopflg) {
                  loopflg=1;
                  i=0;
                  FSEEK(hashkey,i,SEEK_SET);
                  continue;
                  }

            if (feof(hashkey)&&loopflg) {
                  cout << endl << "table full - eof" << endl;
                  exit(EXIT_FAILURE);
                  }

            if (p[0]==1) { // empty slot found
                  k++;

                  if (k>=j) break;

                  continue;
                  }

            k=0;
            i=FTELL(hashkey);
            }

      FSEEK(hashkey,i,SEEK_SET);
      fwrite(ref.c_str(),ref.length()+1,1,hashkey);
      fwrite(str.c_str(),str.length()+1,1,hashkey);

      return str;
      }

Hash::operator const char *() {
      static string r;
      off_t i;
      long j,k,m;
      int c;
      static char p[STR_MAX];
      i=lhash(ref.c_str())%size;
      FSEEK(hashkey,i,SEEK_SET);
      int loopflg=0;

      while (1) {
            j=0;

            while (1) {
                  c=fgetc(hashkey);

                  if (c==EOF&&loopflg) return NULL;

                  if (c==EOF) {
                        loopflg=1;
                        i=0;
                        FSEEK(hashkey,i,SEEK_SET);
                        continue;
                        }

                  // if (c==1) return NULL; // not found
                  if (c==1) continue;

                  if (c==2) continue; // skip

                  p[j++]=c;

                  if (c=='\0') break;
                  }

            if (strcmp(p,ref.c_str())==0) break; // found
            }

      j=0;

      while (1) { // get value
            c=fgetc(hashkey);

            if (c==EOF) return NULL;

            p[j++]=c;

            if (c=='\0') break;
            }

      return p;
      }

void global::DocCorrelate(global B, char * fcn, double threshold) {
      string s;
      s=fcn;
      return DocCorrelate(B,s,threshold);
      }

void global::DocCorrelate(global B, mstring fcn, double threshold) {
      string s;
      s=fcn.s_str();
      return DocCorrelate(B,s,threshold);
      }

void global::DocCorrelate(global B,string fcn, double threshold) {
      string doc1,doc2;
      string term;
      mstring tmp;
      double c;
      int s;

      if (fcn=="Cosine")  s=1;
      else if (fcn=="Sim1") s=2;
      else if (fcn=="Dice") s=3;
      else if (fcn=="Jaccard") s=4;
      else s=1;

      B().Kill();
      doc1="";

      while (1) {
            doc1= (string) (*this)(doc1).Order(1);

            if (doc1=="") break;

            doc2=doc1;

            while (1) {
                  doc2= (string) (*this)(doc2).Order(1);

                  if (doc2=="") break;

                  switch (s) {
                        case 1:
                              c= Cosine((*this)(doc1),(*this)(doc2));
                              break;

                        case 2:
                              c= Sim1((*this)(doc1),(*this)(doc2));
                              break;

                        case 3:
                              c=Dice((*this)(doc1),(*this)(doc2));
                              break;

                        case 4:
                              c= Jaccard((*this)(doc1),(*this)(doc2));
                              break;
                        }

                  if (c>threshold) {
                        B(doc1,doc2)=c;
                        B(doc2,doc1)=c;
                        }
                  }
            }
      }


void global::TermCorrelate(global B) {
      string doc;
      string term1,term2;

      B().Kill();
      doc="";

      while (1) {
            doc= (string) (*this)(doc).Order(1);

            if (doc=="") break;

            term1="";

            while (1) {
                  term1= (string) (*this)(doc,term1).Order(1);

                  if (term1=="") break;

                  term2=term1;

                  while (1) {
                        term2= (string) (*this)(doc,term2).Order(1);

                        if (term2=="") break;

                        if (B(term1,term2).Data()) {
                              B(term1,term2)=B(term1,term2)+1;
                              B(term2,term1)=B(term1,term2);
                              }
                        else {
                              B(term1,term2)=1;
                              B(term2,term1)=1;
                              }
                        }
                  }
            }
      }

double Dice(global A, global B) {

      char ttt[STR_MAX],ctmp[STR_MAX],btmp[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i,j;
      double totD=0.0,totQ=0.0,tot=0.0,val,val1;

      strcpy(tmp,A.ref);
      i=strlen(A.ref);
      j=strlen(B.ref);
      strcpy(ctmp,B.ref);

      if (ctmp[j-2]==1&&ctmp[j-1]==1) {
            j--;
            ctmp[j]=0;
            }

      if (tmp[i-2]==1&&tmp[i-1]==1) {
            i--;
            tmp[i]=0;
            }

      while(1) {
            if (!Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)) break;

            if (strncmp(A.ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL);
            totD=totD+val;

            strcpy(B.ref,ctmp);
            strcat(B.ref,&tmp[i]);
            strcpy(btmp,B.ref);

            if (!B.Data()) continue;

            get_global(btmp,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val1=strtod(rslt,NULL);
            totQ=totQ+val1;

            tot=tot+val*val1;
            }

      val=totD+totQ;

      if (val==0.0) return 0.0;

      return 2*tot/val;
      }


double	Jaccard(global A, global B) {

      char ttt[STR_MAX],ctmp[STR_MAX],btmp[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      char OriginalB[STR_MAX];
      int i,j;
      double totD=0.0,totQ=0.0,tot=0.0,val,val1;

      strcpy(tmp, A.ref);
      i=strlen(A.ref);

      j=strlen(B.ref);
      strcpy(ctmp,B.ref);

      if (ctmp[j-2]==1&&ctmp[j-1]==1) {
            j--;
            ctmp[j]=0;
            }

      strcpy(OriginalB,ctmp);

      if (tmp[i-2]==1&&tmp[i-1]==1) {
            i--;
            tmp[i]=0;
            }

      while(1) {
            if (!Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)) break;

            if (strncmp(A.ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL);  // A(j,k)
            totD=totD+val;

            strcpy(B.ref,ctmp);
            strcat(B.ref,&tmp[i]);
            strcpy(btmp,B.ref);

            if (!B.Data()) continue;

            get_global(btmp,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val1=strtod(rslt,NULL); // B(j,k)

            tot=tot+val*val1;
            }

      strcpy(B.ref,OriginalB);

      while(1) {

            if (!Mglobal(XNEXT,(unsigned char *) ctmp,(unsigned char *) ttt,svPtr)) break;

            if (strncmp(B.ref,ctmp,j)!=0) break;

            strcpy(tmp1,ctmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val1=strtod(rslt,NULL);
            totQ=totQ+val1;
            }

      val=totD+totQ-tot;

      if (val==0.0) return 0.0;

      return tot/val;
      }

void pt(char *t,char *x) {
      int i;
      printf("%s  ",t);

      while(x[i]) {
            printf(" %c ",x[i]);
            i++;
            }

      printf("\n");
      }

double Cosine(global A, global B) {

      char ttt[STR_MAX],ctmp[STR_MAX],btmp[STR_MAX],tmp[STR_MAX],
           tmp1[STR_MAX],rslt[STR_MAX];
      char x01[2]= {1,0},A2[STR_MAX]="";
      int i,j;
      double totD=0.0,totQ=0.0,tot=0.0,val,val1;

      strcpy(tmp,A.ref);
      i=strlen(A.ref);
      j=strlen(B.ref);
      strcpy(ctmp,B.ref);

      if (ctmp[j-2] == 1 && ctmp[j-1] == 1) {
            j--;
            ctmp[j] = 0;
            }

      if (tmp[i-2] == 1 && tmp[i-1] == 1) {
            i--;
            tmp[i] = 0;
            }

      while(1) {

            if (!Mglobal(XNEXT,(unsigned char *) tmp,
                         (unsigned char *) ttt,svPtr)) break;

            if (strncmp(A.ref,tmp,i) != 0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL); // first operand
            totD=totD+val*val;

            strcpy(B.ref,ctmp);
            strcat(B.ref,&tmp[i]);
            strcpy(btmp,B.ref);

            if (!B.Data()) continue;

            get_global(btmp,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val1=strtod(rslt,NULL);

            tot=tot+val*val1;
            }

      if (tot==0.0) return 0.0;

      strcpy(B.ref,ctmp); // restore
      int bflg=1;
      strcpy(ttt,"");

      while(1) {


            if (!Mglobal(XNEXT,(unsigned char *) ctmp,(unsigned char *) ttt,svPtr)) break;

            if (strncmp(B.ref,ctmp,j)!=0) break;

            strcpy(tmp1,ctmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val1=strtod(rslt,NULL);
            totQ=totQ+val1*val1;
            }

      val=sqrt(totD*totQ);

      if (val==0.0) return 0.0;

      return tot/val;
      }

double Sim1(global A, global B) {
      char ttt[STR_MAX],ctmp[STR_MAX],btmp[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i,j;
      double tot=0.0,val,val1;
      strcpy(tmp,A.ref);
      i=strlen(A.ref);
      j=strlen(B.ref);
      strcpy(ctmp,B.ref);

      if (ctmp[j-2]==1&&ctmp[j-1]==1) {
            j--;
            ctmp[j]=0;
            }

      if (tmp[i-2]==1&&tmp[i-1]==1) {
            i--;
            tmp[i]=0;
            }

      while(1) {
            if (!Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)) break;

            if (strncmp(A.ref, tmp, i) !=0 ) break;

            strcpy(B.ref,ctmp);
            strcat(B.ref,&tmp[i]);
            strcpy(btmp,B.ref); // in case this and B are the same

            if (!B.Data()) continue;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL);

            get_global(btmp,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val1=strtod(rslt,NULL);

            tot=tot+val*val1;
            }

      return tot;
      }

double global::Sum() {

      char ttt[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i;
      double tot=0.0,val;

      strcpy(tmp,ref);
      i=strlen(tmp);

      if (tmp[i-1] <32 && tmp[i-2] <32 ) tmp[--i] = 0;

      while(1) {
            if(Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)==0) break;

            if (strncmp(ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL);
            tot=tot+val;
            }

      return tot;
      }

double global::Max() {
      char ttt[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i,j,flg=1;
      double max=0.0,val;

      strcpy(tmp,ref);
      i=strlen(tmp);

      if (tmp[i-1]<32&&tmp[i-2]<32) tmp[--i]=0;

      while(1) {
            if (Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)==0) break;

            if (strncmp(ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL);

            if (flg) {
                  max=val;
                  flg=0;
                  continue;
                  }

            if (val>max) max=val;
            }

      return max;
      }

double global::Min() {
      char ttt[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i,flg=1;
      double min=0.0,val;
      strcpy(tmp,ref);
      i=strlen(tmp);

      if (tmp[i-1]<32&&tmp[i-2]<32) tmp[--i]=0;

      while(1) {
            if(Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)==0) break;

            if (strncmp(ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            val=strtod(rslt,NULL);

            if (flg) {
                  min=val;
                  flg=0;
                  continue;
                  }

            if (val<min) min=val;
            }

      return min;
      }

double global::Avg() {
      char ttt[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i;
      long j=0;
      double tot=0.0,val;
      strcpy(tmp,ref);
      i=strlen(ref);

      if (tmp[i-1]<32&&tmp[i-2]<32) tmp[--i]=0;

      while(1) {
            if (Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)==0)
                  break;

            if (strncmp(ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            if (sscanf(rslt,"%lf",&val)==0) {
                  GlobalExceptionCode=INVALID_CONVERSION;
                  throw MumpsGlobalException();
                  }

            tot=tot+val;
            j++;
            }

      if (j==0) return 0.0;

      return tot/j;
      }

void global::IDF(double DC) {
      char ttt[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i;
      long j=0;
      double val;

      if (DC<=0) {
            GlobalExceptionCode=DATA_RANGE_ERROR;
            throw MumpsGlobalException();
            }

      DC=LOG2(DC);
      strcpy(tmp,ref);
      i=strlen(ref);

      if (tmp[i-1]<32&&tmp[i-2]<32) tmp[--i]=0;

      while(1) {
            if (Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)==0)
                  break;

            if (strncmp(ref,tmp,i)!=0) break;

            strcpy(tmp1,tmp);
            get_global(tmp1,rslt);

            if (!svPtr->tpx) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            if (sscanf(rslt,"%lf",&val)==0) {
                  GlobalExceptionCode=INVALID_CONVERSION;
                  throw MumpsGlobalException();
                  }

            if (val <= 0.0) {
                  GlobalExceptionCode=DATA_RANGE_ERROR;
                  throw MumpsGlobalException();
                  }

            val=DC-LOG2(val)+1;
            sprintf(rslt,"%.1lf",val);
            set_global(tmp1,rslt);
            }
      }

long global::Count() {
      char ttt[STR_MAX],tmp[STR_MAX],tmp1[STR_MAX],rslt[STR_MAX];
      int i;
      long j=0;
      strcpy(tmp,ref);
      i=strlen(tmp);

      if (tmp[i-1]<32&&tmp[i-2]<32) tmp[--i]=0;

      while(1) {
            if (Mglobal(XNEXT,(unsigned char *) tmp,(unsigned char *) ttt,svPtr)==0) break;

            if (strncmp(ref,tmp,i)!=0) break;

            j++;
            }

      return j;
      }

void global::TreePrint(int i,const char c) {
      string indent="",eq="=",a0,a1,a2,a3,a4,a5,a6,a7,a8,a9;

      if (i!=0) {
            for (int j=0; j<i; j++) indent=indent+c;
            }

      a0="";

      while(1) {
            a0= (string) (*this)(a0).Order(1);

            if (a0=="") break;

            cout << a0 ;
            i=(*this)(a0).Data();

            if (i==1 || i==11) cout << eq << (*this)(a0) << endl;
            else cout << endl;

            if (i<2) continue;

            a1="";

            while(1) {
                  a1= (string) (*this)(a0,a1).Order(1);

                  if (a1=="") break;

                  for (i=0; i<1; i++) cout << indent;

                  cout << a1 ;
                  i=(*this)(a0,a1).Data();

                  if (i==1 || i==11) cout << eq << (*this)(a0,a1) << endl;
                  else cout << endl;

                  if (i<2) continue;

                  a2="";

                  while(1) {
                        a2= (string) (*this)(a0,a1,a2).Order(1);

                        if (a2=="") break;

                        for (i=0; i<2; i++) cout << indent;

                        cout << a2 ;
                        i=(*this)(a0,a1,a2).Data();

                        if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2) << endl;
                        else cout << endl;

                        if (i<2) continue;

                        a3="";

                        while(1) {
                              a3= (string) (*this)(a0,a1,a2,a3).Order(1);

                              if (a3=="") break;

                              for (i=0; i<3; i++) cout << indent;

                              cout << a3 ;
                              i=(*this)(a0,a1,a2,a3).Data();

                              if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3) << endl;
                              else cout << endl;

                              if (i<2) continue;

                              a4="";

                              while(1) {
                                    a4= (string) (*this)(a0,a1,a2,a3,a4).Order(1);

                                    if (a4=="") break;

                                    for (i=0; i<4; i++) cout << indent;

                                    cout << a4 ;
                                    i=(*this)(a0,a1,a2,a3,a4).Data();

                                    if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3,a4) << endl;
                                    else cout << endl;

                                    if (i<2) continue;

                                    a5="";

                                    while(1) {
                                          a5= (string) (*this)(a0,a1,a2,a3,a4,a5).Order(1);

                                          if (a5=="") break;

                                          for (i=0; i<5; i++) cout << indent;

                                          cout << a5 ;
                                          i=(*this)(a0,a1,a2,a3,a4,a5).Data();

                                          if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3,a4,a5) << endl;
                                          else cout << endl;

                                          if (i<2) continue;

                                          if ((*this)(a0,a1,a2,a3,a4,a5).Data()<2) continue;

                                          a6="";

                                          while(1) {
                                                a6= (string) (*this)(a0,a1,a2,a3,a4,a5,a6).Order(1);

                                                if (a6=="") break;

                                                for (i=0; i<6; i++) cout << indent;

                                                cout << a6 ;
                                                i=(*this)(a0,a1,a2,a3,a4,a5,a6).Data();

                                                if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3,a4,a5,a6) << endl;
                                                else cout << endl;

                                                if (i<2) continue;

                                                if ((*this)(a0,a1,a2,a3,a4,a5,a6).Data()<2) continue;

                                                a7="";

                                                while(1) {
                                                      a7= (string) (*this)(a0,a1,a2,a3,a4,a5,a6,a7).Order(1);

                                                      if (a7=="") break;

                                                      for (i=0; i<7; i++) cout << indent;

                                                      cout << a7 ;
                                                      i=(*this)(a0,a1,a2,a3,a4,a5,a6,a7).Data();

                                                      if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3,a4,a5,a6,a7) << endl;
                                                      else cout << endl;

                                                      if (i<2) continue;

                                                      a8="";

                                                      while(1) {
                                                            a8= (string) (*this)(a0,a2,a3,a4,a5,a6,a7,a8).Order(1);

                                                            if (a8=="") break;

                                                            for (i=0; i<8; i++) cout << indent;

                                                            cout << a8 << endl;
                                                            i=(*this)(a0,a1,a2,a3,a4,a5,a6,a7,a8).Data();

                                                            if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3,a4,a5,a6,a7,a8) << endl;
                                                            else cout << endl;

                                                            if (i<2) continue;

                                                            a9="";

                                                            while(1) {
                                                                  a9= (string) (*this)(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9).Order(1);

                                                                  if (a9=="") break;

                                                                  for (i=0; i<9; i++) cout << indent;

                                                                  cout << a9 << endl;
                                                                  i=(*this)(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9).Data();

                                                                  if (i==1 || i==11) cout << eq << (*this)(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) << endl;
                                                                  else cout << endl;
                                                                  }
                                                            }
                                                      }
                                                }
                                          }
                                    }
                              }
                        }
                  }
            }
      }

void global::Centroid(global B) {
      string aa="", bb="";
      long i=0;
      double x;
      B().Kill();

      while (1) {
            aa = (string) (*this)(aa).Order(1);

            if (aa=="") break;

            bb="";

            while (1) {
                  bb = (string) (*this)(aa,bb).Order(1);

                  if (bb=="") break;

                  if (!B(bb).Data()) {
                        B(bb)=(*this)(aa,bb);
                        B(bb,"1")=1;
                        }
                  else {
                        B(bb)=B(bb)+(*this)(aa,bb);
                        B(bb,"1")++;
                        }
                  }

            }

      bb="";

      while (1) {
            bb= (string) B(bb).Order(1);

            if (bb=="") break;

            x=B(bb,"1");
            B(bb)/=x;
            }
      }

//-----------------------------------------------------------
//
// 		TRANSPOSE
//
//-----------------------------------------------------------

void global::Transpose(global B) {
      string aa="", bb="";

      B().Kill();

      while ( 1) {
            aa = (string) (*this)(aa).Order(1);

            if (aa=="") break;

            bb="";

            while (1) {
                  bb = (string) (*this)(aa,bb).Order(1);

                  if (bb=="") break;

                  B(bb,aa)=(*this)(aa,bb);
                  }
            }

      }

void Transpose(char * a, char * b) {
      global A(a),B(b);
      string aa="", bb="";

      while ( 1) {
            aa = (string) A(aa).Order(1);

            if (aa=="") break;

            bb="";

            while (1) {
                  bb = (string) A(aa,bb).Order(1);

                  if (bb=="") break;

                  B(bb,aa)=A(aa,bb);
                  }
            }
      }

void Transpose(string a, string b) {
      global A((char *)a.c_str()),B((char *)b.c_str());
      string aa="", bb="";

      while ( 1) {
            aa = (string) A(aa).Order(1);

            if (aa=="") break;

            bb="";
            bb="";

            while (1) {
                  bb = (string) A(aa,bb).Order(1);

                  if (bb=="") break;

                  B(bb,aa)=A(aa,bb);
                  }
            }

      }

void Transpose(mstring a, mstring b) {
      global A((char *)a.c_str()),B((char *)b.c_str());
      string aa="", bb="";

      while ( 1) {
            aa = (string) A(aa).Order(1);

            if (aa=="") break;

            bb="";

            while (1) {
                  bb = (string) A(aa,bb).Order(1);

                  if (bb=="") break;

                  B(bb,aa)=A(aa,bb);
                  }
            }

      }

//-------------------------------------------------------------
//
//	MULTIPLY
//
//-------------------------------------------------------------

void global::Multiply(global B, global C) {
      string i,j,k,m;

      C().Kill();
      i="";

      while (1) {


            i= (string) (*this)(i).Order(1);  // each row

            if (i=="") break;

            j="";

            while (1) {
                  j= (string) (*this)(i,j).Order(1); // each column

                  if (j=="") break;

                  k=j;

                  if (!B(k).Data()) continue;

                  m="";

                  while (1) {
                        m= (string) B(k,m).Order(1);

                        if (m=="") break;

                        if (!C(i,m).Data()) C(i,m)=(*this)(i,j)*B(k,m);
                        else C(i,m)=C(i,m)+(*this)(i,j)*B(k,m);
                        }
                  }
            }

      }

void Multiply(string a, string b, string c) {
      global A((char *)a.c_str());
      global B((char *)b.c_str());
      global C((char *)c.c_str());
      string i,j,k,m;

      C().Kill();
      i="";

      while (1) {

            i= (string) A(i).Order(1);  // each row

            if (i=="") break;

            j="";

            while (1) {
                  j= (string) A(i,j).Order(1); // each column

                  if (j=="") break;

                  k=j;

                  if (!B(k).Data()) continue;

                  m="";

                  while (1) {
                        m= (string) B(k,m).Order(1);

                        if (m=="") break;

                        if (!C(i,m).Data()) C(i,m)=A(i,j)*B(k,m);
                        else C(i,m)=C(i,m)+A(i,j)*B(k,m);
                        }
                  }
            }

      }

void Multiply(char * a, char * b, char * c) {
      global A((char *)a);
      global B((char *)b);
      global C((char *)c);
      string i,j,k,m;

      C().Kill();
      i="";

      while (1) {

            i= (string) A(i).Order(1);  // each row

            if (i=="") break;

            j="";

            while (1) {
                  j= (string) A(i,j).Order(1); // each column

                  if (j=="") break;

                  k=j;

                  if (!B(k).Data()) continue;

                  m="";

                  while (1) {
                        m= (string) B(k,m).Order(1);

                        if (m=="") break;

                        if (!C(i,m).Data()) C(i,m)=A(i,j)*B(k,m);
                        else C(i,m)=C(i,m)+A(i,j)*B(k,m);
                        }
                  }
            }

      }

string stem(string & word) {
      static char tmp[STR_MAX];
      strcpy(tmp,word.c_str());
      int i=wstem((char *)tmp,(int)word.length());
      word = tmp;
      return word;
      }

mstring stem(mstring & word) {
      static char tmp[STR_MAX];
      strcpy(tmp,word.c_str());
      int i=wstem((char *)tmp,(int)strlen(tmp));
      word = tmp;
      return word;
      }

char* stem(char*  word) {
      int i=wstem((char *)word,(int)strlen(word));
      return word;
      }

char * Hash (const char * str) {

      static char result[32];
      char in[STR_MAX]= {0};
      unsigned long a,x=0;
      char *p1=(char *)&a;
      int i;
      strcpy(in,str);

      for (i=0; in[i]!=0; i+=4) {
            *p1=in[i];
            *(p1+1)=in[i+1];
            *(p1+2)=in[i+2];
            *(p1+3)=in[i+3];
            x=x+a;
            }

      sprintf(result,"%lu",x);
      return result;
      }

unsigned long lhash (const char * str) {

      char in[STR_MAX]= {0};
      unsigned long a,x=0;
      char *p1=(char *)&a;
      int i;
      strcpy(in,str);

      for (i=0; in[i]!=0; i+=4) {
            *p1=in[i];
            *(p1+1)=in[i+1];
            *(p1+2)=in[i+2];
            *(p1+3)=in[i+3];
            x=x+a;
            }

      return x;
      }

indirect operator*(string a) {
      static indirect b;
      b.reference = a;
      return b;
      }

indirect indirect::operator=(string a) {
      cout << (*this).reference << " " << a << endl;
      return *this;
      }

int GlobalSet(mstring x,mstring y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x.c_str());
      strcat(tmp,"=\"");
      strcat(tmp,y.c_str());
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int GlobalSet(char * x,mstring y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x);
      strcat(tmp,"=\"");
      strcat(tmp,y.c_str());
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int GlobalSet(mstring x,char * y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x.c_str());
      strcat(tmp,"=\"");
      strcat(tmp,y);
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int GlobalSet(string x,string y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x.c_str());
      strcat(tmp,"=\"");
      strcat(tmp,y.c_str());
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int GlobalSet(char * x,string y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x);
      strcat(tmp,"=\"");
      strcat(tmp,y.c_str());
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int GlobalSet(string x,char * y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x.c_str());
      strcat(tmp,"=\"");
      strcat(tmp,y);
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int GlobalSet(char * x,char * y) {
      char tmp[STR_MAX]="s ";
      strcat(tmp,x);
      strcat(tmp,"=\"");
      strcat(tmp,y);
      strcat(tmp,"\"");
      \

      if (xecute(tmp)==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

char * GlobalGet(char * a) {
      static char r[STR_MAX];
      char tmp[STR_MAX]="set %=";
      mstring t;
      strcat(tmp,a);

      if (xecute(tmp)==0) svPtr->tpx=1;
      else {
            svPtr->tpx=0;
            strcpy(r,"");
            }

      t=SymGet( (char *) "%");
      strcpy(r,t.c_str());
      return r;
      }

mstring GlobalGet(mstring a) {
      mstring rr;
      char tmp[STR_MAX]="set %=";
      strcat(tmp,a.c_str());

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            rr=SymGet( (char *) "%");
            }
      else {
            svPtr->tpx=0;
            rr="";
            }

      return rr;
      }

string GlobalGet(string a) {
      static string rr;
      char tmp[STR_MAX]="set %=";
      strcat(tmp,a.c_str());

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            rr= (string)SymGet( (char *) "%");
            }
      else {
            svPtr->tpx=0;
            rr="";
            }

      return rr;
      }

void	GlobalQS(unsigned char * a, unsigned char * b, unsigned char * c, MSV *svPtr) {

      static char r[STR_MAX];
      mstring t;
      char tmp[STR_MAX]="s %=$qs(";

      strcat(tmp,(char *) a);
      strcat(tmp,",");
      strcat(tmp, (char *) c);
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            t=SymGet( (char *) "%");
            strcpy((char *)b,t.c_str());
            }
      else {
            svPtr->tpx=0;
            strcpy((char *) b,"");
            }

      return;
      }

char * GlobalOrder(char * a, int i) {
      static char r[STR_MAX];
      mstring t;
      char tmp[STR_MAX]="s %=$order(";
      strcat(tmp,a);
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            t=SymGet( (char *) "%");
            strcpy(r,t.c_str());
            }
      else {
            svPtr->tpx=0;
            strcpy(r,"");
            }

      return r;
      }

mstring GlobalOrder(mstring a, int i) {
      char tmp[STR_MAX]="s %=$order(";
      mstring rr;
      strcat(tmp,a.c_str());
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            rr=SymGet( (char *) "%");
            }
      else {
            svPtr->tpx=0;
            rr="";
            }

      return rr;
      }

string GlobalOrder(string a, int i) {
      char r[STR_MAX];
      char tmp[STR_MAX]="s %=$order(";
      static string rr;
      strcat(tmp,a.c_str());
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            rr=(string)SymGet( (char *) "%");
            }
      else {
            svPtr->tpx=0;
            rr="";
            }

      return rr;
      }

int GlobalData(char * a) {
      char tmp[STR_MAX]="s %=$data(";
      char r[STR_MAX]="";
      mstring t;
      int i;
      strcat(tmp,a);
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            t=SymGet((char *) "%");
            i=atoi(t.c_str());
            }
      else {
            svPtr->tpx=0;
            i=-1;
            }

      return i;
      }

int GlobalData(mstring a) {
      char tmp[STR_MAX]="s %=$data(";
      int i;
      mstring t;
      strcat(tmp,a.c_str());
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            t=SymGet( (char *) "%");
            i=atoi(t.c_str());
            }
      else {
            svPtr->tpx=0;
            i=-1;
            }

      return i;
      }

int GlobalData(string a) {
      char tmp[STR_MAX]="s %=$data(";
      int i;
      mstring t;
      strcat(tmp,a.c_str());
      strcat(tmp,")");

      if (xecute(tmp)==0) {
            svPtr->tpx=1;
            t=SymGet((char *)"%");
            i=atoi(t.c_str());
            }
      else {
            svPtr->tpx=0;
            i=-1;
            }

      return i;
      }

int pmw(char *a, char * b, struct MSV * svPtr) {
      return pm((const unsigned char *) a,(const unsigned char *) b,svPtr);
      }

int pmw(string a, char * b, struct MSV * svPtr) {
      return pm((const unsigned char *) a.c_str(),(const unsigned char *) b,svPtr);
      }

int pmw(string a, string b, struct MSV * svPtr) {
      return pm((const unsigned char *) a.c_str(),(const unsigned char *) b.c_str(),svPtr);
      }

int pmw(mstring a, mstring b, struct MSV * svPtr) {
      return pm((const unsigned char *) a.c_str(),(const unsigned char *) b.c_str(),svPtr);
      }

int pmw(char *a, string b, struct MSV * svPtr) {
      return pm((const unsigned char *) a,(const unsigned char *) b.c_str(),svPtr);
      }


mstring mcvt (char * i) {
      mstring tmp;
      tmp = i;
      return i;
      }

mstring mcvt(long i) {
      char a[32];
      mstring tmp;
      sprintf(a,"%ld",i);
      tmp=a;
      return tmp;
      }

mstring mcvt(double i) {
      char a[128];
      mstring tmp;
      sprintf(a,"%g",i);
      tmp=a;
      return tmp;
      }

mstring mcvt(float i) {
      char a[128];
      mstring tmp;
      sprintf(a,"%g",i);
      tmp=a;
      return tmp;
      }

mstring mcvt(int i) {
      char a[32];
      mstring tmp;
      sprintf(a,"%d",i);
      tmp=a;
      return tmp;
      }

char * cvt(long i) {
      static char a[32];
      sprintf(a,"%ld",i);
      return a;
      }

char * cvt(double i) {
      static char a[128];
      sprintf(a,"%g",i);
      return a;
      }

char * cvt(float i) {
      static char a[128];
      sprintf(a,"%g",i);
      return a;
      }

char * cvt(int i) {
      static char a[32];
      sprintf(a,"%d",i);
      return a;
      }

global & global::operator() (indirect a) {
      cout << a.reference << endl;

      // (*this).MakeGlobal(0,(*this).myname, (char *) "a", (char *) "dummy");
      return *this;
      }

global & global::operator() () {
      (*this).MakeGlobal(0,(*this).myname, (char *) "","");
      return *this;
      }

global & global::operator() (string a) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a",a.c_str());
      return *this;
      }

global & global::operator() (string a, string b) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b",a.c_str(),b.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c",a.c_str(),b.c_str(),c.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d",a.c_str(),b.c_str(),c.c_str(),d.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d, string e) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d, string e, string f) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d, string e, string f,
                             string g) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d, string e, string f,
                             string g, string h) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str(),h.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d, string e, string f,
                             string g, string h, string i) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h,i",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str(),h.c_str(),i.c_str());
      return *this;
      }

global & global::operator() (string a, string b, string c, string d, string e, string f,
                             string g, string h, string i, string j) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h,i,j",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str(),h.c_str(),i.c_str(),j.c_str());
      return *this;
      }

global & global::operator() (mstring a) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a",a.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b",a.c_str(),b.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c",a.c_str(),b.c_str(),c.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d",a.c_str(),b.c_str(),c.c_str(),d.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d, mstring e) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d, mstring e, mstring f) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d, mstring e, mstring f,
                             mstring g) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d, mstring e, mstring f,
                             mstring g, mstring h) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str(),h.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d, mstring e, mstring f,
                             mstring g, mstring h, mstring i) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h,i",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str(),h.c_str(),i.c_str());
      return *this;
      }

global & global::operator() (mstring a, mstring b, mstring c, mstring d, mstring e, mstring f,
                             mstring g, mstring h, mstring i, mstring j) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h,i,j",a.c_str(),b.c_str(),c.c_str(),d.c_str(),
                         e.c_str(),f.c_str(),g.c_str(),h.c_str(),i.c_str(),j.c_str());
      return *this;
      }

global & global::operator() (const long int a) {
      char x[32];
      sprintf(x,"%d",a);
      (*this).MakeGlobal(0,(*this).myname, (char *) "a",x);
      return *this;
      }

global & global::operator() (const char * a) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a",a);
      return *this;
      }

global & global::operator() (const char* a, const char* b) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b",a,b);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c",a,b,c);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d",a,b,c,d);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d, const char* e) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e",a,b,c,d,e);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d, const char* e, const char* f) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f",a,b,c,d,e,f);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d, const char* e, const char* f,
                             const char* g) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g",a,b,c,d,e,f,g);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d, const char* e, const char* f,
                             const char* g, const char* h) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h",a,b,c,d,e,f,g,h);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d, const char* e, const char* f,
                             const char* g, const char* h, const char* i) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h,i",a,b,c,d,e,f,g,h,i);
      return *this;
      }

global & global::operator() (const char* a, const char* b, const char* c, const char* d, const char* e, const char* f,
                             const char* g, const char* h, const char* i, const char* j) {
      (*this).MakeGlobal(0,(*this).myname, (char *) "a,b,c,d,e,f,g,h,i,j",a,b,c,d,e,f,g,h,i,j);
      return *this;
      }

int count(char * key) {
      int i=0,j=1;

      for (i=0; key[i]!='\0'; i++) {
            if (key[i]==',') {
                  j++;
                  continue;
                  }

            if (key[i]!='\"') continue;

            i++;

            while(key[i]!='\"'&&key[i]) i++;

            if (key[i]=='\0') ErrorMessage("mis-matched quote",0);
            }

      return j;
      }

double HitRatio(void) {
      double ratio;
      ratio=svPtr->hit/svPtr->count;
      svPtr->hit=0;
      svPtr->count=0;
      return ratio;
      }

mstring EncodeHTML(mstring & str) {
      return EncodeHTML((char *)str.c_str());
      }

mstring EncodeHTML(char *  str) {
      static char v1d[STR_MAX];
      mstring tmp;
      int i;

      strcpy(v1d,str);

      for (i = 0; v1d[i] != 0; i++) {
            if (v1d[i] == ' ') v1d[i] = '+';

            else if (!isalnum(v1d[i])) {
                  char t1[10], t2[STR_MAX];
                  sprintf(t1, "%%%2X\0", v1d[i]);
                  v1d[i] = 0;
                  strmove((unsigned char *) t2, (unsigned char *) &v1d[1]);
                  strcat(t2, t1);
                  strcat(t2, &v1d[i + 1]);
                  strmove((unsigned char *) &v1d[1], (unsigned char *) t2);
                  i = i + 2;
                  }
            }

      tmp=v1d;
      return tmp;
      }

mstring global::Name() {
      mstring tmp1;
      tmp1 = (char *) Sref.c_str();
      return tmp1;
      }

int __test () {
      return svPtr->tpx;
      }

// global ReadLine

bool global::ReadLine() {

      char *i,tmp[STR_MAX];
      i=fgets(tmp,STR_MAX,stdin);
      tmp[strlen(tmp)-1]=0;

      if (tmp[strlen(tmp)-1] < ' ') tmp[strlen(tmp)-1]=0; // CR-LF form

      int f = Mglobal (STORE, (unsigned char *) ref, (unsigned char *) tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return i!=0;
      }

bool global::ReadLine(FILE *u1) {

      char *i,tmp[STR_MAX];
      i=fgets(tmp,STR_MAX,u1);
      tmp[strlen(tmp)-1]=0;

      if (tmp[strlen(tmp)-1] < ' ') tmp[strlen(tmp)-1]=0; // CR-LF form

      int f = Mglobal (STORE, (unsigned char *) ref, (unsigned char *) tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return i!=0;
      }

bool global::ReadLine(istream &u1) {

      char *i,tmp[STR_MAX];
      int f;
      u1.getline(tmp,STR_MAX,'\n');

      if (u1.eof()) {
            tmp[0]=0;
            f = Mglobal (STORE, (unsigned char *) ref, (unsigned char *) tmp, svPtr);

            if (f==0) {
                  GlobalExceptionCode=GLOBAL_NOT_FOUND;
                  throw MumpsGlobalException();
                  }

            return false;
            }

      if (tmp[strlen(tmp)-1] < ' ') tmp[strlen(tmp)-1]=0; // CR-LF form

      f = Mglobal (STORE, (unsigned char *) ref, (unsigned char *) tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return true;
      }

// mstring ReadLine

bool mstring::ReadLine() {

      char *i,tmp[STR_MAX];
      i=fgets(tmp,STR_MAX,stdin);
      tmp[strlen(tmp)-1]=0;

      if (tmp[strlen(tmp)-1] < ' ') tmp[strlen(tmp)-1]=0; // CR-LF form

      data = tmp;
      return i!=0;
      }

bool mstring::ReadLine(FILE *u1) {

      char *i,tmp[STR_MAX];
      i=fgets(tmp,STR_MAX,u1);
      tmp[strlen(tmp)-1]=0;

      if (tmp[strlen(tmp)-1] < ' ') tmp[strlen(tmp)-1]=0; // CR-LF form

      data = tmp;
      return i!=0;
      }

bool mstring::ReadLine(istream &u1) {

      char *i,tmp[STR_MAX];
      u1.getline(tmp,STR_MAX,'\n');

      if (u1.eof()) {
            data="";
            return false;
            }

      if (tmp[strlen(tmp)-1] < ' ') tmp[strlen(tmp)-1]=0; // CR-LF form

      data = tmp;
      return true;
      }

// mstring casts

mstring::operator int() {
      int i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator float() {
      float i;
      i=atof((char *)data.c_str());
      return i;
      }
mstring::operator double() {
      double i;
      i=atof((char *)data.c_str());
      return i;
      }
mstring::operator char() {
      char i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator short() {
      short i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator long() {
      long i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator unsigned long() {
      unsigned long i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator unsigned int() {
      unsigned int i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator unsigned char() {
      unsigned char i;
      i=atoi((char *)data.c_str());
      return i;
      }
mstring::operator unsigned short() {
      unsigned short i;
      i=atoi((char *)data.c_str());
      return i;
      }

// $char Char

// undefined

// $zblanks

mstring mstring::zBlanks() {

      char tmp[STR_MAX], tmp2[STR_MAX];

      strcpy(&tmp[1],"$zblanks");
      strcat(&tmp[1],"\xce");

      sprintf(tmp2, "%s", data.c_str());
      strcat(&tmp[1],tmp2);
      strcat(&tmp[1],"\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);

      return (mstring) tmp2;
      }

mstring global::zBlanks() {

      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (strlen((char *)gtmp) == 0) return -1;

      mstring tmp = (char *) gtmp;

      return tmp.zBlanks();
      }

// $ascii Ascii

int mstring::Ascii() {
      if (data == "") return -1;

      return (int) *(data.c_str());
      }

int global::Ascii() {

      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (strlen((char *)gtmp) == 0) return -1;

      return (int) gtmp[0];
      }

int global::Ascii(int i) {

      unsigned char gtmp[STR_MAX];

      if (i<=0) return -1;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (strlen((char *)gtmp) < i) return -1;

      return (int) gtmp[i-1];
      }

int mstring::Ascii(int i) {
      if (i<=0) return -1;

      if (strlen(data.c_str())<i) return -1;

      if (data == "") return -1;

      i--;
      return (int) *(data.c_str()+i);
      }

int mstring::Length() {
      return strlen(data.c_str());
      }

int global::Length() {
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return strlen((char *)gtmp);
      }

int global::Length(global needle) {
      char * p1;
      unsigned char gtmp[STR_MAX];
      unsigned char gtmp1[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      f = Mglobal (RETRIEVE, (unsigned char *) needle.ref, gtmp1, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      p1= (char *)gtmp;
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,(char *) gtmp1))==NULL) return i; // one piece

            i++;
            p1=p1+strlen((char *) gtmp1);
            }
      }

int global::Length(mstring needle) {
      char * p1;
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      p1= (char *)gtmp;
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,needle.c_str()))==NULL) return i; // one piece

            i++;
            p1=p1+strlen(needle.c_str());
            }
      }

int global::Length(string needle) {
      char * p1;
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      p1= (char *)gtmp;
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,needle.c_str()))==NULL) return i; // one piece

            i++;
            p1=p1+strlen(needle.c_str());
            }
      }

int global::Length(char * needle) {
      char * p1;
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      p1= (char *)gtmp;
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,needle))==NULL) return i; // one piece

            i++;
            p1=p1+strlen(needle);
            }
      }

int mstring::Length(char * needle) {
      char * p1= (char *) data.c_str();
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,needle))==NULL) return i; // one piece

            i++;
            p1=p1+strlen(needle);
            }
      }

int mstring::Length(string needle) {
      char * p1= (char *) data.c_str();
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,needle.c_str()))==NULL) return i; // one piece

            i++;
            p1=p1+strlen(needle.c_str());
            }
      }

int mstring::Length(global needle) {
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) needle.ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      char * p1= (char *) data.c_str();
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,(char *) gtmp))==NULL) return i; // one piece

            i++;
            p1=p1+strlen((char *) gtmp);
            }
      }

int mstring::Length(mstring needle) {
      char * p1= (char *) data.c_str();
      int i=1;

      if (strlen(p1)==0) return 0;

      while (1) {
            if ((p1=strstr(p1,needle))==NULL) return i; // one piece

            i++;
            p1=p1+strlen(needle);
            }
      }

mstring Horolog() {
      static char t[64];
      mstring tmp;
      _horolog((unsigned char *) t);
      tmp = t;
      return tmp;
      }

global::global(const char * n) { // constructor
      strcpy(ref,"");
      strncpy(myname,n,256);
      myname[255]=0;

      if (Eflg>0) return;

      Eflg++;
      atexit(__finish);
      set_terminate(thrownMessage);
      return;
      }

char * global::get() {
      return ref;
      }

ostream& operator<<(ostream& cout, global x) {

      char tmp[STR_MAX];

      get_global(x.get(), tmp);

      if (!svPtr->tpx) {
		printf("\n*** Global not found: %s\n", x.ref);
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (svPtr->tpx==0) ErrorMessage("global not found",-1);

      cout << (char *)tmp;
      return cout;
      }

int global::Merge(global rhs) {
      unsigned char tmp0[STR_MAX],tmp1[STR_MAX];
      int i;
      tmp0[0]='^';
      tmp1[0]='^';
      strcpy((char *)&tmp0[1],ref);
      strcpy((char *)&tmp1[1],rhs.ref);

      for (i=0; tmp0[i]!='\0'; i++) if (tmp0[i]<30) tmp0[i]=128;

      if (tmp0[i-2]==128 && tmp0[i-1]==128) tmp0[i-1]=0;

      for (i=0; tmp1[i]!='\0'; i++) if (tmp1[i]<30) tmp1[i]=128;

      if (tmp1[i-2]==128 && tmp1[i-1]==128) tmp1[i-1]=0;

      return ::Merge((unsigned char *)tmp0,(unsigned char *)tmp1,svPtr);
      }

int   global::Lock() {
      char atmp[32];
      Mltoa(GETPID(),(unsigned char *) atmp);

      if (strcmp(makeLock(ref, (char *) "1",atmp,' ',NULL), (char *) "1")==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

int   global::UnLock() {
      char atmp[32];
      Mltoa(GETPID(),(unsigned char *) atmp);

      if (strcmp(makeLock(ref, (char *) "1",atmp,'-',NULL), (char *) "1")==0) svPtr->tpx=1;
      else svPtr->tpx=0;

      return svPtr->tpx;
      }

void  CleanLocks(void) {
      char atmp[32];
      Mltoa(GETPID(),(unsigned char *) atmp); /* free all locks first */

      if (svPtr->LockDone) cleanLocks(atmp); /* remove all locks this job */

      return;
      }

void CleanAllLocks(void) {
      remove("./Mumps.Locks");
      }

char * global::Set(char * s) {
      set_global(ref,s);
      return s;
      }

string global::Set(string s) {
      set_global(ref,s.c_str());
      return s;
      }

long global::Set(long s) {
      static char tmp[64];
      sprintf(tmp,"%ld",s);
      set_global(ref,tmp);
      return s;
      }

int   global::Set(int s) {
      static char tmp[64];
      sprintf(tmp,"%d",s);
      set_global(ref,tmp);
      return s;
      }

double global::Set(double s) {
      static char tmp[64];
      sprintf(tmp,"%f",s);
      set_global(ref,tmp);
      return s;
      }

char * global::Get(char * s) {
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return tmp;
      }

string global::Get(string &s) {
      get_global((*this).get(),tmp);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      s = tmp;
      return s;
      }

char * global::Get() {
      get_global((*this).get(),tmp);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return tmp;
      }

double global::Get(double * x) {
      get_global((*this).get(),tmp);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      *x=atof(tmp);
      return *x;
      }

long  global::Get(long * x) {
      get_global((*this).get(),tmp);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (svPtr->tpx==0) ErrorMessage("global not found",-1);

      *x=atol(tmp);
      return *x;
      }

void  global::Kill() {

      if (strlen( (char *) ref ) ==0 ) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            cout << "Incomplete global reference - no parens \n";
            throw MumpsGlobalException();
            }

      KiLL(ref,svPtr);
      return;
      }

mstring global::Order(int dir) {
      mstring tmp;
      tmp = (*this).C_Order(dir);
      return tmp;
      }

char * global::C_Order(int dir) {

      if (dir != 1 && dir != -1) ErrorMessage("Unknown parameter to Order\n", 0 );

      if (dir == 1 ) dir = ORDERNEXT;
      else dir = ORDERPREV;

      svPtr->tpx=Mglobal(dir,(unsigned char *)ref,(unsigned char *)tmp,svPtr);

      strcpy(ref,(char *)svPtr->naked1);
      return tmp;
      }

int   global::Order_Next(char * s) {
      svPtr->tpx=Mglobal(ORDERNEXT,(unsigned char *)ref,(unsigned char *)s,svPtr);
      return svPtr->tpx;
      }

int   global::Order_Prior(char * s) {
      svPtr->tpx=Mglobal(ORDERPREV,(unsigned char *)ref,(unsigned char *)s,svPtr);
      return svPtr->tpx;
      }

int global::Data() {
      static char tmp[4];
      static long i;
      i=strlen(ref);

      GlobalData((unsigned char *) ref,(unsigned char *) tmp,svPtr);
      i=atol(tmp);
      return (int) i;
      }

void mstring::Assign(string b) {

      (*this).Assign((char *) b.c_str());

      }

void mstring::Assign(global b) {

      char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) b.ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      (*this).Assign((char *) gtmp);

      }

void mstring::Assign(mstring b) {

      (*this).Assign((char *) b.data.c_str());

      }

void mstring::Assign(int b) {

      char tmp[124];
      sprintf(tmp,"%d",b);
      (*this).Assign((char *) tmp);

      }

void mstring::Assign(double b) {

      char tmp[124];
      sprintf(tmp,"%f",b);
      (*this).Assign((char *) tmp);

      }

void mstring::Assign(long b) {

      char tmp[124];
      sprintf(tmp,"%ld",b);
      (*this).Assign((char *) tmp);

      }

void mstring::Assign(char * b) {

      char tmp[STR_MAX]="set ";
      strcpy (&tmp[4],(char *)data.c_str());
      strcat(tmp,"=\"");
      strcat(tmp,b);
      strcat(tmp,"\"");
      int f=Interpret(tmp,svPtr);

      if (f!=0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return;
      }

int SQL_Disconnect() {

      return Interpret("sql/c",svPtr);

      }

int SQL_Connect(char * s) {

      char cmd[STR_MAX];
      strcpy(cmd, "sql/d ");
      strcat(cmd, s);
      return Interpret(cmd,svPtr);
      }

int SQL_Connect(string s) {

      return SQL_Connect( (char *) s.c_str() );

      }

int SQL_Connect(mstring s) {

      return SQL_Connect( (char *) s.data.c_str() );

      }

int SQL_Format() {

      char cmd[STR_MAX];
      strcpy(cmd, "sql/c ");
      return Interpret(cmd,svPtr);

      }

int SQL_Format(char * s) {

      char cmd[STR_MAX];
      strcpy(cmd, "sql/c ");
      strcat(cmd, s);
      return Interpret(cmd,svPtr);

      }

int SQL_Format(mstring s) {

      return SQL_Format((char *) s.data.c_str() );

      }

int SQL_Format(string s) {

      return SQL_Format((char *) s.c_str() );

      }

int SQL_Command(char * s) {

      char cmd[STR_MAX];
      strcpy (cmd,"sql ");
      strcat(cmd,s);
      return Interpret(cmd,svPtr);
      }

int SQL_Command(mstring s) {

      return SQL_Command((char *) s.data.c_str());

      }

int SQL_Command(string s) {

      return SQL_Command((char *) s.c_str());

      }

mstring SQL_Table() {
      mstring m="$ztable";
      m = m.Eval();
      return m;
      }

mstring SQL_Table(char * s, int c) {

      char cmd [STR_MAX];
      mstring tab;

      tab=s;

      if (c>10) {
            cout << "Table size too large.\n";
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcpy(cmd,"set $table=\"");
      strcat(cmd,s);
      strcat(cmd,"\"");
      Interpret(cmd,svPtr);
      sprintf(cmd,"set $ztabzize=%d",c);
      Interpret(cmd,svPtr);
      return tab;
      }

mstring SQL_Table(string s, int c) {
      return SQL_Table((char *) s.c_str(), c);
      }

mstring SQL_Table(mstring s, int c) {
      return SQL_Table((char *) s.data.c_str(), c);
      }

mstring SQL_Message() {
      mstring m="$zsql";
      m=m.Eval();
      return m;
      }

bool SQL_Open() {
      mstring m="$zsqlOpen";
      m=m.Eval();

      if (m==1) return true;
      else return false;
      }

bool SQL_Native() {
      mstring m="$znative";
      m=m.Eval();

      if (m==1) return true;
      else return false;
      }

mstring global::operator&(double b) {
      return (*this).Concat(b);
      }

mstring global::operator&(long b) {
      return (*this).Concat(b);
      }

mstring global::operator&(int b) {
      return (*this).Concat(b);
      }

mstring global::operator&(global b) {
      return (*this).Concat(b);
      }

mstring global::operator&(char * b) {
      return (*this).Concat(b);
      }

mstring global::operator&(string b) {
      return (*this).Concat(b);
      }

mstring global::operator&(mstring b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(double b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(long b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(int b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(mstring b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(char * b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(global b) {
      return (*this).Concat(b);
      }

mstring mstring::operator&(string b) {
      return (*this).Concat(b);
      }

mstring global::Concat(double b) {

      char gtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      rslt = b;
      strcat(gtmp,rslt.data.c_str());
      rslt=gtmp;

      return rslt;
      }

mstring global::Concat(long b) {

      char gtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      rslt = b;
      strcat(gtmp,rslt.data.c_str());
      rslt=gtmp;

      return rslt;
      }

mstring global::Concat(int b) {

      char gtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      rslt = b;
      strcat(gtmp,rslt.data.c_str());
      rslt=gtmp;

      return rslt;
      }


mstring global::Concat(char * b) {

      char gtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcat(gtmp,b);
      rslt=gtmp;

      return rslt;
      }

mstring global::Concat(string b) {

      char gtmp[STR_MAX];
      char xtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcat(gtmp,(char *) b.c_str());
      rslt=gtmp;

      return rslt;
      }

mstring global::Concat(mstring b) {

      char gtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcat(gtmp,b.c_str());
      rslt=gtmp;

      return rslt;
      }

mstring global::Concat(global b) {

      char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      char gtmp1[STR_MAX];

      f = Mglobal (RETRIEVE, (unsigned char *) b.ref, (unsigned char *) gtmp1, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcat(gtmp,gtmp1);

      mstring rslt;

      rslt=gtmp;

      return rslt;
      }

mstring mstring::Concat(global b) {

      char gtmp[STR_MAX];
      mstring rslt;

      int f = Mglobal (RETRIEVE, (unsigned char *) b.ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      rslt=data+gtmp;

      return rslt;
      }

mstring mstring::Concat(double b) {

      mstring rslt=b;

      rslt = data + rslt.data;

      return rslt;
      }

mstring mstring::Concat(long b) {

      mstring rslt=b;

      rslt = data + rslt.data;

      return rslt;
      }

mstring mstring::Concat(int b) {

      mstring rslt=b;

      rslt = data + rslt.data;

      return rslt;
      }

mstring mstring::Concat(char * b) {

      mstring rslt;

      rslt = data + b;

      return rslt;
      }

mstring mstring::Concat(string b) {

      mstring rslt;

      rslt = data + b;

      return rslt;
      }

mstring mstring::Concat(mstring b) {

      mstring rslt;

      rslt = data + b.data;

      return rslt;
      }

// fix embedded double quote marks

char*   embed1(const char *in) {

      static char tmp[STR_MAX];
      int i,j=0,k=1;
      char *p1, *p2 = tmp;

      tmp[0]=0;

      if (in==NULL) return tmp;

      for (p1=(char *)in; *p1!=0; p1++)
            if (*p1=='\"') {
                  k=0;
                  break;
                  }

      if (k) return (char *)in; // contains no quotes

      for (p1=(char *)in; *p1; p1++) {
            if (*p1!='\"') *(p2++)=*p1;
            else {
                  *(p2++) = '\"';
                  *(p2++) = '\"';
                  }
            }

      *p2 = 0;
      return tmp;
      }


mstring mstring::Soundex() {

      mstring m;
      unsigned char tmp[STR_MAX], tmp2[STR_MAX];

      strcpy( (char *) tmp, " $zzsoundex"); // note blank before fcn name
      strcat( (char *) tmp, "\xce");
      strcat( (char *) tmp, data.c_str());
      strcat( (char *) tmp, "\xcf");

      zfcn((unsigned char *) tmp, (unsigned char *) tmp2, svPtr);

      m =(char *) tmp2;

      return m;

      }

mstring global::Soundex() {

      mstring m;
      char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      m = "$zzsoundex";
      m = m.Concat("\xce");
      m = m.Concat(gtmp);
      m = m.Concat("\xcf");

      strcpy((char *) &svPtr->v1d[1], m.data.c_str());

      fcn(svPtr);

      m =(char *) &svPtr->bd[1];

      return m;
      }

mstring mstring::Translate(mstring a1, mstring a2) {

      mstring m;

      m = "$translate";
      m = m.Concat("\xce");
      m = m.Concat(data);
      m = m.Concat("\xd0");
      m = m.Concat(a1);
      m = m.Concat("\xd0");
      m = m.Concat(a2);
      m = m.Concat("\xcf");

      strcpy((char *) &svPtr->v1d[1], m.data.c_str());
      fcn(svPtr);
      m =(char *) &svPtr->bd[1];

      return m;
      }

mstring mstring::Translate(mstring a1) {

      mstring m;

      m = "$translate";
      m = m.Concat("\xce");
      m = m.Concat(data);
      m = m.Concat("\xd0");
      m = m.Concat(a1);
      m = m.Concat("\xcf");

      strcpy((char *) &svPtr->v1d[1], m.data.c_str());
      fcn(svPtr);
      m =(char *) &svPtr->bd[1];

      return m;
      }

mstring global::Translate(mstring a1, mstring a2) {

      mstring m;
      char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      m = "$translate";
      m = m.Concat("\xce");
      m = m.Concat(gtmp);
      m = m.Concat("\xd0");
      m = m.Concat(a1);
      m = m.Concat("\xd0");
      m = m.Concat(a2);
      m = m.Concat("\xcf");

      strcpy((char *) &svPtr->v1d[1], m.data.c_str());
      fcn(svPtr);
      m =(char *) &svPtr->bd[1];

      return m;
      }

mstring global::Translate(mstring a1) {

      mstring m;
      char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      m = "$translate";
      m = m.Concat("\xce");
      m = m.Concat(gtmp);
      m = m.Concat("\xd0");
      m = m.Concat(a1);
      m = m.Concat("\xcf");

      strcpy((char *) &svPtr->v1d[1], m.data.c_str());
      fcn(svPtr);
      m =(char *) &svPtr->bd[1];

      return m;
      }

mstring global::Reverse() {

      mstring m;
      char gtmp[STR_MAX]="$reverse(";

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, (unsigned char *) &gtmp[9], svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcat(gtmp,")");
      m = gtmp;

      m = m.Eval();
      return m;
      }

mstring mstring::Reverse() {

      mstring m;

      m = "$reverse("+data+")";

      m = m.Eval();
      return m;
      }

mstring mstring::Qsubscript(int j) {

      mstring m;
      char i[256];
      sprintf(i,"%d",j);

      m = "$qsubscript("+data+","+i+")";

      m = m.Eval();
      return m;
      }

mstring global::Qsubscript(int j) {

      mstring m;
      char i[256];
      sprintf(i,"%d",j);

      m = "$qsubscript("+Sref+","+i+")";

      m = m.Eval();
      return m;
      }

int mstring::Qlength() {

      mstring m;
      int i;

      m = "$qlength(";
      m = m & (*this).FixGblRef() & ")";

      i = m.Eval();
      return i;
      }

int global::Qlength() {

      mstring m;
      int i;

      m = "$qlength(^";
      m = m & Sref & ")";

      i = m.Eval();
      return i;
      }

mstring global::Query() {

      mstring m;

      m = "$query(^";
      m = m & Sref & ")";

      return m.Eval();
      }

mstring mstring::FixGblRef() {

// handles specal case mstring ref of the type ^gbl()
// and converts t to ^gbl("")

      mstring b="";
      int i;
      char tmp[STR_MAX];
      int j;

      strcpy(tmp,data.c_str());

      if ( (j=strlen(tmp)) ==0 ) return b;

      if (tmp[0] != '^') {
            return b;
            }

      for (i=1; i<j; i++) {  // avoid ^
            if ( isalpha(tmp[i]) ) continue;

            break;
            }

      if ( tmp[i] != '(') return b;

      if ( tmp[i+1] != ')') {
            return (*this);
            }

      tmp[i+1] = '\"';
      tmp[i+2] = '\"';
      tmp[i+3] = ')';
      tmp[i+4] = 0;
      b=tmp;
      return b;
      }

mstring mstring::Query() {

      mstring m;
      string x;

      m = "$query(";
      m = m & (*this).FixGblRef() & ")";

      return m.Eval();
      }

//====================================================================

int operator+=(int &i, global j) {
      i = i + j;
      return i;
      }

unsigned int operator+=(unsigned int &i, global j) {
      i = i + j;
      return i;
      }

long operator+=(long &i, global j) {
      i = i + j;
      return i;
      }

unsigned long operator+=(unsigned long &i, global j) {
      i = i + j;
      return i;
      }

short operator+=(short &i, global j) {
      i = i + j;
      return i;
      }

unsigned short operator+=(unsigned short &i, global j) {
      i = i + j;
      return i;
      }

float operator+=(float &i, global j) {
      i = i + j;
      return i;
      }

double operator+=(double &i, global j) {
      i = i + j;
      return i;
      }

int operator-=(int &i, global j) {
      i = i - j;
      return i;
      }

unsigned int operator-=(unsigned int &i, global j) {
      i = i - j;
      return i;
      }

long operator-=(long &i, global j) {
      i = i - j;
      return i;
      }

unsigned long operator-=(unsigned long &i, global j) {
      i = i - j;
      return i;
      }

short operator-=(short &i, global j) {
      i = i - j;
      return i;
      }

unsigned short operator-=(unsigned short &i, global j) {
      i = i - j;
      return i;
      }

float operator-=(float &i, global j) {
      i = i - j;
      return i;
      }

double operator-=(double &i, global j) {
      i = i - j;
      return i;
      }

int operator*=(int &i, global j) {
      i = i * j;
      return i;
      }

unsigned int operator*=(unsigned int &i, global j) {
      i = i * j;
      return i;
      }

long operator*=(long &i, global j) {
      i = i * j;
      return i;
      }

unsigned long operator*=(unsigned long &i, global j) {
      i = i * j;
      return i;
      }

short operator*=(short &i, global j) {
      i = i * j;
      return i;
      }

unsigned short operator*=(unsigned short &i, global j) {
      i = i * j;
      return i;
      }

float operator*=(float &i, global j) {
      i = i * j;
      return i;
      }

double operator*=(double &i, global j) {
      i = i * j;
      return i;
      }

int operator/=(int &i, global j) {
      i = i / j;
      return i;
      }

unsigned int operator/=(unsigned int &i, global j) {
      i = i / j;
      return i;
      }

long operator/=(long &i, global j) {
      i = i / j;
      return i;
      }

unsigned long operator/=(unsigned long &i, global j) {
      i = i / j;
      return i;
      }

short operator/=(short &i, global j) {
      i = i / j;
      return i;
      }

unsigned short operator/=(unsigned short &i, global j) {
      i = i / j;
      return i;
      }

float operator/=(float &i, global j) {
      i = i / j;
      return i;
      }

double operator/=(double &i, global j) {
      i = i / j;
      return i;
      }

//===================================================================================

//?????

/*
        mstring global::operator+(mstring & a) {
                mstring b;
                b = Sref.c_str() + a;
                return b;
                }

        mstring global::operator-(mstring & a) {
                mstring b;
                b = Sref.c_str() - a;
                return b;
                }

        mstring global::operator*(mstring & a) {
                mstring b;
                b = Sref.c_str() * a;
                return b;
                }

        mstring global::operator/(mstring & a) {
                mstring b;
                b = Sref.c_str() / a;
                return b;
                }

        mstring global::operator%(mstring & a) {
                mstring b;
                b = Sref.c_str() % a;
                return b;
                }

*/

/*

        mstring global::operator+=(mstring & a) {
                }
        mstring global::operator-=(mstring & a) {
                }
        mstring global::operator*=(mstring & a) {
                }
        mstring global::operator/=(mstring & a) {
                }
        mstring global::operator%=(mstring & a) {
                }
*/


//===================================================================================


mstring global::operator+() {  // unary +
      mstring tmp;
      tmp=*this+0;
      return tmp;
      }

mstring global::operator-() {  // unary -
      mstring tmp;
      tmp=0-*this;
      return tmp;
      }

int global::operator+=(int i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

unsigned int global::operator+=(unsigned int i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

short global::operator+=(short i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

unsigned short global::operator+=(unsigned short i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

long global::operator+=(long i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

unsigned long global::operator+=(unsigned long i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

float global::operator+=(float i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

double global::operator+=(double i) {
      i= (*this)+i;
      *this = i;
      return i;
      }

int global::operator-=(int i) {
      int z=(*this);
      i = z-i;
      *this = i;
      return i;
      }

unsigned int global::operator-=(unsigned int i) {
      unsigned int z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

short global::operator-=(short i) {
      short z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

unsigned short global::operator-=(unsigned short i) {
      unsigned short z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

long global::operator-=(long i) {
      long z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

unsigned long global::operator-=(unsigned long i) {
      unsigned long z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

float global::operator-=(float i) {
      float z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

double global::operator-=(double i) {
      double z=(*this);
      i= z-i;
      *this = i;
      return i;
      }

int global::operator*=(int i) {
      i= (*this)*i;
      *this = i;
      return i;
      }
unsigned int global::operator*=(unsigned int i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

short global::operator*=(short i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

unsigned short global::operator*=(unsigned short i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

long global::operator*=(long i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

unsigned long global::operator*=(unsigned long i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

float global::operator*=(float i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

double global::operator*=(double i) {
      i= (*this)*i;
      *this = i;
      return i;
      }

int global::operator/=(int i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

unsigned int global::operator/=(unsigned int i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

short global::operator/=(short i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

unsigned short global::operator/=(unsigned short i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

long global::operator/=(long i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

unsigned long global::operator/=(unsigned long i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

float global::operator/=(float i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

double global::operator/=(double i) {
      i= (*this)/i;
      *this = i;
      return i;
      }

//==== casts ================================================================

global::operator char*() {
      *this >> tmp;
      return tmp;
      }

global::operator int() {
      int i;
      *this >> i;
      return i;
      }

global::operator unsigned int() {
      unsigned int i;
      *this >> i;
      return i;
      }

global::operator long() {
      long i;
      *this >> i;
      return i;
      }

global::operator unsigned long() {
      unsigned long i;
      *this >> i;
      return i;
      }

global::operator short() {
      short i;
      *this >> i;
      return i;
      }

global::operator unsigned short() {
      unsigned short i;
      *this >> i;
      return i;
      }

global::operator float() {
      float i;
      *this >> i;
      return i;
      }

global::operator double() {
      double i;
      *this >> i;
      return i;
      }

global::operator mstring() {
      mstring i;
      *this >> i;
      return i;
      }

//==================== int ==========================================

int global::operator+(int s) {
      int i;
      *this >> i;
      return s+i;
      }

int operator+(int s, global a) {
      int i;
      a >> i;
      return i+s;
      }

unsigned int global::operator+(unsigned int s) {
      unsigned int i;
      *this >> i;
      return s+i;
      }

unsigned int operator+(unsigned int s,global a) {
      unsigned int i;
      a >> i;
      return i+s;
      }

int global::operator-(int s) {
      int i;
      *this >> i;
      return s-i;
      }

int operator-(int s,global a) {
      int i;
      a >> i;
      return s-i;
      }

unsigned int global::operator-(unsigned int s) {
      unsigned int i;
      *this >> i;
      return s-i;
      }

unsigned int operator-(unsigned int s,global a) {
      unsigned int i;
      a >> i;
      return s-i;
      }

int global::operator*(int s) {
      int i;
      *this >> i;
      return s*i;
      }

int operator*(int s,global a) {
      int i;
      a >> i;
      return s*i;
      }

unsigned int global::operator*(unsigned int s) {
      unsigned int i;
      *this >> i;
      return s*i;
      }

unsigned int operator*(unsigned int s,global a) {
      unsigned int i;
      a >> i;
      return s*i;
      }

int global::operator/(int s) {
      int i;
      *this >> i;
      return i/s;
      }

int operator/(int s,global a) {
      int i;
      a >> i;
      return s/i;
      }

unsigned int global::operator/(unsigned int s) {
      unsigned int i;
      *this >> i;
      return i/s;
      }

unsigned int operator/(unsigned int s,global a) {
      unsigned int i;
      a >> i;
      return s/i;
      }

//=========== global =================================================

double  global::operator+(global s) {
      char tmp1[STR_MAX];
      char tmp2[STR_MAX];
      char *p1;
      double a,b;
      s >> tmp1;
      b=strtod(tmp1,&p1);
      *this >> tmp2;
      a=strtod(tmp2,&p1);
      a=a+b;
      return a;
      }

double global::operator-(global s) {
      char tmp1[STR_MAX];
      char tmp2[STR_MAX];
      char *p1;
      double a,b;
      s >> tmp1;
      b=strtod(tmp1,&p1);
      *this >> tmp2;
      a=strtod(tmp2,&p1);
      a=a-b;
      return a;
      }

double global::operator*(global s) {
      char tmp1[STR_MAX];
      char tmp2[STR_MAX];
      char *p1;
      double a,b;
      s >> tmp1;
      b=strtod(tmp1,&p1);
      *this >> tmp2;
      a=strtod(tmp2,&p1);
      a=a*b;
      return a;
      }

double global::operator/(global s) {
      char tmp1[STR_MAX];
      char tmp2[STR_MAX];
      char *p1;
      double a,b;
      s >> tmp1;
      b=strtod(tmp1,&p1);
      *this >> tmp2;
      a=strtod(tmp2,&p1);
      a=a/b;
      return a;
      }

double global::operator++() {
      double a;
      *this >> a;
      a++;

      if (floor(a)==a)
            *this << (long) a;
      else *this << a;

      return a;
      }

double global::operator--() {
      double a;
      *this >> a;
      a--;

      if (floor(a)==a)
            *this << (long) a;
      else *this << a;

      return a;
      }

double global::operator++(int) {
      double a,b;
      *this >> a;
      b=a+1;

      if (floor(b)==b)
            *this << (long) b;
      else *this << b;

      return a;
      }

double global::operator--(int) {
      double a,b;
      *this >> a;
      b=a-1;

      if (floor(b)==b)
            *this << (long) b;
      else *this << b;

      return a;
      }

//=========== float =================================================

float global::operator+(float s) {
      float i;
      *this >> i;
      return s+i;
      }

float operator+(float s,global a) {
      float i;
      a >> i;
      return s+i;
      }

float global::operator-(float s) {
      float i;
      *this >> i;
      return s-i;
      }

float operator-(float s,global a) {
      float i;
      a >> i;
      return s-i;
      }

float global::operator*(float s) {
      float i;
      *this >> i;
      return s*i;
      }

float operator*(float s,global a) {
      float i;
      a >> i;
      return s*i;
      }

float global::operator/(float s) {
      float i;
      *this >> i;
      return i/s;
      }

float operator/(float s,global a) {
      float i;
      a >> i;
      return s/i;
      }

//=========== double =================================================

double global::operator+(double s) {
      double i;
      *this >> i;
      return s+i;
      }

double operator+(double s,global a) {
      double i;
      a >> i;
      return s+i;
      }

double global::operator-(double s) {
      double i;
      *this >> i;
      return s-i;
      }

double operator-(double s,global a) {
      double i;
      a >> i;
      return s-i;
      }

double global::operator*(double s) {
      double i;
      *this >> i;
      return s*i;
      }

double operator*(double s,global a) {
      double i;
      a >> i;
      return s*i;
      }

double global::operator/(double s) {
      double i;
      *this >> i;
      return i/s;
      }

double operator/(double s,global a) {
      double i;
      a >> i;
      return s/i;
      }

//====== long ================================================

long global::operator+(long s) {
      long i;
      *this >> i;
      return s+i;
      }

long operator+(long s,global a) {
      long i;
      a >> i;
      return s+i;
      }

unsigned long global::operator+(unsigned long s) {
      unsigned long i;
      *this >> i;
      return s+i;
      }

unsigned long operator+(unsigned long s,global a) {
      unsigned long i;
      a >> i;
      return s+i;
      }

long global::operator-(long s) {
      long i;
      *this >> i;
      return s-i;
      }

long operator-(long s,global a) {
      long i;
      a >> i;
      return s-i;
      }

unsigned long global::operator-(unsigned long s) {
      unsigned long i;
      *this >> i;
      return s-i;
      }

unsigned long operator-(unsigned long s,global a) {
      unsigned long i;
      a >> i;
      return s-i;
      }

long global::operator*(long s) {
      long i;
      *this >> i;
      return s*i;
      }

long operator*(long s,global a) {
      long i;
      a >> i;
      return s*i;
      }

unsigned long global::operator*(unsigned long s) {
      long i;
      *this >> i;
      return s*i;
      }

unsigned long operator*(unsigned long s,global a) {
      long i;
      a >> i;
      return s*i;
      }

long global::operator/(long s) {
      long i;
      *this >> i;
      return i/s;
      }

long operator/(long s,global a) {
      long i;
      a >> i;
      return s/i;
      }

unsigned long global::operator/(unsigned long s) {
      unsigned long i;
      *this >> i;
      return i/s;
      }

unsigned long operator/(unsigned long s,global a) {
      unsigned long i;
      a >> i;
      return s/i;
      }

//================= short =========================================

short global::operator+(short s) {
      short i;
      *this >> i;
      return s+i;
      }

short operator+(short s,global a) {
      short i;
      a >> i;
      return s+i;
      }

unsigned short global::operator+(unsigned short s) {
      unsigned short i;
      *this >> i;
      return s+i;
      }

unsigned short operator+(unsigned short s,global a) {
      unsigned short i;
      a >> i;
      return s+i;
      }

short global::operator-(short s) {
      short i;
      *this >> i;
      return s-i;
      }

short operator-(short s,global a) {
      short i;
      a >> i;
      return s-i;
      }

unsigned short global::operator-(unsigned short s) {
      unsigned short i;
      *this >> i;
      return s-i;
      }

unsigned short operator-(unsigned short s,global a) {
      unsigned short i;
      a >> i;
      return s-i;
      }

short global::operator*(short s) {
      short i;
      *this >> i;
      return s*i;
      }

short operator*(short s,global a) {
      short i;
      a >> i;
      return s*i;
      }

unsigned short global::operator*(unsigned short s) {
      short i;
      *this >> i;
      return s*i;
      }

unsigned short operator*(unsigned short s,global a) {
      short i;
      a >> i;
      return s*i;
      }

short global::operator/(short s) {
      short i;
      *this >> i;
      return i/s;
      }

short operator/(short s,global a) {
      short i;
      a >> i;
      return s/i;
      }

unsigned short global::operator/(unsigned short s) {
      unsigned short i;
      *this >> i;
      return i/s;
      }

unsigned short operator/(unsigned short s,global a) {
      unsigned short i;
      a >> i;
      return s/i;
      }

//=========== comparison ==============================================

int global::operator>(global a) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      *this >> tmp1;
      a >> tmp2;

      if (strcmp(tmp1,tmp2)>0) return 1;

      return 0;
      }

int global::operator>(int a) {
      int i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(unsigned int a) {
      unsigned int i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(short a) {
      short i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(unsigned short a) {
      unsigned short i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(long a) {
      long i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(unsigned long a) {
      unsigned long i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(float a) {
      float i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(double a) {
      double i;
      *this >> i;

      if (i>a) return 1;

      return 0;
      }

int global::operator>(char * a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a)>0) return 1;

      return 0;
      }

int global::operator>(string a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())>0) return 1;

      return 0;
      }

// ---- friends

int operator>(int a,global b) {
      int i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(unsigned int a,global b) {
      unsigned int i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(short a,global b) {
      short i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(unsigned short a,global b) {
      unsigned short i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(long a,global b) {
      long i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(unsigned long a,global b) {
      unsigned long i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(float a,global b) {
      float i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(double a,global b) {
      double i;
      b >> i;

      if (a>i) return 1;

      return 0;
      }

int operator>(char * a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a,tmp1)>0) return 1;

      return 0;
      }

int operator>(string a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)>0) return 1;

      return 0;
      }

int global::operator<(global a) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      *this >> tmp1;
      a >> tmp2;

      if (strcmp(tmp1,tmp2)<0) return 1;

      return 0;
      }

int global::operator<(int a) {
      int i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(unsigned int a) {
      unsigned int i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(short a) {
      short i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(unsigned short a) {
      unsigned short i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(long a) {
      long i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(unsigned long a) {
      unsigned long i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(float a) {
      float i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(double a) {
      double i;
      *this >> i;

      if (i<a) return 1;

      return 0;
      }

int global::operator<(char * a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a)<0) return 1;

      return 0;
      }

int global::operator<(string a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())<0) return 1;

      return 0;
      }

int global::operator<(mstring a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())<0) return 1;

      return 0;
      }

// ---- friends

int operator<(int a,global b) {
      int i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(unsigned int a,global b) {
      unsigned int i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(short a,global b) {
      short i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(unsigned short a,global b) {
      unsigned short i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(long a,global b) {
      long i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(unsigned long a,global b) {
      unsigned long i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(float a,global b) {
      float i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(double a,global b) {
      double i;
      b >> i;

      if (a<i) return 1;

      return 0;
      }

int operator<(char * a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a,tmp1)<0) return 1;

      return 0;
      }

int operator<(string a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)<0) return 1;

      return 0;
      }

int operator<(mstring a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)<0) return 1;

      return 0;
      }

int global::operator>=(global a) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      *this >> tmp1;
      a >> tmp2;

      if (strcmp(tmp1,tmp2)>=0) return 1;

      return 0;
      }

int global::operator>=(int a) {
      int i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(unsigned int a) {
      unsigned int i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(short a) {
      short i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(unsigned short a) {
      unsigned short i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(long a) {
      long i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(unsigned long a) {
      unsigned long i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(float a) {
      float i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(double a) {
      double i;
      *this >> i;

      if (i>=a) return 1;

      return 0;
      }

int global::operator>=(char * a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a)>=0) return 1;

      return 0;
      }

int global::operator>=(string a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())>=0) return 1;

      return 0;
      }

// ---- friends

int operator>=(int a,global b) {
      int i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(unsigned int a,global b) {
      unsigned int i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(short a,global b) {
      short i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(unsigned short a,global b) {
      unsigned short i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(long a,global b) {
      long i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(unsigned long a,global b) {
      unsigned long i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(float a,global b) {
      float i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(double a,global b) {
      double i;
      b >> i;

      if (a>=i) return 1;

      return 0;
      }

int operator>=(char * a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a,tmp1)>=0) return 1;

      return 0;
      }

int operator>=(string a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)>=0) return 1;

      return 0;
      }

int global::operator<=(global a) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      *this >> tmp1;
      a >> tmp2;

      if (strcmp(tmp1,tmp2)<=0) return 1;

      return 0;
      }

int global::operator<=(int a) {
      int i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(unsigned int a) {
      unsigned int i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(short a) {
      short i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(unsigned short a) {
      unsigned short i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(long a) {
      long i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(unsigned long a) {
      unsigned long i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(float a) {
      float i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(double a) {
      double i;
      *this >> i;

      if (i<=a) return 1;

      return 0;
      }

int global::operator<=(char * a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a)<=0) return 1;

      return 0;
      }

int global::operator<=(string a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())<=0) return 1;

      return 0;
      }

// ---- friends

int operator<=(int a,global b) {
      int i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(unsigned int a,global b) {
      unsigned int i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(short a,global b) {
      short i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(unsigned short a,global b) {
      unsigned short i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(long a,global b) {
      long i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(unsigned long a,global b) {
      unsigned long i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(float a,global b) {
      float i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(double a,global b) {
      double i;
      b >> i;

      if (a<=i) return 1;

      return 0;
      }

int operator<=(char * a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a,tmp1)<=0) return 1;

      return 0;
      }

int operator<=(string a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)>=0) return 1;

      return 0;
      }

int global::operator==(global a) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      *this >> tmp1;
      a >> tmp2;

      if (strcmp(tmp1,tmp2)==0) return 1;

      return 0;
      }

int global::operator==(int a) {
      int i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(unsigned int a) {
      unsigned int i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(short a) {
      short i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(unsigned short a) {
      unsigned short i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(long a) {
      long i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(unsigned long a) {
      unsigned long i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(float a) {
      float i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(double a) {
      double i;
      *this >> i;

      if (i==a) return 1;

      return 0;
      }

int global::operator==(char * a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a)==0) return 1;

      return 0;
      }

int global::operator==(string a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())==0) return 1;

      return 0;
      }

// ---- friends

int operator==(int a,global b) {
      int i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(unsigned int a,global b) {
      unsigned int i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(short a,global b) {
      short i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(unsigned short a,global b) {
      unsigned short i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(long a,global b) {
      long i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(unsigned long a,global b) {
      unsigned long i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(float a,global b) {
      float i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(double a,global b) {
      double i;
      b >> i;

      if (a==i) return 1;

      return 0;
      }

int operator==(char * a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a,tmp1)==0) return 1;

      return 0;
      }

int operator==(string a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)>=0) return 1;

      return 0;
      }

int global::operator!=(global a) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      *this >> tmp1;
      a >> tmp2;

      if (strcmp(tmp1,tmp2)!=0) return 1;

      return 0;
      }

int global::operator!=(int a) {
      int i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(unsigned int a) {
      unsigned int i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(short a) {
      short i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(unsigned short a) {
      unsigned short i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(long a) {
      long i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(unsigned long a) {
      unsigned long i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(float a) {
      float i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(double a) {
      double i;
      *this >> i;

      if (i!=a) return 1;

      return 0;
      }

int global::operator!=(char * a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a)!=0) return 1;

      return 0;
      }

int global::operator!=(string a) {
      char tmp1[STR_MAX];
      *this >> tmp1;

      if (strcmp(tmp1,a.c_str())!=0) return 1;

      return 0;
      }

// ---- friends

int operator!=(int a,global b) {
      int i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(unsigned int a,global b) {
      unsigned int i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(short a,global b) {
      short i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(unsigned short a,global b) {
      unsigned short i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(long a,global b) {
      long i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(unsigned long a,global b) {
      unsigned long i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(float a,global b) {
      float i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(double a,global b) {
      double i;
      b >> i;

      if (a!=i) return 1;

      return 0;
      }

int operator!=(char * a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a,tmp1)!=0) return 1;

      return 0;
      }

int operator!=(string a,global b) {
      char tmp1[STR_MAX];
      b >> tmp1;

      if (strcmp(a.c_str(),tmp1)>=0) return 1;

      return 0;
      }

//=========== shift in =================================================

global & global::operator<<(char * s) {
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(global g) {
      char s[STR_MAX];
      get_global(g.get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      set_global(ref,s);
      return *this;
      }

global & global::operator<<(int i) {
      char s[32];
      sprintf(s,"%d",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(unsigned int i) {
      char s[32];
      sprintf(s,"%ud",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(short i) {
      char s[32];
      sprintf(s,"%hd",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(unsigned short i) {
      char s[32];
      sprintf(s,"%hu",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(long i) {
      char s[32];
      sprintf(s,"%ld",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(unsigned long i) {
      char s[32];
      sprintf(s,"%lu",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(float i) {
      char s[32];
      sprintf(s,"%f",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(double i) {
      char s[32];
      sprintf(s,"%f",i);
      set_global(ref,s);
      return *this;
      }

global & global::operator<<(string s) {
      set_global(ref,s.c_str());
      return *this;
      }

global & global::operator<<(mstring s) {
      set_global(ref,s.c_str());
      return *this;
      }

//================ shift out =======================================

char * global::operator>>(char * s) {
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return s;
      }

global & global::operator>>(global g) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      set_global(g.ref,s);
      return *this;
      }

int global::operator>>(int & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%d",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

unsigned int global::operator>>(unsigned int & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if(sscanf(s,"%d",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

long global::operator>>(long & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%ld",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

unsigned long global::operator>>(unsigned long & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%lu",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

short global::operator>>(short & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%hd",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

unsigned short global::operator>>(unsigned short & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%hu",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

float global::operator>>(float & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%f",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

double global::operator>>(double & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (sscanf(s,"%lf",&i)==0) {
            GlobalExceptionCode=INVALID_CONVERSION;
            throw MumpsGlobalException();
            }

      return i;
      }

string global::operator>>(string & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      i=s;
      return i;
      }

mstring global::operator>>(mstring & i) {
      char s[STR_MAX];
      get_global((*this).get(),s);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      i=s;
      return i;
      }

global & global::operator=(const char * s) {
      set_global(ref,s);
      return *this;
      }

global & global::operator=(string s) {
      set_global(ref,s.c_str());
      return *this;
      }

global & global::operator=(mstring s) {
      set_global(ref,s.c_str());
      return *this;
      }

global & global::operator=(int s) {
      char tmp[128];
      sprintf(tmp,"%d",s);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(double s) {
      char tmp[128];
      gcvt_wrapper(s,15,tmp);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(global s) {
      char tmp1[STR_MAX];
      s >> tmp1;
      set_global(ref,tmp1);
      return *this;
      }

global & global::operator=(unsigned int s) {
      char tmp[128];
      sprintf(tmp,"%ud",s);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(float s) {
      char tmp[128];
      gcvt_wrapper(s,15,tmp);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(short s) {
      char tmp[128];
      sprintf(tmp,"%hd",s);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(unsigned short s) {
      char tmp[128];
      sprintf(tmp,"%uhd",s);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(long s) {
      char tmp[128];
      sprintf(tmp,"%ld",s);
      set_global(ref,tmp);
      return *this;
      }

global & global::operator=(unsigned long s) {
      char tmp[128];
      sprintf(tmp,"%lud",s);
      set_global(ref,tmp);
      return *this;
      }

global global::MakeGlobal(int flg, char * gblnme, char * key, ...) {
      va_list ap;
      char *x;
      long ix;
      char tmp[32];
      static char buf[STR_MAX];
      int i,j=1;
      Sref=gblnme;
      Sname=gblnme;
      Sref=Sref+"(";

      va_start (ap,key);
      j=count(key);
      ArgCount=j;
      strcpy(buf,"");
      strcatx((unsigned char *) buf,(unsigned char *) gblnme);

      if (strcmp(key,"\"\"")==0) flg=0;

      for (i=0; i<j; i++) {
            if (flg==1) {
                  ix=va_arg(ap,long);
                  sprintf(tmp,"%ld",ix);
                  strcatx((unsigned char *) buf,(unsigned char *) tmp);
                  //strcat(Sref,tmp);
                  Sref=Sref+tmp;
                  //strcat(Sref,",");
                  Sref=Sref+",";
                  }
            else {
                  x=va_arg(ap,char *);
                  strcatx((unsigned char *) buf,(unsigned char *) x);
                  Sref=Sref+"\"";
                  Sref=Sref+x;
                  Sref=Sref+"\"";
                  Sref=Sref+",";
                  }
            }

      va_end(ap);
      strcpy(ref,buf);
      Sref[Sref.length()-1] = ')';
      return *this;
      }

int RunMumps(struct MSV * svPtr, const char * exp, char * key, ... ) {
      va_list ap;
      va_start (ap,key);
      int i,j=1,k;
      char *p1, *x;
      char tmp[STR_MAX];

      strcpy(tmp,key);
      p1=tmp;

      for (; *p1!=0; p1++) if (*p1==',') j++; // count args

      p1=strtok(tmp,",");

      for (i=0; i<j; i++) {
            x=va_arg(ap,char *);

            if (x==NULL) break;

            sym_(0,(unsigned char *) p1,(unsigned char *) x,svPtr);
            p1=strtok(NULL,",");
            }

      va_end(ap);
      svPtr->NOERR=1;
      strcpy(tmp,exp);
      k=Interpret(tmp,svPtr);
      svPtr->ERROR=k;
      va_start (ap,key);
      strcpy(tmp,key);
      p1=strtok(tmp,",");

      for (i=0; i<j; i++) {
            x=va_arg(ap,char *);

            if (x==NULL) break;

            sym_(1,(unsigned char *) p1,(unsigned char *) x,svPtr);
            p1=strtok(NULL," ");
            }

      va_end(ap);
      return k;
      }

int xecute(string exp) {
      int k;
      char tmp[STR_MAX];
      svPtr->NOERR=1;
      strcpy(tmp,exp.c_str());
      k=Interpret(tmp,svPtr);
      svPtr->ERROR=k;
      return k;
      }

int xecute(mstring exp) {
      int k;
      char tmp[STR_MAX];
      svPtr->NOERR=1;
      strcpy(tmp,exp.c_str());
      k=Interpret(tmp,svPtr);
      svPtr->ERROR=k;
      return k;
      }

int xecute(char * exp) {
      int k;
      char tmp[STR_MAX];
      svPtr->NOERR=1;
      strcpy(tmp,exp);
      k=Interpret(tmp,svPtr);
      svPtr->ERROR=k;
      return k;
      }

int WriteMumps1(struct MSV * svPtr, char * exp) {
      int k;
      char tmp[STR_MAX]="write ";
      strcat(tmp,exp);
      svPtr->NOERR=1;
      k=Interpret(tmp,svPtr);
      svPtr->ERROR=k;
      return k;
      }

// MSTRING PATTERN

int mstring::Pattern(string p) {
      return pm((const unsigned char *) (*this).c_str(),(const unsigned char *) p.c_str(),svPtr);
      }

int mstring::Pattern(mstring & p) {
      return pm((const unsigned char *) (*this).c_str(),(const unsigned char *) p.c_str(),svPtr);
      }

int mstring::Pattern(global p) {
      unsigned char tmp[STR_MAX];
      int f = Mglobal (RETRIEVE, (unsigned char *) p.ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return pm((const unsigned char *) (*this).c_str(),(const unsigned char *) tmp,svPtr);
      }

int mstring::Pattern(const char * p) {
      return pm((const unsigned char *) (*this).c_str(),(const unsigned char *) p,svPtr);
      }

// GLOBAL PATTERN

int global::Pattern(global p) {

      unsigned char tmp[STR_MAX];
      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      unsigned char tmp1[STR_MAX];
      f = Mglobal (RETRIEVE, (unsigned char *) p.ref, tmp1, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return pm((const unsigned char *) tmp,(const unsigned char *) tmp1, svPtr);
      }

int global::Pattern(string p) {

      unsigned char tmp[STR_MAX];
      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return pm((const unsigned char *) tmp,(const unsigned char *) p.c_str(),svPtr);
      }

//????????
int global::Pattern(mstring & p) {

      unsigned char tmp[STR_MAX];
      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      return pm((const unsigned char *) tmp,(const unsigned char *) p.c_str(),svPtr);
      }

int global::Pattern(const char * p) {

      unsigned char tmp [STR_MAX];
      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) throw MumpsGlobalException();

      return pm((const unsigned char *) tmp,(const unsigned char *) p,svPtr);
      }

// global piece

mstring global::Piece(const char *pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) gtmp,(unsigned char *) pat,
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

mstring global::Piece(global pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];
      unsigned char gtmp[STR_MAX];
      unsigned char gtmp1[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) pat.ref, gtmp1, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) gtmp,(unsigned char *) gtmp1,
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }


mstring global::Piece(string pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) gtmp,(unsigned char *) pat.c_str(),
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

mstring global::Piece(mstring &pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) gtmp,(unsigned char *) pat.data.c_str(),
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

//	char * Piece

char * Piece(const char * source, char * rslt, const char *pat, int ii, int jj) {

      char cc1[65],cc2[64];

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) rslt, (unsigned char *) source, (unsigned char *) pat,
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);

      return rslt;
      }

// mstring piece

mstring mstring::Piece(const char *pat, int ii, int jj) {

      string tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) data.c_str(),(unsigned char *) pat,
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

mstring mstring::Piece(string pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) data.c_str(),(unsigned char *) pat.c_str(),
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

mstring mstring::Piece(global pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) pat.ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) data.c_str(),(unsigned char *) gtmp,
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

mstring mstring::Piece(mstring &pat, int ii, int jj) {
      mstring tmp1;
      char tmp[STR_MAX],cc1[65],cc2[64];

      if (jj<0) jj=ii;

      sprintf(cc1,"%d",ii);
      sprintf(cc2,"%d",jj);

      _piece((unsigned char *) tmp, (unsigned char *) data.c_str(),(unsigned char *) pat.data.c_str(),
             (unsigned char *) cc1, (unsigned char *) cc2, 0, (unsigned char *) "", svPtr);
      tmp1=tmp;
      return tmp1;
      }

/* global justify */

mstring global::Justify(int c1, int c2) {

      int i,j=1,k,x;
      char cc1[32],cc2[32];
      mstring tmp1;
      unsigned char gtmp[STR_MAX];
      char tmp[STR_MAX];

      sprintf(cc1,"%d",c1);
      sprintf(cc2,"%d",c2);

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      _justify((unsigned char *) tmp,(unsigned char *) gtmp,
               (unsigned char *) cc1,(unsigned char *) cc2);

      tmp1=tmp;
      return tmp1;
      }

mstring mstring::Justify(int c1, int c2) {

      int i,j=1,k,x;
      char cc1[32],cc2[32];
      char tmp[STR_MAX];
      mstring tmp1;

      sprintf(cc1,"%d",c1);
      sprintf(cc2,"%d",c2);

      _justify((unsigned char *) tmp,(unsigned char *) data.c_str(),
               (unsigned char *) cc1,(unsigned char *) cc2);

      tmp1=tmp;

      return tmp1;
      }

mstring global::Fnumber(mstring a, mstring b) {

      char tmp[STR_MAX];
      mstring rslt="";
      unsigned char Open[] = {CodedOpen,0};
      unsigned char Comma[] = {CodedComma,0};
      unsigned char Close[] = {CodedClose,0};
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcpy(tmp,"$fnumber");
      strcat(tmp,(char *) Open);

      strcat(tmp,(char *) gtmp); // invoking mstring
      strcat(tmp,(char *)Comma);

      strcat(tmp,a.c_str()); // first argument required

      if (b.c_str()==0) strcat (tmp,(char *) Close); // second arge optional
      else {
            strcat(tmp,(char *)Comma);
            strcat(tmp,b.c_str());
            strcat(tmp,(char *)Close);
            }

      strcpy((char *) &svPtr->v1d[1],tmp);
      fcn(svPtr);
      rslt = (char *) &svPtr->bd[1];
      return rslt;
      }

mstring global::Fnumber(mstring a) {

      char tmp[STR_MAX];
      mstring rslt="";
      unsigned char Open[] = {CodedOpen,0};
      unsigned char Comma[] = {CodedComma,0};
      unsigned char Close[] = {CodedClose,0};
      unsigned char gtmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, gtmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      strcpy(tmp,"$fnumber");
      strcat(tmp,(char *) Open);

      strcat(tmp,(char *) gtmp); // invoking mstring
      strcat(tmp,(char *)Comma);

      strcat(tmp,a.c_str()); // first argument required
      strcat(tmp,(char *)Close);

      strcpy((char *) &svPtr->v1d[1],tmp);
      fcn(svPtr);
      rslt = (char *) &svPtr->bd[1];
      return rslt;
      }

mstring mstring::Fnumber(mstring a, mstring b) {

      char tmp[STR_MAX];
      mstring rslt="";
      unsigned char Open[] = {CodedOpen,0};
      unsigned char Comma[] = {CodedComma,0};
      unsigned char Close[] = {CodedClose,0};

      strcpy(tmp,"$fnumber");
      strcat(tmp,(char *) Open);

      strcat(tmp,(char *) data.c_str()); // invoking mstring
      strcat(tmp,(char *)Comma);

      strcat(tmp,a.c_str()); // first argument required

      if (b.c_str()==0) strcat (tmp,(char *) Close); // second arge optional
      else {
            strcat(tmp,(char *)Comma);
            strcat(tmp,b.c_str());
            strcat(tmp,(char *)Close);
            }

      strcpy((char *) &svPtr->v1d[1],tmp);
      fcn(svPtr);
      rslt = (char *) &svPtr->bd[1];
      return rslt;
      }

mstring mstring::Fnumber(mstring a) {

      char tmp[STR_MAX];
      mstring rslt="";
      unsigned char Open[] = {CodedOpen,0};
      unsigned char Comma[] = {CodedComma,0};
      unsigned char Close[] = {CodedClose,0};

      strcpy(tmp,"$fnumber");
      strcat(tmp,(char *) Open);

      strcat(tmp,(char *) data.c_str()); // invoking mstring
      strcat(tmp,(char *)Comma);

      strcat(tmp,a.c_str()); // first argument required
      strcat(tmp,(char *)Close);

      strcpy((char *) &svPtr->v1d[1],tmp);
      fcn(svPtr);
      rslt = (char *) &svPtr->bd[1];
      return rslt;
      }

mstring mstring::Extract(int st, int ln) {
      char a[40],b[40];
      char r[STR_MAX];
      mstring rr;
      Mltoa(st,(unsigned char *) a);
      Mltoa(ln,(unsigned char *) b);
      _extract((unsigned char *) r,(unsigned char *) data.c_str(),(unsigned char *) a,(unsigned char *) b);
      rr=r;
      return rr;
      }

mstring global::Extract(int st, int ln) {
      char a[40],b[40];
      char r[STR_MAX];
      mstring rr;
      unsigned char tmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      Mltoa(st,(unsigned char *) a);
      Mltoa(ln,(unsigned char *) b);
      _extract((unsigned char *) r,(unsigned char *) tmp,(unsigned char *) a,(unsigned char *) b);
      rr=r;
      return rr;
      }

int global::Find(char * key, int st) {

      char a[40];
      char r[20];
      int rr;
      unsigned char tmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      Mltoa(st,(unsigned char *) a);
      _find((unsigned char *) r,(unsigned char *) tmp,
            (unsigned char *) key,(unsigned char *) a);
      rr=atoi((char *) r);
      return rr;
      }

int global::Find(global key, int st) {

      char a[40];
      char r[20];
      int rr;
      unsigned char tmp[STR_MAX];
      unsigned char tmp1[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      f = Mglobal (RETRIEVE, (unsigned char *) key.ref, tmp1, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      Mltoa(st,(unsigned char *) a);
      _find((unsigned char *) r,(unsigned char *) tmp,
            (unsigned char *) tmp1,(unsigned char *) a);

      rr=atoi((char *) r);
      return rr;
      }

int global::Find(mstring key, int st) {

      char a[40];
      char r[20];
      int rr;
      unsigned char tmp[STR_MAX];

      int f = Mglobal (RETRIEVE, (unsigned char *) ref, tmp, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      Mltoa(st,(unsigned char *) a);
      _find((unsigned char *) r,(unsigned char *) tmp,
            (unsigned char *) key.c_str(),(unsigned char *) a);
      rr=atoi((char *) r);
      return rr;
      }

int mstring::Find(const char * key, int st) {
      char a[40];
      char r[20];
      int rr;
      Mltoa(st,(unsigned char *) a);
      _find((unsigned char *) r,(unsigned char *) data.c_str(),
            (unsigned char *) key,(unsigned char *) a);
      rr=atoi((char *) r);
      return rr;
      }

int mstring::Find(global key, int st) {
      char a[40];
      char r[20];
      int rr;
      char tmp1[STR_MAX];

      Mltoa(st,(unsigned char *) a);

      int f = Mglobal (RETRIEVE, (unsigned char *) key.ref, (unsigned char *) tmp1, svPtr);

      if (f==0) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      _find((unsigned char *) r,(unsigned char *) data.c_str(),
            (unsigned char *) tmp1,(unsigned char *) a);

      rr=atoi((char *) r);
      return rr;
      }

int mstring::Find(mstring key, int st) {
      char a[40];
      char r[20];
      int rr;

      Mltoa(st,(unsigned char *) a);

      _find((unsigned char *) r,(unsigned char *) data.c_str(),
            (unsigned char *) key.c_str(),(unsigned char *) a);

      rr=atoi((char *) r);
      return rr;
      }

void KiLL(char * gtmp, struct MSV * svPtr) {

      unsigned char tmp14[STR_MAX],tmp15[STR_MAX];
      int _k,_j;

      keyfix((unsigned char *)gtmp);
      _k=strlen(gtmp);

      if (gtmp[_k-1]==1 && gtmp[_k-2] == 1) gtmp[_k-- -1]=0;

      strmove((unsigned char *)tmp14,(unsigned char *) gtmp);

      Mglobal(GKILL,(unsigned char *)gtmp,(unsigned char *)tmp15,svPtr);

      while(1) {
            _j=Mglobal(XNEXT,(unsigned char *)gtmp,tmp15,svPtr);

            if (_j && strncmp((char *)tmp14,gtmp,_k) == 0) {
                  Mglobal(GKILL,(unsigned char *)gtmp,tmp15,svPtr);
                  continue;
                  }

            break;
            }
      }

char * __command__ (char * str) {
      static char rslt[STR_MAX];
      strcpy(rslt,str+1);
      rslt[strlen(rslt)-1]='\0';
      return rslt;
      }

StopList stop;

void StopInit(char * file) {
      string tmp=file;
      StopInit(tmp);
      }

void StopInit(mstring file) {
      StopInit(file.s_str());
      }

void StopInit(string file) {

      FILE *fp;
      char line[512];
      int i;

      stop.clear();

      fp=fopen(file.c_str(),"r");

      if (fp==NULL) {
            cout << "Stoplist file not found." << endl;
            exit (EXIT_FAILURE);
            }

      while (1) {
            if (fgets(line,512,fp)==NULL) break;

            for (i=0; line[i]!='\0'; i++) {
                  if (line[i]==' ') {
                        line[i++]=0;
                        break;
                        }

                  line[i]=tolower(line[i]);
                  }

            line[i-1]='\0';
            stop.insert(line);
            }

      fclose(fp);
      }

int StopLookup(mstring str) {
      return StopLookup(str.s_str());
      }

int StopLookup(string str) {

      if (stop.find(str)==stop.end()) return 0;
      else return 1;
      }

extern "C" int cmpr(const char *a, const Syn *b) {
      return strcmp((const char *) a, (*b).word);
      }

int StopLookup(char * str) {
      string tmp;
      tmp=str;

      if (stop.find(tmp)==stop.end()) return 0;
      else return 1;
      }

Syn syn[SYNMAX];
long synx;

char * SYN(char *word) {
      Syn *p;
      p=(Syn *)bsearch(word,syn,synx,sizeof(struct Syn),
                       (int(*)(const void *,const void *))cmpr);

      if (p==NULL) return word;

      return (*p).alias;
      }

mstring SYN(mstring word) {
      mstring tmp;
      tmp=SYN((char *)word.c_str());
      return tmp;
      }

string SYN(string word) {
      static string tmp;
      tmp=SYN((char *)word.c_str());
      return tmp;
      }

int SynInit(char * file) {
      string tmp=file;
      return SynInit(tmp);
      }

int SynInit(mstring file) {
      return SynInit(file.s_str());
      }

int SynInit(string file) {

      FILE * fp;
      char *word,*p1,*type,line[12000];
      int i,flg;

      fp=fopen(file.c_str(),"r");

      if (fp==NULL) {
            cout << "synonym file missing" << endl;
            exit(EXIT_FAILURE);
            }

      while (1) {
            if (fgets(line,12000,fp)==NULL) break;

            for (i=0; line[i]!='\0'; i++) line[i]=tolower(line[i]);

            line[i-1]='\0';
            p1=strtok(line," ");

            if (p1==NULL) continue;

            word=(char *)malloc(strlen(p1)+1);

            if (word==NULL) {
                  cout << "out of memory" << endl;
                  exit(EXIT_FAILURE);
                  }

            strcpy(word,p1);

            while (1) {
                  p1=strtok(NULL," ");

                  if (p1==NULL) break;

                  if (strlen(p1)==0) break;

                  type=(char *)malloc(strlen(p1)+1);

                  if (type==NULL) {
                        cout << "out of memory" << endl;
                        exit(EXIT_FAILURE);
                        }

                  strcpy(type,p1);
                  syn[synx].word=type;
                  syn[synx].alias=word;
                  synx++;

                  if (synx>=SYNMAX) {
                        cout << "Too many synonyms - quitting " << endl ;
                        return 1;
                        }
                  }
            }

      fclose(fp);
      flg=1;

      while (flg) {
            flg=0;

            for (i=0; i<synx-1; i++) {
                  if (strcmp(syn[i].word,syn[i+1].word)>0) {
                        word=syn[i].word;
                        type=syn[i].alias;
                        syn[i].word=syn[i+1].word;
                        syn[i].alias=syn[i+1].alias;
                        syn[i+1].word=word;
                        syn[i+1].alias=type;
                        flg=1;
                        }
                  }
            }

      return synx;
      }

string Shred (string str, int size) {
      return Shred((char *)str.c_str(),size);
      }

mstring Shred (mstring str, int size) {
      return Shred((char *)str.c_str(),size);
      }

char *Shred(char * str, int size) {

      static char * p1=NULL, *p2;
      static int flg=0,i;
      static char *string=NULL;
      static char word[128];

      if (size>=128) {
            cout << "Shred fragment size too large" << endl;
            sigint(100);
            }

      if (flg==0) {
            p1=str;

            if (string != NULL) free (string);

            string = (char *) malloc (strlen(p1)+1);

            if (string==NULL) {
                  cout << "Out of memory in Token()" << endl;
                  sigint(100);
                  }

            p2=string;

            for (; *p1!='\0'; p1++) { // copy to local string
                  if (isalnum(*p1)) *(p2++)=tolower(*p1); // permit only alphanumerics
                  }

            if (p2==string) {
                  return (char *) "";
                  }

            *p2='\0';
            flg=1; // don't reenter this block until string consumed
            p1=string;
            }

      while (1) {
            strncpy(word,p1,size);
            word[size]='\0';
            p1=p1+size;

            if (p1>p2) {
                  flg=0;
                  free(string);
                  string=NULL;
                  return (char *) "";
                  }

            return word;
            }
      }

string ShredQuery(string str, int size) {
      return ShredQuery((char *)str.c_str(),size);
      }

mstring ShredQuery(mstring str, int size) {
      return ShredQuery((char *)str.c_str(),size);
      }

char * ShredQuery(char * str, int size) {

      static int i=0,j;
      char *p1;

      p1=Shred(&str[i],size);

      if (strlen(p1)==0) {
            i++;

            if (i<size)
                  return Shred(&str[i],size);

            i=0;
            return (char *) "";
            }

      return p1;
      }

#undef kill  // also used in signal.h

/*---------------------------------------------------------------------------
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+ IMPORTANT: sequences MUST have a blank in array position 0
#+
#+ to invoke:
#+
#+    int sw (char *, char *, int, int, int, int, int)
#+
#+    score = sw(seq1, seq2 [,flg1, flg2, gap, mismatch, match]);
#+
#+    score       similarity score between seq1 and seq2
#+    seq1        first sequence whsoe first letter MUST be blank.
#+    seq2        second sequence whose first letter MUST be blank.
#+
#+ The following arguments may be omitted and defaults will apply.
#+
#+    flg1        if 0, do not print alignments (default)
#+                if not zero, print alingments
#+    flg2        if 0, do not print intermediate matrix (default)
#+                if not zero, print intermediate matrix
#+    gap         gap penalty; default -1
#+    mismatch    mismatch penalty; default -1
#+    match       match reward; default 2
#+
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define MAX_ALIGNS 5000000
#define EMPTY_CELL -32768

short **__a;
string S_RESULT;
int Size;
int Size_s;
int Size_t;
int GAP;       // gap penalty
int MISMATCH;  // mis-match penalty
int MATCH;     // match reward
int sx=0;

int   nodash(char * a) {
      int i=0;

      for (; *a!=0; a++) if (*a!='-') i++;

      return i;
      }

inline int Score(char a, char b) {
      if (a=='-' || b=='-') return GAP;

      if (a==b) return MATCH;

      return MISMATCH;
      }

int   prnt(int ss, char *s, char *t, int imax, int jmax) {
      char tmp[16];

      while(1) {
            if (imax<=0 || jmax<=0) break;

            if (__a[imax][jmax]==0) break;

            if (__a[imax][jmax]==EMPTY_CELL) break;

            if (Score(s[imax],t[jmax])+__a[imax-1][jmax-1] == __a[imax][jmax] ) { // on the diagonal
                  tmp[0]=s[imax];
                  tmp[1]=t[jmax];
                  tmp[2]=0;
                  S_RESULT=S_RESULT+tmp;
                  imax--;
                  jmax--;
                  continue;
                  }


            if (Score(s[imax],'-')+__a[imax-1][jmax] == __a[imax][jmax] ) {
                  tmp[0]=s[imax];
                  tmp[1]='-';
                  tmp[2]=0;
                  S_RESULT=S_RESULT+tmp;
                  imax--;
                  continue;
                  }

            if ( Score('-',t[jmax]) + __a[imax][jmax-1] == __a[imax][jmax]) {
                  tmp[0]='-';
                  tmp[1]=t[jmax];
                  tmp[2]=0;
                  S_RESULT=S_RESULT+tmp;
                  jmax--;
                  continue;
                  }
            }

      return 0;
      }

void  init() {
      int i,j;
      short int *p;

      if ( (__a= (short int **) malloc ( (Size_s) * sizeof(int *))) == NULL) {
            cout << "Out of memory allocating Smith-Waterman matrix" << endl;
            raise(SIGSEGV);
            }

      for (i=0; i < (Size_s); i++) {
            if ( (__a[i] = (short int *) malloc ((Size_t)*sizeof(int)) ) == NULL) {
                  cout << "Out of memory allocating Smith-Waterman matrix" << endl;
                  raise(SIGSEGV);
                  }

            for (p=__a[i],j=0; j<Size_t; j++) *(p++)=EMPTY_CELL;
            }

      S_RESULT="";
      return;
      }

int Cost(char *s, char *t, int i, int j) {
      int A,b,c,x;

      if (__a[i][j]!=EMPTY_CELL) return __a[i][j];

      if (i<=0 || j<=0) {
            __a[i][j]=0;
            return 0;
            }

      A=Cost(s,t,i-1,j-1) + (s[i]==t[j]?MATCH:MISMATCH);
      b=Cost(s,t,i-1,j) + GAP;
      c=Cost(s,t,i,j-1) + GAP;

      if (b>A) A=b;

      if (c>A) A=c;

      if (A<=0) A=0;

      return __a[i][j]=A;
      }

int   sw(mstring s, mstring t, int ShowAlign, int ShowMatrix, int gap, int mismatch, int match) {
      return sw((char *) s.c_str(),(char *) t.c_str(),ShowAlign,ShowMatrix,gap,mismatch,match);
      }

int   sw(string s, string t, int ShowAlign, int ShowMatrix, int gap, int mismatch, int match) {
      return sw((char *) s.c_str(),(char *) t.c_str(),ShowAlign,ShowMatrix,gap,mismatch,match);
      }

int   sw(char *s, char *t, int ShowAlign, int ShowMatrix, int gap, int mismatch, int match) {
      char *m1,*m2;
      int Maxscore=0,Maxscorex=-1,score,ix,jx,x,top,i,j,k,m,n,imax,jmax;

      GAP=gap;
      MISMATCH=mismatch;
      MATCH=match;

      Size=Size_s=strlen(s);
      Size_t=strlen(t);

      if (Size_t>Size) Size=Size_t;

      Size+=2;
      sx=0;
      init();

      if (ShowAlign && 0) {
            fprintf(svPtr->out_file[svPtr->io], "S-W Alignments for:\n");
            int m1Length=strlen(s);

            for (int al=0; al<m1Length; al=al+60) {
                  char t1[80];
                  strncpy(t1,s+al,60);
                  t1[60]=0; // 0 to 59
                  fprintf(svPtr->out_file[svPtr->io], "%6d %s\n",al,t1);
                  }

            fprintf(svPtr->out_file[svPtr->io], "\n");
            m1Length=strlen(t);

            for (int al=0; al<m1Length; al=al+60) {
                  char t1[80];
                  strncpy(t1,t+al,60);
                  t1[60]=0; // 0 to 59
                  fprintf(svPtr->out_file[svPtr->io], "%6d %s\n",al,t1);
                  }

            fprintf(svPtr->out_file[svPtr->io], "\n");
            }

      Cost(s,t,Size_s-1,Size_t-1);

      if (ShowMatrix) {
            top=0;
            fprintf(svPtr->out_file[svPtr->io], "      ");

            for (i=0; t[i]!=0; i++) fprintf(svPtr->out_file[svPtr->io], "%5d",i);

            fprintf(svPtr->out_file[svPtr->io], "\n      ");

            for (i=0; t[i]!=0; i++) fprintf(svPtr->out_file[svPtr->io], "    %c",t[i]);

            fprintf(svPtr->out_file[svPtr->io], "\n");

            for (i=0; s[i]!=0; i++) {
                  fprintf(svPtr->out_file[svPtr->io], "%3d %c ",i,s[i]);

                  for (j=0; t[j]!=0; j++) {
                        if (__a[i][j]!=EMPTY_CELL) fprintf(svPtr->out_file[svPtr->io], "%5d",__a[i][j]);
                        else fprintf(svPtr->out_file[svPtr->io], "   ");

                        if (__a[i][j]>top) {
                              top=__a[i][j];
                              imax=i;
                              jmax=j;
                              }
                        }

                  fprintf(svPtr->out_file[svPtr->io], "\n");
                  }
            }
      else {
            top=0;

            for (i=0; s[i]!=0; i++) {
                  for (j=0; t[j]!=0; j++) {
                        if (__a[i][j]>top) {
                              top=__a[i][j];
                              imax=i;
                              jmax=j;
                              }
                        }
                  }
            }

      if (top==0) {
            if (ShowAlign) cout << "No matches" << endl;

            return 0;
            }

      prnt(0,s,t,imax,jmax);
      Maxscorex=-1;
      score=0;
      i=0;
            {
            // no show alignments - score only
            int max=S_RESULT.length()-1;

            for (k=0; k<max; k=k+2) {
                  if (S_RESULT[k]==S_RESULT[k+1]) score=score+MATCH;
                  else if (S_RESULT[k]=='-' || S_RESULT[k+1]=='-') score=score+GAP;
                  else score=score+MISMATCH;
                  }

            if (score>Maxscore) {
                  Maxscore=score;
                  Maxscorex=i;
                  }
            }

//--------------------------------------------------
      score=0;
      i=Maxscorex;

      if (Maxscorex<0) {
            cout << "No alignments" << endl;
            }
      else if (ShowAlign) {
            m=1;
            m=S_RESULT.length();
            k=m=m/2;

            if ( (m1=(char *) malloc(2*m+1)) == NULL) { // assumes gaps will not be more than twice seq lngth
                  cout << "Out of memory" << endl;
                  raise(SIGSEGV);
                  }

            if ( (m2=(char *) malloc(2*m+1)) == NULL) {
                  cout << "Out of memory" << endl;
                  raise(SIGSEGV);
                  }

            m1[k--]=0;
            m2[m--]=0;

            for (j=0; j<S_RESULT.length(); j=j+2) {   // reverse copy
                  m1[k--]=S_RESULT[j];
                  m2[m--]=S_RESULT[j+1];
                  }

            // display alignments and calculate score

            score=0;
            int m1Length=strlen(m1),m2Length=strlen(m2);
            int m1x=0,m2x=0;
            m1x=imax-nodash(m1)+1;
            m2x=jmax-nodash(m2)+1;

            for (int al=0; al<m1Length; al=al+60) {
                  char t1[80],t2[80];

                  if (al>=m2Length) strcpy(t2," ");
                  else strncpy(t2,m2+al,60);

                  strncpy(t1,m1+al,60);
                  t1[60]=0; // 0 to 59
                  t2[60]=0; // 0 to 59

                  // printf("%5d %s %d\n",imax-nodash(m1)+1+al,t1,imax-nodash(m1)+al+60);
                  fprintf(svPtr->out_file[svPtr->io], "%5d %s %d\n",m1x,t1,m1x+nodash(t1));
                  // printf("%s\n",t1);
                  m1x=m1x+nodash(t1)+1;
                  fprintf(svPtr->out_file[svPtr->io], "      ");

                  for (j=0; t1[j]!=0; j++) {
                        if (t1[j]==t2[j]&&t1[j]!='-') {
                              fprintf(svPtr->out_file[svPtr->io], ":");
                              score=score+MATCH;
                              }
                        else if (t1[j]=='-' || t2[j]=='-') {
                              score=score+GAP;
                              fprintf(svPtr->out_file[svPtr->io], " ");
                              }
                        else {
                              score=score+MISMATCH;
                              fprintf(svPtr->out_file[svPtr->io], " ");
                              }
                        }

                  fprintf(svPtr->out_file[svPtr->io], "\n");
                  // printf("%5d %s %d\n\n",jmax-nodash(m2)+al+1,t2,jmax-nodash(m2)+al+60);
                  fprintf(svPtr->out_file[svPtr->io], "%5d %s %d\n\n",m2x,t2,m2x+nodash(t2));
                  //printf("%s\n\n",t2);
                  m2x=m2x+nodash(t2)+1;
                  }

            free (m1);
            free (m2);
            }

//--------------------------------------------------

      for (i=0; i<(Size_s); i++) free (__a[i]);

      free (__a);

      return Maxscore;
      }

/* ???
int bmg_fullsearch(mstring a, mstring buf) {
      return bmg_fullsearch(a.s_str(),buf.s_str());
      }

int bmg_fullsearch(mstring a, global g) {
      string buf;
      buf = (string &) g;
      return bmg_fullsearch(a.s_str(),buf);
      }

int bmg_fullsearch(char * a, global g) {
      string buf;
      buf=(string &)g;
      return bmg_fullsearch(a,buf);
      }

int bmg_fullsearch(string a, global g) {
      string buf;
      buf=(string &)g;
      return bmg_fullsearch(a,buf);
      }

int bmg_fullsearch(string a, string buf) {
      char *p=(char *)malloc(buf.length()+1);
      char *q=(char *)malloc(a.length()+1);
      if (p==NULL || q==NULL) {
            ErrorMessage("Out of memory",__LINE__);
            }
      strcpy(p,a.c_str());
      strcpy(q,buf.c_str());
      int i=bmg_fullsearch(p,q);
      free (p);
      free (q);
      return i;
      }
*/

mstring mstring::Eval() {

// evaluate a mumps experession
// acts like the indirection operator

      int parse_ (struct MSV * );
      mstring tmp;

      strcpy( (char *) &svPtr->xd[0], data.c_str());
      svPtr->xpx=0;
      svPtr->t2 = 1;
      svPtr->pd1len = 0;

      if ((svPtr->ierr = parse_ (svPtr))) {
            GlobalExceptionCode=INTERPRETER_ERROR;
            throw MumpsGlobalException();
            tmp = "err";
            return tmp;
            }
      else {
            tmp = (char *) &svPtr->pd1[svPtr->sdlim];
            return tmp;
            }
      }

//=========================
// mstring constructors
//=========================

mstring::mstring( const char * a) {
      data = a;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring(const mstring &a) {
      name = a.name;
      data = a.data;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring(const string &a) {

      data = a;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring( long a) {
      char tmp[32];
      sprintf(tmp,"%ld",a);
      data = tmp;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring( int a) {
      char tmp[32];
      sprintf(tmp,"%d",a);
      data = tmp;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring( double a) {
      char tmp[32];
      sprintf(tmp,"%lf",a);
      data = tmp;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring( float a) {
      char tmp[32];
      sprintf(tmp,"%f",a);
      data = tmp;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring( char a) {
      char tmp[32];
      tmp[0]=a;
      tmp[1]=0;
      data = tmp;
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

mstring::mstring( ) {
      data="";
//    atexit(__finish);
//    set_terminate(thrownMessage);
      }

const char * mstring::c_str() {
      return data.c_str();
      }

const string mstring::s_str() {
      return data;
      }

int mstring::replace(mstring i, mstring j) {
      pcre *re;
      const char *error;
      int erroffset;
      re = pcre_compile(
                 i.c_str(),          /* the pattern */
                 0,                /* default options */
                 &error,           /* for error message */
                 &erroffset,       /* for error offset */
                 NULL);            /* use default character tables */

      if (re==NULL) {
            cout << "Pattern match error in pattern: " << error << endl;
            GlobalExceptionCode=PATTERN_ERROR;
            throw MumpsGlobalException();
            }

      int rc;
      int ovector[90];
      rc = pcre_exec(
                 re,             /* result of pcre_compile() */
                 NULL,           /* we didn't study the pattern */
                 (*this).c_str(),  /* the subject string */
                 strlen((*this).c_str()),             /* the length of the subject string */
                 0,              /* start at offset 0 in the subject */
                 0,              /* default options */
                 ovector,        /* vector for substring information */
                 90);            /* number of elements in the vector */

      if (rc==1)
            (*this) = (*this).s_str().substr(0,ovector[0]) + j.s_str() + (*this).s_str().substr(ovector[1]);

      return rc;  // see return codes in pcre docs. 1 means match, -1 means no match, other neg are errs

      }

int mstring::begins(mstring i) {
      pcre *re;
      const char *error;
      int erroffset;
      re = pcre_compile(
                 i.c_str(),          /* the pattern */
                 0,                /* default options */
                 &error,           /* for error message */
                 &erroffset,       /* for error offset */
                 NULL);            /* use default character tables */

      if (re==NULL) {
            cout << "Pattern match error in pattern: " << error << endl;
            GlobalExceptionCode=PATTERN_ERROR;
            throw MumpsGlobalException();
            }

      int rc;
      int ovector[90];
      rc = pcre_exec(
                 re,             /* result of pcre_compile() */
                 NULL,           /* we didn't study the pattern */
                 (*this).c_str(),  /* the subject string */
                 strlen((*this).c_str()),             /* the length of the subject string */
                 0,              /* start at offset 0 in the subject */
                 0,              /* default options */
                 ovector,        /* vector for substring information */
                 90);            /* number of elements in the vector */

      if (rc==1) return ovector[0];

      return -1;
      }


int mstring::ends(mstring i) {
      pcre *re;
      const char *error;
      int erroffset;
      re = pcre_compile(
                 i.c_str(),          /* the pattern */
                 0,                /* default options */
                 &error,           /* for error message */
                 &erroffset,       /* for error offset */
                 NULL);            /* use default character tables */

      if (re==NULL) {
            cout << "Pattern match error in pattern: " << error << endl;
            GlobalExceptionCode=PATTERN_ERROR;
            throw MumpsGlobalException();
            }

      int rc;
      int ovector[90];
      rc = pcre_exec(
                 re,             /* result of pcre_compile() */
                 NULL,           /* we didn't study the pattern */
                 (*this).c_str(),  /* the subject string */
                 strlen((*this).c_str()),             /* the length of the subject string */
                 0,              /* start at offset 0 in the subject */
                 0,              /* default options */
                 ovector,        /* vector for substring information */
                 90);            /* number of elements in the vector */

      if (rc==1) return ovector[1];

      return -1;
      }

int mstring::decorate(mstring i, mstring j, mstring k) {
      pcre *re;
      const char *error;
      int erroffset;
      re = pcre_compile(
                 i.c_str(),          /* the pattern */
                 0,                /* default options */
                 &error,           /* for error message */
                 &erroffset,       /* for error offset */
                 NULL);            /* use default character tables */

      if (re==NULL) {
            cout << "Pattern match error in pattern: " << error << endl;
            GlobalExceptionCode=PATTERN_ERROR;
            throw MumpsGlobalException();
            }

      int rc;
      int ovector[90];
      rc = pcre_exec(
                 re,             /* result of pcre_compile() */
                 NULL,           /* we didn't study the pattern */
                 (*this).c_str(),  /* the subject string */
                 strlen((*this).c_str()),             /* the length of the subject string */
                 0,              /* start at offset 0 in the subject */
                 0,              /* default options */
                 ovector,        /* vector for substring information */
                 90);            /* number of elements in the vector */

      if (rc==1) {
            string s1,s2,s3;
            int i=0;

            while (1) {
                  s1 = (*this).s_str().substr(0,ovector[i]) + j.s_str();
                  s2 = (*this).s_str().substr(ovector[i],ovector[i+1]-ovector[i]);

                  for (int j=0; j<s2.length(); j++) s2[j]=toupper(s2[j]);

                  s3 = k.s_str() + (*this).s_str().substr(ovector[i+1]);
                  (*this)=s1+s2+s3;
                  i=i+2;
                  break;

                  if (ovector[i]<0) break;
                  }

            return 1;
            }

      return rc;  // see return codes in pcre docs. 1 means match, -1 means no match, other neg are errs

      }

mstring & mstring::operator[](mstring x) {
      int i;
      i=atoi(x.c_str());
      return *(this+i);
      }

mstring & mstring::operator[](int i) {
      return *(this+i);
      }

mstring & mstring::operator[](const char * x) {
      int i;
      i=atoi(x);
      return *(this+i);
      }

mstring & mstring::operator[](string x) {
      int i;
      i=atoi(x.c_str());
      return *(this+i);
      }

mstring mstring::operator+=(mstring i) {
      return *this+i;
      }

mstring mstring::operator+=(int i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator+=(long i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator+=(double i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator+=(float i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator+=(string i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator+=(const char * i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator+=(global i) {
      (*this)=(*this)+i;
      return (*this);
      }

mstring mstring::operator-=(mstring i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(int i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(long i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(double i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(float i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(string i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(const char * i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator-=(global i) {
      (*this)=(*this)-i;
      return (*this);
      }

mstring mstring::operator*=(mstring i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(int i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(long i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(double i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(float i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(string i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(const char * i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator*=(global i) {
      (*this)=(*this)*i;
      return (*this);
      }

mstring mstring::operator/=(mstring i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(int i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(long i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(double i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(float i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(string i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(const char * i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator/=(global i) {
      (*this)=(*this)/i;
      return (*this);
      }

mstring mstring::operator%=(mstring i) {
      (*this)=(*this)%i;
      return (*this);
      }

mstring mstring::operator%=(int i) {
      (*this)=(*this)%i;
      return (*this);
      }

mstring mstring::operator%=(long i) {
      (*this)=(*this)%i;
      return (*this);
      }

mstring mstring::operator%=(string i) {
      (*this)=(*this)%i;
      return (*this);
      }

mstring mstring::operator%=(const char * i) {
      (*this)=(*this)%i;
      return (*this);
      }

mstring mstring::operator%=(global i) {
      (*this)=(*this)%i;
      return (*this);
      }

ostream & operator<<(ostream & c, mstring i) {
      c << i.data;
      return c;
      }

istream & operator>>(istream & c, mstring &i) {
      c >> i.data;
      return c;
      }

mstring& mstring::operator=(int i) {
      char tmp[32]="";
      sprintf(tmp,"%d",i);
      data=tmp;
      return *this;
      }

mstring& mstring::operator=(long i) {
      char tmp[32]="";
      sprintf(tmp,"%d",i);
      data=tmp;
      return *this;
      }

mstring& mstring::operator=(double i) {
      char tmp[32]="";
      sprintf(tmp,"%g",i);
      data=tmp;
      return *this;
      }

mstring& mstring::operator=(float i) {
      char tmp[32]="";
      sprintf(tmp,"%g",i);
      data=tmp;
      return *this;
      }

mstring& mstring::operator=(string i) {
      data=i;
      return *this;
      }

mstring& mstring::operator=(const char * i) {
      data=i;
      return *this;
      }

mstring& mstring::operator=(mstring i) {
      data=i.data;
      return *this;
      }

mstring& mstring::operator=(global i) {
      char tmp[STR_MAX];
      get_global(i.get(),tmp);

      if (!svPtr->tpx) {
            GlobalExceptionCode=GLOBAL_NOT_FOUND;
            throw MumpsGlobalException();
            }

      data=tmp;
      return *this;
      }

mstring mstring::operator!() {
      mstring tmp;

      if ((*this).Length()==0 || *this=="0") {
            tmp="1";
            return tmp;
            }
      else {
            tmp="0";
            return tmp;
            }
      }

mstring mstring::operator++() {
      *this=*this+1;
      return *this;
      }

mstring mstring::operator--() {
      *this=*this-1;
      return *this;
      }

mstring mstring::operator++(int) {
      mstring tmp=*this;
      *this=*this+1;
      return tmp;
      }

mstring mstring::operator--(int) {
      mstring tmp=*this;
      *this=*this-1;
      return tmp;
      }

mstring mstring::operator+() {  // unary +
      mstring tmp;
      tmp=*this+0;
      return tmp;
      }

mstring mstring::operator-() {  // unary -
      mstring tmp;
      tmp=0-*this;
      return tmp;
      }

mstring mstring::operator+(int i) {
      mstring tmp;
      char t1[64];
      add1((const char *)data.c_str(),i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(long i) {
      mstring tmp;
      char t1[64];
      add1((const char *)data.c_str(),(int)i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(double i) {
      mstring tmp;
      char t1[64];
      add3((char *)data.c_str(),i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(float i) {
      mstring tmp;
      char t1[64];
      add3((char *)data.c_str(),(double)i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(mstring i) {
      mstring tmp;
      char t1[64];
      add((char *)data.c_str(),(char *)i.data.c_str(),t1);
      tmp=t1;
      return (tmp);
      }

mstring mstring::operator+(string i) {
      mstring tmp;
      char t1[64];
      add((char *)data.c_str(),(char *)i.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(global i) {
      mstring tmp;
      char t1[STR_MAX];
      get_global(i.get(),t1);
      add((char *)data.c_str(),t1,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(const char * i) {
      mstring tmp;
      char t1[64];
      add(data.c_str(),i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator+(char * i) {
      mstring tmp;
      char t1[64];
      add(data.c_str(),(const char *)i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator||(mstring i) {
      mstring a;
      a.data=data+i.data;
      return a;
      }

mstring mstring::operator||(int i) {
      mstring a;
      mstring t1;
      t1=i;
      a.data=data+t1.s_str();
      return a;
      }

mstring mstring::operator||(long i) {
      mstring a;
      mstring t1;
      t1=i;
      a.data=data+t1.s_str();
      return a;
      }

mstring mstring::operator||(float i) {
      mstring a;
      mstring t1;
      t1=i;
      a.data=data+t1.s_str();
      return a;
      }

mstring mstring::operator||(double i) {
      mstring a;
      mstring t1;
      t1=i;
      a.data=data+t1.s_str();
      return a;
      }

mstring mstring::operator||(string i) {
      mstring a;
      a.data=data+i;
      return a;
      }

mstring mstring::operator||(global i) {
      mstring a;
      string t1;
      i >> t1;
      a.data=data+t1;
      return a;
      }

mstring mstring::operator||(const char * i) {
      mstring a;
      a.data=data+i;
      return a;
      }

mstring operator||(string i, mstring s) {
      mstring a;
      a.data=i+s.s_str();
      return a;
      }

mstring operator||(global i, mstring s) {
      mstring a;
      string t1;
      i >> t1;
      a.data=t1+s.s_str();
      return a;
      }

mstring operator||(int i, mstring s) {
      mstring a;
      string t1;
      t1=i;
      a.data=t1+s.s_str();
      return a;
      }

mstring operator||(long i, mstring s) {
      mstring a;
      string t1;
      t1=i;
      a.data=t1+s.s_str();
      return a;
      }

mstring operator||(float i, mstring s) {
      mstring a;
      mstring t1;
      t1=i;
      a=t1+s;
      return a;
      }

mstring operator||(double i, mstring s) {
      mstring a;
      mstring t1;
      t1=i;
      a=t1+s;
      return a;
      }

mstring operator||(const char * i, mstring s) {
      mstring a;
      a.data=i+s.s_str();
      return a;
      }

mstring mstring::operator-(int i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),cvt(i),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(long i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),cvt(i),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(double i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),cvt(i),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(float i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),cvt(i),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(mstring i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),(char *)i.data.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(string i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),(char *)i.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(char * i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(const char * i) {
      mstring tmp;
      char t1[64];
      sub((char *)data.c_str(),(char *)i,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator-(global i) {
      mstring tmp;
      char t1[STR_MAX];
      get_global(i.get(),t1);
      sub((char *)data.c_str(),t1,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator%(int i) {
      mstring tmp;
      long j=atol(data.c_str());
      j=j%i;
      tmp = j;
      return tmp;
      }

mstring mstring::operator%(long i) {
      mstring tmp;
      long j=atol(data.c_str());
      j=j%i;
      tmp = j;
      return tmp;
      }

mstring mstring::operator%(mstring i) {
      mstring tmp;
      int j=atol(data.c_str());
      j=j%atol(i.data.c_str());
      tmp = j;
      return tmp;
      }

mstring mstring::operator%(string i) {
      mstring tmp;
      int j=atol(data.c_str());
      j=j%atol(i.c_str());
      tmp = j;
      return tmp;
      }

mstring mstring::operator%(global i) {
      mstring tmp;
      int j=atol(data.c_str());
      int k=i;
      j=j%k;
      tmp = j;
      return tmp;
      }

mstring mstring::operator%(const char * i) {
      mstring tmp;
      int j=atol(data.c_str());
      j=j%atol(i);
      tmp = j;
      return tmp;
      }

mstring mstring::operator*(int i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)cvt(i),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator*(long i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)cvt(i),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator*(double i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)cvt(i),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator*(float i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)cvt(i),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator*(mstring i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)i.data.c_str(),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator*(string i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)i.c_str(),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator*(global i) {
      mstring tmp;
      char t1[STR_MAX];
      get_global(i.get(),t1);
      mult((char *)data.c_str(),(char *)t1,(char *)t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator*(const char * i) {
      mstring tmp;
      char t1[64];
      mult((char *)data.c_str(),(char *)i,(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(int i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),cvt(i),t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(long i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),cvt(i),t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(double i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),cvt(i),t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(float i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),cvt(i),t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(mstring i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),(char *)i.data.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(string i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),(char *)i.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring mstring::operator/(global i) {
      mstring tmp;
      char t1[STR_MAX];
      get_global(i.get(),t1);
      divx((char *)data.c_str(),t1,t1);
      tmp=t1;
      return tmp;
      }

mstring mstring::operator/(const char * i) {
      mstring tmp;
      char t1[64];
      divx((char *)data.c_str(),i,t1);
      tmp = t1;
      return tmp;
      }

// mstring friends

mstring operator%(int i, mstring k) {
      mstring tmp;
      tmp = i % atol( (char *) k.c_str());
      return tmp;
      }

mstring operator%(long i, mstring k) {
      mstring tmp;
      tmp = i % atol( (char *) k.c_str());
      return tmp;
      }

mstring operator%(string i, mstring k) {
      mstring tmp;
      tmp = atol(i.c_str()) % atol( (char *) k.c_str());
      return tmp;
      }

mstring operator%(global i, mstring k) {
      mstring tmp;
      long m;
      i >> m;
      tmp = m % atol( (char *) k.c_str());
      return tmp;
      }

mstring operator%(const char * i, mstring k) {
      mstring tmp;
      tmp = atol(i) % atol( (char *) k.c_str());
      return tmp;
      }


mstring operator+(int i, mstring k) {
      return k+i;
      }

mstring operator+(long i, mstring k) {
      return k+i;
      }

mstring operator+(double i, mstring k) {
      return k+i;
      }

mstring operator+(float i, mstring k) {
      return k+i;
      }

mstring operator+(string i, mstring k) {
      return k+i;
      }

mstring operator+(global i, mstring k) {
      return k+i;
      }

mstring operator+(const char * i, mstring k) {
      return k+i;
      }

mstring operator+(char * i, mstring k) {
      return k+i;
      }

mstring operator-(int i, mstring k) {
      mstring tmp;
      char t1[64];
      sub(cvt(i),(char *)k.data.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator-(long i, mstring k) {
      mstring tmp;
      char t1[64];
      sub(cvt(i),(char *)k.data.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator-(double i, mstring k) {
      mstring tmp;
      char t1[64];
      sub(cvt(i),(char *)k.data.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator-(float i, mstring k) {
      mstring tmp;
      char t1[64];
      sub(cvt(i),(char *)k.data.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator-(string i, mstring k) {
      mstring tmp;
      char t1[64];
      sub((char *)i.c_str(),(char *)k.data.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring operator-(global i, mstring k) {
      mstring tmp;
      char t1[64];
      string t2;
      i >> t2;
      sub((char *)t2.c_str(),(char *)k.data.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring operator-(const char * i, mstring k) {
      mstring tmp;
      char t1[64];
      sub((char *)i,(char *)k.data.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring operator-(char * i, mstring k) {
      mstring tmp;
      char t1[64];
      sub((char *)i,(char *)k.data.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring operator*(int i, mstring k) {
      mstring tmp;
      char t1[64];
      mult((char *)cvt(i),(char *)k.data.c_str(),(char *)t1);
      tmp=t1;
      return tmp;
      }

mstring operator*(long i, mstring k) {
      mstring tmp;
      char t1[64];
      mult((char *)cvt(i),(char *)k.data.c_str(),(char *)t1);
      tmp=t1;
      return tmp;
      }

mstring operator*(double i, mstring k) {
      mstring tmp;
      char t1[64];
      mult((char *)cvt(i),(char *)k.data.c_str(),(char *)t1);
      tmp=t1;
      return tmp;
      }

mstring operator*(float i, mstring k) {
      mstring tmp;
      char t1[64];
      mult((char *)cvt(i),(char *)k.data.c_str(),(char *)t1);
      tmp=t1;
      return tmp;
      }

mstring operator*(string i, mstring k) {
      mstring tmp;
      char t1[64];
      mult((char *)i.c_str(),(char *)k.c_str(),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring operator*(global i, mstring k) {
      mstring tmp;
      char t1[64];
      string t2;
      i >> t2;
      mult((char *)t2.c_str(),(char *)k.c_str(),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring operator*(const char * i, mstring k) {
      mstring tmp;
      char t1[64];
      mult((char *)i,(char *)k.c_str(),(char *)t1);
      tmp = t1;
      return tmp;
      }

mstring operator/(int i, mstring k) {
      mstring tmp;
      char t1[64];
      divx(cvt(i),(char *)k.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator/(long i, mstring k) {
      mstring tmp;
      char t1[64];
      divx(cvt(i),(char *)k.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator/(double i, mstring k) {
      mstring tmp;
      char t1[64];
      divx(cvt(i),(char *)k.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator/(float i, mstring k) {
      mstring tmp;
      char t1[64];
      divx(cvt(i),(char *)k.c_str(),t1);
      tmp=t1;
      return tmp;
      }

mstring operator/(string i, mstring k) {
      mstring tmp;
      char t1[64];
      divx((char *)i.c_str(),(char *)k.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring operator/(global i, mstring k) {
      mstring tmp;
      char t1[64];
      string t2;
      i >> t2;
      divx((char *)t2.c_str(),(char *)k.c_str(),t1);
      tmp = t1;
      return tmp;
      }

mstring operator/(const char * i, mstring k) {
      mstring tmp;
      char t1[64];
      divx(i,(char *)k.c_str(),t1);
      tmp = t1;
      return tmp;
      }

bool mstring::operator<(int i) {
      long j=atol(data.c_str());

      if (j<i) return true;
      else return false;
      }

bool mstring::operator<(long i) {
      long j=atol(data.c_str());

      if (j<i) return true;
      else return false;
      }

bool mstring::operator<(double i) {
      double j=atof(data.c_str());

      if (j<i) return true;
      else return false;
      }

bool mstring::operator<(float i) {
      double j=atof(data.c_str());

      if (j<i) return true;
      else return false;
      }

bool mstring::operator<(char * i) {
      if (data<i) return true;
      else return false;
      }

bool mstring::operator<(const char * i) {
      if (data<i) return true;
      else return false;
      }

bool mstring::operator<(string i) {
      if (data<i) return true;
      else return false;
      }

bool mstring::operator<(global i) {
      string t1;
      i >> t1;

      if (data<t1) return true;
      else return false;
      }

bool mstring::operator<(mstring i) {
      if (data<i.data) return true;
      else return false;
      }

bool mstring::operator>(int i) {
      long j=atol(data.c_str());

      if (j>i) return true;
      else return false;
      }

bool mstring::operator>(long i) {
      long j=atol(data.c_str());

      if (j>i) return true;
      else return false;
      }

bool mstring::operator>(double i) {
      double j=atof(data.c_str());

      if (j>i) return true;
      else return false;
      }

bool mstring::operator>(float i) {
      double j=atof(data.c_str());

      if (j>i) return true;
      else return false;
      }

bool mstring::operator>(char * i) {
      if (data>i) return true;
      else return false;
      }

bool mstring::operator>(const char * i) {
      if (data>i) return true;
      else return false;
      }

bool mstring::operator>(string i) {
      if (data>i) return true;
      else return false;
      }

bool mstring::operator>(global i) {
      string t1;
      i >> t1;

      if (data>t1) return true;
      else return false;
      }

bool mstring::operator>(mstring i) {
      if (data>i.data) return true;
      else return false;
      }

bool mstring::operator>=(int i) {
      long j=atol(data.c_str());

      if (j>=i) return true;
      else return false;
      }

bool mstring::operator>=(long i) {
      long j=atol(data.c_str());

      if (j>=i) return true;
      else return false;
      }

bool mstring::operator>=(double i) {
      double j=atof(data.c_str());

      if (j>=i) return true;
      else return false;
      }

bool mstring::operator>=(float i) {
      double j=atof(data.c_str());

      if (j>=i) return true;
      else return false;
      }

bool mstring::operator>=(char * i) {
      if (data>=i) return true;
      else return false;
      }

bool mstring::operator>=(const char * i) {
      if (data>=i) return true;
      else return false;
      }

bool mstring::operator>=(string i) {
      if (data>=i) return true;
      else return false;
      }

bool mstring::operator>=(global i) {
      string t1;
      i >> t1;

      if (data>=t1) return true;
      else return false;
      }

bool mstring::operator>=(mstring i) {
      if (data>=i.data) return true;
      else return false;
      }

bool mstring::operator<=(int i) {
      long j=atol(data.c_str());

      if (j<=i) return true;
      else return false;
      }

bool mstring::operator<=(long i) {
      long j=atol(data.c_str());

      if (j<=i) return true;
      else return false;
      }

bool mstring::operator<=(double i) {
      double j=atof(data.c_str());

      if (j<=i) return true;
      else return false;
      }

bool mstring::operator<=(float i) {
      double j=atof(data.c_str());

      if (j<=i) return true;
      else return false;
      }

bool mstring::operator<=(char * i) {
      if (data<=i) return true;
      else return false;
      }

bool mstring::operator<=(const char * i) {
      if (data<=i) return true;
      else return false;
      }

bool mstring::operator<=(string i) {
      if (data<=i) return true;
      else return false;
      }

bool mstring::operator<=(global i) {
      string t1;
      i >> t1;

      if (data<=t1) return true;
      else return false;
      }

bool mstring::operator<=(mstring i) {
      if (data<=i.data) return true;
      else return false;
      }

bool mstring::operator==(int i) {
      long j=atol(data.c_str());

      if (j==i) return true;
      else return false;
      }

bool mstring::operator==(long i) {
      long j=atol(data.c_str());

      if (j==i) return true;
      else return false;
      }

bool mstring::operator==(double i) {
      double j=atof(data.c_str());

      if (j==i) return true;
      else return false;
      }

bool mstring::operator==(float i) {
      double j=atof(data.c_str());

      if (j==i) return true;
      else return false;
      }

bool mstring::operator==(const char * i) {
      if (data==i) return true;
      else return false;
      }

bool mstring::operator==(char * i) {
      if (data==i) return true;
      else return false;
      }

bool mstring::operator==(string i) {
      if (data==i) return true;
      else return false;
      }

bool mstring::operator==(global i) {
      string t1;
      i >> t1;

      if (data==t1) return true;
      else return false;
      }

bool mstring::operator==(mstring i) {
      if (data==i.data) return true;
      else return false;
      }

bool mstring::operator!=(int i) {
      long j=atol(data.c_str());

      if (j!=i) return true;
      else return false;
      }

bool mstring::operator!=(long i) {
      long j=atol(data.c_str());

      if (j!=i) return true;
      else return false;
      }

bool mstring::operator!=(double i) {
      double j=atof(data.c_str());

      if (j!=i) return true;
      else return false;
      }

bool mstring::operator!=(float i) {
      double j=atof(data.c_str());

      if (j!=i) return true;
      else return false;
      }

bool mstring::operator!=(char * i) {
      if (data!=i) return true;
      else return false;
      }

bool mstring::operator!=(const char * i) {
      if (data!=i) return true;
      else return false;
      }

bool mstring::operator!=(string i) {
      if (data!=i) return true;
      else return false;
      }

bool mstring::operator!=(global i) {
      string t1;
      i >> t1;

      if (data!=t1) return true;
      else return false;
      }

bool mstring::operator!=(mstring i) {
      if (data!=i.data) return true;
      else return false;
      }

mstring::operator const char *() {
      return data.c_str();
      }

mstring SymGet(mstring str) {
      char tmp[STR_MAX],*p1;
      mstring rslt;
      p1=sym_(11,(unsigned char *)str.c_str(),(unsigned char *)tmp,svPtr);

      if (p1==NULL) {
            GlobalExceptionCode=SYMTAB_ERROR;
            throw MumpsGlobalException();
            }

      rslt=tmp;
      return rslt;
      }

mstring SymGet(char * str) {
      char tmp[STR_MAX],*p1;
      mstring rslt;
      p1=sym_(11,(unsigned char *)str,(unsigned char *)tmp,svPtr);

      if (p1==NULL) {
            GlobalExceptionCode=SYMTAB_ERROR;
            throw MumpsGlobalException();
            }

      rslt=tmp;
      return rslt;
      }

mstring SymGet(global &str) {
      char tmp[STR_MAX],tmp2[STR_MAX],*p1;
      mstring rslt;
      str >> tmp2;
      p1=sym_(11,(unsigned char *)tmp2,(unsigned char *)tmp,svPtr);

      if (p1==NULL) {
            GlobalExceptionCode=SYMTAB_ERROR;
            throw MumpsGlobalException();
            }

      rslt=tmp;
      return rslt;
      }

bool SymPut(mstring &name, mstring &value) {
      __symput(name.c_str(),value.c_str());
      return true;
      }

bool SymPut(mstring &name, global &value) {
      char tmp[STR_MAX];
      value >> tmp;
      __symput(name.c_str(),tmp);
      return true;
      }

bool SymPut (mstring &name, char *value) {
      __symput(name.c_str(),value);
      return true;
      }

bool SymPut (char *name, mstring &value) {
      __symput(name,value.c_str());
      return true;
      }

bool SymPut (char *name, global &value) {
      char tmp[STR_MAX];
      value >> tmp;
      __symput(name,tmp);
      return true;
      }

bool SymPut (char *name, char *value) {
      __symput(name,value);
      return true;
      }

bool SymPut (global &name, char * value) {
      char tmp [STR_MAX];
      name >> tmp;
      __symput(tmp,value);
      return true;
      }

bool SymPut (global &name, mstring &value) {
      char tmp[STR_MAX];
      name >> tmp;
      __symput(tmp,value.c_str());
      return true;
      }

bool SymPut (global &name, global &value) {
      char tmp1[STR_MAX],tmp2[STR_MAX];
      name>>tmp1;
      value>> tmp2;
      __symput(tmp1,tmp2);
      return true;
      }

void  __symput(const char * str, const char * val) {
      char *p1;
      p1=sym_(0,(unsigned char *) str,(unsigned char *) val,svPtr);

      if (p1==NULL) {
            GlobalExceptionCode=SYMTAB_ERROR;
            throw MumpsGlobalException();
            }

      return;
      }

void global::Flush() {
      unsigned char t1[16]="",t2[16]="";
      Mglobal(FLUSH,t1,t2,svPtr);
      }

// BUILTIN FUNCTIONS

char * Replace (struct MSV* svPtr, char * ep, char *arg1, char * arg2, char *arg3) {
      mstring a,b,c;
      static char tmp[STR_MAX];
      a=arg1;
      b=arg2;
      c=arg3;

      if (a.replace(b,c)==1) {
            svPtr->tpx=1;
            strcpy(tmp,(char *)a.c_str());
            }
      else {
            svPtr->tpx=0;
            tmp[0]=0;
            }

      return tmp;
      }

char * Cosine(struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      x= Cosine(a(&arg1[i+1]),b(&arg2[j+1]));

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Input(struct MSV* svptr, char * ep, char * name1) {
      static char tmp[1024],scount[32];
      char line[2*STR_MAX];
      char name[256],tmp1[1024];
      int count=0,i,j=0;
      int off=0;

      svPtr->symflg = 2;       /* kill existing array */
      sym_ (2, (unsigned char *) name1, (unsigned char *) "",svPtr);

reread:

      if (fgets(line,STR_MAX,svPtr->in_file[svPtr->io])==NULL) return "0";

      if (line[0]=='\n') goto reread;

      while(1) {
            tmp[0]=0;
            if (sscanf(&line[off],"%1023s",tmp1)==0) {
                  sprintf(tmp,"%d",count); // number of words read
                  svptr->tpx=0;
                  return tmp;
                  }

            svptr->tpx=1;

            for (j=0,i=0; tmp1[i]!=0 && i < 1023; i++) if (tmp1[i]=='-' || !ispunct(tmp1[i])) tmp[j++]=tmp1[i];

            tmp[j]=0;
            tmp[1023]=0;

            if (tmp[0]==0) goto noword;;

            sprintf(scount,"%d",count);
            count++;
            strcpy(name,name1);
            strcatx((unsigned char *) name,(unsigned char *) "");
            strcatx((unsigned char *) name,(unsigned char *) scount);  // adds x01s
            svPtr->symflg = 0;           /* store */
            sym_ (0, (unsigned char *) name, (unsigned char *) tmp, svPtr);
noword:

            while(line[off]!=0 && line[off]!=' ' && line[off]!='\t') off++;

            if (line[off]==0) {
                  sprintf(tmp,"%d",count); // number of words read
                  svptr->tpx=0;
                  return tmp;
                  }

            off++;
            }
      }

char * Scan(struct MSV* svptr) {
      static char tmp[1024];
      int i, j = 0;

            if ( (i = fscanf(svptr->in_file[svptr->io], "%1024s", tmp)) == EOF) {
                  tmp[0] = 0;
                  svptr->tpx = 0;
                  return tmp;
                  }

		if (i >= 1023) { // word too long
			GlobalExceptionCode=INPUT_ERROR;
			throw MumpsGlobalException();
			}

            svptr->tpx=1;

            tmp[1023] = 0;

            return tmp;
      }

mstring ScanAlnum(FILE *u, int min, int max) {

      char word[8192],tmp1[8192];
      mstring tmp;
      int j=0;

repeat:

      if (fscanf(u,"%8191s",word)==EOF || max > 511) {
            tmp="";
            return tmp;
            }

      j=0;

      for (int i=0; word[i]; i++) {
            if (isalnum(word[i])) tmp1[j++]=tolower(word[i]);
            }

      if ( j == 0  || j < min  || j > max || isdigit(tmp1[0]) ) goto repeat;

      tmp1[j]=0;
      tmp=tmp1;
      return tmp;
      }

mstring ScanAlnum(istream u, int min, int max) {

      char word[8192],tmp1[8192];
      mstring tmp;
      int j=0;

repeat:

      if (! (u >> word) || max > 511) {
            tmp="";
            return tmp;
            }

      j=0;

      for (int i=0; word[i]; i++) {
            if (isalnum(word[i])) tmp1[j++]=tolower(word[i]);
            }

      if ( j == 0  || j < min  || j > max || isdigit(tmp1[0]) ) goto repeat;

      tmp1[j]=0;
      tmp=tmp1;
      return tmp;
      }

char * ScanAlnum(struct MSV* svptr, char * ep, char * toklist = NULL) {

      static char tmp[1024], tmp1[1024];
      int i, j, k;

again: strcpy(tmp1, Scan(svptr));

	if (strlen(tmp1) == 0) {
		svptr->tpx = 0;
		return tmp1;
		}

	if(!isalnum(tmp1[0])) goto again;

	j = 0;

	for (i = 0; i < strlen(tmp1); i++) {

		if (isalnum(tmp1[i])) tmp[j++] = tolower(tmp1[i]);

		}

	tmp[j] = 0;
	svptr->tpx = 1;

	return tmp;

      }

char * Count(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      double x;
      static char tmp[32];

      i = strlen(arg1);

      if (i> 0 && arg1[i-1]==1 && arg1[i-2]==1) arg1[i-2]=0;

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      j = arg1[i];
      arg1[i]=0;

      global a(&arg1[1]); // bypass ^

      if (!j) {
            x = a("").Count();
            }
      else x=a(&arg1[i+1]).Count();

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Max(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      double x;
      static char tmp[32];

      i = strlen(arg1);

      if (i> 0 && arg1[i-1]==1 && arg1[i-2]==1) arg1[i-2]=0;

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      j = arg1[i];
      arg1[i]=0;

      global a(&arg1[1]); // bypass ^

      if (!j) {
            x = a("").Max();
            }
      else x=a(&arg1[i+1]).Max();

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Multiply(struct MSV* svptr, char * ep, char * arg1, char * arg2, char * arg3) {

      int i,j,k;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      if (arg1[i] == 1) {
            printf("\n*** Arg 1 must be top level global\n");
            sigint(100);
            }

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      if (arg2[i] == 1) {
            printf("\n*** Arg 2 must be top level global\n");
            sigint(100);
            }

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      for (k=0; arg3[k]!=0; k++) if (arg3[k]==1) break;

      if (arg3[i] == 1) {
            printf("\n*** Arg 3 must be top level global\n");
            sigint(100);
            }

      arg3[k]=0;
      global c(&arg3[1]); // bypass ^

      a(&arg1[i+1]).Multiply(b(&arg2[j+1]),c(&arg3[k+1]));

      return (char *) "";
      }

char * Avg(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      double x;
      static char tmp[32];

      i = strlen(arg1);

      if (i> 0 && arg1[i-1]==1 && arg1[i-2]==1) arg1[i-2]=0;

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      j = arg1[i];

      arg1[i]=0;

      global a(&arg1[1]); // bypass ^

      if (!j) {
            x = a("").Avg();
            }
      else x=a(&arg1[i+1]).Avg();

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Min(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      double x;
      static char tmp[32];

      i = strlen(arg1);

      if (i> 0 && arg1[i-1]==1 && arg1[i-2]==1) arg1[i-2]=0;

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      j = arg1[i];

      arg1[i]=0;

      global a(&arg1[1]); // bypass ^

      if (!j) {
            x = a("").Min();
            }
      else x=a(&arg1[i+1]).Min();

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Sum(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      double x;
      static char tmp[32];

      i = strlen(arg1);

      if (i> 0 && arg1[i-1]==1 && arg1[i-2]==1) arg1[i-2]=0;

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      j = arg1[i];

      arg1[i]=0;

      global a(&arg1[1]); // bypass ^

      if (!j) {
            x = a("").Sum();
            }
      else x=a(&arg1[i+1]).Sum();

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Transpose(struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      if (arg1[i]==0) arg1[i+1]=0; // array name only - no indices

      if (arg1[i] == 1) {
            printf("\n*** Arg 1 must be top level global\n");
            sigint(100);
            }

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      if (arg2[j]==0) arg2[j+1]=0; // array name only - no indices

      if (arg2[i] == 1) {
            printf("\n*** Arg 2 must be top level global\n");
            sigint(100);
            }

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      a(&arg1[i+1]).Transpose(b(&arg2[j+1]));

      return (char *) "";
      }

char * SmithWaterman (struct MSV *, char * ep, char * s1, char *s2, char *sa, char * sm, char * gap,
                      char * mm, char * ma) {

      int sax,smx,gapx,mmx,max;
      char s1a[STR_MAX],s2a[STR_MAX];
      static char tmp[32];
      s1a[0]=s2a[0]=' ';
      strcpy(&s1a[1],s1);
      strcpy(&s2a[1],s2);
      sscanf(sa,"%d",&sax);
      sscanf(sm,"%d",&smx);
      sscanf(gap,"%d",&gapx);
      sscanf(mm,"%d",&mmx);
      sscanf(ma,"%d",&max);
      sax=sw(s1a,s2a,sax,smx,gapx,mmx,max);
      sprintf(tmp,"%d",sax);
      return tmp;
      }

char * IDF (struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      if (arg1[i] == 1) {
            printf("\n*** Arg 1 must be top level global\n");
            sigint(100);
            }

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^
      sscanf(arg2,"%lf",&x);
      a(&arg1[i+1]).IDF(x);
      return (char *) "";
      }

char * TermCorrelate (struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      a(&arg1[i+1]).TermCorrelate(b(&arg2[j+1]));
      return (char *) "";
      }

char * DocCorrelate (struct MSV* svptr, char * ep, char * arg1, char * arg2, char * arg3, char * arg4) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      sscanf(arg4,"%lf",&x);

      a(&arg1[i+1]).DocCorrelate(b(&arg2[j+1]),arg3,x);
      return (char *) "";
      }

char * Sim1(struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      x= Sim1(a(&arg1[i+1]),b(&arg2[j+1]));

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Jaccard(struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      x= Jaccard(a(&arg1[i+1]),b(&arg2[j+1]));

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * Dice(struct MSV* svptr, char * ep, char * arg1, char * arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      x=Dice(a(&arg1[i+1]),b(&arg2[j+1]));

      sprintf(tmp,"%g",x);
      return tmp;
      }

char * StopInit(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      StopInit(arg1);
      return (char *) "";
      }

char * StopLookup(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;

      if (StopLookup(arg1)) return (char *) "1";

      return (char *) "0";
      }

char * SynInit(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      SynInit(arg1);
      return (char *) "";
      }

char * SynLookup(struct MSV* svptr, char * ep, char * arg1) {

      int i,j;
      return SYN(arg1);
      }

char * Centroid (struct MSV*, char * ep, char *arg1, char *arg2) {

      int i,j;
      double x;
      static char tmp[32];

      for (i=0; arg1[i]!=0; i++) if (arg1[i]==1) break;

      arg1[i]=0;
      global a(&arg1[1]); // bypass ^

      for (j=0; arg2[j]!=0; j++) if (arg2[j]==1) break;

      arg2[j]=0;
      global b(&arg2[1]); // bypass ^

      a(&arg1[i+1]).Centroid(b(&arg2[j+1]));

      return (char *) "";
      }

char * BMGSearch (struct MSV*, char * ep, char *arg1, char *arg2) {

      int i,j;
      double x;
      static char tmp[32];
      char *p1=(char *)malloc(strlen(arg2+2));
      strcpy(p1,arg2);
      strcat(p1," ");

      i=bmg_fullsearch(arg1,p1);
      sprintf(tmp,"%d",i);

      return tmp;
      }

char * Shred (struct MSV*, char * ep, char *arg1, char *arg2) {

      int i;

      i=atoi(arg2);
      return Shred(arg1,i);
      }

char * ShredQuery (struct MSV* svPtr, char * ep, char *arg1, char *arg2) {

      int i;

      i=atoi(arg2);
      return ShredQuery(arg1,i);
      }

