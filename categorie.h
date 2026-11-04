#ifndef CATEGORIE_H
#define CATEGORIE_H

#include "structures.h"

/* =====================================================
   categorie.h / categorie.c
   Gestion des categories. Donnees dans DATABASE/CATEGORIES.dat
   ===================================================== */

int categorie_charger(Categorie tab[]);
void categorie_sauvegarder(Categorie tab[], int n);

int categorie_trouverParId(Categorie tab[], int n, int id);

int categorie_ajouterInteractif(Categorie tab[], int n);
void categorie_afficherTous(Categorie tab[], int n);
void categorie_modifierInteractif(Categorie tab[], int n);
int categorie_supprimerInteractif(Categorie tab[], int n, int nombreLivresAssocies);

#endif
