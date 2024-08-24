#include <iostream>
#include <string>
#include <time.h>
#include<string.h>

using namespace std;

int main() {

char str[1000];
string s1;

time_t t1 = time(0);

for (long i=0; i< 1000000000; i++) {
	strcpy(str,"now is the time for all");
	strcat(str,"good men to come to the aid of the party");
	}

printf("time %ld\n", time(0) - t1);

t1 = time(0);

for (long i=0; i< 1000000000; i++) {
	s1 = "now is the time for all";
	s1 += "good men to come to the aid of the party";
	}

printf("time %ld\n", time(0) - t1);

return 0;
}
