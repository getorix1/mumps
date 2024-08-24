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
//#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+
//#+      July 13, 2022

#ifndef USER_H
#define USER_H

	GtkCssProvider  *cssProviderMain;
	cssProviderMain  = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProviderMain, "gtk.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
	                GTK_STYLE_PROVIDER(cssProviderMain),
	                GTK_STYLE_PROVIDER_PRIORITY_USER);


#endif
