#ifndef RETOUR_H
#define RETOUR_H

#include "structures.h"
#include "emprunt.h"
#include "livre.h"
#include "penalite.h"
#include "reservation.h"
#include "utilisateur.h"

/* =====================================================
   retour.h / retour.c
   Gestion des retours de livres. Donnees dans DATABASE/RETURNS.dat
   ===================================================== */

int retour_charger(Retour tab[]);
void retour_sauvegarder(Retour tab[], int n);

/* Enregistre le retour d'un emprunt (idEmprunt).
   - met a jour l'etat de l'emprunt ("RETOURNE")
   - augmente le stock disponible du livre
   - calcule un retard eventuel et cree une penalite si besoin
     (nPenalites est modifie via son pointeur car son nombre peut augmenter)
   - si une reservation attendait ce livre, elle passe a "DISPONIBLE"
   Renvoie le nouveau nombre de retours (n+1), ou n si erreur. */
int retour_creer(Retour tab[], int n, int idEmprunt,
                  Emprunt emprunts[], int nEmprunts,
                  Livre livres[], int nLivres,
                  Penalite penalites[], int *nPenalites,
                  Reservation reservations[], int nReservations,
                  Utilisateur utilisateurs[], int nUtilisateurs);

void retour_afficherTous(Retour tab[], int n);

#endif
