/* ION Sebastian-Florentin - 314CB */
/*--- TCoada.h ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef COADA
#define COADA

typedef void (*TF)(void*);     /* functie eliberare un element */

void* InitQ(size_t d); /* creeaza coada vida cu elemente de dimensiune d */
int InsQ(void* c, void* ae); /* adauga element la sfarsitul cozii */
/* extrage primul element din coada la adresa ae */
int ExtrQ(void* c, void* ae);
int VidaQ(void* c);                 /* test coada vida */
void ResetQ(void* c, TF freeEl);    /* transforma coada in coada vida */
/* elibereaza intregul spatiu ocupat de coada */
void DistrQ(void** c, TF freeEl);

/* rezultat < 0 - eroare
   rezultat = 0 - sursa vida
   rezultat = 1 - succes */

/* concatenare (muta toate elementele din sursa) */
int ConcatQ(void *ad, void *as);

#endif
