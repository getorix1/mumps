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

//	April 30, 2023

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <semaphore.h>
#include <stdlib.h>

#include <mumpsc/libmpscpp.h>

#define MARK "\1"

#define DF_LIMIT 9999
#define DF_MIN 2

struct STATE {
	int docCount;

	char * max_docs;
	char * prox_load;
	char * reformat;
	char * zipf;
	char * kwic;
	char * tt_correlations;
	char * min_tt_correlation;
	char * DB = getenv("DB");
	char * min_word_length;
	char * min_doc_vector_word_count;
	char * min_word_count;
	char * max_word_count;
	char * min_doc_word_count;
	char * min_idf_vector_count;
	char * min_doc_weight;
	char * min_tt_cooccur;
	char * min_dd_cosine;
	char * min_dd_cohesion;
	char * jaccard_min;
	char * hyper_min;
	char * hyper_wgt;
	char * prox_dist_factor;
	} ;

void	load_params(STATE *);
void    doc_term_matrix(STATE *);
void    compute_idf(STATE *);
void    extract_titles(STATE *);
void    stop_list(STATE *);
void    word_counts(STATE *);
void    build_dict(STATE *);
void    build_df(STATE *);
void    del_high_low(STATE *);
void    prune_doc(STATE *);
void    del_dead_docs(STATE *);
void    idf_weights(STATE *);
void    weighted_matrices(STATE *);
void    write_stems(STATE *);
void    preprocess(STATE *);
void    file_size(char *);
void    file_size1(char *);
void	bash_command(char *);


#endif

