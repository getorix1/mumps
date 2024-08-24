#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	char tmp[32000];
	int MAX = 0, i = 0;

//	if (argc != 2) { // fcn name, MAX and cpus
//		printf("** Arument count (%d) error to count-seq\n", argc);
//		abort();
//		}

//	MAX = atoi(argv[1]);

	while (1)  {
		if (fgets(tmp, 32000, stdin) == NULL) break;
		if (tmp[0] == '>') i++;
//		if (i > MAX) break;
		}

	printf("%d\n", i - 1);

	}

