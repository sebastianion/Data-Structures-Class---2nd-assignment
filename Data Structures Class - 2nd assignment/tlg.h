/* ION Sebastian-Florentin - 314CB */
/*--- tlg.h ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celulag
{
    void* info;            /* adresa informatie */
    struct celulag *urm;   /* adresa urmatoarei celule */
} TCelulaG, *TLG;          /* tipurile Celula, Lista  */

typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TAfi)(void*, FILE*); /* functie afisare un element */
typedef void (*TF)(void*);          /* functie eliberare un element */

/* functii lista generica */
int InsLG(TLG*, void*);        /* inserare la inceput reusita sau nu (1/0) */
void Distruge(TLG* aL, TF fe); /* distruge lista */
/* afiseaza elementele din lista, folosind o functie de tip TFAfi */
void Afisare(TLG*, TAfi, FILE*);

#endif
