/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+
#+     Mumps Bioinformatics Software Library
#+     Copyright (C) 2003, 2004, 2008, 2012, 2014, 2015, 2017 by Kevin C. O'Kane
#+
#+     Kevin C. O'Kane
#+     kc.okane@gmail.com
#+     http://www.cs.uni.edu/~okane
#+     http://threadsafebooks.com/
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

// June 6, 2020

#include <mumpsc/defines.h>
#include <mumpsc/libmpscpp.h>
#include <mumpsc/config.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <malloc.h>
#include <sys/time.h>
#include <signal.h>
#include <semaphore.h>

#define NATIVE





int DBTimePulse;
int DBSem;

char * pad(char * p, int x) {
    char out[512];
    strncpy(out,p,512);
    while (strlen(out)<x) strcat(out," ");
    return out;
    }

void SyncClock() {
    DBSem = 1;
    }

unsigned int set_alarm (unsigned int seconds) {
    struct itimerval old, xnew;
    xnew.it_interval.tv_usec = 0;
    xnew.it_interval.tv_sec = 0 ; // interval between alarms
    xnew.it_value.tv_usec = 0;
    xnew.it_value.tv_sec = (long int) 0;
    if (setitimer (ITIMER_REAL, &xnew, &old) < 0)
        return 0;
    else
        return old.it_value.tv_sec;
    }

int main(int argc, char **argv) {

    static struct C {
        char * s;
        C *next;
        } *S=NULL,*p1, *p2=NULL;

    unsigned char tmp[STR_MAX];
    unsigned char tmp0[STR_MAX];
    unsigned char tmp1[STR_MAX];

    if ( 0) { // no alarm if slice is zero
        signal(SIGALRM, SyncClock);
        set_alarm(1);
        }

//-----------------------------------------------------

#ifdef SQLITE
#define database "; DBMS: SQLITE"
#endif

#ifdef NATIVE

#define database "; DBMS: Native Stand Alone"

#ifdef CLIENT
#define database "; DBMS: Native Client Server"
#endif

#ifdef NATIVE_SHARED
#define database "; DBMS: Native Shared Stand Alone"
#endif

#ifdef READ_ONLY
#define database "; DBMS: Native Read Only"
#endif

#endif

//-----------------------------------------------------

#ifdef BIGFLOAT
#define floatSize "Float: long double"
#else
#define floatSize "Float: double"
#endif

#define HardwareMath "; Hardware math: yes"
#define Ints "; Int: int"
#define FltDigits ";\nFloat digits: 6"
#define Cache "; Cache: 262145"
#define BlkSiz "; Block: 2048"

#ifdef HAVE_HARDWARE_MATH
#define Fpbits "; FP Bits: N/A"
#else
#define Fpbits "; FP Bits: 72"
#endif

    int DirectRead(int, unsigned char *);
    char * cleanLocks(char *); /* global array lock service */

#include <mumpsc/cgi.h>

    if (argc>1) { // program name
        strncpy((char *)svPtr->program, argv[1],512);
        }
    else strncpy((char *)svPtr->program, argv[0], 512);

    if (argc>=2) {
        $SymPut("%",argv[1]);
        }
    else {
        $SymPut("%","");
        }

    if (argc>2) {
        char t[STR_MAX];

        for (int i=2; i<argc; i++) {
            sprintf(t,"%%%d",i-1);
            $SymPut(t,argv[i]);
            // printf("%d .... %s ..... %s\n",i,t,argv[i]);
            }

        sprintf(t,"%d",argc-2);
        $SymPut((char *)"%c",t);
        $SymPut("%0",argv[1]);
        }

    if (argc>1 && strlen(argv[1])>0) {
        strcpy((char *) tmp,argv[1]);

        if (strncmp((char *) tmp,"./",2)==0) {
            int i;

            for (i=2; tmp[i]; i++)
                tmp[i-2]=tmp[i];

            tmp[i-2]='\0';
            }

        strcpy((char *) tmp1,"s %%=\"\"\"^");
        strcat((char *) tmp1,(char *) tmp);
        strcat((char *) tmp1, "\"\"\" g @%%");
        int retcode = Interpret((const char *) tmp1, svPtr);
        GlobalClose;
        return retcode;
        }

    Interpret("w !,\"Mumps Built: " __TIME__ " " __DATE__ "\n"
              floatSize database HardwareMath Ints FltDigits Cache BlkSiz Fpbits
              "\",!,\"http://threadsafebooks.com/\",!,\"Enter $D or HALT to exit\",!",svPtr);

    char *line ;
    printf("\n");

    while(1) {

#ifdef NATIVE_SHARED
        btree(CLOSE,tmp0,tmp1,svPtr); // prevent deadlocks
#endif

        line = readline ("> ");

        if (line == NULL) { // ^d - end of file
            printf("EOF - goodbye ...\n");
            Mglobal(CLOSE,tmp0,tmp1,svPtr);  /* Close Globals */
            Mltoa(getpid(),tmp1);
            cleanLocks((char *) tmp1);
            puts("\n");
            return 0;
            }

        if (strlen(line)==0) {
            free(line);
            continue;
            }

        add_history(line);

        if (strcasecmp(line,"h")==0 ||
                strcasecmp(line,"\\h")==0 ||
                strcasecmp(line,"\\q")==0 ||
                strcasecmp(line,"\\quit")==0 ||
                strcasecmp(line,"halt")==0 ) {
            printf("EOF - goodbye ...\n");
            Mglobal(CLOSE,tmp0,tmp1,svPtr);  /* Close Globals */
            Mltoa(getpid(),tmp1);
            cleanLocks((char *) tmp1);
            puts("\n");
            return 0;
            }

        Interpret(line,svPtr);
        free(line);
        printf("\n");
        }
    }
