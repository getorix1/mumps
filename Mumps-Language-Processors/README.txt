
Jan 26, 2026

-------------------------------
DO NOT RUN configure / make
-------------------------------

For full details, see:

mumps/Mumps-Language-Processors/Mumps-Interpreter-Compiler-Library/Documentation/MumpsUsersGuide.pdf

Installation

The Mumps interpreter, compiler, and MDH Toolkit are all installed at the same time 
either by means of a pre-built installer or compilation from source code. The options 
discussed below apply to all three versions of the code.

In the pre-built installers, options are, for the most part, already set and may 
not be changed. If you build the system from source code, you may change the options. 
The vile build-mumps.script contains invocations of configure which propagates 
option selections through the code prior to compilation. For the most part, 
you will probably not want to alter these. But if you do, see the discussions below.

Using an Installer

Do not run  configure/make. You must use the scripts provided and these will invoke 
configure and make using the correct build parameters.

The software uses two database systems to store the Mumps global arrays. The main 
supported versions of the interpreter, compiler, and MDH library use the Sqlite3 
database. This is the default and the one contained in the installers provided 
on the web sites. The old native installer, however, is still present.

If you have an old or incompatible version already installed, you may need to 
remove it prior to installing the new version with one of the commands:

  sudo dpkg --remove mumps-sqlite3 or
  sudo dpkg --remove mumps-native-single-user

You may either build the interpreter/compiler/library from source code or use 
an installer. When you build from source code, you may select options such as 
which database system to use. The Sqlite3 version installer can be downloaded 
from the web page and also the distro. The native database installer is provided 
as part of the distro. 

Due to variations in libraries from one generation of Linux to the next, 
installers may be labeled with the name and version of the Linux distro they 
were created with. 

To install use a command similar to the following. Use only one of these. If you 
previously installed with a different database option, you will need to remove 
it first (see above).

  sudo dpkg --install mumps-sqlite3-amd64.deb
  sudo dpkg --install mumps-native-single-user-amd64.deb

If this is the first time you have installed the system, you may need to tun an 
additional command to update system software. The distro uses a number of standard Linux 
libraries that are often not part of the a base system install. The following 
command should install these in the even the install above gave missing package 
error messages. If no messages, the following should be unnecessary.  

  sudo apt install -f  

Alternatively, if the graphical installer gdebi is available, in an explorer window, 
double click on the installed .deb file. The installer gdebi should automatically 
install missing packages. 

If you use the Sqlite3 version, be sure to take note of the database creation 
script noted below.

Compiling from Source Code

If you want to compile and build the system yourself, you need to first install 
any system libraries you may be missing. The Mumps code uses a number of libraries 
that are not usually installed by default on many systems. The script provided 
consists of apt statements that will install these"

    sudo get-software.script

Next, configure, compile, and build (make) by running the following script :

    build-mumps.script

This will compile and create an installer for the  Sqlite3 version of the 
interpreter, compile, and MDH libraries. The installer will be named 
mumps-sqlite3-amd64.deb. During execution, you will be asked for the root 
password.

Alternatively, if you want to build the legacy native system, type:

    build-mumps.script native

The installer built will be named mumps-native-single-user-amd64.deb. The database
 used by this version will be stored in a file named mumps.sqlite. During execution, 
you will be asked for the root password.

The native database version that uses the native b-tree database which is stored 
in files named key.dat and data.dat).

The native data base is faster than the Sqlite3 database. For comparison, in a 
test involving extended database activity, the native database version took 4-9 seconds 
while the disk based Sqlite3 database version with default settings took 11,060 seconds. 
Very significant performance improvement of the Sqlite3 database may achieved by using 
pragma statements (see below). 

The disk based Sqlite3 version is ACID compliant while the native version is not.

After you have installed the system, you need to create a Sqlite3 database if you 
are using the Sqlite3 version. The database will be automatically created at first 
use if you are using the legacy native version.

You create a Sqlite3 database with the command: 

  mumps-sqlite3-amd64.deb

This will create the file mumps.sqlite in the current directory. 

The mumps.sqlite database or a symbolic link to it MUST be present in the
directory from which you run mumps. 

The mumps.sqlite database may be moved to other directories and you may create 
symbolic 
links to it.

Multiple instances of mumps programs may access the same Sqlite3 database (or 
symbolic link to same) concurrently. Only one instance may access the native database at once.

---------------

Running Mumps

If you arew using the Sqlite3 version, you must create a Sqlite3 database before
running Mumps. To do this, type:

	mumps-sql-db-create

The installer should remove any iold copy of mumps-sql-db-create.script which
you may have. That version is obsolete. You can remove it manually with:

sudo rm /usr/bin/mumps-sql-db-create.script

to a terminal window. The maximum number of indices permitted in a global array
reference is determined by this database setup. By default, this is normall
8. The value is set in the build-mumps.script.

The database will automatically be created in the native version.

From a terminal window:

	To run mumps type

		mumps

	This runs the interpreter. You will get a mumps prompt to which
	you may type mumps commands. To exit, hit control-D, control-C or
	type halt.

	To compile a mumps program type

		mumpsc pgm.mps

	This will create an executable named "pgm" which may be invoked in the
	usual manner.

	To compile an MDH program type

		mumpsc mdhpgm.cpp

	This will create an executable named "pgm" which may be invoked in the
	usual manner.

	A file of mumps code can be made executable by including the first line:

		#!/usr/bin/mumps
	
	and making the file executable:

		chmod u+x pgm.mps

	now, if you type the full filename (pgm.mps) to a command prompt, the
	interpreter will be invoked and the file executed.

---------------

Sqlite3 Performance Tuning

By default, as is the case with all ACID compliant database systems, Sqlite3 
journals and verifies all file updates. This leads to considerable I/O overhead 
and reduction of the speed at which transactions  that alter the database 
take place.  This insures that a system crash will not result in data loss.

In applications where data loss protection is not required, that is, those 
applications where the database software can be re-run in the event of a crash 
without loss, the strict update policy of Sqlite3 can be overridden with 
significant speed improvement. 

To achieve this, executre the following Mumps commans prior to using the database:

      s i=$zsqlite("pragma","journal_mode=off")
      s i=$zsqlite("pragma","synchronous=off")

These will temporarily reduce overhead during database updates resulting in significat 
speed improvement in operattions altering the database. 

---------------

SQL Access to the mumps database with sqlite3

Sqlite3 comes with a command line app that permits access to a Sqlite3 database.

A mumps.sqlite database may be opened with Sqlite3 with the command:

    sqlite3 mumps.sqlite

From which you may inspect or retrieve data from the mumps table with standard SQL
commands. These may be used to search, alter, or modify the mumps database.

By default, mumps builds a table called "mumps" with 10 columns named a1 through a10.

By default, the global array names are in column a1. The indices of the
global array are stored in columns a2 through a9 and any data stored
is in column a10. So, in a global array created with:

for i=1:1:10000 set ^a(i)=i

the SQL commands to see and format the data from array elements ^a(9000) 
to ^a(9010) would be:

sqlite> .separator " "
sqlite> select "^",a1,"(",a2,") =",a10 from mumps where
        cast (a2 as integer) >= 9000 and
        cast (a2 as integer) <= 9010;
^ a ( 9000 ) = 9000
^ a ( 9001 ) = 9001
^ a ( 9002 ) = 9002
^ a ( 9003 ) = 9003
^ a ( 9004 ) = 9004
^ a ( 9005 ) = 9005
^ a ( 9006 ) = 9006
^ a ( 9007 ) = 9007
^ a ( 9008 ) = 9008
^ a ( 9009 ) = 9009
^ a ( 9010 ) = 9010
sqlite>

Note: By default, this version of mumps stores numbers as character strings
so conversion by SQL cast or function will be necessary when numeric ranges 
are used. 

In the above, the Sqlite3 command ".separator" changes the default separator from | 
to blank. The quoted fields are for output table decoration.

---

