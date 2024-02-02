#define OPTION_S
#define OPTION_S

#include <stdio.h>
#include <stdlib.h>

// Structure représentant un nœud de distance
typedef struct Distance {
    float distance;
    struct Distance* pNext;
} Distance;

// Structure représentant un nœud de l'arbre AVL
typedef struct AVL {
    int routeid;
    Distance* distance;
    struct AVL* pL;
    struct AVL* pR;
    int taille;
    float minDistance;
    float maxDistance;
    float moyenne;
    float moyennedistance;
    int nombredist;
    float sommedist;
} AVL;


Distance* Newdistance(float dist);
AVL* createAVL(int routeNbr, float dist);
AVL* createAVL2(int routeNbr, float maxdist, float mindist, float moyenne, float moyennedist);
int taille(AVL* p);
int getEquilibre(AVL* p);
AVL* rotationDroite(AVL* p);
AVL* rotationGauche(AVL* p);
AVL* insertAVL(AVL* p, int routenbr, float dist);
AVL* insertDistance(AVL* p2, int numid, float maxdist, float mindist, float moyenne, float moyennedistance);
AVL* insertAVL2(AVL* p, AVL* p2);
void displayInfixeInv(AVL* p);
void destroyAVL(AVL* p);
