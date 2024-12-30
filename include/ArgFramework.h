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

void initializeLabels(Label *labels, int size);
int findArgumentIndex(char *arg, ArgFramework *af);
ArgFramework *initArgFramework(int nbArg);
void freeArgFramework(ArgFramework *af);
int argumentExists(ArgFramework *af, char *queryArgument);

void printLabelAF(Label *labels, ArgFramework *af);
int allAttackersOUT(int index, Label *labs, ArgFramework *af);
int oneAttackersIN(int index, Label *labs, ArgFramework *af);
void doCaminadaLabeling(Label *labs, ArgFramework *af);

int isLegalExtension(Label *labs, ArgFramework *af);
void handleSECO(Label *labels, ArgFramework *af);
void handleSEST(Label *labels, ArgFramework *af);
void handleDCST(Label *l, ArgFramework *af, char *query);
void handleDSST(Label *l, ArgFramework *af, char *query);
void handleDSCO(Label *labels, ArgFramework *af, char *query);
void handleDCCO(Label *labels, ArgFramework *af, char *query);

int containsUNDEC(Label *labs, int size);
void printOnlyIN(Label *labels, ArgFramework *af);
int *pinpointUNDEC(Label *l, ArgFramework *af, int *count);
int backtrackST(Label *l, ArgFramework *af, int *undecArg, int undecCount, int currentIndex);

#endif