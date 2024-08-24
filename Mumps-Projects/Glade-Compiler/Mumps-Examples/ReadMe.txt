July 25, 2022

The subdirectories give examples of vrious GTK widgets except the 
Template directory which contains the bare minimum.

This is a work in progress - check back for updates.

To build and compile an appplication:

-	The GtkCompiler.mps file requires installation of the Mumps
	compiler which is contained in this distro. Install the Native 
	Single User version.

-	Using Glade, build a Glade XML project file. Name the project file
	'mumps.glade'. Glade will place in this file the XML code
	for the layout of your project. There is a blank project in the Template
	directory.

-	For each widget in your Glade layout, be sure to give it an
	ID (under the General tab. Most of the commonly used GTK Glade
	widgets are supported. More will be added soon.

-	For each active widget (buttons, slides, input, etc), be sure to
	establish Signals for actions done on the widget (for example,
	"on_mybutton_clicked". Signals are established under the Signals
	tab in Glade. The signal must contain the ID of the widget. By default, 
	Glade will suggest the correct signal name when you begin typing "on..."

-	Some widgets (for exaple sliders, spin butttons, etc) require you
	to establish "adjustments" for them. This is done on the General
	panel and are generally names adjustment1, adjustment2, etc. You 
	use these to establish minimum, maximum and step values as appropriete.

-	Once you have saved the Glade project as 'mumps.glade', you may build the
	project with the command:

		make -B

	This will write and compile code resulting in an executable named gtk.

-	For each signal, there will be a file created with a name like:

		on.widgetId.signalName.h

	Note: the Mumps examples will have a file extension of .mps rather than .h)

	The .h files contiain the actions to be performed when the signal is received.
	By default, these files are populated with a minimal amount of code to
	indicate that the signal is functioning and extract data (such as active/not active). 
	You will add code here to do what you want the signal to do.

Important

-	The compiler will not overwrite code for an existing signal. You must delete
	the appropriate signal header file if you want it to be re-created. 

	However, in the examples, the Makefile contains an rm command that deletes
	the existing signal handlers thus causing the compiler to build new
	handlers. You may want to remove this rm command.

	You will probably forget this and scream a lot wondering why changes are 
	not showing up in the header file or why your code is disappearing

-	The Makefile and some other code use relative directory naming. For example,
	the GtkCompiler.mps is often referenced as:

		../../GtkCompiled.mps

	If you move code around, you will may need to compensate for this.
