#include <mumpsc/btree.h>
#include <mumpsc/global.h>
#include <mumpsc/sym.h>

// April 29, 2024


inline void GlobalOrder(unsigned char * tmp0, unsigned char * tmp1, unsigned char * tmp2, struct MSV * svPtr) {

    for (int i = 0; tmp0[i] != 0; i++) if (tmp0[i] > 127) tmp0[i] = 1;

    if (tmp2==NULL) { // direction arg
        Mglobal(ORDERNEXT,tmp0, tmp1, svPtr);
        return;
        }

    if (strcmp( (char *) tmp2,"1")==0) {
        Mglobal(ORDERNEXT, tmp0, tmp1, svPtr);
        return;
        }

    if (strcmp( (char *) tmp2,"-1")==0) {
        Mglobal(ORDERPREV, tmp0, tmp1, svPtr);
        return;
        }

    ErrorMessage("$order() second argument incorrect", svPtr->LineNumber);
    }


