#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include "structures.h"

/* =====================================================
   utilisateur.h / utilisateur.c
   Gestion des utilisateurs (administrateurs et lecteurs).
   Les donnees sont stockees dans DATABASE/USERS.dat
   ===================================================== */

/* Charge tous les utilisateurs depuis le fichier dans le tableau "tab".
   Renvoie le nombre d'utilisateurs charges. */
int utilisateur_charger(Utilisateur tab[]);

/* Enregistre tout le tableau "tab" (de taille n) dans le fichier */
void utilisateur_sauvegarder(Utilisateur tab[], int n);

/* Cherche un utilisateur par son login. Renvoie l'indice dans le tableau,
   ou -1 si non trouve. */
int utilisateur_trouverParLogin(Utilisateur tab[], int n, const char login[]);

/* Cherche un utilisateur par son id. Renvoie l'indice, ou -1 si non trouve. */
int utilisateur_trouverParId(Utilisateur tab[], int n, int id);

/* Demande les informations au clavier et ajoute un nouvel utilisateur.
   role doit valoir "ADMIN" ou "USER".
   Renvoie le nouveau nombre d'utilisateurs (n+1), ou n si erreur. */
int utilisateur_ajouterInteractif(Utilisateur tab[], int n, const char role[]);

/* Affiche la liste de tous les utilisateurs */
void utilisateur_afficherTous(Utilisateur tab[], int n);

/* Modifie un utilisateur (choisi par son id), en interactif */
void utilisateur_modifierInteractif(Utilisateur tab[], int n);

/* Supprime un utilisateur (choisi par son id) */
int utilisateur_supprimerInteractif(Utilisateur tab[], int n);

/* Verifie le login + mot de passe. Renvoie :
    -1 si login inconnu
    -2 si mot de passe incorrect
    -3 si le compte est bloque
    sinon l'indice de l'utilisateur dans le tableau */
int utilisateur_connexion(Utilisateur tab[], int n, const char login[], const char mdp[]);

/* Change le mot de passe de l'utilisateur a l'indice "index" */
void utilisateur_changerMotDePasse(Utilisateur tab[], int index);

/* "Crypte" un mot de passe en clair et met le resultat dans "resultat".
   (Ici on utilise un simple decalage de lettres, appele chiffrement de Cesar,
   ce n'est pas un vrai systeme securise mais cela respecte la consigne
   "mot de passe enregistre sous forme cryptee" pour un projet d'etude.) */
void utilisateur_crypter(const char clair[], char resultat[]);

#endif
