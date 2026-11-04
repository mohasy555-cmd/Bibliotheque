#include <stdio.h>
#include <string.h>
#include <time.h>
#include "retour.h"
#include "utils.h"

#define CHEMIN_FICHIER "DATABASE/RETURNS.dat"
#define SECONDES_PAR_JOUR (24 * 60 * 60)

int retour_charger(Retour tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Retour), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void retour_sauvegarder(Retour tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des retours.\n");
        return;
    }
    fwrite(tab, sizeof(Retour), n, f);
    fclose(f);
}

static int prochainId(Retour tab[], int n)
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

static void genererRecuRetour(Retour r, const char numeroEmprunt[], const char loginUtilisateur[])
{
    char nomFichier[100];
    /* le numero d'emprunt ressemble a EMP_20260615143510, on reprend la partie date */
    sprintf(nomFichier, "REPORTS/RETURNS/RETURN_%s_%s.txt", numeroEmprunt + 4, loginUtilisateur);

    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        return;
    }

    char texteDate[20];
    utils_dateVersTexte(r.dateRetour, texteDate);

    fprintf(f, "Emprunt concerne  : %s\n", numeroEmprunt);
    fprintf(f, "Date de retour    : %s\n", texteDate);
    fprintf(f, "Jours de retard   : %d\n", r.joursRetard);
    fprintf(f, "Montant penalite  : %d FCFA\n", r.montantPenalite);

    fclose(f);
}

int retour_creer(Retour tab[], int n, int idEmprunt,
                  Emprunt emprunts[], int nEmprunts,
                  Livre livres[], int nLivres,
                  Penalite penalites[], int *nPenalites,
                  Reservation reservations[], int nReservations,
                  Utilisateur utilisateurs[], int nUtilisateurs)
{
    if (n >= MAX_RETOURS) {
        printf("Le nombre maximum de retours est atteint.\n");
        return n;
    }

    int indexEmprunt = emprunt_trouverParId(emprunts, nEmprunts, idEmprunt);
    if (indexEmprunt == -1) {
        printf("Erreur : cet emprunt n'existe pas.\n");
        return n;
    }

    if (strcmp(emprunts[indexEmprunt].etat, "RETOURNE") == 0) {
        printf("Erreur : ce livre a deja ete retourne.\n");
        return n;
    }

    int indexLivre = livre_trouverParId(livres, nLivres, emprunts[indexEmprunt].idLivre);
    int indexUtil = utilisateur_trouverParId(utilisateurs, nUtilisateurs, emprunts[indexEmprunt].idUtilisateur);

    Retour nouveau;
    nouveau.id = prochainId(tab, n);
    nouveau.idEmprunt = idEmprunt;
    nouveau.dateRetour = time(NULL);

    /* Calcul du retard, en nombre de jours complets */
    int joursRetard = 0;
    if (nouveau.dateRetour > emprunts[indexEmprunt].dateRetourPrevue) {
        joursRetard = (int)((nouveau.dateRetour - emprunts[indexEmprunt].dateRetourPrevue) / SECONDES_PAR_JOUR);
        if (joursRetard < 1) {
            joursRetard = 1; /* meme quelques heures de retard comptent pour 1 jour */
        }
    }
    nouveau.joursRetard = joursRetard;
    nouveau.montantPenalite = joursRetard * MONTANT_PENALITE_PAR_JOUR;

    tab[n] = nouveau;
    n++;

    /* Mise a jour de l'emprunt */
    strcpy(emprunts[indexEmprunt].etat, "RETOURNE");

    /* Le stock du livre augmente */
    if (indexLivre != -1) {
        livre_ajusterStock(livres, indexLivre, +1);
    }

    /* Creation de la penalite si retard */
    if (joursRetard > 0 && indexUtil != -1) {
        *nPenalites = penalite_ajouter(penalites, *nPenalites, utilisateurs[indexUtil].id, idEmprunt, joursRetard);
    }

    /* Si un autre utilisateur attendait ce livre, on le previent (etat DISPONIBLE) */
    if (indexLivre != -1) {
        int indexReservation = reservation_trouverPremiereEnAttentePourLivre(reservations, nReservations, livres[indexLivre].id);
        if (indexReservation != -1) {
            reservation_marquerDisponible(reservations, indexReservation);
            printf("Une reservation en attente pour ce livre est maintenant disponible.\n");
        }
    }

    /* Historique + recu */
    if (indexUtil != -1) {
        char operation[150];
        sprintf(operation, "Retour de l'emprunt %s (retard : %d jour(s))",
                emprunts[indexEmprunt].numeroEmprunt, joursRetard);
        utils_ecrireHistorique(utilisateurs[indexUtil].login, operation);

        genererRecuRetour(nouveau, emprunts[indexEmprunt].numeroEmprunt, utilisateurs[indexUtil].login);
    }

    printf("Retour enregistre avec succes.\n");
    if (joursRetard > 0) {
        printf("Retard de %d jour(s) : penalite de %d FCFA.\n", joursRetard, nouveau.montantPenalite);
    }

    return n;
}

void retour_afficherTous(Retour tab[], int n)
{
    int i;
    char texteDate[20];

    if (n == 0) {
        printf("Aucun retour enregistre.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        utils_dateVersTexte(tab[i].dateRetour, texteDate);
        printf("----------------------------------\n");
        printf("Id retour       : %d\n", tab[i].id);
        printf("Id emprunt      : %d\n", tab[i].idEmprunt);
        printf("Date retour     : %s\n", texteDate);
        printf("Jours de retard : %d\n", tab[i].joursRetard);
        printf("Penalite        : %d FCFA\n", tab[i].montantPenalite);
    }
    printf("----------------------------------\n");
}
