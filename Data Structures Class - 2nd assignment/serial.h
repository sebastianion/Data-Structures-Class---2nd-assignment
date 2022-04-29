/* ION Sebastian-Florentin - 314CB */
/*--- serial.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlg.h"
#include "TCoada.h"
#include "TStiva.h"
#include "TCoadaLista.h"
#include "TStivaLista.h"

#ifndef _SERIAL_
#define _SERIAL_

typedef struct {
    int poz; /* pozitia serialului in top (doar pentru serialele din top10) */
    int ID; /* categoria serialului; 
            pentru elementele din top, ID este initializat cu -1 */
    char *name; /* numele serialului */
    float rating; /* rating-ul serialului */
    int nrOfSeasons; /* numarul de sezoane ale serialului */
    void *seasons; /* coada sezaone */
} TSerial;

int cmpSerial(void *a, void *b);
void freeSerial(void *el);
TSerial * findSerial(char *name, TLG L);
TSerial * findSerialQ(char *name, void *queue);
TSerial * findSerialQIns(char *name, void *queue);
TSerial * findSerialS(char *name, void *stack);
TSerial * findSerialSIns(char *name, void *stack);

#endif
