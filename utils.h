#ifndef UTILS_H
#define UTILS_H

#include <time.h>

/* =====================================================
   utils.h / utils.c
   Petites fonctions utilisees par plusieurs modules :
   - creer les dossiers DATABASE/ et REPORTS/
   - transformer une date en texte lisible
   - ecrire une ligne dans l'historique REPORTS/HISTORY.txt
   ===================================================== */

/* Cree tous les dossiers necessaires au programme (a appeler au debut de main) */
void utils_creerDossiers(void);

/* Transforme une date (time_t) en texte "JJ/MM/AAAA" dans le tableau resultat.
   resultat doit avoir au moins 11 caracteres de place. */
void utils_dateVersTexte(time_t date, char resultat[]);

/* Transforme une date en texte complet "JJ/MM/AAAA HH:MM:SS".
   resultat doit avoir au moins 20 caracteres de place. */
void utils_dateHeureVersTexte(time_t date, char resultat[]);

/* Ajoute une ligne dans REPORTS/HISTORY.txt au format :
   [date heure] LOGIN operation */
void utils_ecrireHistorique(const char login[], const char operation[]);

/* Renvoie 1 si le texte contient exactement 6 lettres MAJUSCULES, sinon 0 */
int utils_loginValide(const char login[]);

#endif
