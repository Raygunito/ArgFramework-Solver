#include "TypeArgFramework.h"
#include "Liste.h"
#include <stdio.h>
#define MAX_ARGUMENTS 256
int isConflictFree(int *subset, int subsetSize, ArgFramework *af)
{
    for (int i = 0; i < subsetSize; i++)
    {
        int arg = subset[i];
        Liste attacks = af->succAdj[arg];
        while (!estVideListe(attacks))
        {
            int attacked = donneeListe(attacks);
            for (int j = 0; j < subsetSize; j++)
            {
                if (subset[j] == attacked)
                {
                    return 0; // Not free
                }
            }
            attacks = suivantListe(attacks);
        }
    }
    return 1; // free
}

int isStableExtension(int *subset, int subsetSize, ArgFramework *af)
{
    if (!isConflictFree(subset, subsetSize, af))
        return 0;

    int attacked[MAX_ARGUMENTS] = {0};

    for (int i = 0; i < subsetSize; i++)
    {
        int arg = subset[i];
        Liste attacks = af->succAdj[arg];
        while (!estVideListe(attacks))
        {
            attacked[donneeListe(attacks)] = 1;
            attacks = suivantListe(attacks);
        }
    }

    for (int i = 0; i < af->nbArg; i++)
    {
        int inSubset = 0;
        for (int j = 0; j < subsetSize; j++)
        {
            if (subset[j] == i)
            {
                inSubset = 1;
                break;
            }
        }
        if (!inSubset && !attacked[i])
            return 0; // Not a stable extension
    }

    return 1; // Stable extension
}

void generateSubsets(int *args, int argCount, int *subset, int subsetSize,
                     ArgFramework *af, void (*processSubset)(int *, int, ArgFramework *))
{
    if (argCount == 0)
    {
        processSubset(subset, subsetSize, af);
        return;
    }
    // Exclude current argument
    generateSubsets(args + 1, argCount - 1, subset, subsetSize, af, processSubset);

    // Include current argument
    subset[subsetSize] = args[0];
    generateSubsets(args + 1, argCount - 1, subset, subsetSize + 1, af, processSubset);
}

void processStableSubset(int *subset, int subsetSize, ArgFramework *af)
{
    if (isStableExtension(subset, subsetSize, af))
    {
        printf("[");
        for (int i = 0; i < subsetSize; i++)
        {
            printf("%s", af->tab[subset[i]]);
            if (i < subsetSize - 1)
            {
                printf(",");
            }
        }
        printf("]\n");
    }
}

void calculateStableExtensions(ArgFramework *af)
{
    int args[MAX_ARGUMENTS];
    for (int i = 0; i < af->nbArg; i++)
    {
        args[i] = i;
    }

    int subset[MAX_ARGUMENTS];
    generateSubsets(args, af->nbArg, subset, 0, af, processStableSubset);
}

int isDefended(int arg, int *subset, int subsetSize, ArgFramework *af)
{
    Liste attackers = af->predAdj[arg];
    while (!estVideListe(attackers))
    {
        int attacker = donneeListe(attackers);
        int defended = 0;
        for (int i = 0; i < subsetSize; i++)
        {
            int defender = subset[i];
            Liste defenderAttacks = af->succAdj[defender];
            while (!estVideListe(defenderAttacks))
            {
                if (donneeListe(defenderAttacks) == attacker)
                {
                    defended = 1;
                    break;
                }
                defenderAttacks = suivantListe(defenderAttacks);
            }
            if (defended)
                break;
        }
        if (!defended)
            return 0; // Argument is not defended
        attackers = suivantListe(attackers);
    }
    return 1; // Argument is defended
}

int isCompleteExtension(int *subset, int subsetSize, ArgFramework *af)
{
    if (!isConflictFree(subset, subsetSize, af))
        return 0;

    for (int i = 0; i < subsetSize; i++)
    {
        if (!isDefended(subset[i], subset, subsetSize, af))
        {
            return 0; // Not all arguments are defended
        }
    }

    for (int i = 0; i < af->nbArg; i++)
    {
        if (isDefended(i, subset, subsetSize, af))
        {
            int found = 0;
            for (int j = 0; j < subsetSize; j++)
            {
                if (subset[j] == i)
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
                return 0; // Missing defended argument
        }
    }

    return 1; // Complete extension
}

void processCompleteSubset(int *subset, int subsetSize, ArgFramework *af)
{
    if (isCompleteExtension(subset, subsetSize, af))
    {
        printf("[");
        for (int i = 0; i < subsetSize; i++)
        {
            printf("%s", af->tab[subset[i]]);
            if (i < subsetSize - 1)
            {
                printf(",");
            }
        }
        printf("]\n");
    }
}

void calculateCompleteExtensions(ArgFramework *af)
{
    int args[MAX_ARGUMENTS];
    for (int i = 0; i < af->nbArg; i++)
    {
        args[i] = i;
    }

    int subset[MAX_ARGUMENTS];
    generateSubsets(args, af->nbArg, subset, 0, af, processCompleteSubset);
}
