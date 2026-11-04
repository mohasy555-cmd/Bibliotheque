#ifndef EMPRUNT_H
#define EMPRUNT_H

#include "structures.h"
#include "livre.h"
#include "utilisateur.h"

/* =====================================================
   emprunt.h / emprunt.c
   Gestion des emprunts. Donnees dans DATABASE/BORROWS.dat
   ===================================================== */

#define LIMITE_EMPRUNTS_PAR_UTILISATEUR 3
#define DUREE_EMPRUNT_JOURS 14

int emprunt_charger(Emprunt tab[]);
void emprunt_sauvegarder(Emprunt tab[], int n);

int emprunt_trouverParId(Emprunt tab[], int n, int id);

/* Compte le nombre d'emprunts "EN_COURS" (ou "EN_RETARD") pour un utilisateur */
int emprunt_compterEnCoursPourUtilisateur(Emprunt tab[], int n, int idUtilisateur);

/* Realise un emprunt pour idUtilisateur du livre idLivre.
   Verifie la limite de 3 emprunts et la disponibilite du livre.
   Met a jour le stock du livre, cree le recu, ecrit l'historique.
   Renvoie le nouveau n (ou n si l'emprunt est refuse). */
int emprunt_creer(Emprunt tab[], int n, int idUtilisateur, int idLivre,
                   Livre livres[], int nbLivres,
                   Utilisateur utilisateurs[], int nbUtilisateurs);

void emprunt_afficherTous(Emprunt tab[], int n);

/* Affiche uniquement les emprunts d'un utilisateur donne */
void emprunt_afficherPourUtilisateur(Emprunt tab[], int n, int idUtilisateur);

#endif
