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

int donneeSuccListe(Liste l)
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
        numSom = donneeSuccListe(l);
        printf("(%d)", numSom);
        l = suivantListe(l);
        if (!estVideListe(l))
        {
            printf("-> ");
        }
    }
    printf("\n");
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
