/* ION Sebastian-Florentin - 314CB */
/*--- TCoadaLista.h ---*/
#include "TCoada.h"

#ifndef _COADA_LISTA_
#define _COADA_LISTA_

typedef struct cel
{
    struct cel *urm;
    void *info;
} TCel, *ACel;

typedef struct
{ 
	size_t 	dime; 		/* dimensiunea unui element din coada */
    ACel ic, sc;        /* adresa primei celule; adresa ultimei celule */
} TCoada, *AQ;

/* macrodefinitii - acces campuri */
#define IC(c) 	(((AQ)(c))->ic)
#define SC(c) 	(((AQ)(c))->sc)
#define DIMEC(c) (((AQ)(c))->dime)

/* verifica daca doua cozi au elemente de dimensiuni diferite */
#define DIMDIFC(s,d) 	(DIMEC(s) != DIMEC(d))  
/* verifica daca o coada este vida */
#define VIDAC(c) 		(IC(c) == NULL && SC(c) == NULL)

#endif