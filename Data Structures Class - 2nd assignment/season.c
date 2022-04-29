/* ION Sebastian-Florentin - 314CB */
/*--- season.c ---*/
#include "season.h"

void freeSeason(void *el)
{
    TSeason *season = (TSeason *)el;

    /* eliberare memorie coada episoade */
    DistrQ(&(season->episodes), free);
    free(season->episodes);
    free(season);
    season = NULL;
}