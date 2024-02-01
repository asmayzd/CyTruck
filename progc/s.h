#define OPTION_S
#define OPTION_S

#include <stdio.h>
#include <stdlib.h>

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
