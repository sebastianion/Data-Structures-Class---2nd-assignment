/* ION Sebastian-Florentin - 314CB */
/*--- TStivaLista.c ---*/
#include "TStivaLista.h"

/* creeaza stiva vida cu elemente de dimensiune d */
void *InitS(size_t d)
{ 
	ASt s = (ASt)malloc(sizeof (TStiva));
  	
	if (!s)
		return NULL;

  	s->dime = d;
	s->vf = NULL;

  	return (void *)s;
}

/* pune element in varful stivei */
int Push(void *s, void *ae)
{ 
    ACelSt aux;
    aux = (ACelSt)malloc(sizeof(TCelSt));
    if (!aux)
        return 0;

    aux->info = ae; /* adresa alocata per element */
    aux->urm = VFS(s);
    VFS(s) = aux;

  	return 1;
}

/* extrage elementul din varful stivei la adresa ae */
int Pop(void *s, void *ae)
{ 
	ACelSt aux = VFS(s);
    if (!aux)
        return 0;
    
    memcpy(ae, aux->info, DIMES(s));
    VFS(s) = aux->urm;
	free(aux->info);
    free(aux);

    return 1;
}

/* test stiva vida */
int VidaS(void *s)       
{ 
	return VIDAS(s);
}

/* transforma stiva in stiva vida */
void ResetS(void *s, TF freeEl)
{ 
	ACelSt aux = VFS(s);
    ACelSt p;
    while (aux) {
        p = aux;
        aux = aux->urm;
        freeEl(p->info);
        free(p);
    }
    VFS(s) = NULL;
}

/* elibereaza intregul spatiu ocupat de stiva */
void DistrS(void **s, TF freeEl)
{
    ResetS(*s, freeEl);
	free(*s);
  	*s = NULL;         
}

/* suprapune inversul sursei destinatie */
int Rastoarna(void *sd, void *ss)
{ 
  	if (DIMDIFS(ss,sd)) 
		return -1;      	/* dimensiuni diferite */
  	if (VIDAS(ss)) 
		return 0;           /* sursa vida */

  	ACelSt p = VFS(ss), u;
    
    while (p) {
        u = p->urm;
        p->urm = VFS(sd);
        VFS(sd) = p;
        p = u;
    }

    VFS(ss) = NULL;

	return 1;
}