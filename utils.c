#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/* Creer un dossier fonctionne differemment sous Windows et sous Linux/Mac.
   On utilise donc une "compilation conditionnelle" (#ifdef) pour choisir
   la bonne fonction selon le systeme. */
#ifdef _WIN32
    #include <direct.h>
    #define CREER_DOSSIER(chemin) _mkdir(chemin)
#else
    #include <sys/stat.h>
    #define CREER_DOSSIER(chemin) mkdir(chemin, 0777)
#endif

void utils_creerDossiers(void)
{
    /* On ignore le resultat : si le dossier existe deja, ce n'est pas grave */
    CREER_DOSSIER("DATABASE");
    CREER_DOSSIER("REPORTS");
    CREER_DOSSIER("REPORTS/BORROWS");
    CREER_DOSSIER("REPORTS/RETURNS");
    CREER_DOSSIER("REPORTS/DAILY");
}

void utils_dateVersTexte(time_t date, char resultat[])
{
    struct tm *t = localtime(&date);
    sprintf(resultat, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

void utils_dateHeureVersTexte(time_t date, char resultat[])
{
    struct tm *t = localtime(&date);
    sprintf(resultat, "%02d/%02d/%04d %02d:%02d:%02d",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec);
}

void utils_ecrireHistorique(const char login[], const char operation[])
{
    FILE *f = fopen("REPORTS/HISTORY.txt", "a"); /* "a" = ajouter a la fin */
    if (f == NULL) {
        return;
    }

    char texteDate[20];
    utils_dateHeureVersTexte(time(NULL), texteDate);

    fprintf(f, "%s - %s : %s\n", texteDate, login, operation);
    fclose(f);
}

int utils_loginValide(const char login[])
{
    int longueur = strlen(login);
    if (longueur != 6) {
        return 0;
    }

    int i;
    for (i = 0; i < 6; i++) {
        if (!isupper((unsigned char)login[i])) {
            return 0;
        }
    }
    return 1;
}
