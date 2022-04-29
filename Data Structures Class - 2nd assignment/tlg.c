/* ION Sebastian-Florentin - 314CB */
/*--- tlg.c ---*/
#include "tlg.h"

int InsLG(TLG* aL, void* ae)
{
	TLG aux = malloc(sizeof(TCelulaG));
	if (!aux)
	    return 0;

	aux->info = ae;
	aux->urm = *aL;
	*aL = aux;

	return 1;
}

void Distruge(TLG* aL, TF freeEl) /* distruge lista */
{
	while (*aL != NULL)
    {
        TLG aux = *aL;     	   /* adresa celulei eliminate */
        if (!aux)
            return;

        freeEl(aux->info);     /* eliberare spatiul ocupat de element */
		*aL = aux->urm;    	   /* deconecteaza celula din lista */
        free(aux);   		   /* elibereaza spatiul ocupat de celula */
	}
}

void Afisare(TLG* aL, TAfi afiEL, FILE *out)
{
	if (!*aL) {
        fprintf(out, "[].\n");
        return;
    }

	fprintf(out, "[");
	afiEL((*aL)->info, out);
	aL = &(*aL)->urm;

	for (; *aL; aL = &(*aL)->urm) {
		fprintf(out, ", ");
		afiEL((*aL)->info, out);
    }
	fprintf(out, "].\n");
}
