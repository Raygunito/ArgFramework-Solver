#include "Liste.h"
#include <stdio.h>
Liste initListe()
{
    return NULL;
}

int estVideListe(Liste l)
{
    return l == NULL;
}

Liste createCelListe()
{
    Liste tmp;
    tmp = (Liste)malloc(sizeof(struct cellule));
    tmp->succSuiv = initListe();
    return tmp;
}

int donneeListe(Liste l)
{
    return l->somSucc;
}

Liste suivantListe(Liste l)
{
    return l->succSuiv;
}

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

Liste dernierListe(Liste l)
{
    while (!estVideListe(suivantListe(l)))
    {
        l = suivantListe(l);
    }
    return l;
}

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

Liste inserTeteListe(int numeroSom, Liste l)
{
    Liste tmp = createCelListe();
    tmp->somSucc = numeroSom;
    tmp->succSuiv = l;
    l = tmp;
    return l;
}

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
