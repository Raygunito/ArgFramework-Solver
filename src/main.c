#include <stdio.h>
#include <string.h>
#include "Liste.h"
#include "Utils.h"
#include "ArgFramework.h"
// #include "NaiveWay.h"

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
    for (int i = 0; i < nbArgs; i++)
    {
        af->tab[i] = arguments[i];
    }
    createAdjacencyList(filePath, af);


    Label *labels = (Label *)malloc(nbArgs * sizeof(Label));
    initializeLabels(labels, nbArgs);

    for (int i = 0; i < nbArgs; i++)
    {
        printf("%s=%d\t",af->tab[i],labels[i]);
    }
    printf("\n");
    doCaminadaLabeling(labels,af);  
    for (int i = 0; i < nbArgs; i++)
    {
        printf("%s=%d\t",af->tab[i],labels[i]);
    }
    
    /* NaiveWay import
    printf("Stable Extensions:\n");
    calculateStableExtensions(af);

    printf("Complete Extensions:\n");
    calculateCompleteExtensions(af);
    */

    /* cycle detection
    int cycle = 0;
    parcoursProfondeur(0, af, &cycle);
    printf("Detection cycle : %s\n", cycle == 1 ? "true" : "false");
    */

    free(labels);
    freeArgFramework(af);
    free(arguments);
    return 0;
}