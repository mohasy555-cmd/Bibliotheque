#include <stdio.h>
#include <string.h>
#include <time.h>
#include "auteur.h"
#include "utils.h"

#define CHEMIN_FICHIER "DATABASE/AUTHORS.dat"

int auteur_charger(Auteur tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0;
    }

    while (fread(&tab[n], sizeof(Auteur), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void auteur_sauvegarder(Auteur tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des auteurs.\n");
        return;
    }
    fwrite(tab, sizeof(Auteur), n, f);
    fclose(f);
}

int auteur_trouverParId(Auteur tab[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id == id) {
            return i;
        }
    }
    return -1;
}

static int prochainId(Auteur tab[], int n)
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

int auteur_ajouterInteractif(Auteur tab[], int n)
{
    if (n >= MAX_AUTEURS) {
        printf("Le nombre maximum d'auteurs est atteint.\n");
        return n;
    }

    Auteur nouveau;
    nouveau.id = prochainId(tab, n);

    printf("--- Ajout d'un auteur ---\n");

    printf("Nom complet : ");
    scanf(" %[^\n]", nouveau.nomComplet);

    printf("Nationalite : ");
    scanf(" %[^\n]", nouveau.nationalite);

    int jour, mois, annee;
    printf("Date de naissance (JJ MM AAAA) : ");
    scanf("%d %d %d", &jour, &mois, &annee);

    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_mday = jour;
    t.tm_mon = mois - 1;
    t.tm_year = annee - 1900;
    nouveau.dateNaissance = mktime(&t);

    printf("Biographie : ");
    scanf(" %[^\n]", nouveau.biographie);

    printf("Nombre de livres publies : ");
    scanf("%d", &nouveau.nombreLivresPublies);

    tab[n] = nouveau;
    n++;

    printf("Auteur ajoute avec succes (id = %d).\n", nouveau.id);
    return n;
}

void auteur_afficherTous(Auteur tab[], int n)
{
    int i;
    char texteDate[20];

    if (n == 0) {
        printf("Aucun auteur enregistre.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        utils_dateVersTexte(tab[i].dateNaissance, texteDate);
        printf("----------------------------------\n");
        printf("Id            : %d\n", tab[i].id);
        printf("Nom complet   : %s\n", tab[i].nomComplet);
        printf("Nationalite   : %s\n", tab[i].nationalite);
        printf("Date naissance: %s\n", texteDate);
        printf("Livres publies: %d\n", tab[i].nombreLivresPublies);
    }
    printf("----------------------------------\n");
}

void auteur_modifierInteractif(Auteur tab[], int n)
{
    int id, index;

    printf("Id de l'auteur a modifier : ");
    scanf("%d", &id);

    index = auteur_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Auteur introuvable.\n");
        return;
    }

    printf("Nouvelle nationalite (%s) : ", tab[index].nationalite);
    scanf(" %[^\n]", tab[index].nationalite);

    printf("Nouvelle biographie (%s) : ", tab[index].biographie);
    scanf(" %[^\n]", tab[index].biographie);

    printf("Auteur modifie avec succes.\n");
}

int auteur_supprimerInteractif(Auteur tab[], int n, int nombreLivresAssocies)
{
    int id, index, i;

    printf("Id de l'auteur a supprimer : ");
    scanf("%d", &id);

    index = auteur_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Auteur introuvable.\n");
        return n;
    }

    if (nombreLivresAssocies > 0) {
        printf("Impossible de supprimer : cet auteur possede encore %d livre(s).\n", nombreLivresAssocies);
        return n;
    }

    for (i = index; i < n - 1; i++) {
        tab[i] = tab[i + 1];
    }

    printf("Auteur supprime avec succes.\n");
    return n - 1;
}
