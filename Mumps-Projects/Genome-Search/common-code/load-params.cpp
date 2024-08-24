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

//	May 6, 2023

#include "definitions.h"

void	load_params(STATE *SP) {

	SP->prox_load = getenv("prox_load");
	SP->max_docs = getenv("max_docs");
	SP->reformat = getenv("reformat");
	SP->zipf = getenv("zipf");
	SP->kwic = getenv("kwic");
	SP->tt_correlations = getenv("tt_correlations");
	SP->min_tt_correlation = getenv("min_tt_correlation");
	SP->DB = getenv("DB");
	SP->min_word_length = getenv("min_word_length");
	SP->min_doc_vector_word_count = getenv("min_doc_vector_word_count");
	SP->min_word_count = getenv("min_word_count");
	SP->max_word_count = getenv("max_word_count");
	SP->min_doc_word_count = getenv("min_doc_word_count");
	SP->min_idf_vector_count = getenv("min_idf_vector_count");
	SP->min_doc_weight = getenv("min_doc_weight");
	SP->min_tt_cooccur = getenv("min_tt_cooccur");
	SP->prox_dist_factor = getenv("prox_dist_factor");
	SP->min_dd_cosine = getenv("min_dd_cosine");
	SP->min_dd_cohesion = getenv("min_dd_cohesion");
	SP->jaccard_min = getenv("jaccard_min");
	SP->hyper_min = getenv("hyper_min");
	SP->hyper_wgt = getenv("hyper_wgt");

	if (SP->max_docs) printf("max_docs = %s\n", SP->max_docs);
	if (SP->reformat) printf("reformat = %s\n", SP->reformat);
	if (SP->zipf) printf("zipf = %s\n", SP->zipf);
	if (SP->kwic) printf("kwic = %s\n", SP->kwic);
	if (SP->tt_correlations) printf("tt_correlations = %s\n", SP->tt_correlations);
	printf("\n");
	if (SP->DB) printf("DB = %s\n", SP->DB);
	if (SP->min_word_length) printf("min_word_length = %s\n", SP->min_word_length);
	if (SP->min_doc_vector_word_count) 
		printf("min_doc_vector_word_count = %s\n", SP->min_doc_vector_word_count);
	if (SP->min_word_count) printf("min_word_count = %s\n", SP->min_word_count);
	if (SP->max_word_count) printf("max_word_count = %s\n", SP->max_word_count);
	if (SP->min_doc_word_count) printf("min_doc_word_count = %s\n", SP->min_doc_word_count);
	if (SP->min_idf_vector_count) printf("min_idf_vector_count = %s\n", SP->min_idf_vector_count);
	if (SP->min_doc_weight) printf("min_doc_weight = %s\n", SP->min_doc_weight);
	if (SP->min_tt_correlation) printf("min_tt_correlation = %s\n", SP->min_tt_correlation);
	if (SP->min_tt_cooccur) printf("min_tt_cooccur = %s\n", SP->min_tt_cooccur);
	if (SP->prox_dist_factor) printf("prox_dist_factor = %s\n", SP->prox_dist_factor);
	if (SP->min_dd_cosine) printf("min_dd_cosine = %s\n", SP->min_dd_cosine);
	if (SP->min_dd_cohesion) printf("min_dd_cohesion = %s\n", SP->min_dd_cohesion);
	if (SP->jaccard_min) printf("jaccard_min = %s\n", SP->jaccard_min);
	if (SP->hyper_min) printf("hyper_min = %s\n", SP->hyper_min);
	if (SP->hyper_wgt) printf("hyper_wgt = %s\n", SP->hyper_wgt);

	return;
	}
