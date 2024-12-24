#include "Liste.h"
#include <stdio.h>
/**
 * @brief Retourne NULL mais de "type" Liste
 *
 * @return Liste NULL
 */
Liste initListe()
{
    return NULL;
}

/**
 * @brief vérifie si l est vide ou non càd si
 * on est à la fin de la liste chaînée
 *
 * @param l Liste
 * @return int 1 si vide, 0 sinon
 */
int estVideListe(Liste l)
{
    return l == NULL;
}

/**
 * @brief Alloue et crée une cellule
 *
 * @return Liste alloué mais non initialisé (valeur)
 */
Liste createCelListe()
{
    Liste tmp;
    tmp = (Liste)malloc(sizeof(struct cellule));
    tmp->succSuiv = initListe();
    return tmp;
}

/**
 * @brief retourne la donnée, càd le sommet du 
 * graphe dans notre cas
 *
 * @param l Liste
 * @return int sommet du graphe
 */
int donneeListe(Liste l)
{
    return l->somSucc;
}

/**
 * @brief retourne la cellule suivante contenu 
 * dans la Liste
 * 
 * @param l 
 * @return Liste la cellule suivante
 */
Liste suivantListe(Liste l)
{
    return l->succSuiv;
}

/**
 * @brief retourne le nombre d'éléments restants 
 * de la liste
 * 
 * @param l 
 * @return int 
 */
int tailleSuccListe(Liste l)
{
    int cpt = 0;
    while (!(estVideListe(l)))
    {
        cpt++;
        l = suivantListe(l);
    }
    return cpt;
}

/**
 * @brief retourne la dernière cellule l
 * càd l->next = NULL;
 * 
 * @param l 
 * @return Liste 
 */
Liste dernierListe(Liste l)
{
    while (!estVideListe(suivantListe(l)))
    {
        l = suivantListe(l);
    }
    return l;
}

/// @brief Affiche la liste L de manière lisible
/// @param l Liste
void afficheListe(Liste l)
{
    int numSom;
    while (!estVideListe(l))
    {
        numSom = donneeListe(l);
        printf("(%d)", numSom);
        l = suivantListe(l);
        if (!estVideListe(l))
        {
            printf("-> ");
        }
    }
    printf("\n");
}

/**
 * @brief Détermine la présence d'un élément 
 * dans la liste
 * 
 * @param numeroSom élément à chercher
 * @param l Liste
 * @return int 1 si trouvé, 0 sinon
 */
int rechercheElementListe(int numeroSom, Liste l)
{
    if (estVideListe(l))
    {
        return 0;
    }
    if (l->somSucc == numeroSom)
    {
        return 1;
    }
    return rechercheElementListe(numeroSom, suivantListe(l));
}

/**
 * @brief Insertion en tête : Pointeur -> (NewCellule) -> L
 * 
 * @param numeroSom 
 * @param l 
 * @return Liste 
 */
Liste inserTeteListe(int numeroSom, Liste l)
{
    Liste tmp = createCelListe();
    tmp->somSucc = numeroSom;
    tmp->succSuiv = l;
    l = tmp;
    return l;
}

/**
 * @brief Insertion à la fin : Pointeur -> L ->...->Last->(NewCellule)->NULL 
 * 
 * @param numeroSom 
 * @param l 
 * @return Liste 
 */
Liste inserQueueListe(int numeroSom, Liste l)
{
    if (estVideListe(l))
    {
        return inserTeteListe(numeroSom, l);
    }
    else
    {
        Liste dernier = dernierListe(l);
        dernier->succSuiv = inserTeteListe(numeroSom, suivantListe(dernier));
    }
    return l;
}
