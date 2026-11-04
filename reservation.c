#include <stdio.h>
#include <string.h>
#include <time.h>
#include "reservation.h"

#define CHEMIN_FICHIER "DATABASE/RESERVATIONS.dat"

int reservation_charger(Reservation tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Reservation), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void reservation_sauvegarder(Reservation tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des reservations.\n");
        return;
    }
    fwrite(tab, sizeof(Reservation), n, f);
    fclose(f);
}

int reservation_trouverParId(Reservation tab[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int prochainId(Reservation tab[], int n)
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

int reservation_creer(Reservation tab[], int n, int idUtilisateur, int idLivre, Livre livres[], int nbLivres)
{
    if (n >= MAX_RESERVATIONS) {
        printf("Le nombre maximum de reservations est atteint.\n");
        return n;
    }

    int indexLivre = livre_trouverParId(livres, nbLivres, idLivre);
    if (indexLivre == -1) {
        printf("Erreur : ce livre n'existe pas.\n");
        return n;
    }

    if (livres[indexLivre].exemplairesDisponibles > 0) {
        printf("Impossible de reserver : il reste des exemplaires disponibles, empruntez le livre directement.\n");
        return n;
    }

    Reservation nouvelle;
    nouvelle.id = prochainId(tab, n);
    nouvelle.idUtilisateur = idUtilisateur;
    nouvelle.idLivre = idLivre;
    nouvelle.dateReservation = time(NULL);
    strcpy(nouvelle.etat, "EN_ATTENTE");

    tab[n] = nouvelle;
    n++;

    printf("Reservation enregistree avec succes (id = %d).\n", nouvelle.id);
    return n;
}

void reservation_afficherTous(Reservation tab[], int n)
{
    int i;

    if (n == 0) {
        printf("Aucune reservation enregistree.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        printf("----------------------------------\n");
        printf("Id             : %d\n", tab[i].id);
        printf("Id utilisateur : %d\n", tab[i].idUtilisateur);
        printf("Id livre       : %d\n", tab[i].idLivre);
        printf("Etat           : %s\n", tab[i].etat);
    }
    printf("----------------------------------\n");
}

int reservation_annulerInteractif(Reservation tab[], int n)
{
    int id, index;

    printf("Id de la reservation a annuler : ");
    scanf("%d", &id);

    index = reservation_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Reservation introuvable.\n");
        return n;
    }

    strcpy(tab[index].etat, "ANNULEE");
    printf("Reservation annulee avec succes.\n");
    return n;
}

int reservation_trouverPremiereEnAttentePourLivre(Reservation tab[], int n, int idLivre)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].idLivre == idLivre && strcmp(tab[i].etat, "EN_ATTENTE") == 0) {
            return i; /* comme on parcourt du debut a la fin, c'est la plus ancienne */
        }
    }
    return -1;
}

void reservation_marquerDisponible(Reservation tab[], int index)
{
    strcpy(tab[index].etat, "DISPONIBLE");
}
