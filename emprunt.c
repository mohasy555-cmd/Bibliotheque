#include <stdio.h>
#include <string.h>
#include <time.h>
#include "emprunt.h"
#include "utils.h"

#define CHEMIN_FICHIER "DATABASE/BORROWS.dat"
#define SECONDES_PAR_JOUR (24 * 60 * 60)

int emprunt_charger(Emprunt tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Emprunt), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void emprunt_sauvegarder(Emprunt tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des emprunts.\n");
        return;
    }
    fwrite(tab, sizeof(Emprunt), n, f);
    fclose(f);
}

int emprunt_trouverParId(Emprunt tab[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id == id) {
            return i;
        }
    }
    return -1;
}

int emprunt_compterEnCoursPourUtilisateur(Emprunt tab[], int n, int idUtilisateur)
{
    int i, compteur = 0;
    for (i = 0; i < n; i++) {
        if (tab[i].idUtilisateur == idUtilisateur &&
            (strcmp(tab[i].etat, "EN_COURS") == 0 || strcmp(tab[i].etat, "EN_RETARD") == 0)) {
            compteur++;
        }
    }
    return compteur;
}

static int prochainId(Emprunt tab[], int n)
{
    int max = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id > max) {
            max = tab[i].id;
        }
    }
    return max + 1;
}

/* Construit le numero d'emprunt EMP_AAAAMMJJHHMMSS a partir de la date actuelle */
static void construireNumeroEmprunt(time_t date, char resultat[])
{
    struct tm *t = localtime(&date);
    sprintf(resultat, "EMP_%04d%02d%02d%02d%02d%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
}

/* Cree le fichier "recu" dans REPORTS/BORROWS/ pour cet emprunt */
static void genererRecuEmprunt(Emprunt e, Livre livre, Auteur auteur, Utilisateur utilisateur)
{
    char nomFichier[100];
    sprintf(nomFichier, "REPORTS/BORROWS/BORROW_%s.txt", e.numeroEmprunt + 4); /* on enleve "EMP_" */

    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        return;
    }

    char dateEmprunt[20], dateRetour[20];
    utils_dateVersTexte(e.dateEmprunt, dateEmprunt);
    utils_dateVersTexte(e.dateRetourPrevue, dateRetour);

    fprintf(f, "Numero d'emprunt : %s\n", e.numeroEmprunt);
    fprintf(f, "Nom du lecteur   : %s %s\n", utilisateur.prenom, utilisateur.nom);
    fprintf(f, "Date d'emprunt   : %s\n", dateEmprunt);
    fprintf(f, "Date retour prevue : %s\n", dateRetour);
    fprintf(f, "Livre emprunte   : %s\n", livre.titre);
    fprintf(f, "Auteur           : %s\n", auteur.nomComplet);
    fprintf(f, "ISBN             : %s\n", livre.isbn);

    fclose(f);
}

int emprunt_creer(Emprunt tab[], int n, int idUtilisateur, int idLivre,
                   Livre livres[], int nbLivres,
                   Utilisateur utilisateurs[], int nbUtilisateurs)
{
    if (n >= MAX_EMPRUNTS) {
        printf("Le nombre maximum d'emprunts est atteint.\n");
        return n;
    }

    int indexLivre = livre_trouverParId(livres, nbLivres, idLivre);
    if (indexLivre == -1) {
        printf("Erreur : ce livre n'existe pas.\n");
        return n;
    }

    int indexUtil = utilisateur_trouverParId(utilisateurs, nbUtilisateurs, idUtilisateur);
    if (indexUtil == -1) {
        printf("Erreur : cet utilisateur n'existe pas.\n");
        return n;
    }

    if (livres[indexLivre].exemplairesDisponibles <= 0) {
        printf("Erreur : aucun exemplaire disponible pour ce livre.\n");
        return n;
    }

    int nbEmpruntsEnCours = emprunt_compterEnCoursPourUtilisateur(tab, n, idUtilisateur);
    if (nbEmpruntsEnCours >= LIMITE_EMPRUNTS_PAR_UTILISATEUR) {
        printf("Erreur : cet utilisateur a deja %d emprunts en cours (limite atteinte).\n",
               LIMITE_EMPRUNTS_PAR_UTILISATEUR);
        return n;
    }

    Emprunt nouveau;
    nouveau.id = prochainId(tab, n);
    nouveau.idUtilisateur = idUtilisateur;
    nouveau.idLivre = idLivre;
    nouveau.dateEmprunt = time(NULL);
    nouveau.dateRetourPrevue = nouveau.dateEmprunt + (DUREE_EMPRUNT_JOURS * SECONDES_PAR_JOUR);
    strcpy(nouveau.etat, "EN_COURS");

    construireNumeroEmprunt(nouveau.dateEmprunt, nouveau.numeroEmprunt);

    tab[n] = nouveau;
    n++;

    /* On diminue le stock disponible du livre */
    livre_ajusterStock(livres, indexLivre, -1);

    /* Ecriture de l'historique et du recu */
    char operation[250];
    sprintf(operation, "Emprunt du livre \"%s\" (numero %s)", livres[indexLivre].titre, nouveau.numeroEmprunt);
    utils_ecrireHistorique(utilisateurs[indexUtil].login, operation);

    Auteur auteurVide; /* le recu peut ne pas connaitre l'auteur complet : on met un texte simple */
    strcpy(auteurVide.nomComplet, "Auteur id ");
    /* On construit un texte simple avec l'id de l'auteur si besoin */
    char idAuteurTexte[20];
    sprintf(idAuteurTexte, "%d", livres[indexLivre].idAuteur);
    strcat(auteurVide.nomComplet, idAuteurTexte);

    genererRecuEmprunt(nouveau, livres[indexLivre], auteurVide, utilisateurs[indexUtil]);

    printf("Emprunt enregistre avec succes. Numero : %s\n", nouveau.numeroEmprunt);
    printf("A retourner avant le : ");
    char texteDate[20];
    utils_dateVersTexte(nouveau.dateRetourPrevue, texteDate);
    printf("%s\n", texteDate);

    return n;
}

void emprunt_afficherTous(Emprunt tab[], int n)
{
    int i;
    char dateEmprunt[20], dateRetour[20];

    if (n == 0) {
        printf("Aucun emprunt enregistre.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        utils_dateVersTexte(tab[i].dateEmprunt, dateEmprunt);
        utils_dateVersTexte(tab[i].dateRetourPrevue, dateRetour);
        printf("----------------------------------\n");
        printf("Id emprunt      : %d\n", tab[i].id);
        printf("Numero          : %s\n", tab[i].numeroEmprunt);
        printf("Id utilisateur  : %d\n", tab[i].idUtilisateur);
        printf("Id livre        : %d\n", tab[i].idLivre);
        printf("Date emprunt    : %s\n", dateEmprunt);
        printf("Retour prevu    : %s\n", dateRetour);
        printf("Etat            : %s\n", tab[i].etat);
    }
    printf("----------------------------------\n");
}

void emprunt_afficherPourUtilisateur(Emprunt tab[], int n, int idUtilisateur)
{
    int i, trouve = 0;
    char dateEmprunt[20], dateRetour[20];

    for (i = 0; i < n; i++) {
        if (tab[i].idUtilisateur == idUtilisateur) {
            trouve = 1;
            utils_dateVersTexte(tab[i].dateEmprunt, dateEmprunt);
            utils_dateVersTexte(tab[i].dateRetourPrevue, dateRetour);
            printf("----------------------------------\n");
            printf("Numero       : %s\n", tab[i].numeroEmprunt);
            printf("Id livre     : %d\n", tab[i].idLivre);
            printf("Date emprunt : %s\n", dateEmprunt);
            printf("Retour prevu : %s\n", dateRetour);
            printf("Etat         : %s\n", tab[i].etat);
        }
    }
    if (!trouve) {
        printf("Vous n'avez aucun emprunt.\n");
    } else {
        printf("----------------------------------\n");
    }
}
