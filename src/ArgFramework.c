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
    af->listeAdj = (Liste *)malloc(nbArg * sizeof(Liste));
    af->tab = (char **)malloc(nbArg * sizeof(char *));

    for (int i = 0; i < nbArg; i++)
    {
        af->listeAdj[i] = initListe();
        af->tab[i] = NULL;
    }
    return af;
}

void freeArgFramework(ArgFramework *af)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        free(af->tab[i]);
        Liste l = af->listeAdj[i];
        while (!estVideListe(l))
        {
            Liste tmp = l;
            l = suivantListe(l);
            free(tmp);
        }
    }
    free(af->listeAdj);
    free(af->tab);
    free(af);
}
