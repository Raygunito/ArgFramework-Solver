#ifndef TYPE_LISTE_H
#define TYPE_LISTE_H
typedef struct cellule
{
    int somSucc;
    struct cellule *succSuiv;
}* Liste;

#endif