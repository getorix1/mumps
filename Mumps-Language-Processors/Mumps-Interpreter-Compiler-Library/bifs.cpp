/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2004, 2008, 2012, 2015, 2017, 2022
#+     by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     http://www.cs.uni.edu/~okane
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

// June 9, 2024

// astyle --style=banner

/* bifs.c - Mumps runtime library
 *
 * Mumps Built-In FunctionS
 * ========================
 * The majority of Mumps builtins are translated to calls to the
 * following functions, whose names look like the function implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <mumpsc/defines.h>
#include <mumpsc/fcns.h>
#include <mumpsc/global.h>
#include <mumpsc/builtin.h>
#include <mumpsc/arith.h>
#include <mumpsc/inline.h>

char *sym_(int symflg, unsigned char *a, unsigned char *b, struct MSV * svPtr);

int wstem(char *, int);
void sigint(int);

#define NATIVE

/* Used by zfcn */
#define CLOSE 207
#define COMMA 208

#define NO 0
#define YES 1

/* gcvt(value, numdigits, string) */
#ifdef GCVT
#define gcvt_wrapper(x,i,a) sprintf(a, "%g", x)
#else
#define gcvt_wrapper(x,i,a) gcvt(x, (size_t) i, (char *) a)
#endif

/* TODO: Extern globals prevent compiling this into a dynamic lib. */
// extern const char * (*__text_function)(int);  /* fcn */


void	GtkTreeAdd(GtkTreeIter *itera, GtkTreeIter *iterb, unsigned char * treestore,
                 unsigned char * v1d, short int * iargs, short int nargs) {

//------------------------------------------------------
//	v1d[1]		-> treestore (pointer)
//	v1d[iargs[1]]	-> depth in tree (integer)
//------------------------------------------------------

      static GtkTreeStore *t;

      sscanf((const char *) treestore, "%p", &t); // get GtkTreeStore ptr

      gtk_tree_store_append (t, itera, iterb);

      gtk_tree_store_set(t, itera, 0, &v1d[iargs[2]], -1); // column 0

      if (nargs == 3) return;

      gtk_tree_store_set(t, itera, 1, &v1d[iargs[3]], -1); // column 1

      if (nargs == 4) return;

      gtk_tree_store_set(t, itera, 2, &v1d[iargs[4]], -1); // column 2

      if (nargs == 5) return;

      gtk_tree_store_set(t, itera, 3, &v1d[iargs[5]], -1); // column 3

      if (nargs == 6) return;

      gtk_tree_store_set(t, itera, 4, &v1d[iargs[6]], -1); // column 4

      if (nargs == 7) return;

      gtk_tree_store_set(t, itera, 5, &v1d[iargs[7]], -1); // column 5

      if (nargs == 8) return;

      gtk_tree_store_set(t, itera, 6, &v1d[iargs[8]], -1); // column 6

      if (nargs == 9) return;

      gtk_tree_store_set(t, itera, 7, &v1d[iargs[9]], -1); // column 6

      if (nargs == 10) return;

      gtk_tree_store_set(t, itera, 8, &v1d[iargs[10]], -1); // column 7

      if (nargs == 10) return;

      gtk_tree_store_set(t, itera, 9, &v1d[iargs[11]], -1); // column 8

      if (nargs == 11) return;

      gtk_tree_store_set(t, itera, 10, &v1d[iargs[12]], -1); // column 9

      if (nargs == 12) return;

      gtk_tree_store_set(t, itera, 11, &v1d[iargs[13]], -1); // column 10

      if (nargs == 13) return;

      }


/*===========================================================================*
 *                                   ZFCN                                    *
 *===========================================================================*/

void zfcn(unsigned char v1d[], unsigned char bd[], struct MSV * svPtr) {

      long int fd;
      time_t dtime, timex;
      static char linebuf[STR_MAX];
      static int linebufx;
      int rand();
      FILE *dump;
      char zcode, zcode1, zcode2, zcode3;
      float sq;
      unsigned char tmp2[STR_MAX], tmp3[STR_MAX], tmp4[STR_MAX], gbltbl[128],
               gblstr[STR_MAX];
      int j, k, x;
      short int iargs[10], nargs;
      int WordTrunc=25;
      char fcnName[64]="";
      long i;
      short g;
      double rt;

      static GtkTreeIter iter;
      static GtkTreeIter iter1;
      static GtkTreeIter iter2;
      static GtkTreeIter iter3;
      static GtkTreeIter iter4;
      static GtkTreeIter iter5;
      static GtkTreeIter iter6;
      static GtkTreeIter iter7;
      static GtkTreeIter iter8;
      static GtkTreeIter iter9;
      static GtkTreeIter iter10;
      static GtkTreeStore *t;
      static GtkSpinButton *spin;
      static int column = 0;

#if defined(SQLITE)
      int sql(int, struct MSV * svPtr, char *, char *, char *, const char *);
#endif

      svPtr->ierr = nargs = 0;
      iargs[0] = 1;
      zcode = tolower(v1d[3]);
      zcode1 = tolower(v1d[4]);
      zcode2 = tolower(v1d[5]);
      zcode3 = tolower(v1d[6]);

      for (i=1; v1d[i]!=OPEN && v1d[i]!=0; i++);

      strncpy( (char *) fcnName, (const char *) &v1d[2],i-2);

      while (v1d[1] != OPEN && v1d[1] != 0)
            strmove(&v1d[1], &v1d[2]);

      if (zcode == 'g' && (zcode1 == 'a' || zcode1 == 'c' || zcode1 == 's')) {    /* zglobal */
            strmove(&v1d[1], &v1d[2]);
            for (i = 1; v1d[i] != 0; i++);
            if (v1d[i - 1] != CLOSE || v1d[i - 2] != CLOSE) goto err;
            v1d[i - 1] = 0;
            }

      else {

            if (v1d[1] == OPEN)
                  strmove(&v1d[1], &v1d[2]);

            for (i = 1; v1d[i] != 0; i++)   /* locate arguments */
                  if (v1d[i] == CLOSE || v1d[i] == COMMA) {
                        v1d[i] = 0;
                        iargs[++nargs] = i + 1;
                        }
            }

      svPtr->ierr = 0;

//------------------------------------------------------
//	Branch table - go to appropriate fcn handler
//------------------------------------------------------

//--------------
//	GTK
//--------------

      if (strncasecmp(fcnName, "z~gtk", 5) == 0 || strncasecmp(fcnName, "z~mdh", 5) == 0 ) {

            if (strncasecmp(fcnName,"z~mdh~file~chooser~get~filename",31)==0) goto GtkFileChooser;
            if (strncasecmp(fcnName,"z~mdh~calendar~get~date",23)==0) goto GtkCalendar;
            if (strncasecmp(fcnName,"z~mdh~range~get~adjustment",26)==0) goto GtkGetRangeAdjustment;
            if (strncasecmp(fcnName,"z~mdh~font~chooser~get~font",27)==0) goto GtkFontChooser;
            if (strncasecmp(fcnName,"z~mdh~color~chooser~get~rgba",28)==0) goto GtkColorChooser;
            if (strncasecmp(fcnName,"z~mdh~switch~get~active",23)==0) goto GtkSwitchGetActive;
            if (strncasecmp(fcnName,"z~mdh~toggle~button~get~active",30)==0) goto GtkToggleButtonGetActive;
            if (strncasecmp(fcnName,"z~mdh~toggle~button~set~active",30)==0) goto GtkToggleButtonSetActive;
            if (strncasecmp(fcnName,"z~mdh~spin~button~set~value",27)==0) goto GtkSpinButtonSetValue;
            if (strncasecmp(fcnName,"z~mdh~entry~get~text",20)==0) goto GtkEntryGetText;
            if (strncasecmp(fcnName,"z~mdh~entry~set~text",20)==0) goto GtkEntrySetText;
            if (strncasecmp(fcnName,"z~mdh~text~buffer~set~text",22)==0) goto GtkTextBufferSetText;
            if (strncasecmp(fcnName,"z~mdh~label~set~text",20)==0) goto GtkLabelSetText;
            if (strncasecmp(fcnName,"z~mdh~dialog~new~with~buttons",29)==0) goto GtkDialogWithButtons;
            if (strncasecmp(fcnName,"z~mdh~tree~view~get~selection",29)==0) goto GtkTreeViewGetSelection;
            if (strncasecmp(fcnName,"z~mdh~tree~selection~get~selected",33)==0) goto GtkTreeSelectionGetSelected;
            if (strncasecmp(fcnName,"z~mdh~tree~model~get",20)==0) goto GtkTreeModelGet;
            if (strncasecmp(fcnName,"z~mdh~tree~store~clear",22)==0) goto GtkTreeStoreClear;
            if (strncasecmp(fcnName,"z~mdh~tree~level~add",20)==0) goto GtkTreeLevelAdd;
            if (strncasecmp(fcnName,"z~mdh~spin~button~get~value",27)==0) goto GtkSpinButtonGetValue;
            if (strncasecmp(fcnName,"z~mdh~widget~show",17)==0) goto GtkWidgetShow;
            if (strncasecmp(fcnName,"z~mdh~widget~hide",17)==0) goto GtkWidgetHide;
            goto fcn_not_found;
            }

      if (strncasecmp(fcnName,"zz",2)==0) {
            if (strcasecmp(fcnName,"zzsoundex")==0) goto zzsoundex; /* $zzSoundex */
            if (strcasecmp(fcnName,"zzcosine")==0) goto zzcosine; /* $zzcosine */
            if (strcasecmp(fcnName,"zzinput")==0) goto zzinput; /* $zzInput */
            if (strcasecmp(fcnName,"zzdice")==0) goto zzdice; /* $zzdice */
            if (strcasecmp(fcnName,"zzjaccard")==0) goto zzjaccard; /* $zzJaccard */
            if (strcasecmp(fcnName,"zzsim1")==0) goto zzsim1; /* $zzSim1 */
            if (strcasecmp(fcnName,"zzscanalnum")==0) goto zzscanalnum; /* $zzscanalnum */
            if (strcasecmp(fcnName,"zzscan")==0) goto zzscan; /* $zzScan */
            if (strcasecmp(fcnName,"zzavg")==0) goto zzavg; /* $zzavg */
            if (strcasecmp(fcnName,"zzidf")==0) goto zzidf; /* $zzIDF */
            if (strcasecmp(fcnName,"zztranspose")==0) goto zztranspose; /* $zzTranspose */
            if (strcasecmp(fcnName,"zzcentroid")==0) goto zzcentroid; /* $zzCentroid */
            if (strcasecmp(fcnName,"zztermcorrelate")==0) goto zztermcorrelate; /* $zzTermCorrelate */
            if (strcasecmp(fcnName,"zzdoccorrelate")==0) goto zzdoccorrelate; /* $zzDocCorrelate */
            if (strcasecmp(fcnName,"zzcount")==0) goto zzcount; /* $zzCount */
            if (strcasecmp(fcnName,"zzmax")==0) goto zzmax; /* $zzMax */
            if (strcasecmp(fcnName,"zzmin")==0) goto zzmin; /* $zzMin */
            if (strcasecmp(fcnName,"zzsum")==0) goto zzsum; /* $zzSum */
            if (strcasecmp(fcnName,"zzmultiply")==0) goto zzmultiply; /* $zzMultiply */
            goto fcn_not_found;
            }

      if (strncasecmp(fcnName,"zs",2)==0) {
            if (strcasecmp(fcnName,"zstem")==0) goto zstem; /* $zstem */
            if (strncasecmp(fcnName,"zstopl",6)==0) goto zstoplookup; /* $zStopLookup */
            if (strcasecmp(fcnName,"zsin")==0) goto zsine; /* $zsin() */
            if (strcasecmp(fcnName,"zseek")==0) goto zseek; /* $zseek */
            if (strcasecmp(fcnName,"zsmithwaterman")==0) goto zsmithwaterman; /* $zsmithwaterman */
            if (strcasecmp(fcnName,"zstopinit")==0) goto zstopinit; /* $zStopInit */
            if (strcasecmp(fcnName,"zsyninit")==0) goto zsyninit; /* $zSynInit */
            if (strcasecmp(fcnName,"zsynlookup")==0) goto zsynlookup; /* $zSynLookup */
            if (strcasecmp(fcnName,"zshred")==0) goto zshred; /* $zShred */
            if (strcasecmp(fcnName,"zshredquery")==0) goto zshredquery; /* $zShredQuery */
            if (strcasecmp(fcnName,"zsrand")==0) goto zsrand; /* $zsrand */
            if (strcasecmp(fcnName,"zsqlite")==0) goto zsqlite; /* $zsqlite */
            if (strcasecmp(fcnName,"zsqlopen")==0) goto zsqlOpen; /* $zsqlOpen */
            if (strcasecmp(fcnName,"zsqrt")==0) goto zroot; /* $zsqrt */
            if (strcasecmp(fcnName,"zsystem")==0) goto zsystem; /* $zsystem */
            goto fcn_not_found;
            }

      if (strncasecmp(fcnName,"zlow",4)==0) goto zlower;
      if (strcasecmp(fcnName,"zcos")==0) goto zcosine;
      if (strcasecmp(fcnName,"zgetenv")==0) goto zgetenv;
      if (strcasecmp(fcnName,"ztan")==0) goto ztangent;
      if (strcasecmp(fcnName,"zatan")==0) goto zatangent; /* $zatan() */
      if (strcasecmp(fcnName,"zacos")==0) goto zacosine; /* $zacos() */
      if (strcasecmp(fcnName,"zasin")==0) goto zasine; /* $zasin() */
      if (strcasecmp(fcnName,"zexp")==0) goto zexp; /* $zexp() */
      if (strcasecmp(fcnName,"zexp2")==0) goto zexp2; /* $zexp() */
      if (strcasecmp(fcnName,"zexp10")==0) goto zexp10; /* $zexp() */
      if (strcasecmp(fcnName,"zlog")==0) goto zlog; /* $zlog() */
      if (strcasecmp(fcnName,"zlog10")==0) goto zlog10; /* $zlog10() */
      if (strcasecmp(fcnName,"zlog2")==0) goto zlog2; /* $zlog2() */
      if (strcasecmp(fcnName,"zpow")==0) goto zpow; /* $zpow() */
      if (strcasecmp(fcnName,"zhit")==0) goto zhit; /* $hit() */
      if (strcasecmp(fcnName,"zchdir")==0) goto zchdir; /* $zchdir() */
      if (strcasecmp(fcnName,"zflush")==0) goto zflush; /* $zflush */
      if (strcasecmp(fcnName,"znoblanks")==0) goto znoblanks; /* $znoblanks */
      if (strcasecmp(fcnName,"zpad")==0) goto zpad; /* $zpad */
      if (strcasecmp(fcnName,"zbmgsearch")==0) goto zbmgsearch; /* $zbmgSearch */
      if (strcasecmp(fcnName,"zperlmatch")==0) goto zperlmatch; /* $zPerlMatch */
      if (strcasecmp(fcnName,"zreplace")==0) goto zreplace; /* $zReplace */
      if (strcasecmp(fcnName,"znative")==0) goto znative; /* $znative */
      if (strcasecmp(fcnName,"zdump")==0) goto zcondense; /* $zdump */
      if (strcasecmp(fcnName,"zrestore")==0) goto zrestore; /* $zrestore */
      if (strcasecmp(fcnName,"zcurrentfile")==0) goto zcurrentfile; /* $zcurrentfile */
      if (strcasecmp(fcnName,"zceil")==0) goto zceil; /* $zceil */
      if (strcasecmp(fcnName,"zfloor")==0) goto zfloor; /* $zfloor */
      if (strcasecmp(fcnName,"ztrunc")==0) goto ztrunc; /* $ztrunc */
      if (strcasecmp(fcnName,"zprogram")==0) goto zprogram; /* $zprogram */
      if (strcasecmp(fcnName,"zbasename")==0) goto zbasename; /* $zbasename */
      if (strcasecmp(fcnName,"zfiletest")==0) goto zfiletest; /* $zfiletest */

      if (zcode == 'a') {
            if (zcode1 == 'b') goto zabs;
            goto zalter;
            }

      if (zcode == 'd') goto zdate;
      if (zcode == 'v') goto z_variable;
      if (zcode == 'n') goto znormal;
      if (zcode == 'b') goto zblnk; // $zblanks()
      if (zcode == 'p') goto zpad;
      if (zcode == 'h') goto zhtml;
      if (zcode == 'f') goto zfile;
      if (zcode == 'g') goto zglobal;
      if (zcode == 'l') goto zlog;
      if (zcode == 'm') goto znxtrow;
      if (zcode == 'w') goto zword;
      if (zcode == 't') goto ztell;
      if (zcode == 'u') goto znumeric;

      goto fcn_not_found;

//------------------------
//	GTK interface
//------------------------

GtkDialogWithButtons:

      if (nargs != 2 ) goto err;

            {

            GtkWindow *window;

            sscanf((const char *) &v1d[1], "%p", &window);

            GtkDialogFlags flags = (GtkDialogFlags) (GTK_DIALOG_DESTROY_WITH_PARENT |
                                   GTK_MESSAGE_ERROR | GTK_DIALOG_MODAL |
                                   GTK_DIALOG_USE_HEADER_BAR) ;

            GtkWidget *dialog = gtk_dialog_new_with_buttons  ( (char *) &v1d[iargs[1]],
                                GTK_WINDOW(window),
                                flags,
                                "Yes", 100, "No", 200, NULL);

//--------------------------------------------
//      Expect 200 for NO and 100 for YES
//--------------------------------------------

            gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);

            gint r = gtk_dialog_run (GTK_DIALOG (dialog));

            gtk_widget_destroy (dialog);

            if (r == 100) strcpy( (char *) bd, "1");
            else if (r == 200)  strcpy( (char *) bd, "0");
            else strcpy( (char *) bd, "-1");

            return;

            }

GtkWidgetHide:

            {
            GtkWidget *widget;
            sscanf((const char *) &v1d[1], "%p", &widget);
            gtk_widget_hide(GTK_WIDGET(widget));
            bd[0] = 0;
            return;
            }

GtkWidgetShow:

            {
            GtkWidget *widget;
            sscanf((const char *) &v1d[1], "%p", &widget);
            gtk_widget_show(GTK_WIDGET(widget));
            bd[0] = 0;
            return;
            }

GtkSpinButtonGetValue:

      if (nargs != 1 ) goto err;

            {
            sscanf((const char *) &v1d[1], "%p", &spin);
            gdouble x = gtk_spin_button_get_value (spin);
            sprintf((char *) bd, "%f", x);
            return;
            }

GtkSpinButtonSetValue:

      if (nargs != 2 ) goto err;

            {
            double val;
            sscanf((const char *) &v1d[1], "%p", &spin);
            val = atof( (char *) &v1d[iargs[1]]);
            gtk_spin_button_set_value (spin, val);
            bd[0] = 0;
            return;
            }


GtkTreeStoreClear:

      if (nargs != 1 ) goto err;

            {
            sscanf((const char *) &v1d[1], "%p", &t);
            gtk_tree_store_clear (t);
            strcpy((char *) bd, "1");
            return;
            }


GtkTreeModelGet:

      if (nargs != 5) goto err;

            {
            GtkTreeModel *m;
            gchar *value;
            sscanf((const char *) &v1d[1], "%p", &m);
            sscanf((const char *) &v1d[iargs[1]], "%p", &iter);

//-----------------------------------------------------
//	one col number / value pair
//	can be called multiple times for sep cols
//-----------------------------------------------------

            gtk_tree_model_get(m, &iter, 1, &value,  -1);
            strcpy((char *) bd, value);
            return;
            }

//-----------------------------------------
//	Tree/Table row selection signal
//-----------------------------------------

GtkTreeSelectionGetSelected:

      if (nargs != 2 ) goto err;

            {
            GtkTreeSelection *s;
            GtkTreeModel *m;
            gchar *value;
            int col;

            sscanf((const char *) &v1d[1], "%p", &s);
            sscanf((const char *) &v1d[iargs[1]], "%d", &col);

            if (gtk_tree_selection_get_selected (s, &m, &iter) == FALSE) {
                  strcpy((char *) bd, "0");
                  return;
                  }

            bd[0] = 0;

            gtk_tree_model_get(m, &iter, col, &value,  -1);
            strcpy((char *) bd, value);
            return;
            }

GtkTreeViewGetSelection:

      if (nargs != 1 ) goto err;

            {
            GtkTreeView *t;
            GtkTreeSelection *s;
            sscanf((const char *) &v1d[1], "%p", &t);
            s = gtk_tree_view_get_selection(t);
            sprintf((char *) bd, "%p", s);
            return;
            }

GtkTreeLevelAdd:

      if (nargs < 4 ) goto err;

      int depth;

      sscanf((const char *) &v1d[iargs[1]], "%d", &depth);

      if (depth == 1) goto GtkTreeLevel1Add;
      if (depth == 2) goto GtkTreeLevel2Add;
      if (depth == 3) goto GtkTreeLevel3Add;
      if (depth == 4) goto GtkTreeLevel4Add;
      if (depth == 5) goto GtkTreeLevel5Add;
      if (depth == 6) goto GtkTreeLevel6Add;
      if (depth == 7) goto GtkTreeLevel7Add;
      if (depth == 8) goto GtkTreeLevel8Add;
      if (depth == 9) goto GtkTreeLevel9Add;
      if (depth == 10) goto GtkTreeLevel10Add;

//------------------------------------------------------------
//	v1d[1]		-> treestore
//	v1d[iargs[1]]	-> depth in tree for 1st column
//------------------------------------------------------------

GtkTreeLevel1Add:

            {
            GtkTreeAdd(&iter, NULL, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel2Add:

            {
            GtkTreeAdd(&iter1, &iter, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel3Add:

            {
            GtkTreeAdd(&iter2, &iter1, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel4Add:

            {
            GtkTreeAdd(&iter3, &iter2, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel5Add:

            {
            GtkTreeAdd(&iter4, &iter3, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel6Add:

            {
            GtkTreeAdd(&iter5, &iter4, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel7Add:

            {
            GtkTreeAdd(&iter6, &iter5, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel8Add:

            {
            GtkTreeAdd(&iter7, &iter6, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel9Add:

            {
            GtkTreeAdd(&iter8, &iter7, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }

GtkTreeLevel10Add:

            {
            GtkTreeAdd(&iter9, &iter8, &v1d[1], v1d, iargs, nargs);
            strcpy((char *) bd, "0");
            return;
            }


GtkLabelSetText:

      if (nargs != 2 ) goto err;

            {
            GtkLabel *l;
            sscanf((const char *) &v1d[1], "%p", &l);
            gtk_label_set_text (l, (const gchar *) &v1d[iargs[1]]);
            strcpy((char *) bd, "1");
            return;
            }

GtkTextBufferSetText:

      if (nargs != 2 ) goto err;

            {
            GtkTextBuffer *b;
            sscanf((const char *) &v1d[1], "%p", &b);
            gtk_text_buffer_set_text (b, (const char *) &v1d[iargs[1]], (gint) -1);
            strcpy((char *) bd, "1");
            return;
            }

GtkEntryGetText:

      if (nargs != 1 ) goto err;

            {
            GtkEntry *e;
            sscanf((const char *) &v1d[1], "%p", &e);
            strcpy((char *) bd, gtk_entry_get_text(e));
            return;
            }

GtkEntrySetText:

      if (nargs != 2 ) goto err;

            {
            GtkEntry *e;
            sscanf((const char *) &v1d[1], "%p", &e);
            gtk_entry_set_text(e, (char *) &v1d[iargs[1]] );
            strcpy((char *) bd, "0");
            return;
            }

GtkGetRangeAdjustment:

      if (nargs != 1 ) goto err;

            {
            GtkRange *e;
            sscanf((const char *) &v1d[1], "%p", &e);
            GtkAdjustment *adj = gtk_range_get_adjustment ( (GtkRange*) e);
            sprintf((char *) bd, "%f", gtk_adjustment_get_value(adj));
            return;
            }

GtkFileChooser:
      if (nargs != 1 ) goto err;

            {
            static char tmp[4096];
            GtkFileChooser *e;
            sscanf((const char *) &v1d[1], "%p", &e);
            strncpy((char *) bd, gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(e)), STR_MAX);
            return;
            }

GtkCalendar:

      if (nargs != 1 ) goto err;

            {
            GtkCalendar *e;
            sscanf((const char *) &v1d[1], "%p", &e);
            guint mn, dy, yr;
            gtk_calendar_get_date ((GtkCalendar *) e, &yr, &mn, &dy);
            sprintf((char *) bd, "%d %d %d", yr, mn, dy);
            return;
            }

GtkFontChooser:

      if (nargs != 1 ) goto err;

            {
            GtkFontChooser *e;
            sscanf((const char *) &v1d[1], "%p", &e);
            sprintf((char *) bd, "%s",gtk_font_chooser_get_font(e));
            return;
            }

GtkColorChooser:

      if (nargs != 1 ) goto err;

            {
            GtkColorChooser *c;
            sscanf((const char *) &v1d[1], "%p", &c);
            GdkRGBA r;
            gtk_color_chooser_get_rgba (c, &r);
            sprintf((char *) bd, "%f %f %f %f", r.red, r.green, r.blue, r.alpha );
            return;
            }


GtkToggleButtonSetActive:

      if (nargs != 2 ) goto err;

            {
            GtkToggleButton *t;
            sscanf((const char *) &v1d[1], "%p", &t);

            if (strcmp((char *) &v1d[iargs[1]], "TRUE") == 0) {
                  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(t), TRUE);
                  strcpy((char *) bd, "1");
                  return;
                  }

            if (strcmp((char *) &v1d[iargs[1]], "FALSE") == 0) {
                  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(t), FALSE);
                  strcpy((char *) bd, "1");
                  return;
                  }

            }

      strcpy((char *) bd, "0");
      goto err;


GtkToggleButtonGetActive:

      if (nargs != 1 ) goto err;

            {
            GtkToggleButton *t;
            sscanf((const char *) &v1d[1], "%p", &t);
            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(t)))
                  strcpy((char *) bd, "1");
            else
                  strcpy((char *) bd, "0");
            return;
            }

GtkSwitchGetActive:

      if (nargs != 1 ) goto err;

            {
            GtkSwitch *t;
            sscanf((const char *) &v1d[1], "%p", &t);
            if (gtk_switch_get_active(GTK_SWITCH(t)))
                  strcpy((char *) bd, "1");
            else
                  strcpy((char *) bd, "0");
            return;
            }


zprogram:

      if (nargs != 0 ) goto err;
      strcpy ((char *) bd, svPtr->program);
      return;

zfiletest:

      if (nargs != 2) goto err;

      else {

            FILE *f1;
            char tmp[1024] = "echo `if [ ";

            strncat(tmp, (const char *) &v1d[iargs[1]], 500);
            strcat(tmp," \"");
            strncat(tmp, (const char *) &v1d[1], 500);
            strcat(tmp, "\" ]; then echo \"1\"; fi`");

            f1 = popen(tmp,"r");

            strcpy(tmp,"");
            fgets(tmp, 1000, f1);
            tmp[strlen(tmp)-1]=0;
            if (strcmp(tmp,"1")==0) strcpy( (char *) bd, "1");
            else strcpy( (char *) bd, "0");
            return;
            }

zbasename:

      if ( !(nargs ==1 || nargs ==2) ) goto err;

      if (nargs == 1) {

            FILE *f1;
            char tmp[1024] = "echo `basename \"";

            strncat(tmp, (const char *) &v1d[1], 1000);
            strcat(tmp, "\"`");

            f1 = popen(tmp,"r");
            strcpy(tmp,"");
            fgets(tmp, 1000, f1);
            tmp[strlen(tmp)-1]=0;
            strcpy( (char *) bd, tmp);
            return;
            }

      else {

            FILE *f1;
            char tmp[1024] = "echo `basename \"";

            strncat(tmp, (const char *) &v1d[1], 900);
            strcat(tmp, "\" \"");
            strncat(tmp, (const char *) &v1d[iargs[1]], 32);
            strcat(tmp, "\"`");
            f1 = popen(tmp,"r");
            strcpy(tmp,"");
            fgets(tmp, 1000, f1);
            strcpy( (char *) bd, tmp);
            return;
            }

zfloor:

      if (nargs != 1) goto err;
      mps_floor((char *) bd, (char *) &v1d[1]);
      return;

zceil:

      if (nargs != 1) goto err;
      mps_ceil((char *) bd, (char *) &v1d[1]);
      return;

ztrunc:

      if (nargs != 1) goto err;
      mps_trunc((char *) bd, (char *) &v1d[1]);
      return;

zsqlOpen: /* $zsqlOpen */

      if (svPtr->ConOpen) bd[0]='1';
      else bd[0]='0';
      bd[1]=0;
      return;


znative: /* $znative */

#if defined(SQLITE)
      strcpy((char *) bd, "0");
      return;
#endif

      strcpy((char *) bd, "1");
      return;

zsqlite: /* $zsqlite */

//--------------------
//      Sql Codes
//--------------------

//      BEGIN_TRASACTION
//      COMMIT_TRANSACTION
//      ROLLBACK_TRANSACTION
//      SAVEPOINT
//      PRAGMA


#ifdef SQLITE

      if (nargs == 0 ) {
            strcpy((char *) bd, "1");
            return;
            }

      if (strcasecmp((const char *)&v1d[1], "begin transaction") == 0  && nargs == 1) {
            char XTAB[2] = {'\t', 0};
            char filename[64];
            char dmy[8];
            strcpy(filename, "begin transaction;");
            svPtr->tpx = sql(BEGIN_TRANSACTION, svPtr, (char *) "", filename, dmy,  XTAB);
            sprintf((char *) bd, "%d", svPtr->tpx);
            return;
            }

      if (strcasecmp((const char *)&v1d[1], "commit transaction") == 0  && nargs == 1) {
            char XTAB[2] = {'\t', 0};
            char filename[64];
            char dmy[8];
            strcpy(filename, "commit transaction;");
            svPtr->tpx = sql(COMMIT_TRANSACTION, svPtr, (char *) "", filename, dmy, XTAB);
            sprintf((char *) bd, "%d", svPtr->tpx);
            return;
            }

      if (strcasecmp((const char *)&v1d[1], "savepoint") == 0  && (nargs == 1 || nargs == 2)) {
            char XTAB[2] = {'\t', 0};
            char filename[512];
            char dmy[8];
            strcpy(filename, "savepoint ");
            if (nargs == 2) {
                  strcat(filename, (char *) &v1d[iargs[1]]);
                  strcat(filename, ";");
                  }
            else strcat(filename, " default;");
            svPtr->tpx = sql(SAVEPOINT, svPtr, (char *) "", filename, dmy, XTAB);
            sprintf((char *) bd, "%d", svPtr->tpx);
            return;
            }

      if (strcasecmp((const char *)&v1d[1], "rollback") == 0  && (nargs == 1 || nargs == 2)) {
            char XTAB[2] = {'\t', 0};
            char filename[512];
            char dmy[8];
            strcpy(filename, "rollback to ");
            if (nargs == 2) {
                  strcat(filename, (char *) &v1d[iargs[1]]);
                  strcat(filename, ";");
                  }
            else strcat(filename, " default;");
            svPtr->tpx = sql(ROLLBACK_TRANSACTION, svPtr, (char *) "", filename, dmy, XTAB);
            sprintf((char *) bd, "%d", svPtr->tpx);
            return;
            }

      if (strcasecmp((const char *)&v1d[1], "pragma") == 0  && (nargs == 2)) {
            char XTAB[2] = {'\t', 0};
            char filename[512];
            char dmy[1024]="";
            strcpy(filename, "pragma ");
            strcat(filename, (char *) &v1d[iargs[1]]);
            strcat(filename, ";");
            svPtr->tpx = sql(PRAGMA, svPtr, (char *) "", filename, dmy, XTAB);
            if (strlen(dmy)) strcpy((char *) bd, dmy); // result returned
            else sprintf((char *) bd, "%d", svPtr->tpx);
            return;
            }

      if (strcasecmp((const char *)&v1d[1], "sql") == 0  && nargs == 2) {
            char XTAB[2] = {'\t', 0};
            char filename[512];
            char dmy[1024];
            strcpy(filename, (char *) &v1d[iargs[1]]);
            strcat(filename, ";");
            svPtr->tpx = sql(PRAGMA, svPtr, (char *) "", filename, dmy, XTAB);
            if (strlen(dmy)) strcpy((char *) bd, dmy); // result returned
            else sprintf((char *) bd, "%d", svPtr->tpx);
            return;
            }

      fprintf(stderr, "sqlite arguments not recognized\n");
      goto err;

#endif

      strcpy((char *) bd, "0");
      return;

zsrand: /* $zsrand(seed) */

      if (nargs!=1) goto err;
      i=atoi ( (char *) &v1d[1] );
      srand(i);
      return;


zsmithwaterman: /* $zSmithWaterman() */

      if (nargs!=7) goto err;
      strcpy((char *) bd,
             SmithWaterman( svPtr, (char *) "",
                            (char *) &v1d[1],
                            (char *) &v1d[iargs[1]],
                            (char *) &v1d[iargs[2]],
                            (char *) &v1d[iargs[3]],
                            (char *) &v1d[iargs[4]],
                            (char *) &v1d[iargs[5]],
                            (char *) &v1d[iargs[6]]) );
      return;

zreplace:    /* $zReplace(string,pattern,replacement) */

      if (nargs!=3) goto err;
      strcpy((char *)bd,Replace(svPtr, (char *) "",(char *)&v1d[1],(char *)&v1d[iargs[1]],
                                (char *)&v1d[iargs[2]]));
      return;

znoblanks: /* $zNoBlanks(string) */

      if (nargs!=1) goto err;

            {
            int i,j=0;;
            for (i=1; v1d[i]!='\0'; i++) if (v1d[i]!=' ') bd[j++]=v1d[i];
            bd[j]=0;
            return;
            }

zperlmatch:  /* $zPerlMatch(string,pattern) */

      if (nargs!=2) goto err;
      strcpy((char *)bd,perlmatch(svPtr, (char *) "",(char *)&v1d[1],(char *)&v1d[iargs[1]]));
      return;

zstopinit: /* $zStopInit() */

      if (nargs!=1) goto err;
      StopInit((char *) &v1d[1]);
      return;

zsyninit: /* $zSynInit() */

      if (nargs!=1) goto err;
      SynInit(svPtr, (char *) "", (char *) &v1d[1]);
      return;

zbmgsearch: /* $zBMGSearch(arg1,arg2) */

      if (nargs!=2) goto err;
      strcpy((char *)bd, BMGSearch(svPtr, (char *) "", (char *) &v1d[1], (char *) &v1d[iargs[1]] ));
      return;

zshred: /* $zShred(arg,size) */

      if (nargs!=2) goto err;
      strcpy((char *)bd, Shred(svPtr, (char *) "", (char *) &v1d[1], (char *) &v1d[iargs[1]]) );
      return;

zshredquery: /* $zShredQuery(arg,size) */

      if (nargs!=2) goto err;
      strcpy((char *)bd, ShredQuery(svPtr, (char *) "", (char *) &v1d[1], (char *) &v1d[iargs[1]]) );
      return;

zstoplookup: /* $zStopLookup() */

      if (nargs!=1) goto err;
      if (StopLookup((char *)&v1d[1])) bd[0]='1';
      else bd[0]='0';

      bd[1]=0;
      return;

zsynlookup: /* $zSynLookup() */

      if (nargs!=1) goto err;
      strcpy( (char *) bd, (char *) SynLookup(svPtr,  (char *) "", (char *)&v1d[1]) );
      return;

zzavg: /* $zzavg() */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,AVG,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zgetenv: /* $zgetenv(name) */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy( (char *) tmp1, (char *) &v1d[1] );
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,GETENV,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }


zzcount: /* $zzCount(arg) */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,COUNT,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzmax: /* $zzMax(arg) */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,MAX,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzmin: /* $zzMin(arg) */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,MIN,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzsum: /* $zzSum(arg) */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,SUM,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzscanalnum: /* $zzscanalnum */

      if (nargs!=0 && nargs !=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,SCANALNUM,svPtr);
            strcpy((char *)bd,(char *)tmp3);

            if (nargs==1) {
                  i = atol( (const char *) &v1d[iargs[1]]);
                  bd[i]=0;
                  }

            return;
            }

zzscan: /* $zzscan */

      if (nargs != 0 && nargs != 1) goto err;

            {
            unsigned char tmp1[STR_MAX], tmp2[STR_MAX], tmp3[STR_MAX];
            tmp1[0]=tmp2[0] = 0;
            ServiceInterface(tmp1, tmp2, NULL, NULL, tmp3, SCAN, svPtr);
            strcpy((char *)bd,(char *)tmp3);

            if (nargs==1) {	// truncation length
                  i = atol( (const char *) &v1d[iargs[1]]);
                  bd[i]=0;
                  }

            return;
            }

zzinput: /* $zzInput() */

      if (nargs!=1) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,INPUT,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzcosine: /* $zzcosine() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,COSINE,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzjaccard: /* $zzjaccard() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,JACCARD,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzsim1: /* $zzSim1() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,SIM1,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zztranspose: /* $zztranspose() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,TRANSPOSE,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzsoundex: /* $zzSoundex() */

      if (nargs!=1) goto err;

            {
            int i,j,k;
            bd[0]=v1d[1];

            for (j=1,i=2; v1d[i]!=0; i++) {

                  if (!isalpha(v1d[i])) continue;

                  v1d[i]=tolower(v1d[i]);

                  if (v1d[i]=='a' ||
                              v1d[i]=='e' ||
                              v1d[i]=='h' ||
                              v1d[i]=='i' ||
                              v1d[i]=='o' ||
                              v1d[i]=='w' ||
                              v1d[i]=='u' ||
                              v1d[i]=='y')     continue;

                  if (bd[j-1]!=v1d[i]) bd[j++]=v1d[i];

                  }

            bd[j]=0;

            for (i=1; bd[i]!=0; i++) {

                  switch (bd[i]) {

                        case 'b':
                        case 'f':
                        case 'p':
                        case 'v':

                              bd[i]='1';
                              break;

                        case 'c':
                        case 'g':
                        case 'j':
                        case 'k':
                        case 'q':
                        case 's':
                        case 'x':
                        case 'z':

                              bd[i]='2';
                              break;

                        case 'd':
                        case 't':

                              bd[i]='3';
                              break;

                        case 'l':

                              bd[i]='4';
                              break;

                        case 'm':
                        case 'n':

                              bd[i]='5';
                              break;

                        case 'r':

                              bd[i]='6';
                              break;
                        }
                  }

            bd[4]=0;
            return;
            }

zzmultiply: /* $zzMultiply(gbl1,gbl2,gbl3) */

      if (nargs!=3) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX],tmp4[STR_MAX];
            tmp3[0]=tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            strcpy((char *)tmp3,(char *)&v1d[iargs[2]]);
            printf("%s  %s  %s\n",tmp1,tmp2,tmp3);
            ServiceInterface(tmp1,tmp2,tmp3,NULL,tmp4,MMULTIPLY,svPtr);
            strcpy((char *)bd,(char *)tmp4);
            return;
            }

zzcentroid: /* $zzcentroid() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,CENTROID,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zztermcorrelate: /* $zztermcorrelate() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,TERMCORRELATE,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzdoccorrelate: /* $zzdoccorrelate() */

      if (nargs!=4) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp2a[STR_MAX],tmp2b[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            strcpy((char *)tmp2a,(char *)&v1d[iargs[2]]);
            strcpy((char *)tmp2b,(char *)&v1d[iargs[3]]);
            ServiceInterface(tmp1,tmp2,tmp2a,tmp2b,tmp3,DOCCORRELATE,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzidf: /* $zzIDF() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,INVDF,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zzdice: /* $zzdice() */

      if (nargs!=2) goto err;

            {
            unsigned char tmp1[STR_MAX],tmp2[STR_MAX],tmp3[STR_MAX];
            tmp1[0]=tmp2[0]=0;
            strcpy((char *)tmp1,(char *)&v1d[1]);
            strcpy((char *)tmp2,(char *)&v1d[iargs[1]]);
            ServiceInterface(tmp1,tmp2,NULL,NULL,tmp3,DICE,svPtr);
            strcpy((char *)bd,(char *)tmp3);
            return;
            }

zflush: /* $zflush - flush native btree records to disk */
      Mglobal(FLUSH,tmp2,tmp3,svPtr);
      strcpy( (char * ) bd, "");
      return;

zchdir: /* $zchdir(dir) */

      if (nargs!=1) goto err;

      i=chdir( (char *) &v1d[1]);
      if (i==0) strcpy( (char *) bd,"0");
      else strcpy( (char *) bd,"1");
      return;

zpow: /* zpow() */

      if (nargs!=2) goto err;
      expx((char *) &v1d[1], (char *) &v1d[iargs[1]], (char *) bd);
      return;

zlog2: /* $zlog2() */

      if (nargs!=1) goto err;
      mps_log2((char *) bd, (char *) &v1d[1]);
      return;

zlog10: /* $zlog10() */

      if (nargs!=1) goto err;
      mps_log10((char *) bd, (char *) &v1d[1]);
      return;

zlog: /* $zlog() */

      if (nargs!=1) goto err;
      mps_log((char *) bd, (char *) &v1d[1]);
      return;

zcosine: /* $zcos() */

      if (nargs!=1) goto err;
      mps_cos((char *) bd, (char *) &v1d[1]);
      return;

ztangent: /* $ztan() */

      if (nargs!=1) goto err;
      mps_tan((char *) bd, (char *) &v1d[1]);
      return;

zatangent: /* $zatan() */

      if (nargs!=1) goto err;
      mps_atan((char *) bd, (char *) &v1d[1]);
      return;

zacosine: /* $zacos() */

      if (nargs!=1) goto err;
      mps_acos((char *) bd, (char *) &v1d[1]);
      return;

zsine: /* $zsin() */

      if (nargs!=1) goto err;
      mps_sin((char *) bd, (char *) &v1d[1]);
      return;

zasine: /* $zasin() */

      if (nargs!=1) goto err;
      mps_asin((char *) bd, (char *) &v1d[1]);
      return;

zexp: /* $zexp() */

      if (nargs!=1) goto err;
      mps_exp((char *) bd, (char *) &v1d[1]);
      return;

zexp2: /* $zexp() */

      if (nargs!=1) goto err;
      mps_exp2((char *) bd, (char *) &v1d[1]);
      return;

zexp10: /* $zexp() */

      if (nargs!=1) goto err;
      mps_exp10((char *) bd, (char *) &v1d[1]);
      return;

zhit: /* btree hit ratio */

      if (svPtr->count==0) strcpy( (char *) bd,"0");
      else sprintf( (char *) bd,"%lf",(double)svPtr->hit/svPtr->count);

      svPtr->count=0;
      svPtr->hit=0;
      return;

zlower: /* convert to lower case */

      if (nargs != 1 ) goto err;
            {
            char bdx = 0, *bdtmp = (char *) bd, *v1dtmp = (char *) v1d+1;
            while ( *v1dtmp != 0) {
                  *(bdtmp++) = tolower(*(v1dtmp++));
                  }
            *bdtmp=0;
            }
      return;

      /*#+--------------------------------------------------------------
       *#+      $ZALTER(string)
       *#+      $ZNORMAL(string)
       *#+      Remove punctuation, make upper case, and remove suffixes
       *#+      Used for text processing applications.
       *#+--------------------------------------------------------------*/

znormal:

zalter:

            {
            if (nargs != 1 && nargs !=2 ) goto err;

            j = 0;
            bd[0] = 0;

            for (i = 1; v1d[i] != 0; i++) {
                  if (isalnum(v1d[i]) ) bd[j++] = tolower(v1d[i]);
                  }

            bd[j] = 0;

            if (nargs == 2 ) WordTrunc = atoi((char*) &v1d[iargs[1]]);
            else WordTrunc = 25;

            bd[WordTrunc] = 0;

            return;
            }

      goto err;

zstem: // $zstem

      if (nargs != 1 ) goto err;

      j = 0;
      bd[0] = 0;

      for (i = 1; v1d[i] != 0; i++) {
            bd[i-1] = tolower(v1d[i]);
            }

      bd[i-1] = 0;

      /* use char * Stem (char * ep, char * str) now */
      /* remove English language suffixes. */
      wstem((char *) bd, strlen((char *) bd));
      return;


      /*------------------------------------------
       *      $zseek
       *      move to designated byte
      *------------------------------------------*/

zseek:

      if (nargs != 1 ) goto err;

      if (svPtr->io < 0 || svPtr->io > 5 ||
                  svPtr->in_file[svPtr->io]==NULL) {
            bd[0] = 0;
            return;
            }

            {
            static fpos_t i;

            if (sizeof(i)==4) sscanf((char*) &v1d[1], "%ld", &i);
            else sscanf((char*) &v1d[1], "%lld", &i);

            fsetpos(svPtr->in_file[svPtr->io], &i);
            strcpy( (char *) bd,"0");
            return;
            }

      /*------------------------------------------
       *      $ztell
       *      Return byte offset in current file
      *------------------------------------------*/

ztell:

      if (svPtr->io < 0 || svPtr->io > 5) {
            strcpy( (char *) bd,"-1");
            return;
            }

            {
            fpos_t i;
            FILE *p;

            if (svPtr->in_file[svPtr->io]!=NULL)
                  p=svPtr->in_file[svPtr->io];
            else if (svPtr->out_file[svPtr->io]!=NULL)
                  p=svPtr->out_file[svPtr->io];
            else {
                  strcpy( (char *) bd,"-1");
                  return;
                  }

            fgetpos(p, &i);

            if (sizeof(i)==4) sprintf((char*) bd, "%ld", i);
            else sprintf((char*) bd, "%lld", i);

            return;
            }

      /*#+--------------------------------------------------------------------
       *#+      This function is used with IS&R experiments.
       *#+      It loads a buffer (linebuf) with a line of text
       *#+      then parses and returns words one by one.
       *#+
       *#+      $ZWI(string) - initialize line buffer with "string"
       *#+      $ZWN - extract next word from line buffer - return "" if none
       *#+             remain. Remove punctuation.
       *#+      $ZWP - extract next token from buffer - "" if done.
       *#+--------------------------------------------------------------------*/

zword:

      /*ifdef BUF_PARSE*/

      if (zcode1 == 's') {        /* initialize */
            strmove((unsigned char*) linebuf, &v1d[1]);
            linebufx = 0;
            bd[0] = 0;
            return;
            }


      if (zcode1 == 'i') {        /* initialize */
            char *p1;
            strmove((unsigned char*) linebuf, &v1d[1]);
            linebufx = 0;

            for (p1=linebuf; *p1!=0; p1++) *p1=tolower(*p1);

            bd[0] = 0;
            return;
            }

      if (tolower(zcode1) == 'n') {
            if (linebuf[linebufx] == 0) {   /* empty */
                  bd[0] = 0;
                  return;
                  }

            while (linebuf[linebufx] != 0 && linebuf[linebufx] == ' ')
                  linebufx++;

            if (linebuf[linebufx] == 0) {   /* end of buffer */
                  bd[0] = 0;
                  return;
                  }

            i = 0;

            while (linebuf[linebufx]=='-'||
                        linebuf[linebufx]=='_') linebufx++;

            while (1) {             /* extract next word */
                  bd[i] = linebuf[linebufx];

                  if (linebuf[linebufx] == 0 || linebuf[linebufx] == ' ') {
                        bd[i] = 0;
                        return;
                        }

                  i++;
                  linebufx++;
                  }
            }

      if (zcode1 == 'p') {        /*#+++  parse */

            if (linebuf[linebufx] == 0) {   /*#+++ empty */
                  bd[0] = 0;
                  return;
                  }

            while (linebuf[linebufx] != 0 && linebuf[linebufx] == ' ')
                  linebufx++;

            if (linebuf[linebufx] == 0) {   /*#+++ end of buffer */
                  bd[0] = 0;
                  return;
                  }

            if (linebuf[linebufx]!='*'&&linebuf[linebufx]!='-'&&ispunct(linebuf[linebufx])) {
                  bd[0] = linebuf[linebufx];
                  bd[1] = 0;
                  linebufx++;
                  return;
                  }

            i = 0;

            while (1) {             /*#+++ extract next word */
                  bd[i] = linebuf[linebufx];

                  if (linebuf[linebufx] == 0 ||
                              (linebuf[linebufx]!='*'&&linebuf[linebufx]!='-'&&ispunct(linebuf[linebufx]))
                              || linebuf[linebufx] == ' ') {
                        bd[i] = 0;
                        return;
                        }

                  i++;
                  linebufx++;
                  }
            }

      /*endif*/

      goto err;


      /*#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *#+
       *#+    $Znumeric(string) - is string non-numeric?
       *#+
       *#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

znumeric:

      if (nargs != 1)
            goto err;

      k = 0;

      for (i = 1; v1d[i] != 0; i++)
            if (isdigit(v1d[i]) || v1d[i] == '.' || v1d[i] == '+'
                        || v1d[i] == '-')
                  continue;
            else {
                  bd[0] = '0';
                  bd[1] = 0;
                  return;
                  }

      bd[0] = '1';
      bd[1] = 0;
      return;

      /*#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *#+
       *#+    $ZBLANKS(string) - remove leading and double blanks
       *#+
       *#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zblnk:

      if (nargs != 1) goto err;

            {
            char bdx = 0, *bdtmp = (char *) bd, *v1dtmp = (char *) v1d+1;

            while (*v1dtmp) if ( *(v1dtmp) != ' ' ) goto zbl1;
                  else v1dtmp++;

            *bdtmp=0;
            return;

zbl1:

            while (*v1dtmp) {
                  if (bdx == ' ' && *v1dtmp == ' ') { // bd has a blank?
                        v1dtmp++; // yes
                        continue;
                        }
                  bdx = *(bdtmp++) = *(v1dtmp++);
                  }

            *bdtmp = 0;
            }

      return;

      /*#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *#+
       *#+    $ZCurrentFile - currnt executing program file
       *#+
       *#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zcurrentfile:

      strcpy((char *) bd, svPtr->CurrentFile);
      return;


      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+     $ZHTML(string) - convert codes for html transmission
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zhtml:

      if (nargs != 1)
            goto err;

      k = 1;

      for (i = 1; v1d[i] != 0; i++) {
            if (v1d[i] == ' ')
                  v1d[i] = '+';
            else if (!isalnum(v1d[i])) {
                  char t1[10], t2[STR_MAX];
                  sprintf(t1, "%%%2X\0", v1d[i]);
                  v1d[i] = 0;
                  strmove((unsigned char*) t2, &v1d[1]);
                  strcat(t2, t1);
                  strcat(t2, (char*) &v1d[i + 1]);
                  strmove(&v1d[1], (unsigned char*) t2);
                  i = i + 2;
                  }
            }

      strmove(bd, &v1d[1]);
      return;


      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZCONDENSE - Dump Global Arrays to Disk
       *+
       *+       $ZCD[(start)] - dump to file ``dump''
       *+       $ZCL - load from file ``dump''
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zcondense:

#if defined(SQLITE)
      return; // not avail in sqlite
#endif

      if (nargs > 1) goto err;

      if (nargs != 1) {           /* generate file name from time */
            dtime = time(NULL);
            sprintf((char*) tmp2, "%lu", dtime);
            strcat((char*) tmp2, ".dmp");
            }
      else if (nargs == 1) {      /* user supplied file name */
            strmove(tmp2, &v1d[1]);
            }
      else
            goto err;               /* too many arguments */

      dump = fopen((char*) tmp2, "w+");

      if (dump == NULL)
            goto err;

      tmp2[0] = ' ';              /* lowest printable */
      tmp2[1] = 1;
      tmp2[2] = 1;
      tmp2[3] = 0;

      while (1) {                 /* dump global arrays */
            g = XNEXT;
            i = Mglobal (g, tmp2, tmp3, svPtr);

            if (i == 0) break;

            g = RETRIEVE;
            Mglobal (g, tmp2, tmp4, svPtr);

            for (i = 0; tmp2[i] != 0; i++) {
                  if (tmp2[i] < ' ') tmp3[i] = '~';
                  else
                        tmp3[i] = tmp2[i];
                  }

            tmp3[i] = 0;
            fputs((char*) tmp3, dump);
            fputs("\n", dump);      /* key */
            fputs((char*) tmp4, dump);
            fputs("\n", dump);      /* data */
            }

      fclose(dump);
      bd[0] = '1';
      bd[1] = 0;
      return;

zrestore:

#if defined(SQLITE)
      return; // not avail in sqlite
#endif

      if (nargs > 1) goto err;

      if (nargs != 1) {           /* use default file name */
            strcat((char*) tmp2, "dump");
            }
      else if (nargs == 1) {      /* user supplied file name */
            strmove(tmp2, &v1d[1]);
            }
      else
            goto err;               /* too many arguments */

      dump = fopen((char*) tmp2, "r");

      if (dump == NULL)
            goto err;

      while (1) {                 /* load */
            if (fgets((char*) tmp2, STR_MAX-1, dump) == NULL)
                  break;

            fgets((char*) tmp3, STR_MAX-1, dump);

            for (i = 0; tmp3[i] != 0; i++)
                  if (tmp3[i] < 32)
                        tmp3[i] = 0;

            for (i = 0; tmp2[i] != 0; i++)
                  if (tmp2[i] == '~')
                        tmp2[i] = 1;
                  else if (tmp2[i] < 32)
                        tmp2[i] = 0;

            g = STORE;
            Mglobal (g, tmp2, tmp3, svPtr);
            }

      fclose(dump);
      bd[0] = '1';
      bd[1] = 0;
      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZM(Global) -
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

znxtrow:

      if (nargs == 1) {           /* argument prvides starting point */
            strmove(gblstr, &v1d[1]);
            strmove(gbltbl, &v1d[1]);
            }

      strmove(tmp2, gblstr);

      g = XNEXT;
      i = Mglobal (g, tmp2, tmp3, svPtr);

      if (i == 0) {
            bd[0] = 0;
            return;
            }

      g = RETRIEVE;
      Mglobal (g, tmp2, tmp4, svPtr);

      for (i = 0; tmp2[i] != 0; i++) {
            if (tmp2[i] < ' ')
                  tmp3[i] = '~';
            else
                  tmp3[i] = tmp2[i];
            }

      tmp3[i] = 0;

      for (i = 0; gbltbl[i] == tmp3[i] && gbltbl[i] != 0 && tmp3[i] != '~';
                  i++);

      if (gbltbl[i] == 0 && tmp3[i] == '~') {
            for (i = 0; (gblstr[i] = tmp2[i]) != 0; i++);

            strmove(bd, tmp3);
            return;
            }

      bd[0] = 0;
      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZABS(nbr) - absolute value fcn
       *+
       *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zabs:

      if (nargs != 1) goto err;

      if (mps_abs((char *) &v1d[1], (char *) bd)) goto err;

      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZPAD(str,len) - pad a string with blanks
       *+
       *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zpad:

      if (nargs != 2)
            goto err;

      i = atol( (const char *) &v1d[iargs[1]]);
      strmove(bd, &v1d[1]);

      for (j = strlen((char*) &v1d[1]); j < i; j++)
            bd[j] = ' ';

      bd[j] = 0;
      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $Zvariable(str) - get value from symbol table
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

z_variable:

      if (nargs != 1) goto err;

      if (sym_(1, &v1d[1], bd, svPtr) == NULL) bd[0] = 0;

      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZDATE - date and time of day (Wed Jan 01, 1992 14:36:00)
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zdate:

      timex = time(&timex);

      if (zcode1 == '1')
            sprintf((char*) bd, "%ld", timex);      /* internal code */

      else if (zcode1 == '2') {   /* internal -> printable */
            timex = atol( (const char *) &v1d[1]);
            strmove(bd, (unsigned char*) ctime(&timex));
            bd[24] = 0;
            }

      else if (zcode1 == '3') {   /* $zd3(year,month,day) -> DayOfYear */

            struct tm date;
            int year, d, m;

            if (nargs != 3)
                  goto err;

            year = atol( (const char *) &v1d[1]);
            m = atol( (const char *) &v1d[iargs[1]]);
            d = atol( (const char *) &v1d[iargs[2]]);

            if (year<1900 || m<1 || m>12 || d<1 || d>31) goto err;

            if (year > 1900) date.tm_year = year - 1900;
            else date.tm_year = year;

            date.tm_mon = m - 1;    /* range is 0 to 11 */
            date.tm_mday = d;
            date.tm_hour = 12;
            date.tm_min = 0;
            date.tm_sec = 0;
            date.tm_yday = 0;
            date.tm_isdst = 1;

            mktime(&date);

            sprintf((char*) bd, "%d", date.tm_yday + 1);    /* range is 0-365 */
            }

      else if (zcode1 == '4') {   /* $zd4(year,dayofyear) -> gregorian */

            struct tm date;
            int year, d, m, dd;

#ifdef GnuDos
            strcpy( (char *) bd," $zd4 not available ***");
            return;
#endif

            if (nargs != 2) goto err;

            year = atol( (const char *) &v1d[1]);

            if (year > 1900) year = year - 1900;

            d = atol( (const char *) &v1d[iargs[1]]);

            if (year<0 || d>366 || d<1) goto err;

            d--;
            m = 0;
            dd = 1;

            while (1) {
                  date.tm_year = year;
                  date.tm_mon = m;
                  date.tm_mday = dd;
                  date.tm_hour = 12;
                  date.tm_min = 0;
                  date.tm_sec = 0;
                  date.tm_yday = 0;
                  date.tm_isdst = 1;
                  mktime(&date);

                  if (date.tm_yday == d)
                        break;

                  dd++;

                  if (m == 0 && dd > 31) {
                        m++;
                        dd = 1;
                        }                   /* jan */
                  else if (m == 1 && (year % 4) == 0) {
                        if (dd > 29) {
                              m++;
                              dd = 1;
                              }               /* feb */
                        }
                  else if (m == 1 && (year % 4) != 0) {
                        if (dd > 28) {
                              m++;
                              dd = 1;
                              }               /* feb */
                        }
                  else if (m == 2 && dd > 31) {
                        m++;
                        dd = 1;
                        }                   /* mar */
                  else if (m == 3 && dd > 30) {
                        m++;
                        dd = 1;
                        }                   /* apr */
                  else if (m == 4 && dd > 31) {
                        m++;
                        dd = 1;
                        }                   /* may */
                  else if (m == 5 && dd > 30) {
                        m++;
                        dd = 1;
                        }                   /* jun */
                  else if (m == 6 && dd > 31) {
                        m++;
                        dd = 1;
                        }                   /* jul */
                  else if (m == 7 && dd > 31) {
                        m++;
                        dd = 1;
                        }                   /* aug */
                  else if (m == 8 && dd > 30) {
                        m++;
                        dd = 1;
                        }                   /* sep */
                  else if (m == 9 && dd > 31) {
                        m++;
                        dd = 1;
                        }                   /* oct */
                  else if (m == 10 && dd > 30) {
                        m++;
                        dd = 1;
                        }                   /* nov */
                  else if (m == 11 && dd > 31)
                        goto err;       /* dec */
                  }

            if (date.tm_year < 100)
                  sprintf((char*) bd, "19%d %d %d",
                          date.tm_year, date.tm_mon + 1, date.tm_mday);
            else
                  sprintf((char*) bd, "20%02d %d %d",
                          date.tm_year - 100, date.tm_mon + 1, date.tm_mday);
            }

      else if (zcode1 == '5') {
            /* $zd5(year,mn,dy) -> yr,dy,dw (days since sunday) */

            struct tm date;
            int year, d, m;

            if (nargs != 3) goto err;

            year = atol( (const char *) &v1d[1]);
            m = atol( (const char *) &v1d[iargs[1]]);
            d = atol( (const char *) &v1d[iargs[2]]);

            if (year<1900 ||
                        m<1 || m>12 ||
                        d<1 || d>31) goto err;

            if (year > 1900) date.tm_year = year - 1900 ;
            else date.tm_year = year;

            date.tm_mon = m - 1;    /* range is 0 to 11 */
            date.tm_mday = d;
            date.tm_hour = 12;
            date.tm_min = 0;
            date.tm_sec = 0;
            date.tm_yday = 0;
            date.tm_wday = 0;
            date.tm_isdst = 0;

            mktime(&date);

            if (date.tm_year < 100)
                  sprintf((char *) bd, "19%d,%d,%d",
                          date.tm_year, date.tm_yday + 1, date.tm_wday);
            else
                  sprintf((char *) bd, "20%02d,%d,%d",
                          date.tm_year - 100 - 1, date.tm_yday + 2, date.tm_wday);
            }

      else if (zcode1 == '6') {   /* $zd6 -> HH:SS */
            strmove(bd, (unsigned char*) ctime(&timex) + 11);
            bd[5] = 0;
            }

      else if (zcode1 == '7' || zcode1 == '8') {  /* current as year,month,day */
            if (nargs == 1)
                  timex = atol( (const char *) &v1d[1]);

            strmove(tmp2, (unsigned char*) ctime(&timex));
            bd[1] = tmp2[20];
            bd[2] = tmp2[21];
            bd[3] = tmp2[22];
            bd[4] = tmp2[23];
            bd[5] = '-';
            tmp2[7] = 0;

            if (strcmp((char*) &tmp2[4], "Jan") == 0) {
                  bd[6] = '0';
                  bd[7] = '1';
                  }
            else if (strcmp((char*) &tmp2[4], "Feb") == 0) {
                  bd[6] = '0';
                  bd[7] = '2';
                  }
            else if (strcmp((char*) &tmp2[4], "Mar") == 0) {
                  bd[6] = '0';
                  bd[7] = '3';
                  }
            else if (strcmp((char*) &tmp2[4], "Apr") == 0) {
                  bd[6] = '0';
                  bd[7] = '4';
                  }
            else if (strcmp((char*) &tmp2[4], "May") == 0) {
                  bd[6] = '0';
                  bd[7] = '5';
                  }
            else if (strcmp((char*) &tmp2[4], "Jun") == 0) {
                  bd[6] = '0';
                  bd[7] = '6';
                  }
            else if (strcmp((char*) &tmp2[4], "Jul") == 0) {
                  bd[6] = '0';
                  bd[7] = '7';
                  }
            else if (strcmp((char*) &tmp2[4], "Aug") == 0) {
                  bd[6] = '0';
                  bd[7] = '8';
                  }
            else if (strcmp((char*) &tmp2[4], "Sep") == 0) {
                  bd[6] = '0';
                  bd[7] = '9';
                  }
            else if (strcmp((char*) &tmp2[4], "Oct") == 0) {
                  bd[6] = '1';
                  bd[7] = '0';
                  }
            else if (strcmp((char*) &tmp2[4], "Nov") == 0) {
                  bd[6] = '1';
                  bd[7] = '1';
                  }
            else if (strcmp((char*) &tmp2[4], "Dec") == 0) {
                  bd[6] = '1';
                  bd[7] = '2';
                  }

            bd[8] = '-';
            bd[9] = tmp2[8];
            bd[10] = tmp2[9];
            bd[11] = 0;

            for (j = 0; j < 11; j++)

                  if (bd[j] == ' ')
                        bd[j] = '0';

            if (zcode1 == '8') {
                  bd[11] = ',';
                  strmove(&bd[12], (unsigned char*) ctime(&timex) + 11);
                  bd[17] = 0;
                  }

            strmove(bd, &bd[1]);
            }

      else {
            if (nargs != 0)
                  goto err;

            strmove(bd, (unsigned char*) ctime(&timex));
            bd[24] = 0;
            }

      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZFILE(filename) - does file exist?
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zfile:

      fd = open((char*) &v1d[1], 0);
      x = fd;
      close(fd);

      if (x == -1) {
            bd[0] = '0';
            bd[1] = 0;
            return;
            }

      bd[0] = '1';
      bd[1] = 0;
      return;

      /*#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *#+
       *#+       $ZGLOBAL - file system status
       *#+
       *#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zglobal:

      g = 5;                      /* count empties */
      Mglobal (g, v1d, bd, svPtr);
      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZROOT(nbr) - square root
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zroot:

      if (nargs != 1) goto err;

      mps_sqrt((char *) bd, (char *) &v1d[1]);
      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       $ZSYSTEM(command) - spawn shell and exec command
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

zsystem:

      if (nargs != 1)
            goto err;
      x = system((char*) &v1d[1]);
      sprintf((char*) bd, "%d", x);
      return;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
       *+
       *+       Error exit
       *+
       *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

fcn_not_found:

      if (svPtr->NOERR) {
            svPtr->NOERR=-1;
            return;
            }

      printf("\n*** Z function: \"%s\" not found in or near line %d\n", fcnName, svPtr->LineNumber);
      sigint(100);

err:

      if (svPtr->NOERR) {
            svPtr->NOERR=-1;
            return;
            }

      fprintf(stderr, "\n*** zFunction error in or near line %d\n", svPtr->LineNumber);
      sigint(100);

      }

unsigned char * Mget(unsigned char * s,struct MSV * svPtr) {
      char tmp[128];
      return (unsigned char *) sym_(1, (unsigned char *) s, (unsigned char *) tmp,svPtr);
      }
