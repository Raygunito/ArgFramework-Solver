#ifndef ARGFRAMEWORK_H
#define ARGFRAMEWORK_H
#include "TypeArgFramework.h"

/**
 * @brief We will use Caminada's labeling
 */
typedef enum
{
    IN,
    OUT,
    UNDECIDED
} Label;

void initializeLabels(Label* labels, int size);
int findArgumentIndex(char* arg, ArgFramework* af);
ArgFramework *initArgFramework(int nbArg);
void freeArgFramework(ArgFramework *af);

int allAttackersOUT(int index, Label *labs, ArgFramework *af);
int oneAttackersIN(int index, Label *labs, ArgFramework *af);
void doCaminadaLabeling(Label *labs, ArgFramework *af);

#endif