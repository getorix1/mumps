// April 5, 2024

/*===========================================================================*
 *                                  keyfix                                   *
 *===========================================================================*/

static inline void keyfix (unsigned char *vxd) { 

    /************************************************************************
    *                                                                       *
    *       remove leading ^.  convert high codes to 1 (comma, paren)       *
    *                                                                       *
    ************************************************************************/

    if (*vxd == '^') {
        while (*vxd) { 
		*vxd = *(vxd + 1) & 0x80 ? 1 : *(vxd + 1); 
		vxd++; 
		}
	return;
	}

    /************************************************************************
    *                                                                       *
    *       convert markers to 1's                                          *
    *                                                                       *
    ************************************************************************/

    while (*vxd) {
        if (*vxd & 0x80 ) *vxd = 1 ;
        vxd++;
	}

    return;

    }

