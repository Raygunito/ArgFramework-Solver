#ifndef TYPE_ARG_FRAMEWORK_H
#define TYPE_ARG_FRAMEWORK_H
#include "TypeListe.h"
typedef struct ArgFramework
{
    int nbArg;
    Liste* listeAdj;
    char** tab;
} ArgFramework;

#endif