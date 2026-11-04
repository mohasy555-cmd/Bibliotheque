#ifndef AUTEUR_H
#define AUTEUR_H

#include "structures.h"

/* =====================================================
   auteur.h / auteur.c
   Gestion des auteurs. Donnees stockees dans DATABASE/AUTHORS.dat
   ===================================================== */

int auteur_charger(Auteur tab[]);
void auteur_sauvegarder(Auteur tab[], int n);

int auteur_trouverParId(Auteur tab[], int n, int id);

int auteur_ajouterInteractif(Auteur tab[], int n);
void auteur_afficherTous(Auteur tab[], int n);
void auteur_modifierInteractif(Auteur tab[], int n);

/* Supprime un auteur seulement si "nombreLivresAssocies" vaut 0.
   (Ce nombre est calcule par le module livre et transmis ici.) */
int auteur_supprimerInteractif(Auteur tab[], int n, int nombreLivresAssocies);

#endif
