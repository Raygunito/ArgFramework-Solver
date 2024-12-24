#include "ArgFramework.h"
#include "Liste.h"
#include "Utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void printLabelAF(Label *labels, ArgFramework *af)
{
    int n = af->nbArg;
    for (int i = 0; i < n; i++)
    {
        switch (labels[i])
        {
        case IN:
            printf("%s=IN\n", af->tab[i]);
            break;

        case OUT:
            printf("%s=OUT\n", af->tab[i]);
            break;

        default:
            printf("%s=UNDEC\n", af->tab[i]);
            break;
        }
    }
}

/**
 * @brief vérifie que tous les attaquants de index sont OUT
 * 
 * @param index 
 * @param labs 
 * @param af 
 * @return int 
 */
int allAttackersOUT(int index, Label *labs, ArgFramework *af)
{
    Liste pred = af->predAdj[index];
    while (!estVideListe(pred))
    {
        if (labs[pred->somSucc] != OUT)
        {
            return 0; // False, not all attackers are OUT.
        }
        pred = suivantListe(pred);
    }
    return 1; // all attacker out
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
        return 1; //true
    }
    return 0; //false
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

int *noeudAvecPlusAtk(Label *labels, ArgFramework *af)
{
    int n = af->nbArg;
    int *tabAtk = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        if (labels[i] != UNDECIDED)
        {
            tabAtk[i] = 0;
        }
        else
        {
            tabAtk[i] = tailleSuccListe(af->succAdj[i]);
        }
    }

    return tabAtk;
}

int argumentExists(ArgFramework *af, char *queryArgument)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        if (strcmp(af->tab[i], queryArgument) == 0)
        {
            return 1; // Argument found
        }
    }
    return 0; // Argument not found
}

void handleSECO(Label *labels, ArgFramework *af)
{
    int n = af->nbArg;
    int i = 0;
    int hasChange = 0;
    do
    {
        i = 0;
        hasChange = 0;
        while (i < n && labels[i] == UNDECIDED)
        {

            if (allAttackersOUT(i, labels, af))
            {
                labels[i] = IN;
                hasChange = 1;
            }
            else if (oneAttackersIN(i, labels, af))
            {
                labels[i] = OUT;
                hasChange = 1;
            }

            i++;
        }
    } while (hasChange);

    printf("[");
    int first = 1;
    for (int i = 0; i < n; i++)
    {
        if (labels[i] == IN)
        {
            if (!first)
            {
                printf(",");
            }
            printf("%s", af->tab[i]);
            first = 0;
        }
    }
    printf("]\n");
}

void handleSEST(Label *labels, ArgFramework *af)
{
    // first pass
    doCaminadaLabeling(labels, af);
    int n = af->nbArg;
    if (containsUNDEC(labels, n))
    {
        int count;
        int *undecArgs = pinpointUNDEC(labels, af, &count);
        int res = backtrackST(labels, af, undecArgs, count, 0);
        if (res)
        {
            printOnlyIN(labels, af);
        }
        else
        {
            printf("NO\n");
        }
        free(undecArgs);
    }
    else
    {
        printOnlyIN(labels, af);
    }
}

int containsUNDEC(Label *labs, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (labs[i] == UNDECIDED)
        {
            return 1; // true there is a UNDEC
        }
    }
    return 0; // false
}

void printOnlyIN(Label *labels, ArgFramework *af)
{
    printf("[");
    int first = 1;
    for (int i = 0; i < af->nbArg; i++)
    {
        if (labels[i] == IN)
        {
            if (!first)
            {
                printf(",");
            }
            printf("%s", af->tab[i]);
            first = 0;
        }
    }
    printf("]\n");
}

int *pinpointUNDEC(Label *l, ArgFramework *af, int *count)
{
    int n = af->nbArg;
    int *arrayIndex = (int *)malloc(n * sizeof(int));
    (*count) = 0;
    for (int i = 0; i < n; i++)
    {
        if (l[i] == UNDECIDED)
        {
            arrayIndex[*count] = i;
            (*count)++;
        }
    }
    return arrayIndex;
}

int backtrackST(Label *l, ArgFramework *af, int *undecArg, int undecCount, int currentIndex)
{
    // end case
    if (currentIndex == undecCount)
    {
        if (containsUNDEC(l, af->nbArg))
        {
            // there is still some undec
            int nb;
            int *undecArray = pinpointUNDEC(l, af, &nb);
            if (backtrackST(l, af, undecArray, nb, 0))
            {
                free(undecArray);
                return 1;
            }
            free(undecArray);
            return 0;
        }
        else
        {
            // we have no UNDEC
            if (isStableExtension(l, af))
            {
                return 1;
            }
            return 0;
        }
    }

    // retrieve which arg to set
    int argIndex = undecArg[currentIndex];
    Label *lcopy = copyArray(l, af->nbArg);
    lcopy[argIndex] = IN;
    doCaminadaLabeling(lcopy, af);
    if (backtrackST(lcopy, af, undecArg, undecCount, currentIndex + 1))
    {
        for (int i = 0; i < af->nbArg; i++)
        {
            l[i] = lcopy[i]; // Save the state of lcopy into l
        }
        free(lcopy);
        return 1; // If we found a valid stable extension, return success
    }

    free(lcopy);
    lcopy = copyArray(l, af->nbArg);
    lcopy[argIndex] = OUT;
    doCaminadaLabeling(lcopy, af);
    if (backtrackST(lcopy, af, undecArg, undecCount, currentIndex + 1))
    {
        for (int i = 0; i < af->nbArg; i++)
        {
            l[i] = lcopy[i]; // Save the state of lcopy into l
        }
        free(lcopy);
        return 1;
    }

    free(lcopy);
    return 0;
}

int isStableExtension(Label *labs, ArgFramework *af)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        if (labs[i] == OUT && !oneAttackersIN(i, labs, af))
        {
            return 0; // there is no one who is IN =/> OUT
        }

        if (labs[i] == IN && oneAttackersIN(i, labs, af))
        {
            return 0; // incompatible IN => IN
        }
    }
    return 1; // Valid stable extension
}