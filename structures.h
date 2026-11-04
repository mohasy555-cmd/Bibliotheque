#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <time.h>

/* =====================================================
   Ce fichier contient toutes les "structures" (struct)
   du projet. Une structure regroupe plusieurs informations
   qui vont ensemble, par exemple : un utilisateur a un nom,
   un prenom, un login, etc.
   ===================================================== */

/* Tailles maximales pour les textes (chaines de caracteres) */
#define MAX_NOM         50
#define MAX_PRENOM      50
#define MAX_TEL         20
#define MAX_ADRESSE     100
#define MAX_EMAIL       60
#define MAX_LOGIN       7      /* 6 lettres + le caractere de fin '\0' */
#define MAX_MDP         50
#define MAX_ROLE        10
#define MAX_ETAT        15

#define MAX_NATIONALITE 40
#define MAX_BIO         300

#define MAX_LIBELLE     50
#define MAX_DESCRIPTION 200

#define MAX_ISBN        20
#define MAX_TITRE       100
#define MAX_EDITEUR     60
#define MAX_LANGUE      20
#define MAX_EMPLACEMENT 30
#define MAX_RESUME      300

#define MAX_NUM_EMPRUNT 25

/* Nombre maximum d'elements qu'on peut garder en memoire.
   (On aurait pu utiliser des tableaux "dynamiques", mais pour
   rester simple on utilise des tableaux de taille fixe.) */
#define MAX_UTILISATEURS  200
#define MAX_AUTEURS       200
#define MAX_CATEGORIES    100
#define MAX_LIVRES        500
#define MAX_EMPRUNTS      1000
#define MAX_RETOURS       1000
#define MAX_RESERVATIONS  500
#define MAX_PENALITES     1000

/* ---------- 1. Utilisateur (administrateur ou lecteur) ---------- */
typedef struct {
    int  id;
    char nom[MAX_NOM];
    char prenom[MAX_PRENOM];
    char telephone[MAX_TEL];
    char adresse[MAX_ADRESSE];
    char email[MAX_EMAIL];
    char login[MAX_LOGIN];
    char motDePasse[MAX_MDP];   /* mot de passe "crypte" (voir utilisateur.c) */
    char role[MAX_ROLE];        /* "ADMIN" ou "USER" */
    char etat[MAX_ETAT];        /* "ACTIF" ou "BLOQUE" */
    time_t dateCreation;
    time_t derniereConnexion;
    int  doitChangerMdp;        /* 1 = doit changer son mot de passe, 0 = non */
} Utilisateur;

/* ---------- 2. Auteur ---------- */
typedef struct {
    int  id;
    char nomComplet[MAX_NOM + MAX_PRENOM];
    char nationalite[MAX_NATIONALITE];
    time_t dateNaissance;
    char biographie[MAX_BIO];
    int  nombreLivresPublies;
} Auteur;

/* ---------- 3. Categorie ---------- */
typedef struct {
    int  id;
    char libelle[MAX_LIBELLE];
    char description[MAX_DESCRIPTION];
    time_t dateCreation;
} Categorie;

/* ---------- 4. Livre ---------- */
typedef struct {
    int  id;
    char isbn[MAX_ISBN];
    char titre[MAX_TITRE];
    int  idAuteur;
    int  idCategorie;
    char editeur[MAX_EDITEUR];
    int  anneePublication;
    char langue[MAX_LANGUE];
    int  nombrePages;
    int  nombreExemplaires;
    int  exemplairesDisponibles;
    char emplacement[MAX_EMPLACEMENT];
    char resume[MAX_RESUME];
    time_t dateAjout;
} Livre;

/* ---------- 5. Emprunt ---------- */
typedef struct {
    int  id;
    char numeroEmprunt[MAX_NUM_EMPRUNT]; /* format EMP_AAAAMMJJHHMMSS */
    int  idUtilisateur;
    int  idLivre;
    time_t dateEmprunt;
    time_t dateRetourPrevue;
    char etat[MAX_ETAT]; /* "EN_COURS", "RETOURNE", "EN_RETARD" */
} Emprunt;

/* ---------- 6. Retour ---------- */
typedef struct {
    int  id;
    int  idEmprunt;
    time_t dateRetour;
    int  joursRetard;
    int  montantPenalite;
} Retour;

/* ---------- 7. Reservation ---------- */
typedef struct {
    int  id;
    int  idUtilisateur;
    int  idLivre;
    time_t dateReservation;
    char etat[MAX_ETAT]; /* "EN_ATTENTE", "DISPONIBLE", "ANNULEE" */
} Reservation;

/* ---------- 8. Penalite ---------- */
typedef struct {
    int  id;
    int  idUtilisateur;
    int  idEmprunt;
    int  joursRetard;
    int  montant;
    time_t date;
} Penalite;

#endif
