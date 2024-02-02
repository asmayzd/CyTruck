#define OPTION_S
#define OPTION_S

#include <stdio.h>
#include <stdlib.h>

typedef struct _DistanceNode {
    float distance;
    struct _DistanceNode* pNext;
} Distance;    //stocker plusieurs distances ayant le même numéro de routes

typedef struct _AVL{
    int routeid;
    Distance* distance;
    struct _AVL* pL;
    struct _AVL* pR;
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
