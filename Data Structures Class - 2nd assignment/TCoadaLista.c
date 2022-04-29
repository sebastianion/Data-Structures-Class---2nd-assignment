/* ION Sebastian-Florentin - 314CB */
/*--- TCoadaLista.c ---*/
#include "TCoadaLista.h"

/* creeaza coada vida cu elemente de dimensiune d */
void* InitQ(size_t d)
{ 
	AQ c;
    c = (AQ)malloc(sizeof(TCoada));
    if (!c)
        return NULL;
    
    c->dime = d;
    c->ic = NULL;
    c->sc = NULL;   

  	return (void*)c;
}

/* adauga element la sfarsitul cozii */
int InsQ(void *c, void *ae)
{   
    ACel aux;
    aux = (ACel)malloc(sizeof(TCel));
    if (!aux)
        return -1;
        
    aux->info = ae;       /* ae alocat per element */
    aux->urm = NULL;

    if (VIDAC(c)) {       /* coada vida */
        IC(c) = aux;
        SC(c) = aux;
    } else {              /* coada nevida */
        SC(c)->urm = aux;
        SC(c) = aux;
    }

  	return 1;
}

/* extrage primul element din coada la adresa ae */
int ExtrQ (void *c, void *ae)
{   
	if (VIDAC(c)) 
		return 0;

    ACel aux = IC(c);

    IC(c) = aux->urm;
    
    /* copiaza elementul de la inceputul cozii */
	memcpy(ae, aux->info, DIMEC(c));
    
    free(aux->info);
    free(aux);

    if (IC(c) == NULL) /* test daca a fost un singur element in coada */
        SC(c) = NULL;

  	return 1;
}

/* test coada vida */
int VidaQ(void *c) 
{ 
	return VIDAC(c); 
}

/* transforma coada in coada vida */
void ResetQ(void *c, TF freeEl)
{ 
    while (IC(c) != NULL)
    {
        ACel aux = IC(c);   /* adresa celulei eliminate */
        if (!aux)
            return;

        freeEl(aux->info);  /* elib.spatiul ocupat de element*/
        IC(c) = aux->urm;   /* deconecteaza celula din lista */
        free(aux);          /* elibereaza spatiul ocupat de celula */
    }

	IC(c) = SC(c) = NULL;   /* inceput si sfarsit coada = inceput */
}  

/* elibereaza intregul spatiu ocupat de coada */
void DistrQ(void **c, TF freeEl) 
{ 
    ResetQ(*c, freeEl);
  	free(*c);		/* elibereaza spatiul alocat pentru descriptor 	*/
  	*c = NULL;
}

/* concatenare (muta toate elementele din sursa) */
int ConcatQ(void *cd, void *cs)
{ 
	if (DIMDIFC(cs, cd)) 
		return -1;  		/* dimensiuni diferite */
  	if (VIDAC(cs)) 
		return 0;        	/* sursa vida */
    if (VIDAC(cd)) {
        IC(cd) = IC(cs);
        SC(cd) = SC(cs);
        IC(cs) = SC(cs) = NULL;
        return 1;
    }

    SC(cd)->urm = IC(cs);
    SC(cd) = SC(cs);
    IC(cs) = SC(cs) = NULL;

  	return 1;
}