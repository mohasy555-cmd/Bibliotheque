#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utilisateur.h"
#include "utils.h"

#define CHEMIN_FICHIER "DATABASE/USERS.dat"

int utilisateur_charger(Utilisateur tab[])
{
    FILE *f = fopen(CHEMIN_FICHIER, "rb");
    int n = 0;

    if (f == NULL) {
        return 0; /* le fichier n'existe pas encore : il y a 0 utilisateur */
    }

    /* On lit les enregistrements un par un jusqu'a la fin du fichier */
    while (fread(&tab[n], sizeof(Utilisateur), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

void utilisateur_sauvegarder(Utilisateur tab[], int n)
{
    FILE *f = fopen(CHEMIN_FICHIER, "wb");
    if (f == NULL) {
        printf("Erreur : impossible d'ecrire le fichier des utilisateurs.\n");
        return;
    }
    fwrite(tab, sizeof(Utilisateur), n, f);
    fclose(f);
}

int utilisateur_trouverParLogin(Utilisateur tab[], int n, const char login[])
{
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(tab[i].login, login) == 0) {
            return i;
        }
    }
    return -1;
}

int utilisateur_trouverParId(Utilisateur tab[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++) {
        if (tab[i].id == id) {
            return i;
        }
    }
    return -1;
}

void utilisateur_crypter(const char clair[], char resultat[])
{
    int i;
    int longueur = strlen(clair);
    int decalage = 3; /* on decale chaque lettre de 3 positions */

    for (i = 0; i < longueur; i++) {
        resultat[i] = clair[i] + decalage;
    }
    resultat[longueur] = '\0';
}

/* Calcule le prochain id disponible (le plus grand id + 1) */
static int prochainId(Utilisateur tab[], int n)
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

int utilisateur_ajouterInteractif(Utilisateur tab[], int n, const char role[])
{
    if (n >= MAX_UTILISATEURS) {
        printf("Le nombre maximum d'utilisateurs est atteint.\n");
        return n;
    }

    Utilisateur nouveau;
    nouveau.id = prochainId(tab, n);

    printf("--- Ajout d'un utilisateur (%s) ---\n", role);

    printf("Nom : ");
    scanf(" %[^\n]", nouveau.nom);

    printf("Prenom : ");
    scanf(" %[^\n]", nouveau.prenom);

    printf("Telephone : ");
    scanf(" %[^\n]", nouveau.telephone);

    printf("Adresse : ");
    scanf(" %[^\n]", nouveau.adresse);

    printf("Email : ");
    scanf(" %[^\n]", nouveau.email);

    /* On demande le login jusqu'a ce qu'il soit valide et unique */
    while (1) {
        printf("Login (exactement 6 lettres MAJUSCULES) : ");
        scanf(" %[^\n]", nouveau.login);

        if (!utils_loginValide(nouveau.login)) {
            printf("Login invalide : il faut exactement 6 lettres majuscules.\n");
            continue;
        }
        if (utilisateur_trouverParLogin(tab, n, nouveau.login) != -1) {
            printf("Ce login existe deja. Choisissez-en un autre.\n");
            continue;
        }
        break;
    }

    /* Mot de passe par defaut impose par le cahier des charges */
    utilisateur_crypter("Library123", nouveau.motDePasse);
    nouveau.doitChangerMdp = 1;

    strcpy(nouveau.role, role);
    strcpy(nouveau.etat, "ACTIF");
    nouveau.dateCreation = time(NULL);
    nouveau.derniereConnexion = 0;

    tab[n] = nouveau;
    n++;

    printf("Utilisateur ajoute avec succes (id = %d). Mot de passe par defaut : Library123\n", nouveau.id);
    return n;
}

void utilisateur_afficherTous(Utilisateur tab[], int n)
{
    int i;
    char texteDate[20];

    if (n == 0) {
        printf("Aucun utilisateur enregistre.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        utils_dateVersTexte(tab[i].dateCreation, texteDate);
        printf("----------------------------------\n");
        printf("Id       : %d\n", tab[i].id);
        printf("Nom      : %s %s\n", tab[i].prenom, tab[i].nom);
        printf("Login    : %s\n", tab[i].login);
        printf("Role     : %s\n", tab[i].role);
        printf("Etat     : %s\n", tab[i].etat);
        printf("Telephone: %s\n", tab[i].telephone);
        printf("Email    : %s\n", tab[i].email);
        printf("Cree le  : %s\n", texteDate);
    }
    printf("----------------------------------\n");
}

void utilisateur_modifierInteractif(Utilisateur tab[], int n)
{
    int id, index;

    printf("Id de l'utilisateur a modifier : ");
    scanf("%d", &id);

    index = utilisateur_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Utilisateur introuvable.\n");
        return;
    }

    printf("Nouveau telephone (%s) : ", tab[index].telephone);
    scanf(" %[^\n]", tab[index].telephone);

    printf("Nouvelle adresse (%s) : ", tab[index].adresse);
    scanf(" %[^\n]", tab[index].adresse);

    printf("Nouvel email (%s) : ", tab[index].email);
    scanf(" %[^\n]", tab[index].email);

    printf("Nouvel etat (ACTIF/BLOQUE) (%s) : ", tab[index].etat);
    scanf(" %[^\n]", tab[index].etat);

    printf("Utilisateur modifie avec succes.\n");
}

int utilisateur_supprimerInteractif(Utilisateur tab[], int n)
{
    int id, index, i;

    printf("Id de l'utilisateur a supprimer : ");
    scanf("%d", &id);

    index = utilisateur_trouverParId(tab, n, id);
    if (index == -1) {
        printf("Utilisateur introuvable.\n");
        return n;
    }

    /* On decale tous les elements suivants d'une case vers la gauche */
    for (i = index; i < n - 1; i++) {
        tab[i] = tab[i + 1];
    }

    printf("Utilisateur supprime avec succes.\n");
    return n - 1;
}

int utilisateur_connexion(Utilisateur tab[], int n, const char login[], const char mdp[])
{
    int index = utilisateur_trouverParLogin(tab, n, login);
    if (index == -1) {
        return -1;
    }

    if (strcmp(tab[index].etat, "BLOQUE") == 0) {
        return -3;
    }

    char mdpCrypte[MAX_MDP];
    utilisateur_crypter(mdp, mdpCrypte);

    if (strcmp(tab[index].motDePasse, mdpCrypte) != 0) {
        return -2;
    }

    tab[index].derniereConnexion = time(NULL);
    return index;
}

void utilisateur_changerMotDePasse(Utilisateur tab[], int index)
{
    char nouveauMdp[MAX_MDP];
    char confirmation[MAX_MDP];

    printf("--- Changement de mot de passe obligatoire ---\n");
    printf("Nouveau mot de passe : ");
    scanf(" %[^\n]", nouveauMdp);

    printf("Confirmez le nouveau mot de passe : ");
    scanf(" %[^\n]", confirmation);

    if (strcmp(nouveauMdp, confirmation) != 0) {
        printf("Les deux mots de passe ne correspondent pas. Reessayez.\n");
        utilisateur_changerMotDePasse(tab, index);
        return;
    }

    utilisateur_crypter(nouveauMdp, tab[index].motDePasse);
    tab[index].doitChangerMdp = 0;
    printf("Mot de passe modifie avec succes.\n");
}
