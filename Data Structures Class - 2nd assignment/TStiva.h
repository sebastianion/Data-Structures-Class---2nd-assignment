/* ION Sebastian-Florentin - 314CB */
/*--- TStiva.h ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef STIVA
#define STIVA

typedef void (*TF)(void*);    /* functie eliberare un element */

void* InitS(size_t d); /* initializeaza stiva cu elemente de dimensiune d */
int Push(void* s, void* ae);  /* pune element in varful stivei */
/* extrage elementul din varful stivei la adresa ae */
int Pop (void* s, void* ae);
int VidaS(void* s);                     /* test stiva vida */
void ResetS (void* s, TF freeEl);       /* transforma stiva in stiva vida */
/* elibereaza intregul spatiu ocupat de stiva */
void DistrS (void** s, TF freeEl);

/* rezultat < 0 - eroare
   rezultat = 0 - sursa vida
   rezultat = 1 - succes */

/* suprapune inversul sursei peste destinatie */
int Rastoarna(void* ad, void* as);

#endif
