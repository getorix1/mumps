#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Internet Radio Automation & Encoding Toolkit
#+
#+     Copyright (C) 2018, 2019, 2020, 2021, 2023, 2024
#+     by Kevin C. O'Kane
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
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#       March 26, 2024

SHELL = /bin/sh
CPP = g++
CPP_FLAGS = -pipe -w -fpermissive \
        -I /usr/lib/include -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

LIBS = -lm -lreadline -L/usr/lib -lmpscpp -lmumps -lmpsglobal_native -lpcre  \
	-lpthread -export-dynamic

PKG_LIBS = `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gtk+-3.0` 

dep =	/usr/bin/mumps definitions.h  globals.h  MDH-interface.h

OBJ = FrequencyFilter.o preprocess.o Document_Term_Matrix.o weighted-matrices.o \
	doc-doc-weighted.o  \
	idf-weights.o term-term-cohesion.o discrim-coeff.o proxLoad.o \
	cluster-docs.o prune-doc.o write-doc-term.o proximity.o \
	cluster-terms.o extract-titles.o bash-command.o soundex.o \
	write-stems.o hyper-clusters.o file_size.o load-params.o

all:	isr medlineRetrieve

isr:	FrequencyFilter.o preprocess.o Document_Term_Matrix.o weighted-matrices.o \
	RetrieveMedlineMumps doc-doc-weighted.o  \
	idf-weights.o term-term-cohesion.o discrim-coeff.o proxLoad.o \
	cluster-docs.o prune-doc.o write-doc-term.o proximity.o soundex.o \
	cluster-terms.o extract-titles.o write-stems.o hyper-clusters.o \
	file_size.o load-params.o reformat bash-command.o showcount show-rslts

isr:	isr.cpp $(dep)
	$(CPP) -o isr isr.cpp $(OBJ) $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

RetrieveMedlineMumps: RetrieveMedlineMumps.mps $(dep)
	mumpsc RetrieveMedlineMumps.mps

medlineRetrieve: medlineRetrieve.cpp $(dep)
	$(CPP) -o medlineRetrieve medlineRetrieve.cpp load-params.o $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

reformat : reformat.mps $(dep)
	mumpsc reformat.mps

showcount : showcount.mps $(dep)
	mumpsc showcount.mps

show-rslts : show-rslts.mps $(dep)
	mumpsc show-rslts.mps

soundex.o : soundex.cpp $(dep)
	$(CPP) -c soundex.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

proxLoad.o : proxLoad.cpp $(dep)
	$(CPP) -c proxLoad.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

load-params.o : load-params.cpp $(dep)
	$(CPP) -c load-params.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

bash-command.o : bash-command.cpp $(dep)
	$(CPP) -c bash-command.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

hyper-clusters.o : hyper-clusters.cpp $(dep)
	$(CPP) -c hyper-clusters.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

file_size.o : file_size.cpp $(dep)
	$(CPP) -c file_size.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

FrequencyFilter.o : FrequencyFilter.cpp $(dep)
	$(CPP) -c FrequencyFilter.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

write-stems.o : write-stems.cpp $(dep)
	$(CPP) -c write-stems.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

preprocess.o : preprocess.cpp $(dep)
	$(CPP) -c preprocess.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

extract-titles.o : extract-titles.cpp $(dep)
	$(CPP) -c extract-titles.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

cluster-terms.o : cluster-terms.cpp $(dep)
	$(CPP) -c cluster-terms.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

proximity.o : proximity.cpp $(dep)
	$(CPP) -c proximity.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

write-doc-term.o : write-doc-term.cpp $(dep)
	$(CPP) -c write-doc-term.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

prune-doc.o : prune-doc.cpp $(dep)
	$(CPP) -c prune-doc.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

Document_Term_Matrix.o : Document_Term_Matrix.cpp $(dep)
	$(CPP) -c Document_Term_Matrix.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

cluster-docs.o : cluster-docs.cpp $(dep)
	$(CPP) -c cluster-docs.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

weighted-matrices.o : weighted-matrices.cpp $(dep)
	$(CPP) -c weighted-matrices.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

doc-doc-weighted.o : doc-doc-weighted.cpp $(dep)
	$(CPP) -c doc-doc-weighted.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

idf-weights.o : idf-weights.cpp $(dep)
	$(CPP) -c idf-weights.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

term-term-cohesion.o : term-term-cohesion.cpp $(dep)
	$(CPP) -c term-term-cohesion.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)

discrim-coeff.o : discrim-coeff.cpp $(dep)
	$(CPP) -c  discrim-coeff.cpp $(CPP_FLAGS) $(LIBS) $(PKG_LIBS)


clean:
	rm -f *.tmp
	rm -f *.txt
	rm -f *.o
	rm -f key.dat data.dat titles.list nohup.out ohsu.stemmed isr
	rm -f -r segment*
	rm -f DocTerm FreqFilt idf idfcut nedlineRetrieve preproc prox prxld
	rm -f reformat RetrieveMedlineMumps rslt* showcount show-rslts
	rm -f sndx starttime wgt
	rm -f titles.list titles.data ../titles.data ../ohsu.converted
