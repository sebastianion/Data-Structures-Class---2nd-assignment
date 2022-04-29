/* ION Sebastian-Florentin - 314CB */
/*--- TStivaLista.h ---*/
#include "TStiva.h"

#ifndef _STIVA_LISTA_
#define _STIVA_LISTA_

typedef struct celst
{
    struct celst *urm; /* adresa celulei urmatoare */
    void *info;        /* adresa informatie */
} TCelSt, *ACelSt;

typedef struct stiva
{ 
	size_t 	dime; 	   /* dimensiunea unui element al stivei */
	ACelSt 	vf; 	   /* adresa celulei din varful stivei */
} TStiva, *ASt;

/* macrodefinitii - acces campuri */
#define DIMES(s) (((ASt)(s))->dime)
#define VFS(s) 	(((ASt)(s))->vf)

/* verifica daca doua stive au elemente de dimensiuni diferite */
#define DIMDIFS(s,d)	(DIMES(s) != DIMES(d))
/* verifica daca o stiva este vida */
#define VIDAS(s)  	(VFS(s) == NULL)

#endif
