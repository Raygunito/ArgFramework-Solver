#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TypeArgFramework.h>
#include "ArgFramework.h"
#include "Liste.h"
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

            if (attackerIndex != -1 && targetIndex != -1)
            {
                af->listeAdj[attackerIndex] = inserTeteListe(targetIndex, af->listeAdj[attackerIndex]);
            }
        }
    }

    fclose(file);
}