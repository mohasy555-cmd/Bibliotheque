#ifndef PENALITE_H
#define PENALITE_H

#include "structures.h"

/* =====================================================
   penalite.h / penalite.c
   Gestion des penalites de retard. Donnees dans DATABASE/PENALTIES.dat
   ===================================================== */

#define MONTANT_PENALITE_PAR_JOUR 500 /* 500 FCFA par jour de retard */

int penalite_charger(Penalite tab[]);
void penalite_sauvegarder(Penalite tab[], int n);

/* Ajoute une penalite (appele automatiquement par le module retour).
   Renvoie le nouveau n. */
int penalite_ajouter(Penalite tab[], int n, int idUtilisateur, int idEmprunt, int joursRetard);

void penalite_afficherTous(Penalite tab[], int n);

/* Affiche seulement les penalites d'un utilisateur */
void penalite_afficherPourUtilisateur(Penalite tab[], int n, int idUtilisateur);

/* Calcule le montant total de toutes les penalites (utile pour les statistiques) */
int penalite_calculerTotal(Penalite tab[], int n);

#endif
