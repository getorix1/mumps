#!/usr/bin/mumps

# Jan 9, 2021

+ #include "gtk1.h"

	Zmain

+ #include "gtk2.h"

	do ^gtk4.h

+ gtk_main();

	write "Goodbye!",!
	zexit

+ #include "gtk3.h"

