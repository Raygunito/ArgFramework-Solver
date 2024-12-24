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
        fprintf(stderr, "Problem types:\n");
        fprintf(stderr, "  SE-CO : Give a complete extension of the argumentation framework.\n");
        fprintf(stderr, "  DC-CO : Determine if <argument> belongs to a complete extension.\n");
        fprintf(stderr, "  DS-CO : Determine if <argument> belongs to all complete extensions.\n");
        fprintf(stderr, "  SE-ST : Give a stable extension of the argumentation framework or NO.\n");
        fprintf(stderr, "  DC-ST : Determine if <argument> belongs to a stable extension.\n");
        fprintf(stderr, "  DS-ST : Determine if <argument> belongs to all stable extensions.\n");
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

    int hasCycle = 0; // no cycle by default
    parcoursProfondeur(0, af, &hasCycle);

    // Start process
    switch (problemType[0])
    {
    case 'S':
        if (!hasCycle)
        {
            doCaminadaLabeling(labels,af);
            printOnlyIN(labels,af);
            break;
        }
        
        if (strcmp(problemType, "SE-CO") == 0)
        {
            handleSECO(labels, af);
        }
        else if (strcmp(problemType, "SE-ST") == 0)
        {
            handleSEST(labels, af);
        }
        else
        {
            printf("Unknown problem type.\n");
        }
        break;
    case 'D':
        // Check if argv[6] != NULL and ArgName exist in the AF
        if (queryArgument == NULL || !argumentExists(af, queryArgument))
        {
            fprintf(stderr, "Argument not specified or does not exist in the argumentation framework.\n");
            break;
        }
        if (strcmp(problemType, "DC-CO") == 0)
        {
            handleDCCO( labels,af, queryArgument);
        }
        else if (strcmp(problemType, "DS-CO") == 0)
        {
            handleDSCO( labels,af, queryArgument);
        }
        else if (strcmp(problemType, "DC-ST") == 0)
        {
            handleDCST( labels,af, queryArgument);
        }
        else if (strcmp(problemType, "DS-ST") == 0)
        {
            handleDSST( labels,af, queryArgument);
        }
        else
        {
            printf("Unknown problem type.\n");
        }
        break;
    default:
        printf("Unknown problem type.\n");
    }

    free(labels);
    freeArgFramework(af);
    free(arguments);
    return 0;
}