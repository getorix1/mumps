/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Information Storage and Retrieval Kit
#+
#+     Copyright (C) 2021 by Kevin C. O'Kane
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
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//	March 26, 2024

#include "MDH-interface.h"

#include "definitions.h"

void	showtasks(char *task, int tasknbr, struct STATE *SP) {

	int cpu = atoi(SP->cpu_count);
//	for (int i=0; i < cpu; i++) fprintf(stderr, ".\t");
//	fprintf(stderr, ".\r");
	for (int i=0; i < tasknbr; i++) fprintf(stderr, "\t");
//	fprintf(stderr, "%c%-8s\r", '.', task);
	fprintf(stderr, "%-8s\r", task);
	}

int	main() {

#include "globals.h"

	time_t time1 = time(0), time2;

	struct STATE *SP;

	SP = (STATE *) malloc(sizeof(STATE));

	printf( "Begin isr indexing \n") ;

	load_params(SP);

	int tasknbr = atoi(SP->task);

	SP->docCount = 0;

	char tmp[2048], tmp1[2048], tmpdoc[16];

	FILE *f1 = fopen("titles.list", "r");
	int i;
	if (f1 == NULL) { printf("Missing titles.list\n"); abort();}

	while (fgets(tmp, 2048, f1) != NULL) {
		tmp[strlen(tmp)-1] = 0;
		strcpy(tmp1, tmp);
		strcpy(tmpdoc, strtok(tmp, " "));
		title(tmpdoc) = tmp1;
		}	
	fclose(f1);

	showtasks("preproc", tasknbr, SP);

	preprocess(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("freq", tasknbr, SP);

	FrequencyFilter(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("doc-term", tasknbr, SP);

	doc_term_matrix(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("idf", tasknbr, SP);

	idf_weights(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("wgt mat", tasknbr, SP);

	weighted_matrices(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("soundex", tasknbr, SP);

	soundex(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	if (strcmp(SP->tt_correlations, "yes") == 0) {

		showtasks("term coh", tasknbr, SP);

		term_term_cohesion(SP);
		time2 = time(0);
		printf("	Time elapsed: %d\n", time2 - time1);
		}

	if (strcmp(SP->_discrim, "yes") == 0) {

		showtasks("discrim", tasknbr, SP);

		discrim_coef(SP);

		time2 = time(0);
		printf("	Time elapsed: %d\n", time2 - time1);
		}

	if (strcmp(SP->_proximity, "yes") == 0) {

		showtasks("prox", tasknbr, SP);

		proximity(SP);
		time2 = time(0);
		printf("	Time elapsed: %d\n", time2 - time1);
		}

	if (strcmp(SP->doc_doc_matrix, "yes") == 0) {

		showtasks("doc-doc", tasknbr, SP);

		doc_doc_weighted_matrix(SP);
		time2 = time(0);
		printf("	Time elapsed: %d\n", time2 - time1);
		}

	if (strcmp(SP->document_cluster, "yes") == 0) {

		showtasks("clstr dc", tasknbr, SP);

		cluster_docs(SP);
		time2 = time(0);
		printf("	Time elapsed: %d\n", time2 - time1);

		for (int i=0; i < tasknbr; i++) fprintf(stderr, "\t");
		fprintf(stderr, "%-9s\n", "hyp clstr");

		hyper_clusters(SP);
		time2 = time(0);
		printf("	Time elapsed: %d\n", time2 - time1);
		}

	showtasks("clstr tm", tasknbr, SP);

	cluster_terms(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("prox ld", tasknbr, SP);

	proxLoad(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("wrt stem", tasknbr, SP);

	write_stems(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	printf("End...\n");
	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	showtasks("finished", tasknbr, SP);

	GlobalClose;

	fflush(stdout);
	return 1;
	}
