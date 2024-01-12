#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int stepID;
    char townA[50];
    char townB[50];
} Step;

typedef struct AVL_Etapes {
    Step etape;
    struct AVL_Etapes *fg;
    struct AVL_Etapes *fd;
    int equilibre;
} AVL_Etapes;

typedef struct AVL {
    char ville[50];
    int trajetsTotal;
    int trajetsDepart;
    struct AVL_Etapes *etapes;
    struct AVL *fg;
    struct AVL *fd;
    int equilibre;
} AVL;

// Prototypes des fonctions
AVL *creerNoeud(char ville[]);
AVL_Etapes *creerNoeudEtape(Step etape);
int equilibre(AVL *noeud);
int max(int a, int b);
AVL *rotationDroite(AVL *y);
AVL *rotationGauche(AVL *x);
AVL *rotationDoubleDroite(AVL *racine);
AVL *rotationDoubleGauche(AVL *racine);
AVL_Etapes *rotationDroiteGaucheEtape(AVL_Etapes *racine);
AVL_Etapes *rotationGaucheDroiteEtape(AVL_Etapes *racine);
AVL_Etapes *rotationGaucheEtape(AVL_Etapes *x);
AVL_Etapes *rotationDroiteEtape(AVL_Etapes *y);
int equilibreEtape(AVL_Etapes *noeud);
int getEquilibre(void *noeud);
int getEquilibreEtape(AVL_Etapes *noeud);
AVL_Etapes *insererEtape(AVL_Etapes *racine, Step etape, AVL_Etapes *precedenteEtape);
AVL *inserer(AVL *racine, Step etape, int isDepart, AVL_Etapes *precedenteEtape);
void parcoursInfixe(FILE *fichierSortie, AVL *racine);
void lireTrajetsDepuisFichier(AVL **racine, int *nbEtapes, const char *nomFichier);
typedef struct {
    char ville[50];
    int trajetsTotal;
    int trajetsDepart;
} InfoTop10;
void mettreAJourTop10(AVL *racine, InfoTop10 top10[], int *compteur);
void ecrireTop10DansFichier(FILE *fichierSortie, InfoTop10 top10[]);
void libererAVL(AVL *racine);
//

AVL *creerNoeud(char ville[]) {
    AVL *nouveauNoeud = (AVL *)malloc(sizeof(AVL));
    strcpy(nouveauNoeud->ville, ville);
    nouveauNoeud->trajetsTotal = 1;
    nouveauNoeud->trajetsDepart = 1;
    nouveauNoeud->fg = nouveauNoeud->fd = NULL;
    nouveauNoeud->equilibre = 1;
    return nouveauNoeud;
}

int equilibre(AVL *noeud) {
    if (noeud == NULL)
        return 0;
    return noeud->equilibre;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVL *rotationDroite(AVL *y) {
    AVL *x = y->fg;
    AVL *T2 = x->fd;

    x->fd = y;
    y->fg = T2;

    y->equilibre = max(equilibre(y->fg), equilibre(y->fd)) + 1;
    x->equilibre = max(equilibre(x->fg), equilibre(x->fd)) + 1;

    return x;
}

AVL *rotationGauche(AVL *x) {
    AVL *y = x->fd;
    AVL *T2 = y->fg;

    y->fg = x;
    x->fd = T2;

    x->equilibre = max(equilibre(x->fg), equilibre(x->fd)) + 1;
    y->equilibre = max(equilibre(y->fg), equilibre(y->fd)) + 1;

    return y;
}

AVL *rotationDoubleDroite(AVL *racine) {
    racine->fg = rotationGauche(racine->fg);
    return rotationDroite(racine);
}

AVL *rotationDoubleGauche(AVL *racine) {
    racine->fd = rotationDroite(racine->fd);
    return rotationGauche(racine);
}

AVL_Etapes *creerNoeudEtape(Step etape) {
    AVL_Etapes *nouveauNoeud = (AVL_Etapes *)malloc(sizeof(AVL_Etapes));
    nouveauNoeud->etape = etape;
    nouveauNoeud->fg = nouveauNoeud->fd = NULL;
    nouveauNoeud->equilibre = 1;
    return nouveauNoeud;
}

int equilibreEtape(AVL_Etapes *noeud) {
    if (noeud == NULL)
        return 0;
    return noeud->equilibre;
}

int getEquilibre(void *noeud) {
    if (noeud == NULL)
        return 0;
    return ((AVL *)noeud)->equilibre;
}


AVL_Etapes *insererEtape(AVL_Etapes *racine, Step etape, AVL_Etapes *precedenteEtape) {
    if (racine == NULL)
        return creerNoeudEtape(etape);

    if (etape.stepID < racine->etape.stepID) {
        racine->fg = insererEtape(racine->fg, etape, precedenteEtape);
    } else if (etape.stepID > racine->etape.stepID) {
        racine->fd = insererEtape(racine->fd, etape, precedenteEtape);
    } else {
        // Même étape, vérifier les villes
        if (precedenteEtape != NULL && etape.stepID == precedenteEtape->etape.stepID) {
            // La même étape, ne pas augmenter le nombre de passages de l'étape actuelle
            return racine;
        } else if (strcmp(etape.townB, racine->etape.townA) == 0) {
            // Les villes correspondent, ne pas augmenter le nombre de passages
            racine->etape.stepID = etape.stepID;  // Mettre à jour le numéro de l'étape
            return racine;
        } else {
        // Les villes ne correspondent pas, incrémenter les passages
        racine->etape.stepID = etape.stepID;  // Mettre à jour le numéro de l'étape
        racine->equilibre = max(equilibreEtape(racine->fg), equilibreEtape(racine->fd)) + 1;
        racine->etape->trajetsDepart++;  // Incrémenter le nombre de départs
    }
    }

    // Mettre à jour l'équilibre du nœud actuel
    racine->equilibre = max(equilibreEtape(racine->fg), equilibreEtape(racine->fd)) + 1;

    // Calculer le facteur d'équilibre
    int equilibre = getEquilibreEtape(racine);

    // Rééquilibrer l'arbre si nécessaire
    if (equilibre > 1 && etape.stepID < racine->fg->etape.stepID)
        return rotationDroiteGaucheEtape(racine);

    if (equilibre < -1 && etape.stepID > racine->fd->etape.stepID)
        return rotationGaucheDroiteEtape(racine);

    if (equilibre > 1 && etape.stepID > racine->fg->etape.stepID) {
        racine->fg = rotationGaucheEtape(racine->fg);
        return rotationDroiteEtape(racine);
    }

    if (equilibre < -1 && etape.stepID < racine->fd->etape.stepID) {
        racine->fd = rotationDroiteEtape(racine->fd);
        return rotationGaucheEtape(racine);
    }

    return racine;
}

AVL *inserer(AVL *racine, Step etape, int isDepart, AVL_Etapes *precedenteEtape) {
    if (racine == NULL) {
        AVL *nouveauNoeud = creerNoeud(etape.townA);
        nouveauNoeud->etapes = insererEtape(nouveauNoeud->etapes, etape, precedenteEtape);
        return nouveauNoeud;
    }

    int resultatComparaison = strcmp(etape.townA, racine->ville);

    if (resultatComparaison == 0) {
        // La ville est déjà présente, mise à jour des totaux
        racine->trajetsTotal++;

        // Vérifier si la ville apparaît comme ville de départ ou d'arrivée
        if (isDepart) {
            racine->trajetsDepart++;
        } else {
            // Si la ville apparaît comme ville d'arrivée,
            // alors c'est la deuxième occurrence, réinitialiser isDepart pour la prochaine étape
            isDepart = 1;
        }

        racine->etapes = insererEtape(racine->etapes, etape, precedenteEtape);
    } else if (resultatComparaison < 0) {
        racine->fg = inserer(racine->fg, etape, isDepart, precedenteEtape);
    } else {
        racine->fd = inserer(racine->fd, etape, isDepart, precedenteEtape);
    }

    racine->equilibre = 1 + max(equilibre(racine->fg), equilibre(racine->fd));

    int equilibre = getEquilibre(racine);

    // Rééquilibrage de l'arbre
    if (equilibre > 1 && strcmp(etape.townA, racine->fg->ville) > 0)
        return rotationDoubleDroite(racine);

    if (equilibre < -1 && strcmp(etape.townA, racine->fd->ville) < 0)
        return rotationDoubleGauche(racine);

    if (equilibre > 1 && strcmp(etape.townA, racine->fg->ville) < 0) {
        racine->fg = rotationGauche(racine->fg);
        return rotationDroite(racine);
    }

    if (equilibre < -1 && strcmp(etape.townA, racine->fd->ville) > 0) {
        racine->fd = rotationDroite(racine->fd);
        return rotationGauche(racine);
    }

    return racine;
}


void parcoursInfixe(FILE *fichierSortie, AVL *racine) {
    if (racine != NULL) {
        parcoursInfixe(fichierSortie, racine->fd);
        fprintf(fichierSortie, "%s - Trajets Total: %d, Trajets Départ: %d\n", racine->ville, racine->trajetsTotal, racine->trajetsDepart);
        parcoursInfixe(fichierSortie, racine->fg);
    }
}

void lireTrajetsDepuisFichier(AVL **racine, int *nbEtapes, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[1000];
    char villeA[50];
    char villeB[50];
    int stepID;

    typedef struct {
        int stepID;
        char townA[50];
        char townB[50];
    } Step;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        if (sscanf(ligne, "%d;%49[^;];%49[^;\n]", &stepID, villeA, villeB) == 3) {
            // Ajouter l'étape à l'arbre AVL
            Step nouvelleEtape;
            nouvelleEtape.stepID = stepID;
            strcpy(nouvelleEtape.townA, villeA);
            strcpy(nouvelleEtape.townB, villeB);

            *racine = insererEtape(*racine, nouvelleEtape, NULL);
            (*nbEtapes)++;
        } else {
            fprintf(stderr, "Erreur de format dans la ligne : %s\n", ligne);
        }
    }

    // Fermer le fichier après la lecture
    if (fclose(fichier) != 0) {
        perror("Erreur lors de la fermeture du fichier");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour parcourir l'arbre en infixe et mettre à jour les informations du top 10
void mettreAJourTop10(AVL *racine, InfoTop10 top10[], int *compteur) {
    if (racine != NULL) {
        mettreAJourTop10(racine->fg, top10, compteur);

        // Mettre à jour le top 10 avec les informations actuelles
        int index = *compteur;
        if (index < 10) {
            strcpy(top10[index].ville, racine->ville);
            top10[index].trajetsTotal = racine->trajetsTotal;
            top10[index].trajetsDepart = racine->trajetsDepart;
            (*compteur)++;
        } else {
            // Trouver la position où insérer si le tableau est plein
            for (int i = 0; i < 10; i++) {
                if (racine->trajetsTotal > top10[i].trajetsTotal) {
                    // Décaler les éléments pour faire de la place pour le nouveau
                    for (int j = 9; j > i; j--) {
                        strcpy(top10[j].ville, top10[j - 1].ville);
                        top10[j].trajetsTotal = top10[j - 1].trajetsTotal;
                        top10[j].trajetsDepart = top10[j - 1].trajetsDepart;
                    }
                    // Insérer le nouveau
                    strcpy(top10[i].ville, racine->ville);
                    top10[i].trajetsTotal = racine->trajetsTotal;
                    top10[i].trajetsDepart = racine->trajetsDepart;
                    break;
                }
            }
        }

        mettreAJourTop10(racine->fd, top10, compteur);
    }
}

// Fonction pour afficher le top 10 dans le fichier
void ecrireTop10DansFichier(FILE *fichierSortie, InfoTop10 top10[]) {
    for (int i = 0; i < 10; i++) {
        fprintf(fichierSortie, "%s;%d;%d\n", top10[i].ville, top10[i].trajetsTotal, top10[i].trajetsDepart);
    }
}

// Fonction pour libérer la mémoire allouée pour les nœuds de l'AVL
void libererAVL(AVL *racine) {
    if (racine != NULL) {
        libererAVL(racine->fg);
        libererAVL(racine->fd);
        free(racine);
    }
}

int main() {
    AVL *racine = NULL;
    AVL_Etapes *racineEtapes = NULL;
    Step *etapes = NULL;
    int nbEtapes = 0;

    // Spécifiez le chemin complet du fichier contenant les trajets
    const char *nomFichierEtapes = "./temp/temp_t.csv";
    const char *nomFichierSortie = "./temp/histogram_datat.csv";

    // Lecture des trajets à partir du fichier et mise à jour des étapes
    lireTrajetsDepuisFichier(&racineEtapes, &nbEtapes, nomFichierEtapes);

    // Vérification si l'arbre est vide
    if (racine == NULL) {
        fprintf(stderr, "Erreur : Aucune donnée n'a été lue depuis le fichier ou le fichier est vide.\n");
        return EXIT_FAILURE;
    }
    
    // Vérification si l'arbre est vide
    if (racineEtapes == NULL) {
        fprintf(stderr, "Erreur : Aucune donnée n'a été lue depuis le fichier ou le fichier est vide.\n");
        return EXIT_FAILURE;
    }

    // Ouvrir le fichier de sortie en écriture
    FILE *fichierSortie = fopen("./temp/histogram_datat.csv", "w");
    if (fichierSortie == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return EXIT_FAILURE;
    }

    // Initialiser le tableau pour le top 10
    InfoTop10 top10[10];
    for (int i = 0; i < 10; i++) {
        top10[i].trajetsTotal = 0;
    }

    // Mettre à jour les informations du top 10
    int compteur = 0;
    mettreAJourTop10(racine, top10, &compteur);

    // Écrire le top 10 dans le fichier
    ecrireTop10DansFichier(fichierSortie, top10);

    // Fermer le fichier de sortie
    if (fclose(fichierSortie) != 0) {
        perror("Erreur lors de la fermeture du fichier de sortie");
        return EXIT_FAILURE;
    }

    // Libération de la mémoire allouée pour les nœuds de l'AVL
    libererAVL(racine);

    // Libération de la mémoire allouée pour les étapes
    free(etapes);

    return EXIT_SUCCESS;
}
