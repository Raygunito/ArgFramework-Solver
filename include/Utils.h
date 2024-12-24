#ifndef UTILS_H
#define UTILS_H
#include "TypeArgFramework.h"
#include "ArgFramework.h"
void recupArgument(char *chemin, int *size, char ***buff);
int nbArgument(char *chemin);
char *extractArgument(char *line);

void createAdjacencyList(char *fileName, ArgFramework *af);

Label *copyArray(Label *original, int size);

void parcoursProfondeur(int sd, ArgFramework *g,int* cycleDetected);
int reParcoursProfondeur(int s, int *visite, int *inRecursion, ArgFramework *g, int *nbSomVisite);
#endif