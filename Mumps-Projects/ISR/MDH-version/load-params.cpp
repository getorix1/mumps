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

#include "definitions.h"

void	load_params(STATE *SP) {

	SP->max_docs = getenv("max_docs");
	SP->reformat = getenv("reformat");
	SP->zipf = getenv("zipf");
	SP->kwic = getenv("kwic");
	SP->_discrim = getenv("discrim");
	SP->_proximity = getenv("proximity");
	SP->doc_doc_matrix = getenv("doc_doc_matrix");
	SP->document_cluster = getenv("document_cluster");
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
	SP->proximity_min_count = getenv("proximity_min_count");
	SP->prox_dist_factor = getenv("prox_dist_factor");
	SP->min_doc_doc_cooccur = getenv("min_doc_doc_cooccur");
	SP->min_dd_cosine = getenv("min_dd_cosine");
	SP->min_dd_cohesion = getenv("min_dd_cohesion");
	SP->jaccard_min = getenv("jaccard_min");
	SP->min_cluster_cosine = getenv("min_cluster_cosine");
	SP->hyper_min = getenv("hyper_min");
	SP->hyper_wgt = getenv("hyper_wgt");
	SP->min_tt_cluster = getenv("min_tt_cluster");
	SP->task = getenv("task");
	SP->cpu_count = getenv("cpu_count");

	if (SP->max_docs != NULL) printf("max_docs = %s\n", SP->max_docs);
	if (SP->reformat != NULL) printf("reformat = %s\n", SP->reformat);
	if (SP->zipf != NULL) printf("zipf = %s\n", SP->zipf);
	if (SP->kwic != NULL) printf("kwic = %s\n", SP->kwic);
	if (SP->_discrim != NULL) printf("_discrim = %s\n", SP->_discrim);
	if (SP->_proximity != NULL) printf("_proximity = %s\n", SP->_proximity);
	if (SP->doc_doc_matrix != NULL) printf("doc_doc_matrix = %s\n", SP->doc_doc_matrix);
	if (SP->document_cluster != NULL) printf("document_cluster = %s\n", SP->document_cluster);
	if (SP->tt_correlations != NULL) printf("tt_correlations = %s\n", SP->tt_correlations);
	if (SP->DB != NULL) printf("DB = %s\n", SP->DB);
	if (SP->min_word_length != NULL) printf("min_word_length = %s\n", SP->min_word_length);
	if (SP->min_doc_vector_word_count != NULL) printf("min_doc_vector_word_count = %s\n", SP->min_doc_vector_word_count);
	if (SP->min_word_count != NULL) 
		printf("min_word_count = %s\n", SP->min_word_count);
	if (SP->max_word_count != NULL) 
		printf("max_word_count = %s\n", SP->max_word_count);
	if (SP->min_doc_word_count != NULL) 
		printf("min_doc_word_count = %s\n", SP->min_doc_word_count);
	if (SP->min_idf_vector_count != NULL) 
		printf("min_idf_vector_count = %s\n", SP->min_idf_vector_count);
	if (SP->min_doc_weight != NULL) 
		printf("min_doc_weight = %s\n", SP->min_doc_weight);
	if (SP->min_tt_correlation != NULL) 
		printf("min_tt_correlation = %s\n", SP->min_tt_correlation);
	if (SP->min_tt_cooccur != NULL) 
		printf("min_tt_cooccur = %s\n", SP->min_tt_cooccur);
	if (SP->proximity_min_count != NULL) 
		printf("proximity_min_count = %s\n", SP->proximity_min_count);
	if (SP->prox_dist_factor != NULL) 
		printf("prox_dist_factor = %s\n", SP->prox_dist_factor);
	if (SP->min_doc_doc_cooccur != NULL) 
		printf("min_doc_doc_cooccur = %s\n", SP->min_doc_doc_cooccur);
	if (SP->min_dd_cosine != NULL) 
		printf("min_dd_cosine = %s\n", SP->min_dd_cosine);
	if (SP->min_dd_cohesion != NULL) 
		printf("min_dd_cohesion = %s\n", SP->min_dd_cohesion);
	if (SP->jaccard_min != NULL) 
		printf("jaccard_min = %s\n", SP->jaccard_min);
	if (SP->min_cluster_cosine != NULL) 
		printf("min_cluster_cosine = %s\n", SP->min_cluster_cosine);
	if (SP->hyper_min != NULL) 
		printf("hyper_min = %s\n", SP->hyper_min);
	if (SP->hyper_wgt != NULL) 
		printf("hyper_wgt = %s\n", SP->hyper_wgt);
	if (SP->min_tt_cluster != NULL) 
		printf("min_tt_cluster = %s\n", SP->min_tt_cluster);

	return;
	}
