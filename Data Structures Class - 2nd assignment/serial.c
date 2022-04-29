/* ION Sebastian-Florentin - 314CB */
/*--- serial.c ---*/
#include "serial.h"
#include "season.h"

int cmpSerial(void *a, void *b)
{
    TSerial *serial1 = (TSerial *)a;
    TSerial *serial2 = (TSerial *)b;
    if (serial1->rating < serial2->rating)
        return -1;                                   /* rating 1 < rating 2 */
    else if (serial1->rating > serial2->rating)
        return 1;                                    /* rating 1 > rating 2 */
    
    if (strcmp(serial1->name, serial2->name) < 0)    /* rating 1 == rating 2 */
        return 1;
    else if (strcmp(serial1->name, serial2->name) > 0)
        return -1;    

    return 0;                                       
}

void freeSerial(void *el)
{
    TSerial *serial = (TSerial *)el;
    
    /* eliberare memorie coada sezoane */
    DistrQ(&(serial->seasons), freeSeason);
    free(serial->seasons);
    free(serial->name);
    free(serial);
    serial = NULL;
}

/* cautare serial in listele categoriilor */
TSerial * findSerial(char *name, TLG L)
{
    TLG u1 = L;
    for (; u1 != 0; u1 = u1->urm) {
        if (strcmp(((TSerial *)(u1->info))->name, name) == 0) {
            return (TSerial *)(u1->info);
        }
    }

    return NULL;
}

/* cautare serial in coada, fara inserare */
TSerial * findSerialQ(char *name, void *queue)
{
    int r;
    TSerial *auxSerial, *serial = NULL;
    void *aux = InitQ(sizeof(TSerial)); /* coada auxiliara */
    while (!VidaQ(queue)) {
        auxSerial = malloc(sizeof(TSerial));
        ExtrQ(queue, auxSerial);
        if (strcmp(name, auxSerial->name) == 0) {
            serial = auxSerial;
        } else {
            r = InsQ(aux, auxSerial);
            if (r != 1)
                return NULL;
        }
    }

    if (!VidaQ(aux)) {
        r = ConcatQ(queue, aux);
        if (r == 0)
            return NULL;
    }

    DistrQ(&aux, freeSerial);

    return serial;
}

/* cautare serial in coada + inserare inapoi */
TSerial * findSerialQIns(char *name, void *queue)
{
    int r;
    TSerial *auxSerial, *serial = NULL;
    void *aux = InitQ(sizeof(TSerial)); /* coada auxiliara */
    while (!VidaQ(queue)) {
        auxSerial = malloc(sizeof(TSerial));
        ExtrQ(queue, auxSerial);
        if (strcmp(name, auxSerial->name) == 0)
            serial = auxSerial;
        r = InsQ(aux, auxSerial);
        if (r != 1)
            return NULL;
    }

    if (!VidaQ(aux)) {
        r = ConcatQ(queue, aux);
        if (r == 0)
            return NULL;
    }

    DistrQ(&aux, freeSerial);

    return serial;
}

/* cautare serial in stiva, fara inserare */
TSerial * findSerialS(char *name, void *stack)
{
    int r;
    TSerial *auxSerial, *serial = NULL;
    void *aux = InitS(sizeof(TSerial)); /* stiva auxiliara */
    while (!VidaS(stack)) {
        auxSerial = malloc(sizeof(TSerial));
        Pop(stack, auxSerial);
        if (strcmp(name, auxSerial->name) == 0)
            serial = auxSerial;
        else {
            r = Push(aux, auxSerial);
            if (r != 1)
                return NULL;
        }
    }
    
    if (!VidaS(aux)) {
        r = Rastoarna(stack, aux);
        if (r == 0)
            return NULL;
    }

    DistrS(&aux, freeSerial);

    return serial;
}

/* cautare serial in stiva + inserare inapoi */
TSerial * findSerialSIns(char *name, void *stack)
{
    int r;
    TSerial *auxSerial, *serial = NULL;
    void *aux = InitS(sizeof(TSerial)); /* stiva auxiliara */
    while (!VidaS(stack)) {
        auxSerial = malloc(sizeof(TSerial));
        Pop(stack, auxSerial);
        if (strcmp(name, auxSerial->name) == 0)
            serial = auxSerial;
        r = Push(aux, auxSerial);
        if (r != 1)
            return NULL;
    }
    
    if (!VidaS(aux)) {
        r = Rastoarna(stack, aux);
        if (r == 0)
            return NULL;
    }

    DistrS(&aux, freeSerial);

    return serial;
}