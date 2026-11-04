#include <stdio.h>
#include "utils.h"
#include "menu.h"

/* =====================================================
   Point d'entree du programme.
   Ce fichier est volontairement tout petit :
   tout le travail est fait dans les autres modules.
   ===================================================== */
int main(void)
{
    /* On cree les dossiers DATABASE/ et REPORTS/ s'ils n'existent pas */
    utils_creerDossiers();

    /* On lance le menu principal du programme */
    menu_lancer();

    return 0;
}
