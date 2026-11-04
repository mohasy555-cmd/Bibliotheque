#include <stdio.h>
#include <time.h>
#include "penalite.h"

#define CHEMIN_FICHIER "DATABASE/PENALTIES.dat"

int penalite_charger(Penalite tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Penalite), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void penalite_sauvegarder(Penalite tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des penalites.\n");
        return;
    }
    fwrite(tab, sizeof(Penalite), n, f);
    fclose(f);
}

static int prochainId(Penalite tab[], int n)
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

int penalite_ajouter(Penalite tab[], int n, int idUtilisateur, int idEmprunt, int joursRetard)
{
    if (n >= MAX_PENALITES) {
        printf("Le nombre maximum de penalites est atteint.\n");
        return n;
    }

    Penalite nouvelle;
    nouvelle.id = prochainId(tab, n);
    nouvelle.idUtilisateur = idUtilisateur;
    nouvelle.idEmprunt = idEmprunt;
    nouvelle.joursRetard = joursRetard;
    nouvelle.montant = joursRetard * MONTANT_PENALITE_PAR_JOUR;
    nouvelle.date = time(NULL);

    tab[n] = nouvelle;
    n++;

    return n;
}

void penalite_afficherTous(Penalite tab[], int n)
{
    int i;

    if (n == 0) {
        printf("Aucune penalite enregistree.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        printf("----------------------------------\n");
        printf("Id             : %d\n", tab[i].id);
        printf("Id utilisateur : %d\n", tab[i].idUtilisateur);
        printf("Jours de retard: %d\n", tab[i].joursRetard);
        printf("Montant        : %d FCFA\n", tab[i].montant);
    }
    printf("----------------------------------\n");
}

void penalite_afficherPourUtilisateur(Penalite tab[], int n, int idUtilisateur)
{
    int i, trouve = 0;

    for (i = 0; i < n; i++) {
        if (tab[i].idUtilisateur == idUtilisateur) {
            trouve = 1;
            printf("----------------------------------\n");
            printf("Jours de retard : %d\n", tab[i].joursRetard);
            printf("Montant         : %d FCFA\n", tab[i].montant);
        }
    }
    if (!trouve) {
        printf("Vous n'avez aucune penalite.\n");
    } else {
        printf("----------------------------------\n");
    }
}

int penalite_calculerTotal(Penalite tab[], int n)
{
    int i, total = 0;
    for (i = 0; i < n; i++) {
        total += tab[i].montant;
    }
    return total;
}
