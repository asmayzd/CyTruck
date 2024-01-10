#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_LOT 1000

typedef struct ville{
  char nom[100];
  int nbPassage;
  int nbDepart;
  int hauteur;
  int equilibre;
} Ville;

typedef struct Arbre{
  Ville v;
  struct Arbre* fd;
  struct Arbre* fg;
} Arbre;

Arbre* creerArbre(Ville a){
    Arbre* new = malloc(sizeof(Arbre));
    if(new == NULL){
        exit(1);
    }
    else{
      strcpy(new->v.nom, a.nom);
        new->v.nbPassage = a.nbPassage;
        new->v.nbDepart = a.nbDepart;
        new->v.hauteur = a.hauteur;
        new->v.equilibre = a.equilibre;
        new->fd = NULL;
        new->fg = NULL;
    }
    return new;
}

int estVilleDepart(Arbre* arbre, char* nomVille) {
    if (arbre == NULL) {
        return 0;
    }

    if (strcmp(arbre->v.nom, nomVille) == 0) {
        return arbre->v.nbDepart > 0; 
    }

    return estVilleDepart(arbre->fg, nomVille) || estVilleDepart(arbre->fd, nomVille);
}

int hauteur(Arbre* a) {
    if (a == NULL) {
        return 0;
    }
    int hauteur_fg = hauteur(a->fg);
    int hauteur_fd = hauteur(a->fd);
    return 1 + (hauteur_fg > hauteur_fd ? hauteur_fg : hauteur_fd);
}

int equilibre(Arbre* a) {
    if (a == NULL) {
        return 0;
    }
    return hauteur(a->fg) - hauteur(a->fd);
}

Arbre* rotation_fg(Arbre* x) {
    Arbre* y = x->fd;
    x->fd = y->fg;
    y->fg = x;

    x->v.hauteur = hauteur(x);
    y->v.hauteur = hauteur(y);

    return y;
}

Arbre* rotation_fd(Arbre* y) {
    Arbre* x = y->fg;
    y->fg = x->fd;
    x->fd = y;

    y->v.hauteur = hauteur(y);
    x->v.hauteur = hauteur(x);

    return x;
}


Arbre* insertion(Arbre* a, Ville v) {

    if (a == NULL) {
        a = creerArbre(v);
    } 

    else {

        if (strcmp(a->v.nom, v.nom) > 0) {
            a->fg = insertion(a->fg, v);
        } 

        else if (strcmp(a->v.nom, v.nom) < 0) {
            a->fd = insertion(a->fd, v);
        } 

        else if (strcmp(a->v.nom, v.nom) == 0) {
            a->v.nbPassage++;

            if (estVilleDepart(a, v.nom)) {
                a->v.nbDepart++;
            }
        }
    }

    a->v.hauteur = hauteur(a);
    a->v.equilibre = equilibre(a);

    int balance = a->v.equilibre;

    if (balance > 1) {

        if (a->fg->v.equilibre < 0) {
            a->fg = rotation_fg(a->fg);
        }

        a = rotation_fd(a);
    } 

    else if (balance < -1) {

        if (a->fd->v.equilibre > 0) {
            a->fd = rotation_fd(a->fd);
        }

        a = rotation_fg(a);
    }

    return a;
}

void afficherTopDixPassages(Arbre* a, int* count) {
    if (a != NULL && *count < 10) {
        printf("Affichage avant a->fd\n");
        afficherTopDixPassages(a->fd, count);

        if (*count < 10) {
            printf("Ville: %s\n", a->v.nom);
            printf("Nombre de passages: %d\n", a->v.nbPassage);
            printf("Nombre de départs: %d\n\n", a->v.nbDepart);
            (*count)++;
        }

        printf("Affichage avant a->fg\n");
        afficherTopDixPassages(a->fg, count);
    }
}


int main() {

    FILE* fichier = fopen("../temp/temp_t.csv", "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    Arbre* dernierVilleInseree = NULL;
    Arbre* arbre = NULL;
    int stepID;
    char villeDepart[100], villeArrivee[100];
    
    int lines_read = 0;
    int batch_count = 0;

    while (fscanf(fichier, "%d;%99[^;];%99[^\n]", &stepID, villeDepart, villeArrivee) == 3) {
      printf("Step ID: %d, Ville de départ: %s, Ville d'arrivée: %s\n", stepID, villeDepart, villeArrivee);
        Ville nouvelleVilleDepart = {0};
        Ville nouvelleVilleArrivee = {0};

        strcpy(nouvelleVilleDepart.nom, villeDepart);
        strcpy(nouvelleVilleArrivee.nom, villeArrivee);

        if (dernierVilleInseree != NULL && strcmp(dernierVilleInseree->v.nom, villeDepart) == 0) {
            nouvelleVilleDepart.nbDepart++;
        }

        arbre = insertion(arbre, nouvelleVilleDepart);
        arbre = insertion(arbre, nouvelleVilleArrivee);

        dernierVilleInseree = arbre;
        
        lines_read++;
        batch_count++;
        
    if (batch_count >= MAX_LOT) {
            // Traitement du lot actuel
            int count = 0;
            afficherTopDixPassages(arbre, &count);

            // Réinitialisation pour le prochain lot
            batch_count = 0;
            arbre = NULL; // Réinitialisation de l'arbre pour le prochain lot
        }
    }

    // Vérification si des données restent à traiter après la fin du fichier
    if (batch_count > 0) {
        int count = 0;
        afficherTopDixPassages(arbre, &count);
    }

    fclose(fichier);
  
  int count = 0;
  afficherTopDixPassages(arbre, &count);

    return 0;
}
