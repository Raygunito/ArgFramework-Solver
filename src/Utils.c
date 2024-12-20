#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TypeArgFramework.h>
#include "ArgFramework.h"
#include "Liste.h"

static int somSuivant(int n, int *visite);

// recupere le chemin du fichier, change la valeur de size et char** est un tableau de string
void recupArgument(char *chemin, int *size, char ***buff)
{
    FILE *file = fopen(chemin, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", chemin);
        exit(EXIT_FAILURE);
    }
    *size = nbArgument(chemin);
    *buff = (char **)malloc(*size * sizeof(char *));
    if (*buff == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[256];
    int index = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *arg = extractArgument(line);
        if (arg != NULL)
        {
            (*buff)[index++] = arg;
        }
    }
    fclose(file);
}

int nbArgument(char *chemin)
{
    FILE *file = fopen(chemin, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file in function nbArgument %s\n", chemin);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char buffer[8];
    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "arg(", 4) == 0)
        {
            count++;
        }
    }

    fclose(file);
    return count;
}

char *extractArgument(char *line)
{
    if (strncmp(line, "arg(", 4) != 0)
    {
        return NULL;
    }

    char *start = line + 4;
    char *end = strchr(start, ')');
    if (end != NULL)
    {
        *end = '\0';
        return strdup(start);
    }
    return NULL;
}

void createAdjacencyList(char *fileName, ArgFramework *af)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    if (af == NULL)
    {
        fprintf(stderr, "Error: Argumentation Framework not initialized %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "att(", 4) == 0)
        {
            char *start = line + 4;
            char *comma = strchr(start, ',');
            char *end = strchr(comma, ')');

            if (!comma || !end)
                continue;

            *comma = '\0';
            *end = '\0';

            char *attacker = start;
            char *target = comma + 1;

            int attackerIndex = findArgumentIndex(attacker, af);
            int targetIndex = findArgumentIndex(target, af);

            if (attackerIndex == targetIndex)
            {
                fprintf(stderr, "FATAL: Self Attacking Argument not supported %s\n", fileName);
                exit(EXIT_FAILURE);
            }
            if (attackerIndex != -1 && targetIndex != -1)
            {
                if (rechercheElementListe(targetIndex, af->succAdj[attackerIndex]))
                {
                    fprintf(stderr, "FATAL: Duplicate attack found: %s -> %s\n", attacker, target);
                    exit(EXIT_FAILURE);
                }

                // Insert in the successor and predecessor lists
                af->succAdj[attackerIndex] = inserQueueListe(targetIndex, af->succAdj[attackerIndex]);
                af->predAdj[targetIndex] = inserQueueListe(attackerIndex, af->predAdj[targetIndex]);
            }
            else
            {
                fprintf(stderr, "Error: Argument(s) not found in the framework: %s -> %s\n", attacker, target);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

void parcoursProfondeur(int sd, ArgFramework *g, int *cycleDetected)
{
    int n = g->nbArg;
    int *visite = (int *)malloc(n * sizeof(int));
    int *inRecursion = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        visite[i] = 0;
        inRecursion[i] = 0;
    }

    (*cycleDetected) = 0; // No cycle

    int s = sd;
    int nbSomVisite = 0;
    int finParcours = 0; // false

    while (!finParcours)
    {
        if (reParcoursProfondeur(s, visite, inRecursion, g, &nbSomVisite))
        {
            *cycleDetected = 1;
            finParcours = 1;
        }
        else if (nbSomVisite == n)
        {
            finParcours = 1;
        }
        else
        {
            // Find the next unvisited node
            s = somSuivant(n, visite);
            if (s == -1)
            {
                finParcours = 1; // No unvisited nodes left
            }
        }
    }
    free(visite);
    free(inRecursion);
}

int reParcoursProfondeur(int s, int *visite, int *inRecursion, ArgFramework *g, int *nbSomVisite)
{
    // if already in the recursion stack
    if (inRecursion[s] == 1)
    {
        return 1;
    }
    // if the node visited and full explored => skip
    if (visite[s] == 1)
    {
        return 0;
    }

    visite[s] = 1;
    inRecursion[s] = 1;
    (*nbSomVisite)++;

    Liste succListe = g->succAdj[s];
    int t;
    while (!estVideListe(succListe))
    {
        t = donneeListe(succListe);
        // explore successors
        if (reParcoursProfondeur(t, visite, inRecursion, g, nbSomVisite))
        {
            return 1;
        }
        succListe = suivantListe(succListe);
    }
    // remove node from tracking inRecursion
    inRecursion[s] = 0;

    return 0;
}

static int somSuivant(int n, int *visite)
{
    // Find the first unvisited node
    for (int i = 0; i < n; i++)
    {
        if (visite[i] == 0)
        {
            return i;
        }
    }
    return -1;
}