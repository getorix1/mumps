#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Internet Radio Automation & Encoding Toolkit
#+
#+     Copyright (C) 2018, 2019, 2020, 2021, 2023 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     https://www.cs.uni.edu/~okane
#+     https://threadsafebooks.com/
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
#+ May 7, 2023
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SHELL = /bin/sh
CPP = g++
COMMON_CODE=../../common-code
COM=.
MAIN=..

CPP_FLAGS = -O3 -pipe -w -fpermissive \
        -I /usr/lib/include -I $(COMMON_CODE) \
	-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

LIBS = -lm -lreadline -L/usr/lib -lmpscpp -lmumps -lmpsglobal_native -lpcre  \
	-lpthread -export-dynamic

PKG_LIBS = `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gtk+-3.0` 

dep =	/usr/bin/mumps $(COMMON_CODE)/definitions.h  $(COMMON_CODE)/globals.h \
	$(COMMON_CODE)/MDH-interface.h $(COM)/indexing-controls.script 

OBJ = preprocess.o Document_Term_Matrix.o weighted-matrices.o \
	file_size.o \
	idf-weights.o \
	extract-titles.o \
	file_size1.o load-params.o

all:	genome RetrieveGenomeMumps count-seq chop extractNT extractDNA

genome:	preprocess.o Document_Term_Matrix.o weighted-matrices.o \
 	idf-weights.o \
 	extract-titles.o \
 	file_size.o file_size1.o load-params.o extractNT extractDNA reformat

chop:	$(COM)/chop.cpp
	$(CPP) -o chop $(COM)/chop.cpp

count-seq:	$(COM)/count-seq.cpp
	$(CPP) -o count-seq $(COM)/count-seq.cpp

genome:	$(COM)/genome.cpp $(dep)
	$(CPP) -o genome $(COM)/genome.cpp $(OBJ) $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

RetrieveGenomeMumps: $(COM)/RetrieveGenomeMumps.mps $(dep)
	mumpsc $(COM)/RetrieveGenomeMumps.mps

reformat : $(COMMON_CODE)/reformat.mps $(dep)
	mumpsc $(COMMON_CODE)/reformat.mps

extractNT : $(COMMON_CODE)/extractNT.mps $(dep)
	mumpsc $(COMMON_CODE)/extractNT.mps

extractDNA : $(COMMON_CODE)/extractDNA.mps $(dep)
	mumpsc $(COMMON_CODE)/extractDNA.mps

load-params.o :	../../common-code/load-params.cpp $(dep)
	$(CPP) -c $(COMMON_CODE)/load-params.cpp $(PKG_LIBS) $(CPP_FLAGS) $(LIBS)

file_size.o : $(COM)/file_size.cpp $(dep)
	$(CPP) -c $(COM)/file_size.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

file_size1.o : $(COM)/file_size1.cpp $(dep)
	$(CPP) -c $(COM)/file_size1.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

preprocess.o : $(COM)/preprocess.cpp $(dep)
	$(CPP) -c $(COM)/preprocess.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

extract-titles.o : $(COM)/extract-titles.cpp $(dep)
	$(CPP) -c $(COM)/extract-titles.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

Document_Term_Matrix.o : $(COM)/Document_Term_Matrix.cpp $(dep)
	$(CPP) -c $(COM)/Document_Term_Matrix.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

weighted-matrices.o : $(COM)/weighted-matrices.cpp $(dep)
	$(CPP) -c $(COM)/weighted-matrices.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

idf-weights.o : $(COM)/idf-weights.cpp $(dep)
	$(CPP) -c $(COM)/idf-weights.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)


clean:
	rm -f $(MAIN)/*.tmp
	rm -f $(MAIN)/*.txt
	rm -f $(COM)/*.o
	rm -f $(MAIN)/key.dat $(MAIN)/data.dat
	rm -f $(MAIN)/db.stemmed
	rm -f $(MAIN)/db.converted
	rm -f $(MAIN)/titles.list
	rm -f $(MAIN)/*.list
	rm -f $(MAIN)/genome $(MAIN)/reformat $(MAIN)/starttime $(MAIN)/titles.list $(MAIN)/medline
	rm -f $(COM)/genome $(COM)/reformat $(COM)/starttime $(COM)/titles.list $(COM)/medline
	rm -f $(MAIN)/medlinrRetrieve.cpp $(MAIN)/full.seq
	rm -f $(COM)/extractDNA $(MAIN)/extractNT
	rm -f $(MAIN)/extractDNA $(MAIN)/extractNT
	rm -f $(MAIN)/RetrieveGenomeMumps $(COM)/RetrieveGenomeMumps
	rm -f -r $(MAIN)/seg*

