#define OPTION_S

#include "s.h"

// Fonction pour créer un nouveau nœud de distance
Distance* NewDistance(float dist) {
    Distance* newDistance = malloc(sizeof(Distance));
    if(newDistance == NULL){
        exit(1);
    }
    newDistance->distance = dist;
    newDistance->pNext = NULL;
    return newDistance;
}

// Fonction pour créer un nouveau nœud de l'arbre AVL
AVL* createAVL(int routeNbr, float dist) {
    AVL* newNode = malloc(sizeof(AVL));
    if(newNode == NULL){
        exit(2);
    }
    newNode->routeid = routeNbr;
    newNode->distance = NewDistance(dist);
    newNode->pL = NULL;
    newNode->pR = NULL;
    newNode->taille = 1;
    newNode->minDistance = dist;
    newNode->maxDistance = dist;
    newNode->moyenne = 0;
    newNode->moyennedistance = dist;
    newNode->nombredist = 1;
    newNode->sommedist = dist;
    return newNode;
}

// Fonction pour créer un second nœud AVL avec des informations supplémentaires
AVL* createAVL2(int routeNbr, float maxdist, float mindist, float moyenne, float moyennedist) {
    AVL* newNode = malloc(sizeof(AVL));
    if(newNode == NULL){
        exit(2);
    }
    newNode->routeid = routeNbr;
    newNode->pL = NULL;
    newNode->pR = NULL;
    newNode->taille = 1;
    newNode->minDistance = mindist;
    newNode->maxDistance = maxdist;
    newNode->moyenne = moyenne;
    newNode->moyennedistance = moyennedist;
    return newNode;
}

// Fonction pour obtenir la taille de l'arbre AVL
int size(AVL* node) {
    if (node == NULL){
        return 0;
    }
    return node->taille;
}

// Fonction pour calculer le facteur d'équilibre d'un nœud dans l'arbre AVL
int getBalance(AVL* node) {
    if (node == NULL){
        return 0;
    }
    return size(node->pL) - size(node->pR);
}

// Fonction pour effectuer une rotation à droite
AVL* rotateRight(AVL* node) {
    AVL* leftChild = node->pL;
    AVL* subtree = leftChild->pR;

    leftChild->pR = node;
    node->pL = subtree;

    node->taille = 1 + ((size(node->pL) > size(node->pR)) ? size(node->pL) : size(node->pR));
    leftChild->taille = 1 + ((size(leftChild->pL) > size(leftChild->pR)) ? size(leftChild->pL) : size(leftChild->pR));

    return leftChild;
}

// Fonction pour effectuer une rotation à gauche
AVL* rotateLeft(AVL* node) {
    AVL* rightChild = node->pR;
    AVL* subtree = rightChild->pL;

    rightChild->pL = node;
    node->pR = subtree;

    node->taille = 1 + ((size(node->pL) > size(node->pR)) ? size(node->pL) : size(node->pR));
    rightChild->taille = 1 + ((size(rightChild->pL) > size(rightChild->pR)) ? size(rightChild->pL) : size(rightChild->pR));

    return rightChild;
}

// Fonction pour insérer un nouveau nœud dans l'arbre AVL
AVL* insertAVL(AVL* node, int routenbr, float dist) {
    if (node == NULL){
        return createAVL(routenbr, dist);
    }
    if (routenbr < node->routeid){
        node->pL = insertAVL(node->pL, routenbr, dist);
    }
    else if (routenbr > node->routeid){
        node->pR = insertAVL(node->pR, routenbr, dist);
    }
    else { // Même numéro de route, donc comparaison des distances
        Distance* newDistance = NewDistance(dist);
        newDistance->distance = node->distance->distance;
        node->distance = newDistance;

        // Mise à jour des distances et de la moyenne
        node->minDistance = (node->minDistance < dist) ? node->minDistance : dist;
        node->maxDistance = (node->maxDistance > dist) ? node->maxDistance : dist;
        node->moyenne = node->maxDistance - node->minDistance;
        
        // Mise à jour de la moyenne
        node->sommedist += dist;
        node->nombredist++;
        node->moyennedistance = node->sommedist / node->nombredist;
    }

    node->taille = 1 + ((size(node->pL) > size(node->pR)) ? size(node->pL) : size(node->pR));

    int balance = getBalance(node);

    // Effectuer des rotations pour maintenir la propriété AVL
    if (balance > 1 && routenbr < node->pL->routeid){
        return rotateRight(node);
    }
    if (balance < -1 && routenbr > node->pR->routeid){
        return rotateLeft(node);
    }
    if (balance > 1 && routenbr > node->pL->routeid){
        node->pL = rotateLeft(node->pL);
        return rotateRight(node);
    }

    if (balance < -1 && routenbr < node->pR->routeid) {
        node->pR = rotateRight(node->pR);
        return rotateLeft(node);
    }

    return node;
}

// Fonction pour insérer un nœud dans l'arbre AVL p2 trié par la distance
AVL* insertDistance(AVL* p2, int numid, float maxdist, float mindist, float moyenne, float moyennedistance) {
    if (p2 == NULL) {
        return createAVL2(numid, maxdist, mindist, moyenne, moyennedistance);
    }

    // Insérer en fonction de la moyenne
    if (moyenne < p2->moyenne) {
        p2->pL = insertDistance(p2->pL, numid, maxdist, mindist, moyenne, moyennedistance);
    } else if (moyenne > p2->moyenne) {
        p2->pR = insertDistance(p2->pR, numid, maxdist, mindist, moyenne, moyennedistance);
    }

    // Mettre à jour la taille et effectuer les rotations nécessaires
    p2->taille = 1 + ((size(p2->pL) > size(p2->pR)) ? size(p2->pL) : size(p2->pR));
    int balance = getBalance(p2);

    if (balance > 1 && moyenne < p2->pL->moyenne) {
        return rotateRight(p2);
    }
    if (balance < -1 && moyenne > p2->pR->moyenne) {
        return rotateLeft(p2);
    }
    if (balance > 1 && moyenne > p2->pL->moyenne) {
        p2->pL = rotateLeft(p2->pL);
        return rotateRight(p2);
    }
    if (balance < -1 && moyenne < p2->pR->moyenne) {
        p2->pR = rotateRight(p2->pR);
        return rotateLeft(p2);
    }

    return p2;
}

// Fonction pour parcourir le premier arbre AVL et copier ses données dans le second arbre AVL
AVL* insertAVL2(AVL* p, AVL* p2) {
    if (p == NULL) {
        return p2; // Pas de données à copier
    }

    // Copier le nœud actuel dans p2
    p2 = insertDistance(p2, p->routeid, p->maxDistance, p->minDistance, p->moyenne, p->moyennedistance);

    // Copier les sous-arbres gauche et droit
    p2 = insertAVL2(p->pL, p2);
    p2 = insertAVL2(p->pR, p2);

    return p2;
}

// Fonction pour afficher l'arbre AVL en ordre infixe inversé
void displayInfixeInv(AVL* node) {
    if (node != NULL) {
        // Afficher les nœuds du sous-arbre droit
        displayInfixeInv(node->pR);

        // Afficher le nœud actuel
        printf("%d ; %f ; %f; %f; %f\n", node->routeid, node->minDistance, node->moyennedistance, node->maxDistance, node->moyenne);

        // Afficher les nœuds du sous-arbre gauche
        displayInfixeInv(node->pL);
    }
}

// Fonction pour libérer la mémoire de l'arbre AVL
void destroyAVL(AVL* node) {
    if (node != NULL) {
        destroyAVL(node->pL);
        destroyAVL(node->pR);
        while(node->distance != NULL){
            Distance* temp = node->distance->pNext;
            free(node->distance);
            node->distance = temp;
        }
        free(node);
    }
}

int main() {
    AVL* AVL1 = NULL;
    AVL* AVL2 = NULL;
    float dist;
    int routeid;
    // Lecture des idroutes et distances pour remplir l'arbre AVL1
    while (scanf("%d;%f\n", &routeid, &dist) == 2) {
        AVL1 = insertAVL(AVL1, routeid, dist);
    }
    AVL2 = insertAVL2(AVL1, AVL2);
    // Afficher le contenu de AVL2 (trié par moyenne dans l'ordre décroissant)
    displayInfixeInv(AVL2);
    // Libérer la mémoire
    destroyAVL(AVL1);
    destroyAVL(AVL2);
    return 0;
}
