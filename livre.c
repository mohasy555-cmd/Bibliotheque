#include <stdio.h>
#include <string.h>
#include <time.h>
#include "livre.h"

#define CHEMIN_FICHIER "DATABASE/BOOKS.dat"

int livre_charger(Livre tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Livre), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void livre_sauvegarder(Livre tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des livres.\n");
        return;
    }
    fwrite(tab, sizeof(Livre), n, f);
    fclose(f);
}

int livre_trouverParId(Livre tab[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id == id) {
            return i;
        }
    }
    return -1;
}

int livre_trouverParIsbn(Livre tab[], int n, const char isbn[])
{
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(tab[i].isbn, isbn) == 0) {
            return i;
        }
    }
    return -1;
}

int livre_compterParAuteur(Livre tab[], int n, int idAuteur)
{
    int i, compteur = 0;
    for (i = 0; i < n; i++) {
        if (tab[i].idAuteur == idAuteur) {
            compteur++;
        }
    }
    return compteur;
}

int livre_compterParCategorie(Livre tab[], int n, int idCategorie)
{
    int i, compteur = 0;
    for (i = 0; i < n; i++) {
        if (tab[i].idCategorie == idCategorie) {
            compteur++;
        }
    }
    return compteur;
}

static int prochainId(Livre tab[], int n)
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

int livre_ajouterInteractif(Livre tab[], int n, Auteur auteurs[], int nbAuteurs,
                             Categorie categories[], int nbCategories)
{
    if (n >= MAX_LIVRES) {
        printf("Le nombre maximum de livres est atteint.\n");
        return n;
    }

    Livre nouveau;
    nouveau.id = prochainId(tab, n);

    printf("--- Ajout d'un livre ---\n");

    printf("ISBN : ");
    scanf(" %[^\n]", nouveau.isbn);

    if (livre_trouverParIsbn(tab, n, nouveau.isbn) != -1) {
        printf("Erreur : cet ISBN existe deja. Le livre n'a pas ete ajoute.\n");
        return n;
    }

    printf("Titre : ");
    scanf(" %[^\n]", nouveau.titre);

    printf("Id de l'auteur : ");
    scanf("%d", &nouveau.idAuteur);
    if (auteur_trouverParId(auteurs, nbAuteurs, nouveau.idAuteur) == -1) {
        printf("Erreur : cet auteur n'existe pas. Le livre n'a pas ete ajoute.\n");
        return n;
    }

    printf("Id de la categorie : ");
    scanf("%d", &nouveau.idCategorie);
    if (categorie_trouverParId(categories, nbCategories, nouveau.idCategorie) == -1) {
        printf("Erreur : cette categorie n'existe pas. Le livre n'a pas ete ajoute.\n");
        return n;
    }

    printf("Editeur : ");
    scanf(" %[^\n]", nouveau.editeur);

    printf("Annee de publication : ");
    scanf("%d", &nouveau.anneePublication);

    printf("Langue : ");
    scanf(" %[^\n]", nouveau.langue);

    printf("Nombre de pages : ");
    scanf("%d", &nouveau.nombrePages);

    printf("Nombre total d'exemplaires : ");
    scanf("%d", &nouveau.nombreExemplaires);
    nouveau.exemplairesDisponibles = nouveau.nombreExemplaires;

    printf("Emplacement dans la bibliotheque : ");
    scanf(" %[^\n]", nouveau.emplacement);

    printf("Resume : ");
    scanf(" %[^\n]", nouveau.resume);

    nouveau.dateAjout = time(NULL);

    tab[n] = nouveau;
    n++;

    printf("Livre ajoute avec succes (id = %d).\n", nouveau.id);
    return n;
}

void livre_afficherTous(Livre tab[], int n)
{
    int i;

    if (n == 0) {
        printf("Aucun livre enregistre.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        printf("----------------------------------\n");
        printf("Id           : %d\n", tab[i].id);
        printf("ISBN         : %s\n", tab[i].isbn);
        printf("Titre        : %s\n", tab[i].titre);
        printf("Id auteur    : %d\n", tab[i].idAuteur);
        printf("Id categorie : %d\n", tab[i].idCategorie);
        printf("Editeur      : %s\n", tab[i].editeur);
        printf("Annee        : %d\n", tab[i].anneePublication);
        printf("Exemplaires  : %d disponibles sur %d\n", tab[i].exemplairesDisponibles, tab[i].nombreExemplaires);
        printf("Emplacement  : %s\n", tab[i].emplacement);
    }
    printf("----------------------------------\n");
}

void livre_modifierInteractif(Livre tab[], int n)
{
    int id, index;

    printf("Id du livre a modifier : ");
    scanf("%d", &id);

    index = livre_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Livre introuvable.\n");
        return;
    }

    printf("Nouveau titre (%s) : ", tab[index].titre);
    scanf(" %[^\n]", tab[index].titre);

    printf("Nouvel emplacement (%s) : ", tab[index].emplacement);
    scanf(" %[^\n]", tab[index].emplacement);

    printf("Nouveau resume (%s) : ", tab[index].resume);
    scanf(" %[^\n]", tab[index].resume);

    printf("Livre modifie avec succes.\n");
}

int livre_supprimerInteractif(Livre tab[], int n)
{
    int id, index, i;

    printf("Id du livre a supprimer : ");
    scanf("%d", &id);

    index = livre_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Livre introuvable.\n");
        return n;
    }

    for (i = index; i < n - 1; i++) {
        tab[i] = tab[i + 1];
    }

    printf("Livre supprime avec succes.\n");
    return n - 1;
}

void livre_ajusterStock(Livre tab[], int index, int variation)
{
    tab[index].exemplairesDisponibles += variation;

    if (tab[index].exemplairesDisponibles < 0) {
        tab[index].exemplairesDisponibles = 0;
    }
    if (tab[index].exemplairesDisponibles > tab[index].nombreExemplaires) {
        tab[index].exemplairesDisponibles = tab[index].nombreExemplaires;
    }
}
