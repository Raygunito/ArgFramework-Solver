#include <stdio.h>
#include <string.h>
#include "Liste.h"
#include "Utils.h"
#include "ArgFramework.h"
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s -p <problem_type> -f <file_path> [-a <argument>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *problemType = argv[2];
    char *filePath = argv[4];
    char *queryArgument = (argc > 5 && strcmp(argv[5], "-a") == 0) ? argv[6] : NULL;

    int nbArgs = nbArgument((char *)filePath);
    char **arguments;
    recupArgument(filePath, &nbArgs, &arguments);

    ArgFramework *af = initArgFramework(nbArgs);

    Label *labels = (Label *)malloc(nbArgs * sizeof(Label));
    initializeLabels(labels, nbArgs);

    for (int i = 0; i < nbArgs; i++)
    {
        af->tab[i] = arguments[i];
    }

    createAdjacencyList(filePath, af);
    for (int i = 0; i < af->nbArg; i++)
    {
        printf("Argument %s attacks: ", af->tab[i]);
        afficheListe(af->listeAdj[i]);
    }

    free(labels);
    freeArgFramework(af);
    free(arguments);
    return 0;
}