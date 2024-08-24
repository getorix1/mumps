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

//	Nov 15, 2022

#include <mumpsc/libmpscpp.h>

#include "MDH-interface.h"

#include "definitions.h"

int	main() {

#include "globals.h"

	time_t time1 = time(0), time2;

	struct STATE *SP;

	SP = (STATE *) malloc(sizeof(STATE));

	printf( "Begin isr indexing \n") ;

	load_params(SP);

	SP->docCount = 0;

	compute_idf(SP);

	time2 = time(0);
	printf("	Time elapsed: %d\n", time2 - time1);

	printf("End...\n");
	time2 = time(0);
	printf("	Time elapsed: %d seconds, %d minutes\n", 
		time2 - time1, (time2 - time1) / 60);

	GlobalClose;

	fflush(stdout);
	return 1;
	}
