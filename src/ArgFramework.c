#include "ArgFramework.h"
#include "Liste.h"
#include "Utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/// @brief Initialise les labels à UNDEC
/// @param labels tableau de label
/// @param size nombre d'élément
void initializeLabels(Label *labels, int size)
{
    for (int i = 0; i < size; i++)
    {
        labels[i] = UNDECIDED;
    }
}

/// @brief Retourne l'index de l'argument "arg" dans af->tab[?]
/// @param arg nom de l'argument
/// @param af système d'argumentation
/// @return la position de l'argument dans le système d'argumentation
int findArgumentIndex(char *arg, ArgFramework *af)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        if (strcmp(af->tab[i], arg) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief initialise et alloue le système d'argumentation avec la
 * bonne taille
 *
 * @param nbArg nombre d'argument total
 * @return ArgFramework*
 */
ArgFramework *initArgFramework(int nbArg)
{
    ArgFramework *af = (ArgFramework *)malloc(sizeof(ArgFramework));
    af->nbArg = nbArg;
    af->succAdj = (Liste *)malloc(nbArg * sizeof(Liste));
    af->predAdj = (Liste *)malloc(nbArg * sizeof(Liste));
    af->tab = (char **)malloc(nbArg * sizeof(char *));

    for (int i = 0; i < nbArg; i++)
    {
        af->succAdj[i] = initListe();
        af->predAdj[i] = initListe();
        af->tab[i] = NULL;
    }
    return af;
}

/// @brief Libère proprement l'AS
/// @param af Système d'argumentation
void freeArgFramework(ArgFramework *af)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        free(af->tab[i]);
        Liste l = af->succAdj[i];
        while (!estVideListe(l))
        {
            Liste tmp = l;
            l = suivantListe(l);
            free(tmp);
        }

        l = af->predAdj[i];
        while (!estVideListe(l))
        {
            Liste tmp = l;
            l = suivantListe(l);
            free(tmp);
        }
    }
    free(af->succAdj);
    free(af->predAdj);
    free(af->tab);
    free(af);
}

/// @brief Affiche les labels pour le debugging
/// @param labels tableau de labels de l'AS
/// @param af Système d'argumentation
void printLabelAF(Label *labels, ArgFramework *af)
{
    int n = af->nbArg;
    for (int i = 0; i < n; i++)
    {
        switch (labels[i])
        {
        case IN:
            printf("%s=IN\n", af->tab[i]);
            break;

        case OUT:
            printf("%s=OUT\n", af->tab[i]);
            break;

        default:
            printf("%s=UNDEC\n", af->tab[i]);
            break;
        }
    }
}

/**
 * @brief Vérifie si tous les prédécesseurs de index sont OUT
 *
 * @param index noeud où on doit vérifier ses prédecesseurs
 * @param labs tableau de labels de l'AS
 * @param af Système d'argumentation
 * @return int 1 si tous les attaquants de index sont OUT
 */
int allAttackersOUT(int index, Label *labs, ArgFramework *af)
{
    Liste pred = af->predAdj[index];
    while (!estVideListe(pred))
    {
        if (labs[pred->somSucc] != OUT)
        {
            return 0; // False, not all attackers are OUT.
        }
        pred = suivantListe(pred);
    }
    return 1; // all attacker out
}

/**
 * @brief Vérifie si au moins un attaquant de l'argument est IN.
 *
 * @param index Index de l'argument
 * @param labs Tableau des labels
 * @param af Système d'argumentation
 * @return 1 si un attaquant est IN, 0 sinon.
 */
int oneAttackersIN(int index, Label *labs, ArgFramework *af)
{
    Liste pred = af->predAdj[index];
    while ((!estVideListe(pred)) && labs[pred->somSucc] != IN)
    {
        pred = suivantListe(pred);
    }
    // there is someone who attacks my Arg
    if (!estVideListe(pred))
    {
        return 1; // true
    }
    return 0; // false
}

/**
 * @brief Effectue le labeling selon la méthode de Caminada.
 *
 * @param labs Tableau des labels
 * @param af Système d'argumentation
 */
void doCaminadaLabeling(Label *labs, ArgFramework *af)
{
    int n = af->nbArg;
    int i = 0;
    int hasChange = 0;
    do
    {
        i = 0;
        hasChange = 0;
        while (i < n && labs[i] == UNDECIDED)
        {

            if (allAttackersOUT(i, labs, af))
            {
                labs[i] = IN;
                hasChange = 1;
            }
            else if (oneAttackersIN(i, labs, af))
            {
                labs[i] = OUT;
                hasChange = 1;
            }

            i++;
        }
    } while (hasChange);
}

/**
 * @brief Vérifie si un argument existe dans le système d'argumentation
 *
 * @param af Système d'argumentation
 * @param queryArgument Argument à rechercher
 * @return 1 si l'argument existe, 0 sinon
 */
int argumentExists(ArgFramework *af, char *queryArgument)
{
    for (int i = 0; i < af->nbArg; i++)
    {
        if (strcmp(af->tab[i], queryArgument) == 0)
        {
            return 1; // Argument found
        }
    }
    return 0; // Argument not found
}

/**
 * @brief Trouve une extension complète. on peut avoir []
 * si il y a des cycles, ou un ensemble si acyclique.
 *
 * @param labels Tableau des labels.
 * @param af Système d'argumentation.
 */
void handleSECO(Label *labels, ArgFramework *af)
{
    doCaminadaLabeling(labels, af);
    printOnlyIN(labels, af);
}

/**
 * @brief Trouve une extension stable avec la méthode par
 * backtracking, jusqu'à trouver un labeling ou il n'y a pas
 * de UNDEC, ou bien aucun extension stable "NO"
 *
 * @param labels Tableau des labels.
 * @param af Système d'argumentation.
 */
void handleSEST(Label *labels, ArgFramework *af)
{
    // first pass
    doCaminadaLabeling(labels, af);

    if (containsUNDEC(labels, af->nbArg))
    {
        int undecCount;
        int *undecArgs = pinpointUNDEC(labels, af, &undecCount);
        // found one stable extension ? ok print it, end
        if (backtrackST(labels, af, undecArgs, undecCount, 0))
        {
            printOnlyIN(labels, af);
        }
        else
        {
            printf("NO\n");
        }
        free(undecArgs);
    }
    else
    {
        printOnlyIN(labels, af);
    }
}

// Credulous Stable = query is in at least one stable extension
/**
 * @brief Cherche si query est dans au moins 1 extension stable.
 * Méthode par backtracking. Affiche "YES" ou "NO"
 *
 * @param l Tableau des labels.
 * @param af Système d'argumentation.
 * @param query Argument à vérifier.
 */
void handleDCST(Label *l, ArgFramework *af, char *query)
{
    // first pass
    doCaminadaLabeling(l, af);
    int n = af->nbArg;
    int target = findArgumentIndex(query, af);
    if (containsUNDEC(l, n))
    {
        int count;
        int *undecArgs = pinpointUNDEC(l, af, &count);
        Label *lcopy = copyArray(l, af->nbArg);
        for (int i = 0; i < count; i++)
        {
            // reset label
            memcpy(lcopy, l, n * sizeof(Label));
            // if we found a stable extension AND if query is IN at least once then we can break;
            if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == IN)
            {
                free(undecArgs);
                free(lcopy);
                printf("YES\n");
                return;
            }
        }
        printf("NO\n");
        free(lcopy);
        free(undecArgs);
    }
    else
    {
        printf(l[target] == IN ? "YES\n" : "NO\n");
    }
}

// Skeptical Stable = query is IN in all possible stable extension
/**
 * @brief Cherche si query est dans toutes les extensions stables.
 * Méthode par backtracking. Affiche "YES" ou "NO". Pour vérifier si
 * c'est dans toutes les extensions on parcourt tous les UNDEC qui reste
 * en testant chaqu'un leur tour.
 * @param l Tableau des labels.
 * @param af Système d'argumentation.
 * @param query Argument à vérifier.
 */
void handleDSST(Label *l, ArgFramework *af, char *query)
{
    // first pass
    doCaminadaLabeling(l, af);
    int n = af->nbArg;
    int target = findArgumentIndex(query, af);
    if (containsUNDEC(l, n))
    {
        int count;
        int *undecArgs = pinpointUNDEC(l, af, &count);
        Label *lcopy = copyArray(l, af->nbArg);
        for (int i = 0; i < count; i++)
        {
            // reset label
            memcpy(lcopy, l, n * sizeof(Label));

            // we have a stable extension , and query is OUT at least once then we can break;
            if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == OUT)
            {
                free(undecArgs);
                free(lcopy);
                printf("NO\n");
                return;
            }
        }
        printf("YES\n");
        free(lcopy);
        free(undecArgs);
    }
    else
    {
        printf(l[target] == IN ? "YES\n" : "NO\n");
    }
}

/**
 * @brief Vérifie si le tableau contient des labels UNDECIDED.
 *
 * @param labs Tableau des labels.
 * @param size Taille du tableau.
 * @return 1 si un label est UNDECIDED, 0 sinon.
 */
int containsUNDEC(Label *labs, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (labs[i] == UNDECIDED)
        {
            return 1; // true there is a UNDEC
        }
    }
    return 0; // false
}

/**
 * @brief Affiche uniquement les arguments avec le label IN.
 *
 * @param labels Tableau des labels.
 * @param af Système d'argumentation.
 */
void printOnlyIN(Label *labels, ArgFramework *af)
{
    printf("[");
    int first = 1;
    for (int i = 0; i < af->nbArg; i++)
    {
        if (labels[i] == IN)
        {
            if (!first)
            {
                printf(",");
            }
            printf("%s", af->tab[i]);
            first = 0;
        }
    }
    printf("]\n");
}

/**
 * @brief Identifie les arguments marqués UNDECIDED.
 *
 * @param l Tableau des labels.
 * @param af Système d'argumentation.
 * @param count Pointeur vers le compteur des arguments UNDECIDED.
 * @return Tableau des indices UNDECIDED.
 */
int *pinpointUNDEC(Label *l, ArgFramework *af, int *count)
{
    int n = af->nbArg;
    int *arrayIndex = (int *)malloc(n * sizeof(int));
    (*count) = 0;
    for (int i = 0; i < n; i++)
    {
        if (l[i] == UNDECIDED)
        {
            arrayIndex[*count] = i;
            (*count)++;
        }
    }
    return arrayIndex;
}

/**
 * @brief Backtrack pour trouver une extension stable.
 * @param l Tableau des labels.
 * @param af Système d'argumentation.
 * @param undecArg Tableau des indices UNDECIDED.
 * @param undecCount Nombre d'UNDECIDED.
 * @param currentIndex Index actuel dans le backtrack.
 * @return 1 si une extension stable est trouvée, 0 sinon.
 *
 * ### Détails de l'algorithme :
 *
 * - Cas terminal :
 *
 *  - Lorsque `currentIndex` atteint `undecCount`, tous les arguments UNDECIDED ont été assignés.
 *  - Si des labels UNDECIDED subsistent, la fonction les traite via un nouvel appel de backtrack.
 *  - Si il n'y a aucun UNDECIDED, on vérifie si la configuration actuelle est une extension stable.
 *
 * - Exploration récursive :
 *
 *   - Pour chaque argument UNDECIDED à l'indice `currentIndex` :
 *     1. Le label est temporairement défini à IN :
 *        - Le système argumentation est mis à jour en propagant ce choix avec `doCaminadaLabeling`.
 *        - Si cette configuration est valide, l'exploration continue avec l'index suivant.
 *     2. Le label est ensuite temporairement défini à OUT :
 *        - Les conséquences de ce choix sont propagées.
 *        - Si cette configuration est valide, l'exploration continue avec l'index suivant.
 *   - Si ni IN ni OUT ne permettent de construire une extension stable, la fonction retourne 0.
 */
int backtrackST(Label *l, ArgFramework *af, int *undecArg, int undecCount, int currentIndex)
{
    // end case
    if (currentIndex == undecCount)
    {
        if (containsUNDEC(l, af->nbArg))
        {
            // there is still some undec
            int nb;
            int *undecArray = pinpointUNDEC(l, af, &nb);
            // we found a stable extension with the remaining UNDEC
            if (backtrackST(l, af, undecArray, nb, 0))
            {
                free(undecArray);
                return 1;
            }
            free(undecArray);
            // didn't found a stable extension
            return 0;
        }
        else
        {
            // we have no UNDEC
            if (isLegalExtension(l, af))
            {
                return 1;
            }
            // something illegal like OUT=>OUT or IN=>IN
            return 0;
        }
    }

    // retrieve which arg to set
    int argIndex = undecArg[currentIndex];
    Label *lcopy = copyArray(l, af->nbArg);
    lcopy[argIndex] = IN;
    // spread the newly IN
    doCaminadaLabeling(lcopy, af);
    // if its legal we go in otherwise there is a IN=>IN or OUT=>OUT somewhere
    if (isLegalExtension(lcopy, af))
    {
        if (backtrackST(lcopy, af, undecArg, undecCount, currentIndex + 1))
        {
            // save found stable extension to original
            memcpy(l, lcopy, af->nbArg * sizeof(Label));
            free(lcopy);
            return 1; // we found a valid stable extension, return success
        }
    }

    free(lcopy);
    lcopy = copyArray(l, af->nbArg);
    lcopy[argIndex] = OUT;
    // spread the newly OUT
    doCaminadaLabeling(lcopy, af);
    // if its legal we go in otherwise there is a IN=>IN or OUT=>OUT somewhere
    if (isLegalExtension(lcopy, af))
    {
        if (backtrackST(lcopy, af, undecArg, undecCount, currentIndex + 1))
        {
            // save found stable extension to original
            memcpy(l, lcopy, af->nbArg * sizeof(Label));
            free(lcopy);
            return 1;
        }
    }

    free(lcopy);
    return 0;
}

/**
 * @brief Vérifie si le labeling est légal.
 *
 * @param labs Tableau des labels.
 * @param af Système d'argumentation.
 * @return 1 si l'extension est valide, 0 sinon.
 */
int isLegalExtension(Label *labs, ArgFramework *af)
{
    // Initially called isStableExtension, but hey this code is more about whether the labeling is legal.
    for (int i = 0; i < af->nbArg; i++)
    {
        if (labs[i] == OUT && !oneAttackersIN(i, labs, af))
        {
            return 0; // there is no one who is IN =/> OUT
        }

        if (labs[i] == IN && oneAttackersIN(i, labs, af))
        {
            return 0; // incompatible IN => IN
        }
    }
    return 1; // Valid stable extension
}

/**
 * @brief Cherche si query est dans au moins 1 extension complète.
 * Méthode par backtracking. Affiche "YES" ou "NO". Ici on abuse du fait
 * qu'une extension stable est une extension complète. (optimisable)
 * @param labels Tableau des labels.
 * @param af Système d'argumentation.
 * @param query Argument à vérifier.
 */
void handleDCCO(Label *labels, ArgFramework *af, char *query)
{
    doCaminadaLabeling(labels, af);
    int target = findArgumentIndex(query, af);
    int n = af->nbArg;
    int count;
    int *undecArgs = pinpointUNDEC(labels, af, &count);
    Label *lcopy = copyArray(labels, n);

    for (int i = 0; i < count; i++)
    {
        memcpy(lcopy, labels, n * sizeof(Label));
        if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == IN)
        {
            printf("YES\n");
            free(undecArgs);
            free(lcopy);
            return;
        }
    }
    printf("NO\n");
    free(undecArgs);
    free(lcopy);
}

/**
 * @brief Cherche si query est dans toutes les extensions complètes.
 * Méthode par backtracking. Affiche "YES" ou "NO". Pour vérifier si
 * c'est dans toutes les extensions on parcourt tous les UNDEC qui reste
 * en testant chaqu'un leur tour. Ici on abuse du fait
 * qu'une extension stable est une extension complète. (optimisable)
 * @param labels Tableau des labels.
 * @param af Système d'argumentation.
 * @param query Argument à vérifier.
 */
void handleDSCO(Label *labels, ArgFramework *af, char *query)
{
    doCaminadaLabeling(labels, af);
    if (containsUNDEC(labels, af->nbArg))
    {
        printf("NO\n");
        return;
    }

    int target = findArgumentIndex(query, af);
    int n = af->nbArg;
    int count;
    int *undecArgs = pinpointUNDEC(labels, af, &count);
    Label *lcopy = copyArray(labels, n);

    for (int i = 0; i < count; i++)
    {
        memcpy(lcopy, labels, n * sizeof(Label));
        if (backtrackST(lcopy, af, undecArgs, count, i) && lcopy[target] == OUT)
        {
            printf("NO\n");
            free(undecArgs);
            free(lcopy);
            return;
        }
    }
    printf("YES\n");
    free(undecArgs);
    free(lcopy);
}