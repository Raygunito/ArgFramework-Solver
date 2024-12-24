#ifndef TYPE_ARG_FRAMEWORK_H
#define TYPE_ARG_FRAMEWORK_H
#include "TypeListe.h"
/**
 * @brief Structure de données utilisé 
 * pour représenter un système d'argumentation, 
 * on a un accès au prédecesseurs et successeurs
 * de chaque argument. On a de plus le nom des 
 * arguments.
 */
typedef struct ArgFramework
{
    int nbArg;
    Liste* succAdj;
    Liste* predAdj;
    char** tab;
} ArgFramework;

#endif