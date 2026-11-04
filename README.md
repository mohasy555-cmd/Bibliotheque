# E-LIBRARY ISI DAKAR - Gestion d'une bibliotheque numerique

Projet en langage C pour Code::Blocks.

## Comment ouvrir le projet

1. Decompresser le dossier `bibliotheque` a un endroit simple, par exemple `C:\Projets\bibliotheque`.
2. Ouvrir Code::Blocks.
3. Menu `File > Open...` puis choisir le fichier `bibliotheque.cbp`.
4. Cliquer sur `Build > Build and Run` (ou la fleche verte) pour compiler et lancer.

Au premier lancement, le programme cree automatiquement :
- un dossier `DATABASE/` (fichiers binaires .dat)
- un dossier `REPORTS/` (recus et historique)
- un compte administrateur par defaut :
  - Login : `ADMINI`
  - Mot de passe : `Library123`
  - Le programme obligera a changer ce mot de passe des la premiere connexion.

## Organisation des fichiers du projet

| Fichier               | Role                                                        |
|------------------------|-------------------------------------------------------------|
| `structures.h`         | Toutes les structures (Utilisateur, Auteur, Livre, etc.)     |
| `utils.h` / `utils.c`  | Fonctions communes : dossiers, dates, historique             |
| `utilisateur.h/.c`     | Gestion des administrateurs et lecteurs                       |
| `auteur.h/.c`          | Gestion des auteurs                                          |
| `categorie.h/.c`       | Gestion des categories                                       |
| `livre.h/.c`           | Gestion des livres                                           |
| `emprunt.h/.c`         | Gestion des emprunts                                         |
| `retour.h/.c`          | Gestion des retours + calcul des penalites                    |
| `reservation.h/.c`     | Gestion des reservations                                     |
| `penalite.h/.c`        | Gestion des penalites de retard (500 FCFA/jour)               |
| `menu.h/.c`            | Tous les menus (administrateur et lecteur)                    |
| `main.c`               | Point d'entree du programme (tres court)                      |

## Points importants pour comprendre le code (niveau L1)

- Chaque "entite" (utilisateur, auteur, livre...) est un `struct` defini dans `structures.h`.
- Les donnees sont gardees dans des **tableaux de taille fixe** (pas de `malloc`), ce qui est
  plus simple a comprendre en debut d'apprentissage.
- Chaque module a deux fonctions cle :
  - `xxx_charger(tableau)` : lit le fichier binaire au demarrage et remplit le tableau.
  - `xxx_sauvegarder(tableau, n)` : reecrit tout le tableau dans le fichier binaire.
  Ces deux fonctions utilisent `fread`/`fwrite` avec `sizeof(struct)`, exactement ce qui est vu
  en cours sur la gestion de fichiers.
- Le mot de passe est "crypte" avec un simple decalage de lettres (chiffrement de Cesar) dans
  `utilisateur_crypter()`. Ce n'est pas un vrai systeme securise, mais cela respecte la consigne
  du cahier des charges pour un projet d'etude.
- Le fichier `menu.c` est volontairement le plus gros fichier : c'est lui qui affiche les menus
  et appelle les fonctions des autres modules. Il n'y a rien de complique dedans, seulement
  beaucoup de `printf`/`scanf` et des `switch`.

## Limites connues (a ameliorer si tu veux aller plus loin)

- Le rapport journalier automatique (`REPORTS/DAILY/REPORT_AAAAMMJJ.txt`) n'est pas encore
  genere automatiquement ; les statistiques globales sont disponibles dans le menu
  (choix "9. Statistiques") mais elles portent sur toute la duree de vie de la base de donnees,
  pas seulement sur la journee.
- Une seule reservation "en attente" est prevenue a la fois (la plus ancienne), comme demande
  dans le cahier des charges.
