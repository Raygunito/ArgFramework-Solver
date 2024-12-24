#ifndef TYPE_LISTE_H
#define TYPE_LISTE_H
/**
 * @brief Structure de données pour
 * créer une liste chaînée contenant 
 * le sommet du successeur.
 */
typedef struct cellule
{
    int somSucc;
    struct cellule *succSuiv;
} *Liste;

#endif