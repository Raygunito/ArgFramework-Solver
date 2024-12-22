#include "ArgFramework.h"
#include "Liste.h"
#include <stdlib.h>
#include <string.h>
void initializeLabels(Label *labels, int size)
{
    for (int i = 0; i < size; i++)
    {
        labels[i] = UNDECIDED;
    }
}

int findArgumentIndex(char *arg, ArgFramework *af)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        if (strcmp(af->tab[i], arg) == 0)
        {
            return i;
        }
    }
    return -1;
}

ArgFramework *initArgFramework(int nbArg)
{
    ArgFramework *af = (ArgFramework *)malloc(sizeof(ArgFramework));
    af->nbArg = nbArg;
    af->succAdj = (Liste *)malloc(nbArg * sizeof(Liste));
    af->predAdj = (Liste *)malloc(nbArg * sizeof(Liste));
    af->tab = (char **)malloc(nbArg * sizeof(char *));

    for (int i = 0; i < nbArg; i++)
    {
        af->succAdj[i] = initListe();
        af->predAdj[i] = initListe();
        af->tab[i] = NULL;
    }
    return af;
}

void freeArgFramework(ArgFramework *af)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        free(af->tab[i]);
        Liste l = af->succAdj[i];
        while (!estVideListe(l))
        {
            Liste tmp = l;
            l = suivantListe(l);
            free(tmp);
        }

        l = af->predAdj[i];
        while (!estVideListe(l))
        {
            Liste tmp = l;
            l = suivantListe(l);
            free(tmp);
        }
    }
    free(af->succAdj);
    free(af->predAdj);
    free(af->tab);
    free(af);
}

int allAttackersOUT(int index, Label *labs, ArgFramework *af)
{
    Liste pred = af->predAdj[index];
    while ((!estVideListe(pred)) && labs[pred->somSucc] == OUT)
    {
        pred = suivantListe(pred);
    }
    if (estVideListe(pred))
    {
        return 1;
    }
    return 0;
}

int oneAttackersIN(int index, Label *labs, ArgFramework *af)
{
    Liste pred = af->predAdj[index];
    while ((!estVideListe(pred)) && labs[pred->somSucc] != IN)
    {
        pred = suivantListe(pred);
    }
    // there is someone who attacks my Arg
    if (!estVideListe(pred))
    {
        return 1;
    }
    return 0;
}

void doCaminadaLabeling(Label *labs, ArgFramework *af)
{
    int n = af->nbArg;
    int i = 0;
    int hasChange = 0;
    do
    {
        i = 0;
        hasChange = 0;
        while (i < n && labs[i] == UNDECIDED)
        {

            if (allAttackersOUT(i, labs, af))
            {
                labs[i] = IN;
                hasChange = 1;
            }
            else if (oneAttackersIN(i, labs, af))
            {
                labs[i] = OUT;
                hasChange = 1;
            }

            i++;
        }
    } while (hasChange);
}