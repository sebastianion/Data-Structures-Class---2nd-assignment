/* ION Sebastian-Florentin - 314CB */
/*--- season.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlg.h"
#include "TCoada.h"
#include "TStiva.h"
#include "TCoadaLista.h"
#include "TStivaLista.h"

#ifndef _SEASON_
#define _SEASON_

typedef struct {
    int nrOfEp; /* numar de episoade pentru un sezon */
    void *episodes; /* coada episaode */
} TSeason;

void freeSeason(void *el);

#endif
