March 31, 2024


DO NOT RUN configure / make
DO NOT RUN configure / make
DO NOT RUN configure / make
DO NOT RUN configure / make

Please see MumpsUsersGuide.pdf in the Doc directory for full details.

To compile, you need to run one of the Configure scripts
followed by one of the Compile scripts. These need to be run
as root.

The simplest are:

ConfigureNativeMumps.script

followed by

CompileNativeSingleUserMumps.script

The Configure scripts download and install needed system libraries
if you do not have them. They have no effect if your libraries are
present and up to date. The downloads are from standard Linux
repositories.

The code is compatible with Windows Subsystem for Linux Version 2
(WSL 2) with Ubuntu 20.04

A test suite of information and retrieval code is in the directory:
    ISR
The 'make' will compile the Mumps code to C++ and then compile the
C++ to binary executables:

make -B

The script file 'index.script' will index the documents found in 
'osu.converted'. 

If you want to run this code in the interpreter, edit the line
that sets 'compile' from 'yes' no 'no'.

The data set is the OSU Medline collection used at the National Institute 
of Standards (NIST) Text Retrieval Conference 9 (TREC-9) [NIST 2000]. 

https://trec.nist.gov/pubs/trec9/t9_proceedings.html

File Ownership

Files created During configuration and compilation, bot done as root,
will be owned by root. To return them to your ownership, as root,
type:

chown -R your-name *
chgrp -R your-group *

Execute these Bash command rom the top level Mumps-Compiler-Interpreter-MDH
directory. Your group name is probably the same as your user name. 
