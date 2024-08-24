//#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+
//#+     GTK Compiler
//#+
//#+     Copyright (C) 2018, 2019, 2020, 2021 by Kevin C. O'Kane
//#+
//#+     Kevin C. O'Kane
//#+     kc.okane@gmail.com
//#+     https://www.cs.uni.edu/~okane
//#+     https://threadsafebooks.com/
//#+
//#+ This program is free software; you can redistribute it and/or modify
//#+ it under the terms of the GNU General Public License as published by
//#+ the Free Software Foundation; either version 2 of the License, or
//#+ (at your option) any later version.
//#+
//#+ This program is distributed in the hope that it will be useful,
//#+ but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#+ GNU General Public License for more details.
//#+
//#+ You should have received a copy of the GNU General Public License
//#+ along with this program; if not, write to the Free Software
//#+ Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+
//#+ August 6, 2022
//#+
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdarg.h>

//------------------------
//	GTK interface
//------------------------

//-----------------------------------------------------------------------
//	This code loads tables or trees into a GtkTreeStore.
//	If a tree is being loaded, it is loaded into the
//	first column of the tree store. Subsequent columns
//	may have flat data.
//
//	A table is a special case where the tree in column 1
//	consists of entries at the root level only.
//
//	The first parameter is the GtkTreeStore pointer.
//	The second parameter is the number of columns.
//	The third parameter is the depth in the tree in
//	    column 1 of this entry (always 1 when a table
//	    is being simulated).
//	The fourth entry is the tree or table element to be shown in
//      the first column of the treestore.
//	Subsequent entries are values for columns 2 and 
//	    Sfollowing on this row.
//-----------------------------------------------------------------------

int	GtkTreeLevelAdd(GtkTreeStore *t, int cols, int depth, ...) {

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

	va_list valist;
	char tmp[4096] = {0};
        GtkTreeIter iter_tmp;

//--------------------------------------------------------------------
//	'depth' is the name of the last known arg before the elipsis.
//--------------------------------------------------------------------

	va_start(valist, depth); // 'depth' is the name of the last known arg

	if (depth < 1 ) { 
		printf("GtkTreeLevelAdd depth < 1\n"); 
		return 0; 
		}

	if (depth == 1) {
		gtk_tree_store_append (t, &iter, NULL);
		gtk_tree_store_set(t, &iter, 0, va_arg(valist, char *), -1); // column 0
		iter_tmp =  iter;
		}

	else if (depth == 2) {
		gtk_tree_store_append (t, &iter1, &iter);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter1, 0, tmp, -1); // column 0
		iter_tmp =  iter1;
		}

	else if (depth == 3) {
		gtk_tree_store_append (t, &iter2, &iter1);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter2, 0, tmp, -1); // column 0
		iter_tmp =  iter2;
		}

	else if (depth == 4) {
		gtk_tree_store_append (t, &iter3, &iter2);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter3, 0, tmp, -1); // column 0
		iter_tmp =  iter3;
		}

	else if (depth == 5) {
		gtk_tree_store_append (t, &iter4, &iter3);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter4, 0, tmp, -1); // column 0
		iter_tmp =  iter4;
		}

	else if (depth == 6) {
		gtk_tree_store_append (t, &iter5, &iter4);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter5, 0, tmp, -1); // column 0
		iter_tmp =  iter5;
		}

	else if (depth == 7) {
		gtk_tree_store_append (t, &iter6, &iter5);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter6, 0, tmp, -1); // column 0
		iter_tmp =  iter6;
		}

	else if (depth == 8) {
		gtk_tree_store_append (t, &iter7, &iter6);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter7, 0, tmp, -1); // column 0
		iter_tmp =  iter7;
		}

	else if (depth == 9) {
		gtk_tree_store_append (t, &iter8, &iter7);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter8, 0, tmp, -1); // column 0
		iter_tmp =  iter8;
		}

	else if (depth == 10) {
		gtk_tree_store_append (t, &iter9, &iter8);
		strcpy(tmp, va_arg(valist, char *));
		gtk_tree_store_set(t, &iter9, 0, tmp, -1); // column 0
		iter_tmp =  iter9;
		}

	if (cols > 1) { // has data column(s)
		for (int i = 1; i < cols; i++) {
			strcpy(tmp, va_arg(valist, char *));
			gtk_tree_store_set(t, &iter_tmp, i, tmp, -1); // column i
			}
		}

	va_end(valist);
	return 1;

	}

#endif
