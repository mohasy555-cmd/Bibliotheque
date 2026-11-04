#include <stdio.h>
#include <string.h>
#include <time.h>
#include "menu.h"
#include "utils.h"
#include "utilisateur.h"
#include "auteur.h"
#include "categorie.h"
#include "livre.h"
#include "emprunt.h"
#include "retour.h"
#include "reservation.h"
#include "penalite.h"

/* ---------------------------------------------------------
   Petite fonction pour "vider" ce qui reste sur la ligne
   apres un scanf("%d", ...), sinon les scanf suivants buguent.
   --------------------------------------------------------- */
static void viderBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* on ne fait rien, on avance juste jusqu'a la fin de la ligne */
    }
}

/* ---------------------------------------------------------
   Statistiques simples (utilisees par le menu admin)
   --------------------------------------------------------- */
static void afficherStatistiques(Utilisateur utilisateurs[], int nUtilisateurs,
                                  Livre livres[], int nLivres,
                                  Emprunt emprunts[], int nEmprunts,
                                  Retour retours[], int nRetours,
                                  Reservation reservations[], int nReservations,
                                  Penalite penalites[], int nPenalites)
{
    int i;

    printf("\n===== STATISTIQUES =====\n");
    printf("Nombre d'utilisateurs : %d\n", nUtilisateurs);
    printf("Nombre de livres      : %d\n", nLivres);
    printf("Nombre d'emprunts     : %d\n", nEmprunts);
    printf("Nombre de retours     : %d\n", nRetours);
    printf("Nombre de reservations: %d\n", nReservations);
    printf("Nombre de penalites   : %d\n", nPenalites);
    printf("Montant total penalites : %d FCFA\n", penalite_calculerTotal(penalites, nPenalites));

    /* Livre le plus emprunte */
    if (nLivres > 0 && nEmprunts > 0) {
        int meilleurIndexLivre = 0;
        int meilleurCompte = -1;

        for (i = 0; i < nLivres; i++) {
            int compte = 0;
            int j;
            for (j = 0; j < nEmprunts; j++) {
                if (emprunts[j].idLivre == livres[i].id) {
                    compte++;
                }
            }
            if (compte > meilleurCompte) {
                meilleurCompte = compte;
                meilleurIndexLivre = i;
            }
        }
        printf("Livre le plus emprunte : %s (%d emprunt(s))\n",
               livres[meilleurIndexLivre].titre, meilleurCompte);
    }

    /* Utilisateur le plus actif (celui qui a le plus d'emprunts) */
    if (nUtilisateurs > 0 && nEmprunts > 0) {
        int meilleurIndexUtil = 0;
        int meilleurCompte = -1;

        for (i = 0; i < nUtilisateurs; i++) {
            int compte = 0;
            int j;
            for (j = 0; j < nEmprunts; j++) {
                if (emprunts[j].idUtilisateur == utilisateurs[i].id) {
                    compte++;
                }
            }
            if (compte > meilleurCompte) {
                meilleurCompte = compte;
                meilleurIndexUtil = i;
            }
        }
        printf("Utilisateur le plus actif : %s %s (%d emprunt(s))\n",
               utilisateurs[meilleurIndexUtil].prenom,
               utilisateurs[meilleurIndexUtil].nom,
               meilleurCompte);
    }

    printf("=========================\n");
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des utilisateurs
   --------------------------------------------------------- */
static void menuGestionUtilisateurs(Utilisateur utilisateurs[], int *nUtilisateurs)
{
    int choix;

    do {
        printf("\n--- Gestion des utilisateurs ---\n");
        printf("1. Ajouter un administrateur\n");
        printf("2. Ajouter un lecteur\n");
        printf("3. Afficher tous les utilisateurs\n");
        printf("4. Modifier un utilisateur\n");
        printf("5. Supprimer un utilisateur\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                *nUtilisateurs = utilisateur_ajouterInteractif(utilisateurs, *nUtilisateurs, "ADMIN");
                utilisateur_sauvegarder(utilisateurs, *nUtilisateurs);
                break;
            case 2:
                *nUtilisateurs = utilisateur_ajouterInteractif(utilisateurs, *nUtilisateurs, "USER");
                utilisateur_sauvegarder(utilisateurs, *nUtilisateurs);
                break;
            case 3:
                utilisateur_afficherTous(utilisateurs, *nUtilisateurs);
                break;
            case 4:
                utilisateur_modifierInteractif(utilisateurs, *nUtilisateurs);
                utilisateur_sauvegarder(utilisateurs, *nUtilisateurs);
                break;
            case 5:
                *nUtilisateurs = utilisateur_supprimerInteractif(utilisateurs, *nUtilisateurs);
                utilisateur_sauvegarder(utilisateurs, *nUtilisateurs);
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des auteurs
   --------------------------------------------------------- */
static void menuGestionAuteurs(Auteur auteurs[], int *nAuteurs, Livre livres[], int nLivres)
{
    int choix;

    do {
        printf("\n--- Gestion des auteurs ---\n");
        printf("1. Ajouter un auteur\n");
        printf("2. Afficher tous les auteurs\n");
        printf("3. Modifier un auteur\n");
        printf("4. Supprimer un auteur\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                *nAuteurs = auteur_ajouterInteractif(auteurs, *nAuteurs);
                auteur_sauvegarder(auteurs, *nAuteurs);
                break;
            case 2:
                auteur_afficherTous(auteurs, *nAuteurs);
                break;
            case 3:
                auteur_modifierInteractif(auteurs, *nAuteurs);
                auteur_sauvegarder(auteurs, *nAuteurs);
                break;
            case 4: {
                /* On demande d'abord l'id pour compter les livres associes,
                   la fonction elle-meme redemandera l'id pour supprimer */
                printf("(Rappel : un auteur ne peut etre supprime que s'il n'a aucun livre associe)\n");
                int idTemp;
                printf("Id de l'auteur : ");
                scanf("%d", &idTemp);
                viderBuffer();
                int nb = livre_compterParAuteur(livres, nLivres, idTemp);
                /* on remet l'id dans le "flux" en le redemandant simplement via une petite astuce :
                   ici on recree une fonction locale simplifiee */
                int index = auteur_trouverParId(auteurs, *nAuteurs, idTemp);
                if (index == -1) {
                    printf("Auteur introuvable.\n");
                } else if (nb > 0) {
                    printf("Impossible de supprimer : cet auteur possede encore %d livre(s).\n", nb);
                } else {
                    int i;
                    for (i = index; i < *nAuteurs - 1; i++) {
                        auteurs[i] = auteurs[i + 1];
                    }
                    (*nAuteurs)--;
                    printf("Auteur supprime avec succes.\n");
                }
                auteur_sauvegarder(auteurs, *nAuteurs);
                break;
            }
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des categories
   --------------------------------------------------------- */
static void menuGestionCategories(Categorie categories[], int *nCategories, Livre livres[], int nLivres)
{
    int choix;

    do {
        printf("\n--- Gestion des categories ---\n");
        printf("1. Ajouter une categorie\n");
        printf("2. Afficher toutes les categories\n");
        printf("3. Modifier une categorie\n");
        printf("4. Supprimer une categorie\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                *nCategories = categorie_ajouterInteractif(categories, *nCategories);
                categorie_sauvegarder(categories, *nCategories);
                break;
            case 2:
                categorie_afficherTous(categories, *nCategories);
                break;
            case 3:
                categorie_modifierInteractif(categories, *nCategories);
                categorie_sauvegarder(categories, *nCategories);
                break;
            case 4: {
                printf("(Rappel : une categorie ne peut etre supprimee que si aucun livre ne l'utilise)\n");
                int idTemp;
                printf("Id de la categorie : ");
                scanf("%d", &idTemp);
                viderBuffer();
                int nb = livre_compterParCategorie(livres, nLivres, idTemp);
                int index = categorie_trouverParId(categories, *nCategories, idTemp);
                if (index == -1) {
                    printf("Categorie introuvable.\n");
                } else if (nb > 0) {
                    printf("Impossible de supprimer : %d livre(s) utilisent cette categorie.\n", nb);
                } else {
                    int i;
                    for (i = index; i < *nCategories - 1; i++) {
                        categories[i] = categories[i + 1];
                    }
                    (*nCategories)--;
                    printf("Categorie supprimee avec succes.\n");
                }
                categorie_sauvegarder(categories, *nCategories);
                break;
            }
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des livres
   --------------------------------------------------------- */
static void menuGestionLivres(Livre livres[], int *nLivres, Auteur auteurs[], int nAuteurs,
                               Categorie categories[], int nCategories)
{
    int choix;

    do {
        printf("\n--- Gestion des livres ---\n");
        printf("1. Ajouter un livre\n");
        printf("2. Afficher tous les livres\n");
        printf("3. Modifier un livre\n");
        printf("4. Supprimer un livre\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                *nLivres = livre_ajouterInteractif(livres, *nLivres, auteurs, nAuteurs, categories, nCategories);
                livre_sauvegarder(livres, *nLivres);
                break;
            case 2:
                livre_afficherTous(livres, *nLivres);
                break;
            case 3:
                livre_modifierInteractif(livres, *nLivres);
                livre_sauvegarder(livres, *nLivres);
                break;
            case 4:
                *nLivres = livre_supprimerInteractif(livres, *nLivres);
                livre_sauvegarder(livres, *nLivres);
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des emprunts (cote administrateur)
   --------------------------------------------------------- */
static void menuGestionEmprunts(Emprunt emprunts[], int *nEmprunts,
                                 Livre livres[], int nLivres,
                                 Utilisateur utilisateurs[], int nUtilisateurs)
{
    int choix;

    do {
        printf("\n--- Gestion des emprunts ---\n");
        printf("1. Enregistrer un emprunt\n");
        printf("2. Afficher tous les emprunts\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1: {
                int idUtil, idLivre;
                printf("Id de l'utilisateur : ");
                scanf("%d", &idUtil);
                printf("Id du livre : ");
                scanf("%d", &idLivre);
                viderBuffer();
                *nEmprunts = emprunt_creer(emprunts, *nEmprunts, idUtil, idLivre,
                                            livres, nLivres, utilisateurs, nUtilisateurs);
                emprunt_sauvegarder(emprunts, *nEmprunts);
                livre_sauvegarder(livres, nLivres);
                break;
            }
            case 2:
                emprunt_afficherTous(emprunts, *nEmprunts);
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des retours (cote administrateur)
   --------------------------------------------------------- */
static void menuGestionRetours(Retour retours[], int *nRetours,
                                Emprunt emprunts[], int nEmprunts,
                                Livre livres[], int nLivres,
                                Penalite penalites[], int *nPenalites,
                                Reservation reservations[], int nReservations,
                                Utilisateur utilisateurs[], int nUtilisateurs)
{
    int choix;

    do {
        printf("\n--- Gestion des retours ---\n");
        printf("1. Enregistrer un retour\n");
        printf("2. Afficher tous les retours\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1: {
                int idEmprunt;
                printf("Id de l'emprunt a retourner : ");
                scanf("%d", &idEmprunt);
                viderBuffer();
                *nRetours = retour_creer(retours, *nRetours, idEmprunt,
                                          emprunts, nEmprunts,
                                          livres, nLivres,
                                          penalites, nPenalites,
                                          reservations, nReservations,
                                          utilisateurs, nUtilisateurs);
                retour_sauvegarder(retours, *nRetours);
                emprunt_sauvegarder(emprunts, nEmprunts);
                livre_sauvegarder(livres, nLivres);
                penalite_sauvegarder(penalites, *nPenalites);
                break;
            }
            case 2:
                retour_afficherTous(retours, *nRetours);
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des reservations (cote administrateur)
   --------------------------------------------------------- */
static void menuGestionReservations(Reservation reservations[], int *nReservations, Livre livres[], int nLivres)
{
    int choix;

    do {
        printf("\n--- Gestion des reservations ---\n");
        printf("1. Ajouter une reservation\n");
        printf("2. Afficher toutes les reservations\n");
        printf("3. Annuler une reservation\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1: {
                int idUtil, idLivre;
                printf("Id de l'utilisateur : ");
                scanf("%d", &idUtil);
                printf("Id du livre : ");
                scanf("%d", &idLivre);
                viderBuffer();
                *nReservations = reservation_creer(reservations, *nReservations, idUtil, idLivre, livres, nLivres);
                reservation_sauvegarder(reservations, *nReservations);
                break;
            }
            case 2:
                reservation_afficherTous(reservations, *nReservations);
                break;
            case 3:
                *nReservations = reservation_annulerInteractif(reservations, *nReservations);
                reservation_sauvegarder(reservations, *nReservations);
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Sous-menu : Gestion des penalites (cote administrateur)
   --------------------------------------------------------- */
static void menuGestionPenalites(Penalite penalites[], int nPenalites)
{
    int choix;

    do {
        printf("\n--- Gestion des penalites ---\n");
        printf("1. Afficher toutes les penalites\n");
        printf("2. Voir le montant total\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                penalite_afficherTous(penalites, nPenalites);
                break;
            case 2:
                printf("Montant total des penalites : %d FCFA\n", penalite_calculerTotal(penalites, nPenalites));
                break;
            case 0:
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Menu principal de l'administrateur
   --------------------------------------------------------- */
static void menuAdmin(Utilisateur utilisateurs[], int *nUtilisateurs,
                       Auteur auteurs[], int *nAuteurs,
                       Categorie categories[], int *nCategories,
                       Livre livres[], int *nLivres,
                       Emprunt emprunts[], int *nEmprunts,
                       Retour retours[], int *nRetours,
                       Reservation reservations[], int *nReservations,
                       Penalite penalites[], int *nPenalites)
{
    int choix;

    do {
        printf("\n========= MENU ADMINISTRATEUR =========\n");
        printf("1. Gestion des utilisateurs\n");
        printf("2. Gestion des auteurs\n");
        printf("3. Gestion des categories\n");
        printf("4. Gestion des livres\n");
        printf("5. Gestion des emprunts\n");
        printf("6. Gestion des retours\n");
        printf("7. Gestion des reservations\n");
        printf("8. Gestion des penalites\n");
        printf("9. Statistiques\n");
        printf("0. Se deconnecter\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                menuGestionUtilisateurs(utilisateurs, nUtilisateurs);
                break;
            case 2:
                menuGestionAuteurs(auteurs, nAuteurs, livres, *nLivres);
                break;
            case 3:
                menuGestionCategories(categories, nCategories, livres, *nLivres);
                break;
            case 4:
                menuGestionLivres(livres, nLivres, auteurs, *nAuteurs, categories, *nCategories);
                break;
            case 5:
                menuGestionEmprunts(emprunts, nEmprunts, livres, *nLivres, utilisateurs, *nUtilisateurs);
                break;
            case 6:
                menuGestionRetours(retours, nRetours, emprunts, *nEmprunts, livres, *nLivres,
                                    penalites, nPenalites, reservations, *nReservations,
                                    utilisateurs, *nUtilisateurs);
                break;
            case 7:
                menuGestionReservations(reservations, nReservations, livres, *nLivres);
                break;
            case 8:
                menuGestionPenalites(penalites, *nPenalites);
                break;
            case 9:
                afficherStatistiques(utilisateurs, *nUtilisateurs, livres, *nLivres,
                                      emprunts, *nEmprunts, retours, *nRetours,
                                      reservations, *nReservations, penalites, *nPenalites);
                break;
            case 0:
                printf("Deconnexion...\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Menu principal du lecteur
   --------------------------------------------------------- */
static void menuLecteur(Livre livres[], int nLivres,
                         Emprunt emprunts[], int *nEmprunts,
                         Retour retours[], int *nRetours,
                         Reservation reservations[], int *nReservations,
                         Penalite penalites[], int *nPenalites,
                         Utilisateur utilisateurs[], int nUtilisateurs,
                         int idUtilisateurConnecte)
{
    int choix;

    do {
        printf("\n========= MENU LECTEUR =========\n");
        printf("1. Consulter les livres\n");
        printf("2. Emprunter un livre\n");
        printf("3. Retourner un livre\n");
        printf("4. Reserver un livre\n");
        printf("5. Mes emprunts\n");
        printf("6. Mes penalites\n");
        printf("0. Se deconnecter\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix) != 1) { choix = 0; break; }
        viderBuffer();

        switch (choix) {
            case 1:
                livre_afficherTous(livres, nLivres);
                break;
            case 2: {
                int idLivre;
                printf("Id du livre a emprunter : ");
                scanf("%d", &idLivre);
                viderBuffer();
                *nEmprunts = emprunt_creer(emprunts, *nEmprunts, idUtilisateurConnecte, idLivre,
                                            livres, nLivres, utilisateurs, nUtilisateurs);
                emprunt_sauvegarder(emprunts, *nEmprunts);
                livre_sauvegarder(livres, nLivres);
                break;
            }
            case 3: {
                int idEmprunt;
                printf("Id de votre emprunt a retourner (voir 'Mes emprunts') : ");
                scanf("%d", &idEmprunt);
                viderBuffer();
                *nRetours = retour_creer(retours, *nRetours, idEmprunt,
                                          emprunts, *nEmprunts, livres, nLivres,
                                          penalites, nPenalites,
                                          reservations, *nReservations,
                                          utilisateurs, nUtilisateurs);
                retour_sauvegarder(retours, *nRetours);
                emprunt_sauvegarder(emprunts, *nEmprunts);
                livre_sauvegarder(livres, nLivres);
                penalite_sauvegarder(penalites, *nPenalites);
                break;
            }
            case 4: {
                int idLivre;
                printf("Id du livre a reserver : ");
                scanf("%d", &idLivre);
                viderBuffer();
                *nReservations = reservation_creer(reservations, *nReservations, idUtilisateurConnecte, idLivre, livres, nLivres);
                reservation_sauvegarder(reservations, *nReservations);
                break;
            }
            case 5:
                emprunt_afficherPourUtilisateur(emprunts, *nEmprunts, idUtilisateurConnecte);
                break;
            case 6:
                penalite_afficherPourUtilisateur(penalites, *nPenalites, idUtilisateurConnecte);
                break;
            case 0:
                printf("Deconnexion...\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ---------------------------------------------------------
   Fonction principale du menu (appelee depuis main.c)
   --------------------------------------------------------- */
void menu_lancer(void)
{
    /* On declare ici tous les tableaux : c'est notre "base de donnees"
       chargee en memoire pendant toute l'execution du programme. */
    static Utilisateur utilisateurs[MAX_UTILISATEURS];
    static Auteur      auteurs[MAX_AUTEURS];
    static Categorie   categories[MAX_CATEGORIES];
    static Livre       livres[MAX_LIVRES];
    static Emprunt     emprunts[MAX_EMPRUNTS];
    static Retour      retours[MAX_RETOURS];
    static Reservation reservations[MAX_RESERVATIONS];
    static Penalite    penalites[MAX_PENALITES];

    int nUtilisateurs = utilisateur_charger(utilisateurs);
    int nAuteurs      = auteur_charger(auteurs);
    int nCategories   = categorie_charger(categories);
    int nLivres       = livre_charger(livres);
    int nEmprunts     = emprunt_charger(emprunts);
    int nRetours      = retour_charger(retours);
    int nReservations = reservation_charger(reservations);
    int nPenalites    = penalite_charger(penalites);

    /* Si aucun utilisateur n'existe encore, on cree un administrateur
       par defaut pour pouvoir se connecter la premiere fois. */
    if (nUtilisateurs == 0) {
        Utilisateur admin;
        admin.id = 1;
        strcpy(admin.nom, "Admin");
        strcpy(admin.prenom, "Principal");
        strcpy(admin.telephone, "770000000");
        strcpy(admin.adresse, "ISI Dakar");
        strcpy(admin.email, "admin@elibrary.sn");
        strcpy(admin.login, "ADMINI");
        utilisateur_crypter("Library123", admin.motDePasse);
        strcpy(admin.role, "ADMIN");
        strcpy(admin.etat, "ACTIF");
        admin.dateCreation = time(NULL);
        admin.derniereConnexion = 0;
        admin.doitChangerMdp = 1;

        utilisateurs[0] = admin;
        nUtilisateurs = 1;
        utilisateur_sauvegarder(utilisateurs, nUtilisateurs);

        printf("Aucun utilisateur trouve : un compte administrateur a ete cree.\n");
        printf("Login : ADMINI   Mot de passe : Library123\n");
    }

    printf("\n=====================================\n");
    printf("   BIENVENUE SUR E-LIBRARY ISI DAKAR\n");
    printf("=====================================\n");

    int continuer = 1;

    while (continuer) {
        char login[MAX_LOGIN];
        char motDePasse[MAX_MDP];
        int indexConnecte;

        printf("\n--- Connexion ---\n");
        printf("Login (ou 0 pour quitter) : ");
        if (scanf(" %[^\n]", login) != 1) {
            break; /* plus rien a lire : on arrete proprement le programme */
        }

        if (strcmp(login, "0") == 0) {
            break;
        }

        printf("Mot de passe : ");
        if (scanf(" %[^\n]", motDePasse) != 1) {
            break;
        }

        indexConnecte = utilisateur_connexion(utilisateurs, nUtilisateurs, login, motDePasse);

        if (indexConnecte == -1) {
            printf("Login inconnu.\n");
            continue;
        }
        if (indexConnecte == -2) {
            printf("Mot de passe incorrect.\n");
            continue;
        }
        if (indexConnecte == -3) {
            printf("Ce compte est bloque. Contactez un administrateur.\n");
            continue;
        }

        printf("Connexion reussie. Bienvenue %s %s !\n",
               utilisateurs[indexConnecte].prenom, utilisateurs[indexConnecte].nom);

        if (utilisateurs[indexConnecte].doitChangerMdp) {
            utilisateur_changerMotDePasse(utilisateurs, indexConnecte);
            utilisateur_sauvegarder(utilisateurs, nUtilisateurs);
        }

        if (strcmp(utilisateurs[indexConnecte].role, "ADMIN") == 0) {
            menuAdmin(utilisateurs, &nUtilisateurs,
                      auteurs, &nAuteurs,
                      categories, &nCategories,
                      livres, &nLivres,
                      emprunts, &nEmprunts,
                      retours, &nRetours,
                      reservations, &nReservations,
                      penalites, &nPenalites);
        } else {
            menuLecteur(livres, nLivres,
                        emprunts, &nEmprunts,
                        retours, &nRetours,
                        reservations, &nReservations,
                        penalites, &nPenalites,
                        utilisateurs, nUtilisateurs,
                        utilisateurs[indexConnecte].id);
        }

        /* On sauvegarde tout apres chaque session, par securite */
        utilisateur_sauvegarder(utilisateurs, nUtilisateurs);
        auteur_sauvegarder(auteurs, nAuteurs);
        categorie_sauvegarder(categories, nCategories);
        livre_sauvegarder(livres, nLivres);
        emprunt_sauvegarder(emprunts, nEmprunts);
        retour_sauvegarder(retours, nRetours);
        reservation_sauvegarder(reservations, nReservations);
        penalite_sauvegarder(penalites, nPenalites);

        printf("\nVoulez-vous laisser un autre utilisateur se connecter ? (1 = oui, 0 = non) : ");
        if (scanf("%d", &continuer) != 1) {
            continuer = 0;
        }
        viderBuffer();
    }

    printf("\nMerci d'avoir utilise E-LIBRARY ISI DAKAR. A bientot !\n");
}
