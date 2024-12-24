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
        return 1; // true
    }
    return 0; // false
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

    printOnlyIN(labels, af);
}

void handleSEST(Label *labels, ArgFramework *af)
{
    // first pass
    doCaminadaLabeling(labels, af);

    if (containsUNDEC(labels, af->nbArg))
    {
        int undecCount;
        int *undecArgs = pinpointUNDEC(labels, af, &undecCount);
        // found one stable extension ? ok print it, end
        if (backtrackST(labels, af, undecArgs, undecCount, 0))
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

// Credulous Stable = query is in at least one stable extension
void handleDCST(Label *l, ArgFramework *af, char *query)
{
    // first pass
    doCaminadaLabeling(l, af);
    int n = af->nbArg;
    int target = findArgumentIndex(query, af);
    if (containsUNDEC(l, n))
    {
        int count;
        int *undecArgs = pinpointUNDEC(l, af, &count);
        Label *lcopy = copyArray(l, af->nbArg);
        for (int i = 0; i < count; i++)
        {
            // reset label
            memcpy(lcopy, l, n * sizeof(Label));
            // if we found a stable extension AND if query is IN at least once then we can break;
            if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == IN)
            {
                free(undecArgs);
                free(lcopy);
                printf("YES\n");
                return;
            }
        }
        printf("NO\n");
        free(lcopy);
        free(undecArgs);
    }
    else
    {
        printf(l[target] == IN ? "YES\n" : "NO\n");
    }
}

// Skeptical Stable = query is IN in all possible stable extension
void handleDSST(Label *l, ArgFramework *af, char *query)
{
    // first pass
    doCaminadaLabeling(l, af);
    int n = af->nbArg;
    int target = findArgumentIndex(query, af);
    if (containsUNDEC(l, n))
    {
        int count;
        int *undecArgs = pinpointUNDEC(l, af, &count);
        Label *lcopy = copyArray(l, af->nbArg);
        for (int i = 0; i < count; i++)
        {
            // reset label
            memcpy(lcopy, l, n * sizeof(Label));

            // we have a stable extension , and query is OUT at least once then we can break;
            if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == OUT)
            {
                free(undecArgs);
                free(lcopy);
                printf("NO\n");
                return;
            }
        }
        printf("YES\n");
        free(lcopy);
        free(undecArgs);
    }
    else
    {
        printf(l[target] == IN ? "YES\n" : "NO\n");
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
            // we found a stable extension with the remaining UNDEC
            if (backtrackST(l, af, undecArray, nb, 0))
            {
                free(undecArray);
                return 1;
            }
            free(undecArray);
            // didn't found a stable extension
            return 0;
        }
        else
        {
            // we have no UNDEC
            if (isStableExtension(l, af))
            {
                return 1;
            }
            // something illegal like OUT=>OUT or IN=>IN
            return 0;
        }
    }

    // retrieve which arg to set
    int argIndex = undecArg[currentIndex];
    Label *lcopy = copyArray(l, af->nbArg);
    lcopy[argIndex] = IN;
    // spread the newly IN
    doCaminadaLabeling(lcopy, af);
    if (backtrackST(lcopy, af, undecArg, undecCount, currentIndex + 1))
    {
        // save found stable extension to original
        memcpy(l, lcopy, af->nbArg * sizeof(Label));
        free(lcopy);
        return 1; // we found a valid stable extension, return success
    }

    free(lcopy);
    lcopy = copyArray(l, af->nbArg);
    lcopy[argIndex] = OUT;
    // spread the newly OUT
    doCaminadaLabeling(lcopy, af);
    if (backtrackST(lcopy, af, undecArg, undecCount, currentIndex + 1))
    {
        // save found stable extension to original
        memcpy(l, lcopy, af->nbArg * sizeof(Label));
        free(lcopy);
        return 1;
    }

    free(lcopy);
    return 0;
}

int isStableExtension(Label *labs, ArgFramework *af)
{
    // it is more like "isLegalLabeling" to be honest since we don't care about UNDEC just legal
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

void handleDCCO(Label *labels, ArgFramework *af, char *query)
{
    doCaminadaLabeling(labels, af);
    int target = findArgumentIndex(query, af);
    int n = af->nbArg;
    int count;
    int *undecArgs = pinpointUNDEC(labels, af, &count);
    Label *lcopy = copyArray(labels, n);

    for (int i = 0; i < count; i++)
    {
        memcpy(lcopy, labels, n * sizeof(Label));
        if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == IN)
        {
            printf("YES\n");
            free(undecArgs);
            free(lcopy);
            return;
        }
    }
    printf("NO\n");
    free(undecArgs);
    free(lcopy);
}

void handleDSCO(Label *labels, ArgFramework *af, char *query)
{
    doCaminadaLabeling(labels, af);
    if (containsUNDEC(labels, af->nbArg))
    {
        printf("NO\n");
        return;
    }

    int target = findArgumentIndex(query, af);
    int n = af->nbArg;
    int count;
    int *undecArgs = pinpointUNDEC(labels, af, &count);
    Label *lcopy = copyArray(labels, n);

    for (int i = 0; i < count; i++)
    {
        memcpy(lcopy, labels, n * sizeof(Label));
        if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == OUT)
        {
            printf("NO\n");
            free(undecArgs);
            free(lcopy);
            return;
        }
    }
    printf("YES\n");
    free(undecArgs);
    free(lcopy);
}