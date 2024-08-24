#*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Internet Radio Automation & Encoding Toolkit
#+
#+     Copyright (C) 2018, 2019, 2020, 2021 by Kevin C. O'Kane
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

#       May 15, 2024

dep	=	/usr/bin/mumps

isr:	zipf kwic kwoc trunc showcount show-rslts sentences \
	DocumentTermMatrix idf idf-cutoff tt1 proximity \
	docdoc1 discrim weight ttt loadTitles clusterdd hypercluster \
	soundex proxLoad RetrieveMedlineMumps jaccardtt cohesion \
	starttime endtime show-rslts clustertt map preprocess reformat chop boolean

chop : chop.mps $(dep)
	mumpsc chop.mps

sentences : sentences.mps $(dep)
	mumpsc sentences.mps

show-rslts : show-rslts.mps $(dep)
	mumpsc show-rslts.mps

showcount : showcount.mps $(dep)
	mumpsc showcount.mps

boolean : boolean.mps $(dep)
	mumpsc boolean.mps

starttime : starttime.mps $(dep)
	mumpsc starttime.mps

endtime : endtime.mps $(dep)
	mumpsc endtime.mps

zipf : zipf.mps $(dep)
	mumpsc zipf.mps

kwic : kwic.mps $(dep)
	mumpsc kwic.mps

kwoc : kwoc.mps $(dep)
	mumpsc kwoc.mps

trunc : trunc.mps $(dep)
	mumpsc trunc.mps

DocumentTermMatrix : DocumentTermMatrix.mps $(dep)
	mumpsc DocumentTermMatrix.mps

idf : idf.mps $(dep)
	mumpsc idf.mps

idf-cutoff : idf-cutoff.mps $(dep)
	mumpsc idf-cutoff.mps

tt1 : tt1.mps $(dep)
	mumpsc tt1.mps

proximity : proximity.mps $(dep)
	mumpsc proximity.mps

docdoc1 : docdoc1.mps $(dep)
	mumpsc docdoc1.mps

discrim : discrim.mps $(dep)
	mumpsc discrim.mps

weight : weight.mps $(dep)
	mumpsc weight.mps

ttt : ttt.mps $(dep)
	mumpsc ttt.mps

loadTitles : loadTitles.mps $(dep)
	mumpsc loadTitles.mps

clusterdd : clusterdd.mps $(dep)
	mumpsc clusterdd.mps

hypercluster : hypercluster.mps $(dep)
	mumpsc hypercluster.mps

soundex : soundex.mps $(dep)
	mumpsc soundex.mps

proxLoad : proxLoad.mps $(dep)
	mumpsc proxLoad.mps

RetrieveMedlineMumps : RetrieveMedlineMumps.mps $(dep)
	mumpsc RetrieveMedlineMumps.mps

jaccardtt : jaccardtt.mps $(dep)
	mumpsc jaccardtt.mps

cohesion : cohesion.mps $(dep)
	mumpsc cohesion.mps

clustertt : clustertt.mps $(dep)
	mumpsc clustertt.mps

map : map.mps $(dep)
	mumpsc map.mps

reformat : reformat.mps $(dep)
	mumpsc reformat.mps

preprocess : preprocess.mps $(dep)
	mumpsc preprocess.mps

clean:
	rm -f *.tmp
	rm -f *.txt
	rm -f *.cpp
	rm -f *.o
	rm -f DocumentTermMatrix idf idf-cutoff tt1 proximity \
		docdoc1 discrim weight ttt loadTitles clusterdd hypercluster \
		soundex proxLoad RetrieveMedlineMumps jaccardtt cohesion \
		jLoad dcount kwic kwoc query1 query2 query3 show-rslts \
		trunc sentences fasterRetrieval hclusterprint \
		simpleRetrieval slides.boolean showcount starttime WordList zipf \
		endtime  jLoad clustertt map preprocess reformat chop boolean 
	rm -f stop.words key.dat data.dat DBPREFIX DocCount tmp tmp1 titles.list \
		MAXDOCS nohup.out  titles.data
	rm -r -f segment*
	rm -f *.cpp
	rm -f *.orig
	rm -f ../*.converted ../titles.*
