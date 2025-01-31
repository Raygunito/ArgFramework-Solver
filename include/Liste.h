#ifndef LISTE_H
#define LISTE_H
#include <stdlib.h>
#include "TypeListe.h"

Liste initListe();
Liste createCelListe();
int estVideListe(Liste l);

int donneeListe(Liste l);
Liste suivantListe(Liste l);
int tailleSuccListe(Liste l);
Liste dernierListe(Liste l);

void afficheListe(Liste l);

int rechercheElementListe(int numeroSom, Liste l);
Liste inserTeteListe(int numeroSom, Liste l);
Liste inserQueueListe(int numeroSom, Liste l);
#endif