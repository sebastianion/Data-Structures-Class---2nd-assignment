/* ION Sebastian-Florentin - 314CB */
/*--- main.c ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlg.h"
#include "TCoada.h"
#include "TStiva.h"
#include "TCoadaLista.h"
#include "TStivaLista.h"
#include "serial.h"
#include "season.h"

#define MAX 33

void AfiEl(void *el, FILE *out)
{
    TSerial *serial = (TSerial *)el;
    fprintf(out, "(%s, %.1f)", serial->name, serial->rating);
}

int AfisareQ(FILE *out, void *queue)
{
    void *serial;
    void *aux = InitQ(sizeof(TSerial)); /* coada auxiliara */
    if (VidaQ(queue))
        fprintf(out, "[].\n");
    else {
        serial = malloc(sizeof(TSerial));
        ExtrQ(queue, serial);
        fprintf(out, "[(%s, %.1f)", ((TSerial *)serial)->name,
                ((TSerial *)serial)->rating);
        InsQ(aux, serial);

        while (!VidaQ(queue)) {
            serial = malloc(sizeof(TSerial));
            ExtrQ(queue, serial);
            fprintf(out, ", (%s, %.1f)", ((TSerial *)serial)->name,
                    ((TSerial *)serial)->rating);
            InsQ(aux, serial);
        }
        fprintf(out, "].\n");
        int r = ConcatQ(queue, aux);
        if (r == 0)
            return -1;
    }

    DistrQ(&aux, freeSerial); /* distruge coada auxiliara */

    return 1;
}

int AfisareS(FILE *out, void *stack)
{
    void *serial;
    void *aux = InitS(sizeof(TSerial)); /* stiva auxiliara */
    if (VidaS(stack))
        fprintf(out, "[].\n");
    else {
        serial = malloc(sizeof(TSerial));
        Pop(stack, serial);
        fprintf(out, "[(%s, %.1f)", ((TSerial *)serial)->name,
                ((TSerial *)serial)->rating);
        Push(aux, serial);

        while (!VidaS(stack)) {
            serial = malloc(sizeof(TSerial));
            Pop(stack, serial);
            fprintf(out, ", (%s, %.1f)", ((TSerial *)serial)->name,
                    ((TSerial *)serial)->rating);
            Push(aux, serial);
        }
        fprintf(out, "].\n");
        int r = Rastoarna(stack, aux);
        if (r == 0)
            return -1;
    }
    
    DistrS(&aux, freeSerial); /* distruge stiva auxiliara */

    return 1;
}

int inserareCat(TLG *L, void *serial, int *dimL, FILE *out, TFCmp cmp)
{
    int r; /* cod returnat de inserarea in lista */
    if (!*L) { /* lista vida, inserare pe prima pozitie */
        r = InsLG(L, serial);
        if (r == 0)
            return -1;
        (*dimL)++;
        fprintf(out, "Serialul %s a fost adaugat la pozitia 1.\n",
                ((TSerial *)((*L)->info))->name);
        return 1;
    }

    int p = 1, order;
    TLG aux = *L, u;
    do {
        order = cmpSerial(aux->info, serial);
        if (order == -1) {
            r = InsLG(&aux, serial);
            if (r == 0)
                return -1;
            (*dimL)++;
            fprintf(out, "Serialul %s a fost adaugat la pozitia %d.\n",
                    ((TSerial *)(aux->info))->name, p);

            if (p == 1) {  /* inserare pe prima pozitie */
                *L = aux;
            } else
                u->urm = aux; /* refacerea legaturilor */

            return 1;
        }
        p++;
        u = aux;
        aux = aux->urm;
    } while (aux);

    r = InsLG(&(u->urm), serial);
    if (r == 0)
        return -1;
    (*dimL)++;
    fprintf(out, "Serialul %s a fost adaugat la pozitia %d.\n",
            ((TSerial *)(u->urm->info))->name, p);

    return 1;
}

int inserareTop(TLG *L, void *serial, int *dimL, FILE *out)
{
    int r; /* cod returnat de inserarea in lista */
    if (!*L) { /* lista vida, inserare pe prima pozitie */
        r = InsLG(L, serial);
        if (r == 0)
            return -1;
        (*dimL)++;
        return 1;
    }
    
    int p = ((TSerial *)serial)->poz;
    TLG aux = *L;
    int i;
    
    for (i = 0; i < p - 2; ++i)
        aux = aux->urm; /* ajunge pe pozitia la care se efectueaza inserarea */

    if (p == 1) {
        r = InsLG(L, serial);
        if (r == 0)
            return -1;
    } else { 
        r = InsLG(&(aux->urm), serial);
        if (r == 0)
            return -1;
    }

    (*dimL)++;
    
    if ((*dimL) > 10) {
        TLG last = *L, prelast;
        for (i = 0; i < 10; ++i) {
            prelast = last;
            last = last->urm;
        }

        Distruge(&last, freeSerial);
        prelast->urm = NULL;
        *dimL = 10;
    }

    return 1;
}

int remainingTime(TSerial *serial, int offset)
{
    int i, minutes = 0, quit = 0, r;
    TSeason *season;
    void *duration;
    void *seasonAux = InitQ(sizeof(int));
    void *serialAux = InitQ(sizeof(TSeason));

    while (!VidaQ(serial->seasons)) {
        season = malloc(sizeof(TSeason));
        r = ExtrQ(serial->seasons, season);
        if (r != 1)
            return -1;

        for (i = 0; i < season->nrOfEp; ++i) {
            duration = malloc(sizeof(int));
            r = ExtrQ(season->episodes, duration);
            if (r != 1)
                return -1;

            if (i == 0 && quit == 0) {
                *(int *)duration -= offset;
                quit = 1; /* offset scazut o singura data */
            }

            minutes += *(int *)duration;
            InsQ(seasonAux, duration);
            if (r != 1)
                return -1;
        }

        ConcatQ(season->episodes, seasonAux);
        InsQ(serialAux, (void *)season);
        if (r != 1)
            return -1;
    }

    r = ConcatQ(serial->seasons, serialAux);
    if (r != 1)
        return -1;
    
    DistrQ(&seasonAux, freeSeason);
    DistrQ(&serialAux, freeSerial);

    return minutes;
}

TLG updateList(TSerial *serial, TLG *L1, TLG *L2, TLG *L3, TLG *LTOP,
                int *dimL1, int *dimL2, int *dimL3, int *dimLTOP)
{
    int ID = serial->ID;
    TLG *L, ultim, copie;
    if (ID == 1) {
        L = L1;
        --(*dimL1);
    }
    else if (ID == 2) {
        L = L2;
        --(*dimL2);
    }
    else if (ID == 3) {
        L = L3;
        --(*dimL3);
    }
    else if (ID == -1) {
        L = LTOP;
        --(*dimLTOP);
    }

    copie = *L;
    ultim = NULL;
    while (((TSerial *)((*L)->info))->name != serial->name) {
        ultim = *L;
        *L = (*L)->urm;
    }

    if (ultim == NULL) {
        TLG aux = *L;
        *L = (*L)->urm;
        copie = *L;
        free(aux);
    } else {
        TLG aux = *L;
        ultim->urm = (*L)->urm;
        *L = copie;
        free(aux);
    }

    return *L;
}

int add (FILE *in, FILE *out, TLG *L1, TLG *L2, TLG *L3, int *dimL1,
        int *dimL2, int *dimL3)
{
    int i, j, r;
    TSeason *season;
    TSerial *serial;

    serial = (TSerial *)malloc(sizeof(TSerial));
    if (!serial) {
        fclose(in);
        fclose(out);
        return -1;
    }

    serial->ID = atoi(strtok(NULL, " "));
    char *name = strtok(NULL, " ");
    serial->name = malloc(MAX);
    strcpy(serial->name, name);
    serial->rating = atof(strtok(NULL, " "));
    serial->nrOfSeasons = atoi(strtok(NULL, " "));
    int *durationOfEp;

    serial->seasons = InitQ(sizeof(TSeason));

    for (i = 0; i < serial->nrOfSeasons; ++i) {
        season = (TSeason *)malloc(sizeof(TSeason));
        if (!season) {
            fclose(in);
            fclose(out);
            return -1;
        }

        season->nrOfEp = atoi(strtok(NULL, " "));
        season->episodes = InitQ(sizeof(int));

        for (j = 0; j < season->nrOfEp; ++j) {
            durationOfEp = malloc(sizeof(int));
            *durationOfEp = atoi(strtok(NULL, " "));
            r = InsQ(season->episodes, (void *) durationOfEp);
            if (r != 1)
                return -1;
        }
        r = InsQ(serial->seasons, (void *) season);
        if (r != 1)
            return -1;
    }

    if (serial->ID == 1) {
        r = inserareCat(L1, (void *) serial, dimL1, out, cmpSerial);
        if (r != 1)
            return -1;
    } else if (serial->ID == 2) {
        r = inserareCat(L2, (void *) serial, dimL2, out, cmpSerial);
        if (r != 1)
            return -1;
    } else if (serial->ID == 3) {
        r = inserareCat(L3, (void *) serial, dimL3, out, cmpSerial);
        if (r != 1)
            return -1;
    }

    return 1;
}

int add_sez(FILE *in, FILE *out, TLG *L1, TLG *L2, TLG *L3, TLG *LTOP,
            void *watch_later, void *currently_watching)
{
    int i, r;
    TSeason *season;

    season = (TSeason *)malloc(sizeof(TSeason));
    if (!season) {
        fclose(in);
        fclose(out);
        return -1;
    }

    char *name = strtok(NULL, " ");
    int *durationOfEp;
            
    season->nrOfEp = atoi(strtok(NULL, " "));
    season->episodes = InitQ(sizeof(int));

    for (i = 0; i < season->nrOfEp; ++i) {
        durationOfEp = malloc(sizeof(int));
        *durationOfEp = atoi(strtok(NULL, " "));
        r = InsQ(season->episodes, (void *) durationOfEp);
        if (r != 1)
            return -1;
    }

    TSerial *serial = findSerial(name, *L1);
    if (serial == NULL)
        serial = findSerial(name, *L2);
    if (serial == NULL)
        serial = findSerial(name, *L3);
    if (serial == NULL)
        serial = findSerial(name, *LTOP); 
    if (serial == NULL)
        serial = findSerialQIns(name, watch_later);
    if (serial == NULL)
        serial = findSerialSIns(name, currently_watching);

    if (serial == NULL)
        return -1;

    r = InsQ(serial->seasons, season);
    if (r != 1)
        return -1;
    ++serial->nrOfSeasons;
    fprintf(out, "Serialul %s are un sezon nou.\n", serial->name);

    return 1;
}

int add_top(FILE *in, FILE *out, TLG *LTOP, int *dimLTOP)
{
    int i, j, r;
    TSeason *season;
    TSerial *serial;

    serial = (TSerial *)malloc(sizeof(TSerial));
    if (!serial) {
        fclose(in);
        fclose(out);
        return -1;
    }

    serial->ID = -1;
    serial->poz = atoi(strtok(NULL, " "));
    char *name = strtok(NULL, " ");
    serial->name = malloc(MAX);
    strcpy(serial->name, name);
    serial->rating = atof(strtok(NULL, " "));
    serial->nrOfSeasons = atoi(strtok(NULL, " "));
    int *durationOfEp;

    serial->seasons = InitQ(sizeof(TSeason));

    for (i = 0; i < serial->nrOfSeasons; ++i) {
        season = (TSeason *)malloc(sizeof(TSeason));
        if (!season) {
            fclose(in);
            fclose(out);
            return -1;
        }

        season->nrOfEp = atoi(strtok(NULL, " "));
        season->episodes = InitQ(sizeof(int));

        for (j = 0; j < season->nrOfEp; ++j) {
            durationOfEp = malloc(sizeof(int));
            *durationOfEp = atoi(strtok(NULL, " "));
            InsQ(season->episodes, (void *) durationOfEp);
        }

        r = InsQ(serial->seasons, season);
        if (r != 1)
            return -1;
    }

    inserareTop(LTOP, (void *) serial, dimLTOP, out);
            
    fprintf(out, "Categoria top10: ");
    Afisare(LTOP, AfiEl, out);

    return 1;
}

int later(FILE *in, FILE *out, TLG *L1, TLG *L2, TLG *L3, TLG *LTOP,
                int *dimL1, int *dimL2, int *dimL3, int *dimCLater,
                int *dimLTOP, void *watch_later)
{
    char *name = strtok(NULL, " \n");
    TSerial *serial = findSerial(name, *L1);
    if (serial == NULL)
        serial = findSerial(name, *L2);
    if (serial == NULL)
        serial = findSerial(name, *L3);
    if (serial == NULL)
        serial = findSerial(name, *LTOP);
            
    if (serial == NULL)
        return -1;

    int r = InsQ(watch_later, serial);
    if (r == 0)
        return -1;
            
    fprintf(out,
        "Serialul %s se afla in coada de asteptare pe pozitia %d.\n",
        serial->name, ++(*dimCLater));

    updateList(serial, L1, L2, L3, LTOP, dimL1, dimL2, dimL3, dimLTOP);

    return 1;
}

int watch(FILE *in, FILE *out, TLG *L1, TLG *L2, TLG *L3, TLG *LTOP,
            int *dimL1, int *dimL2, int *dimL3, int *dimCLater,
            int *dimLTOP, void *watch_later, void *currently_watching,
            void *history)
{
    char *name = strtok(NULL, " ");
    int duration = atoi(strtok(NULL, " \n"));
    TSerial *serial = findSerial(name, *L1);
    if (serial == NULL)
        serial = findSerial(name, *L2);
    if (serial == NULL)
        serial = findSerial(name, *L3);
    if (serial == NULL)
        serial = findSerial(name, *LTOP);
    
    if (serial != NULL) { /* serial gasit intr-una dintre cele 4 categorii */
        Push(currently_watching, serial);

        TLG tmp = updateList(serial, L1, L2, L3, LTOP, dimL1, dimL2, dimL3,
                            dimLTOP);

        if (serial->ID == -1) {
            while (tmp != NULL) {
                ((TSerial *)(tmp->info))->poz--;
                tmp = tmp->urm;
            }
        }
    }
    
    if (serial == NULL) {
        serial = findSerialQ(name, watch_later);

        if (serial != NULL) { /* serial gasit in coada watch_later */
            --(*dimCLater);
            Push(currently_watching, serial);
        }
    }

    if (serial == NULL) {
        serial = findSerialS(name, currently_watching);

        if (serial != NULL) /* serial gasit in stiva currently_watching */
            Push(currently_watching, serial);
    }
    
    if (serial == NULL)
        return -1;

    int minutes = remainingTime(serial, duration);

    if (minutes <= 0) { /* serial vizionat integral */
        TSerial *auxSerial;
        void *aux = InitS(sizeof(TSerial)); /* stiva auxiliara */
        while (!VidaS(currently_watching)) {
            auxSerial = malloc(sizeof(TSerial));
            Pop(currently_watching, auxSerial);
            if (strcmp(name, auxSerial->name) == 0) {
                Push(history, auxSerial);
                fprintf(out, "Serialul %s a fost vizionat integral.\n",
                        auxSerial->name);
            }
            else
                Push(aux, auxSerial);
        }

        if (!VidaS(aux)) {
            int r = Rastoarna(currently_watching, aux);
            if (r == 0)
                return -1;
        }

        DistrS(&aux, freeSerial);
    }

    return 1;
}

int show(FILE *in, FILE *out, TLG *L1, TLG *L2, TLG *L3, TLG *LTOP,
        void *watch_later, void *currently_watching, void *history)
{
    int r;
    char *index = strtok(NULL, " \n");
    fprintf(out, "Categoria %s: ", index);
    if (strcmp(index, "1") == 0)
        Afisare(L1, AfiEl, out);
    if (strcmp(index, "2") == 0)
        Afisare(L2, AfiEl, out);
    if (strcmp(index, "3") == 0)
        Afisare(L3, AfiEl, out);
    if (strcmp(index, "top10") == 0)
        Afisare(LTOP, AfiEl, out);
    if (strcmp(index, "later") == 0) {
        r = AfisareQ(out, watch_later);
        if (r == -1)
            return -1;
    }
    if (strcmp(index, "watching") == 0) {
        r = AfisareS(out, currently_watching);
        if (r == -1)
            return -1;
    }
    if (strcmp(index, "history") == 0) {
        r = AfisareS(out, history);
        if (r == -1)
            return -1;
    }

    return 1;
}

int main(int argc, char **argv)
{
    FILE *in, *out; /* fisiere input, output */

    in = fopen(argv[1], "rt");
    out = fopen(argv[2], "wt");

    if (in == NULL || out == NULL)
        return -1;

    char *line = (char *)malloc(MAX * sizeof(char));
    size_t len = MAX;
    int r;
    /* listele corespunzatoare categoriilor */
    TLG L1 = NULL, L2 = NULL, L3 = NULL, LTOP = NULL;
    /* dimensiunile categoriilor si a cozii watch_later */
    int dimL1 = 0, dimL2 = 0, dimL3 = 0, dimLTOP = 0, dimCLater = 0;
    void *watch_later = InitQ(sizeof(TSerial));    /* coada watch_later */
    /* stiva currently_watching */
    void *currently_watching = InitS(sizeof(TSerial));
    void *history = InitS(sizeof(TSerial));  /* stiva history */

    /* citire linie cu linie din fisierul de input */
    while (getline(&line, &len, in) != -1) {
        char *action = strtok(line, " ");

        if (strcmp(action, "add") == 0) {
            r = add(in, out, &L1, &L2, &L3, &dimL1, &dimL2, &dimL3);
            if (r == -1)
                return -1;

        } else if (strcmp(action, "add_sez") == 0) {
            r = add_sez(in, out, &L1, &L2, &L3, &LTOP, watch_later,
                        currently_watching);
            if (r == -1)
                return -1;
            
        } else if (strcmp(action, "add_top") == 0) {
            r = add_top(in, out, &LTOP, &dimLTOP);
            if (r == -1)
                return -1;

        } else if (strcmp(action, "later") == 0) {
            r = later(in, out, &L1, &L2, &L3, &LTOP, &dimL1, &dimL2,
                            &dimL3, &dimCLater, &dimLTOP, watch_later);
            if (r == -1)
                return -1;

        } else if (strcmp(action, "watch") == 0) {
            r = watch(in, out, &L1, &L2, &L3, &LTOP, &dimL1, &dimL2, &dimL3,
                        &dimCLater, &dimLTOP, watch_later, currently_watching,
                        history);
            if (r == -1)
                return -1;
          
        } else if (strcmp(action, "show") == 0) {
            r = show(in, out, &L1, &L2, &L3, &LTOP, watch_later,
                    currently_watching, history);
            if (r == -1)
                return -1;
        }
    }
    
    /* inchidere fisiere + eliberari de memorie */
    fclose(in);
    fclose(out);

    free(line);
    
    DistrQ(&watch_later, freeSerial);
    DistrS(&currently_watching, freeSerial);
    DistrS(&history, freeSerial);

    Distruge(&L1, freeSerial);
    Distruge(&L2, freeSerial);
    Distruge(&L3, freeSerial);
    Distruge(&LTOP, freeSerial);

    return 0;
}