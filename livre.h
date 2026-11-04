#ifndef LIVRE_H
#define LIVRE_H

#include "structures.h"
#include "auteur.h"
#include "categorie.h"

/* =====================================================
   livre.h / livre.c
   Gestion des livres. Donnees dans DATABASE/BOOKS.dat
   ===================================================== */

int livre_charger(Livre tab[]);
void livre_sauvegarder(Livre tab[], int n);

int livre_trouverParId(Livre tab[], int n, int id);
int livre_trouverParIsbn(Livre tab[], int n, const char isbn[]);

/* Compte combien de livres sont lies a un auteur (utile avant suppression) */
int livre_compterParAuteur(Livre tab[], int n, int idAuteur);

/* Compte combien de livres sont lies a une categorie */
int livre_compterParCategorie(Livre tab[], int n, int idCategorie);

/* Ajoute un livre. Verifie que l'auteur et la categorie existent bien,
   et que l'ISBN n'est pas deja utilise. Renvoie le nouveau n (ou n si erreur). */
int livre_ajouterInteractif(Livre tab[], int n, Auteur auteurs[], int nbAuteurs,
                             Categorie categories[], int nbCategories);

void livre_afficherTous(Livre tab[], int n);
void livre_modifierInteractif(Livre tab[], int n);
int livre_supprimerInteractif(Livre tab[], int n);

/* Augmente ou diminue le nombre d'exemplaires disponibles d'un livre.
   variation = -1 pour un emprunt, +1 pour un retour, par exemple. */
void livre_ajusterStock(Livre tab[], int index, int variation);

#endif
