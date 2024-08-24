#!/usr/bin/mumps

# Jan 9, 2021

+ #include "gtk1.h"

	Zmain

+ #include "gtk2.h"

	do ^gtk4.h

	do ^BuildMeshTree.mps

 set x="This is a test message"_$c(10)_"If it had been an actual message, it"
 set x=x_"would have said something important"
 set %=$z~mdh~text~buffer~set~text(textbuffer1,x)


+ gtk_main();

	write "Goodbye!",!
	zexit

+ #include "gtk3.h"

