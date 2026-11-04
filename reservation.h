#ifndef RESERVATION_H
#define RESERVATION_H

#include "structures.h"
#include "livre.h"

/* =====================================================
   reservation.h / reservation.c
   Gestion des reservations. Donnees dans DATABASE/RESERVATIONS.dat
   ===================================================== */

int reservation_charger(Reservation tab[]);
void reservation_sauvegarder(Reservation tab[], int n);

int reservation_trouverParId(Reservation tab[], int n, int id);

/* Un utilisateur ne peut reserver que si le stock disponible du livre est 0.
   Renvoie le nouveau n (ou n si refuse). */
int reservation_creer(Reservation tab[], int n, int idUtilisateur, int idLivre, Livre livres[], int nbLivres);

void reservation_afficherTous(Reservation tab[], int n);
int reservation_annulerInteractif(Reservation tab[], int n);

/* Cherche la premiere reservation "EN_ATTENTE" pour un livre donne
   (la plus ancienne). Renvoie son indice, ou -1 si aucune. */
int reservation_trouverPremiereEnAttentePourLivre(Reservation tab[], int n, int idLivre);

/* Passe une reservation a l'etat "DISPONIBLE" */
void reservation_marquerDisponible(Reservation tab[], int index);

#endif
