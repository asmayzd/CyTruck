#define OPTION_S
#define OPTION_S

#include "s.h"

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

Distance* Newdistance(float dist) {
    Distance* p = malloc(sizeof(Distance));
    if(p == NULL){
    exit(1);
    }
    p->distance = dist;
    p->pNext = NULL;
    return p;
}

AVL* createAVL(int routeNbr, float dist) {
    AVL* pNew = malloc(sizeof(AVL));
    if(pNew == NULL){
    exit(2);
    }
    pNew->routeid = routeNbr;
    pNew->distance = Newdistance(dist);
    pNew->pL = NULL;
    pNew->pR = NULL;
    pNew->taille = 1;
    pNew->minDistance = dist;
    pNew->maxDistance = dist;
    pNew->moyenne = 0;
    pNew->moyennedistance = dist;
    pNew->nombredist = 1;
    pNew->sommedist = dist;
    return pNew;
}

//utile pour le remplissage du 2ème AVL
AVL* createAVL2(int routeNbr, float maxdist, float mindist, float moyenne, float moyennedist) {
    AVL* pNew = malloc(sizeof(AVL));
    if(pNew == NULL){
    exit(2);
    }
    pNew->routeid = routeNbr;
    pNew->pL = NULL;
    pNew->pR = NULL;
    pNew->taille = 1;
    pNew->minDistance = mindist;
    pNew->maxDistance = maxdist;
    pNew->moyenne = moyenne;
    pNew->moyennedistance = moyennedist;
    return pNew;
}

int taille(AVL* p) {
    if (p == NULL){
        return 0;
    }
    return p->taille;
}

int getEquilibre(AVL* p) {
    if (p == NULL){
        return 0;
    }
    return taille(p->pL) - taille(p->pR);
}

AVL* rotationDroite(AVL* p) {
    AVL* p1= p->pL;
    AVL* p2 = p1->pR;

    p1->pR = p;
    p->pL = p2;

    p->taille = 1 + ((taille(p->pL) > taille(p->pR)) ? taille(p->pL) : taille(p->pR));
    p1->taille = 1 + ((taille(p1->pL) > taille(p1->pR)) ? taille(p1->pL) : taille(p1->pR));

    return p1;
}

AVL* rotationGauche(AVL* p) {
    AVL* p1 = p->pR;
    AVL* p2 = p1->pL;

    p1->pL = p;
    p->pR = p2;

    p->taille = 1 + ((taille(p->pL) > taille(p->pR)) ? taille(p->pL) : taille(p->pR));
    p1->taille = 1 + ((taille(p1->pL) > taille(p1->pR)) ? taille(p1->pL) : taille(p1->pR));

    return p1;
}

AVL* insertAVL(AVL* p, int routenbr, float dist) {
    if (p == NULL){
        return createAVL(routenbr, dist);
	}
    if (routenbr < p->routeid){
        p->pL = insertAVL(p->pL, routenbr, dist);
	}
    else if (routenbr > p->routeid){
        p->pR = insertAVL(p->pR, routenbr, dist);
	}
    else { //même numéro de route donc on compare les distances
        Distance* pNew = Newdistance(dist);
        pNew->distance = p->distance->distance;
        p->distance = pNew;

        // Mettre à jour les distances et la moyenne
        p->minDistance = (p->minDistance < dist) ? p->minDistance : dist;
        p->maxDistance = (p->maxDistance > dist) ? p->maxDistance : dist;
        p->moyenne = p->maxDistance - p->minDistance;
	
	// Mettre à jour la moyenne
        p->sommedist += dist;
        p->nombredist++;
	p->moyennedistance = p->sommedist / p->nombredist;
    }

    p->taille = 1 + ((taille(p->pL) > taille(p->pR)) ? taille(p->pL) : taille(p->pR));

    int balance = getEquilibre(p);

    if (balance > 1 && routenbr < p->pL->routeid){
        return rotationDroite(p);
	}
    if (balance < -1 && routenbr > p->pR->routeid){
        return rotationGauche(p);
	}
    if (balance > 1 && routenbr > p->pL->routeid){
        p->pL = rotationGauche(p->pL);
        return rotationDroite(p);
    }

    if (balance < -1 && routenbr < p->pR->routeid) {
        p->pR = rotationDroite(p->pR);
        return rotationGauche(p);
    }

    return p;
}


// Fonction utilitaire pour insérer un nœud dans l'AVL p2 trié par distance
AVL* insertDistance(AVL* p2, int numid, float maxdist, float mindist, float moyenne, float moyennedistance) {
    if (p2 == NULL) {
        return createAVL2(numid, maxdist, mindist, moyenne, moyennedistance);
    }

    if (moyenne < p2->moyenne) {
        p2->pL = insertDistance(p2->pL,numid, maxdist, mindist, moyenne, moyennedistance);
    } else if (moyenne > p2->moyenne) {
        p2->pR = insertDistance(p2->pR, numid, maxdist, mindist, moyenne, moyennedistance);
    }

    p2->taille = 1 + ((taille(p2->pL) > taille(p2->pR)) ? taille(p2->pL) : taille(p2->pR));

    int balance = getEquilibre(p2);

    if (balance > 1 && moyenne < p2->pL->moyenne) {
        return rotationDroite(p2);
    }
    if (balance < -1 && moyenne > p2->pR->moyenne) {
        return rotationGauche(p2);
    }
    if (balance > 1 && moyenne > p2->pL->moyenne) {
        p2->pL = rotationGauche(p2->pL);
        return rotationDroite(p2);
    }
    if (balance < -1 && moyenne < p2->pR->moyenne) {
        p2->pR = rotationDroite(p2->pR);
        return rotationGauche(p2);
    }

    return p2;
}

//fonction pour parcourir notre premier AVL et mettres ses données dans le 2ème AVL
AVL* insertAVL2(AVL* p, AVL* p2) {
    if (p == NULL) {
        return p2; // Aucune donnée à copier
    }

    // Copier le nœud actuel dans p2
    p2 = insertDistance(p2, p->routeid, p->maxDistance, p->minDistance, p->moyenne, p->moyennedistance);

    // Copier les sous-arbres gauche et droit
    p2 = insertAVL2(p->pL, p2);
    p2 = insertAVL2(p->pR, p2);

    return p2;
}

void displayInfixeInv(AVL* p) {
    if (p != NULL) {
        // Afficher les nœuds du sous-arbre droit
        displayInfixeInv(p->pR);

        // Afficher le nœud actuel
       printf("%d ; %f ; %f; %f; %f\n", p->routeid, p->minDistance, p->moyennedistance, p->maxDistance, p->moyenne);

        // Afficher les nœuds du sous-arbre gauche
        displayInfixeInv(p->pL);
    }
}

void destroyAVL(AVL* p) {
    if (p != NULL) {
        destroyAVL(p->pL);
        destroyAVL(p->pR);
        while( p -> distance != NULL){
	Distance* temp = p-> distance -> pNext;
	free(p->distance);
	p-> distance = temp;
	}
        free(p);
    }
}


int main() {
    AVL* AVL1 = NULL;
    AVL* AVL2 = NULL;
    float dist;
    int routeid;
    // Lecture des idroutes et distances pour remplir l'arbre root1
    while (scanf("%d;%f\n", &routeid, &dist) == 2) {
        AVL1 = insertAVL(AVL1, routeid, dist);
    }
	AVL2 = insertAVL2(AVL1,AVL2);
    // Afficher le contenu de AVL2 (trié par moyenne dans l'ordre décroissant)
    displayInfixeInv(AVL2);
    return 0;
}

