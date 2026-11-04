#include <stdio.h>
#include <string.h>
#include <time.h>
#include "categorie.h"
#include "utils.h"

#define CHEMIN_FICHIER "DATABASE/CATEGORIES.dat"

int categorie_charger(Categorie tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Categorie), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void categorie_sauvegarder(Categorie tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des categories.\n");
        return;
    }
    fwrite(tab, sizeof(Categorie), n, f);
    fclose(f);
}

int categorie_trouverParId(Categorie tab[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int prochainId(Categorie tab[], int n)
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

int categorie_ajouterInteractif(Categorie tab[], int n)
{
    if (n >= MAX_CATEGORIES) {
        printf("Le nombre maximum de categories est atteint.\n");
        return n;
    }

    Categorie nouvelle;
    nouvelle.id = prochainId(tab, n);

    printf("--- Ajout d'une categorie ---\n");

    printf("Libelle (ex: Informatique) : ");
    scanf(" %[^\n]", nouvelle.libelle);

    printf("Description : ");
    scanf(" %[^\n]", nouvelle.description);

    nouvelle.dateCreation = time(NULL);

    tab[n] = nouvelle;
    n++;

    printf("Categorie ajoutee avec succes (id = %d).\n", nouvelle.id);
    return n;
}

void categorie_afficherTous(Categorie tab[], int n)
{
    int i;

    if (n == 0) {
        printf("Aucune categorie enregistree.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        printf("----------------------------------\n");
        printf("Id          : %d\n", tab[i].id);
        printf("Libelle     : %s\n", tab[i].libelle);
        printf("Description : %s\n", tab[i].description);
    }
    printf("----------------------------------\n");
}

void categorie_modifierInteractif(Categorie tab[], int n)
{
    int id, index;

    printf("Id de la categorie a modifier : ");
    scanf("%d", &id);

    index = categorie_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Categorie introuvable.\n");
        return;
    }

    printf("Nouveau libelle (%s) : ", tab[index].libelle);
    scanf(" %[^\n]", tab[index].libelle);

    printf("Nouvelle description (%s) : ", tab[index].description);
    scanf(" %[^\n]", tab[index].description);

    printf("Categorie modifiee avec succes.\n");
}

int categorie_supprimerInteractif(Categorie tab[], int n, int nombreLivresAssocies)
{
    int id, index, i;

    printf("Id de la categorie a supprimer : ");
    scanf("%d", &id);

    index = categorie_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Categorie introuvable.\n");
        return n;
    }

    if (nombreLivresAssocies > 0) {
        printf("Impossible de supprimer : %d livre(s) utilisent cette categorie.\n", nombreLivresAssocies);
        return n;
    }

    for (i = index; i < n - 1; i++) {
        tab[i] = tab[i + 1];
    }

    printf("Categorie supprimee avec succes.\n");
    return n - 1;
}
