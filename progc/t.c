#include "t.h"

NODE * createNode (char word[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){ //création d'un nouveau noeud
    NODE* pNew = malloc(sizeof(NODE));
    if (pNew == NULL){
        exit(2);
    }
    strcpy(pNew->ville , word);    //copie du nom de ville : strcpy(destination de la copie, source);
    pNew->nb_apparition_par_trajet = NB_apparition_par_trajet;
    pNew->nb_apparition_ville_depart_trajet = NB_apparition_ville_depart_trajet;
    pNew->pNext = NULL;
    return pNew;
}

NODE* addStart (NODE * pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){  //ajout en début de liste 
    NODE * pNew = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    if (pNew == NULL){
        exit(2601); //pour strcmp()
    }
    if (pHead == NULL){
        pHead = pNew;
        return pHead;
    }
    else{
        pNew->pNext = pHead;
        pHead = pNew;
        return pHead;
    }
}

NODE * addEnd(NODE * pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){  //ajout en fin de liste 
    NODE* pNew = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    NODE* pList = pHead;
    if (pNew == NULL){
        exit(2602);
    }
    if (pHead == NULL){
        pHead = pNew;
    }
    else{
        while(pList->pNext!=NULL){
            pList = pList->pNext;
        }
        pList->pNext = pNew;
    }
    return pHead;
}

void addBetweenNodes(NODE* pList, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){        //ajoute le noeud entre pList et pList->pNext
    if (pList == NULL){
        exit(2603);
    }
    NODE* pNew = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    if (pNew == NULL){
        exit(2604);
    }
    pNew->pNext = pList->pNext;
    pList->pNext = pNew;
}

NODE* insertNode(NODE* pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){ // inserer dans la liste
    //CAS 1 : pas de mot dans la liste
    if (pHead == NULL){
        pHead = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    }
    //CAS 2 : 1er mot de la liste
    else if (strcmp(nom, pHead->ville) < 0) {
        pHead = addStart(pHead, nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    }
    else{
        NODE* pList = pHead;
        NODE* pPrevious = NULL;
        while(strcmp(nom, pList->ville) > 0 && pList->pNext != NULL){
            pPrevious = pList;
            pList = pList->pNext;
        }
        //CAS 4 : on arrive sur dernier mot de liste + mot à placer après -> ajout du mot
        if (pList->pNext == NULL && strcmp(nom, pList->ville) > 0){
            pList = addEnd(pList, nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
        }
        //CAS 3 : Insertion du mot avant le mot sur lequel on est (cad là où on veut)
        else if (strcmp(nom, pList->ville) < 0){
            addBetweenNodes(pPrevious, nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
        }
        else{
            //strcmp(nom, phead->ville) == 0
            printf("ERREUR! Arrivée en bout de liste: le mot n'est ni le dernier ni l'avant-dernier mot");
            exit(2605);
        }
    }
    return pHead;
}

void displayList(NODE* pHead){  //afficher la liste
    if (pHead == NULL){
        exit(2606);
    }
    /*printf("Affichage Liste (ordre alphabétique) :     pHead : %s\n", pHead->ville);*/
    while(pHead != NULL){
        printf("%s;%d;%d\n", pHead->ville, pHead->nb_apparition_par_trajet, pHead->nb_apparition_ville_depart_trajet);
        pHead= pHead->pNext;
    }
}

void destroyNode(NODE* pNode) {  //supprimer un chainon
    if (pNode != NULL) {
        destroyNode(pNode->pNext);
        free(pNode);
        //printf("Libération Node réussie.\n");
    }
}

//Création des NOEUDS 

// 1) CreateNode spécifique pour Route_ID
Node_RouteID* create_Node_RouteID(int RouteID){
    Node_RouteID* pNew = malloc(sizeof(Node_RouteID));
    if(pNew == NULL){
        printf("Erreur 1500 : pointeur NULL.\n");
        exit(1500);
    }
    pNew->Route_ID = RouteID;
    pNew->equilibre = 0;
    pNew->pL = NULL;
    pNew->pR = NULL;
    return pNew;
}

// 2) creer arbre général
Tree* createTree(char nom[], int Route_ID){
    Tree* pNew = malloc(sizeof(Tree));
    if(pNew == NULL){
        printf("Erreur 1 : pointeur NULL.\n");
        exit(1);
    }
    int i = 0;
    while(nom[i] != '\0'){
        if (i >= 99){
            printf("ERREUR 2 : Nom de ville de plus de 99 caractères.\n");
            exit(2);
        }
        pNew->ville[i] = *(nom + i);
        i++;
    }
    pNew->AVL_RouteID = create_Node_RouteID(Route_ID);
    pNew->nb_apparition_ville_depart_trajet = 0;
    pNew->nb_apparition_ville_arrivee_etape = 0;
    pNew->nb_apparition_par_trajet = 0;
    pNew->doublons_ville_par_trajet = 0;
    pNew->equilibre = 0;
    pNew->pL = NULL;
    pNew->pR = NULL;
    return pNew;
}

//ROTATIONS SIMPLES:
// 1) Rotations spécifiques pour Route_ID
Node_RouteID* RotationGauche_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pR == NULL){
        exit(1910);
    }
    Node_RouteID* pivot = p_RouteID->pR;

    //Equilibrage
    if(p_RouteID->equilibre == 2){
        if(pivot->equilibre == 0){
            pivot->equilibre = -1;
            p_RouteID->equilibre = 1;
        }
        else if(pivot->equilibre == 1){
            pivot->equilibre = 0;
            p_RouteID->equilibre = 0;
        }
        else if(pivot->equilibre == 2){
            pivot->equilibre = 0;
            p_RouteID->equilibre = -1;
        }
        else{
            printf("Cas non pris en charge 1911. \n");
            exit(1911);
        }
    }

    else if(p_RouteID->equilibre == 1){
        if(pivot->equilibre == -1){
            pivot->equilibre = -2;
            p_RouteID->equilibre = 0;
        }
        else if(pivot->equilibre == 0){
            pivot->equilibre = -1;
            p_RouteID->equilibre = 0;
        }
        else if(pivot->equilibre == 1){
            pivot->equilibre = -1;
            p_RouteID->equilibre = -1;
        }
        else{
            printf("Cas non pris en charge 1912. \n");
            exit(1912);
        }
    }

    else{
        printf("Cas non pris en charge 1913. \n");
        exit(1913);
    }

    //Rotation
    p_RouteID->pR = pivot->pL;
    pivot->pL = p_RouteID;
    if (pivot == NULL){
        printf("Erreur 1914 : pointeur NULL.\n");
        exit(1914);
    }

    return pivot;
}

Node_RouteID* RotationDroite_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pL == NULL){
        exit(1915);
    }
    Node_RouteID* pivot = p_RouteID->pL;

    //Equilibrage
    if(p_RouteID->equilibre == -2){
        //Rotation simple 1
        if(pivot->equilibre == -1){
            pivot->equilibre = 0;
            p_RouteID->equilibre = 0;
        }
        //Rotation Simple 2
        else if(pivot->equilibre == 0){
            pivot->equilibre = 1;
            p_RouteID->equilibre = -1;
        }
        else if(pivot->equilibre == -2){
            pivot->equilibre = 0;
            p_RouteID->equilibre = 1;
        }
        else{
            printf("Cas non pris en charge 1916. \n");
            exit(1916);
        }
    }

    else if(p_RouteID->equilibre == -1){
        if(pivot->equilibre == -1){
            pivot->equilibre = 1;
            p_RouteID->equilibre = 1;
        }
        else if(pivot->equilibre == 0){
            pivot->equilibre = 1;
            p_RouteID->equilibre = 0;
        }
        else if(pivot->equilibre == 1){
            pivot->equilibre = 2;
            p_RouteID->equilibre = 0;
        }
        else{
            printf("Cas non pris en charge 1917. \n");
            exit(1917);
        }
    }

    else{
        printf("Cas non pris en charge 1918. \n");
        exit(1918);
    }

    //Rotation
    p_RouteID->pL = pivot->pR;
    pivot->pR = p_RouteID;
    if (pivot == NULL){
        printf("Erreur 1919 : pointeur NULL.\n");
        exit(1919);
    }

    return pivot;
}

// 2) Rotations générales
Tree* RotationGauche(Tree* p){
    if (p==NULL || p->pR == NULL){
        exit(10);
    }
    Tree* pivot = p->pR;

    //Equilibrage
    if(p->equilibre == 2){
        if(pivot->equilibre == 0){
            pivot->equilibre = -1;
            p->equilibre = 1;
        }
        else if(pivot->equilibre == 1){
            pivot->equilibre = 0;
            p->equilibre = 0;
        }
        else if(pivot->equilibre == 2){
            pivot->equilibre = 0;
            p->equilibre = -1;
        }
        else{
            printf("Cas non pris en charge 11. \n");
            exit(11);
        }
    }

    else if(p->equilibre == 1){
        if(pivot->equilibre == -1){
            pivot->equilibre = -2;
            p->equilibre = 0;
        }
        else if(pivot->equilibre == 0){
            pivot->equilibre = -1;
            p->equilibre = 0;
        }
        else if(pivot->equilibre == 1){
            pivot->equilibre = -1;
            p->equilibre = -1;
        }
        else{
            printf("Cas non pris en charge 12. \n");
            exit(12);
        }
    }

    else{
        printf("Cas non pris en charge 13. \n");
        exit(13);
    }

    //Rotation
    p->pR = pivot->pL;
    pivot->pL = p;
    if (pivot == NULL){
        printf("Erreur 14 : pointeur NULL.\n");
        exit(14);
    }

    return pivot;
}


Tree* RotationDroite(Tree* p){
    if (p==NULL || p->pL == NULL){
        exit(15);
    }
    Tree* pivot = p->pL;

    //Equilibrage
    if(p->equilibre == -2){
        //Rotation simple 1
        if(pivot->equilibre == -1){
            pivot->equilibre = 0;
            p->equilibre = 0;
        }
        //Rotation Simple 2
        else if(pivot->equilibre == 0){
            pivot->equilibre = 1;
            p->equilibre = -1;
        }
        else if(pivot->equilibre == -2){
            pivot->equilibre = 0;
            p->equilibre = 1;
        }
        else{
            printf("Cas non pris en charge 16. \n");
            exit(16);
        }
    }

    else if(p->equilibre == -1){
        if(pivot->equilibre == -1){
            pivot->equilibre = 1;
            p->equilibre = 1;
        }
        else if(pivot->equilibre == 0){
            pivot->equilibre = 1;
            p->equilibre = 0;
        }
        else if(pivot->equilibre == 1){
            pivot->equilibre = 2;
            p->equilibre = 0;
        }
        else{
            printf("Cas non pris en charge 17. \n");
            exit(17);
        }
    }

    else{
        printf("Cas non pris en charge 18. \n");
        exit(18);
    }

    //Rotation
    p->pL = pivot->pR;
    pivot->pR = p;
    if (pivot == NULL){
        printf("Erreur 19 : pointeur NULL.\n");
        exit(19);
    }

    return pivot;
}


//DOUBLES ROTATIONS 

// 1) Doubles rotations spécifiques pour Route_ID
Node_RouteID* DoubleRotationGauche_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pR==NULL || p_RouteID->equilibre!=2 || p_RouteID->pR->equilibre!=-1){
        printf("Cas non pris en charge 1820 : pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(1820);
    }

    p_RouteID->pR = RotationDroite_RouteID(p_RouteID->pR);
    if (p_RouteID->pR == NULL){
        printf("Erreur 1821 : pointeur NULL.\n");
        exit(1821);
    }
    p_RouteID = RotationGauche_RouteID(p_RouteID);
    if (p_RouteID == NULL){
        printf("Erreur 1822 : pointeur NULL.\n");
        exit(1822);
    }
    return p_RouteID;
}

Node_RouteID* DoubleRotationDroite_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pL==NULL || p_RouteID->equilibre!=-2 || p_RouteID->pL->equilibre!=1){
        printf("Cas non pris en charge 1800 : pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(1800);
    }

    p_RouteID->pL = RotationGauche_RouteID(p_RouteID->pL);
    if (p_RouteID->pL == NULL){
        printf("Erreur 1801 : pointeur NULL.\n");
        exit(1801);
    }

    p_RouteID = RotationDroite_RouteID(p_RouteID);
    if (p_RouteID == NULL){
        printf("Erreur 1802 : pointeur NULL.\n");
        exit(1802);
    }
    return p_RouteID;
}


// 2) Doubles rotations générales
Tree* DoubleRotationGauche(Tree* p){
    if (p==NULL || p->pR==NULL || p->equilibre!=2 || p->pR->equilibre!=-1){
        printf("Cas non pris en charge 20 : pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(20);
    }

    p->pR = RotationDroite(p->pR);
    if (p->pR == NULL){
        printf("Erreur 21 : pointeur NULL.\n");
        exit(21);
    }
    p = RotationGauche(p);
    if (p == NULL){
        printf("Erreur 22 : pointeur NULL.\n");
        exit(22);
    }
    return p;
}

Tree* DoubleRotationDroite(Tree* p){                                                                
    if (p==NULL || p->pL==NULL || p->equilibre!=-2 || p->pL->equilibre!=1){
        printf("Cas non pris en charge 30 : pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(30);
    }

    p->pL = RotationGauche(p->pL);
    if (p->pL == NULL){
        printf("Erreur 31 : pointeur NULL.\n");
        exit(31);
    }

    p = RotationDroite(p);
    if (p == NULL){
        printf("Erreur 32 : pointeur NULL.\n");
        exit(32);
    }
    return p;
}



// EQUILIBRAGE 

//1) Equilibrage spécifique pour Route_ID
Node_RouteID* equilibrerAVL_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID == NULL){
        printf("Erreur 1700 : pointeur NULL.\n");
        exit(1700);
    }

    if (p_RouteID->equilibre == 2){
        if (p_RouteID->pR == NULL){
            printf("Erreur 1701 : pointeur NULL.\n");
            exit(1701);
        }

        //Double rotation Gauche (RD puis RG)
        if (p_RouteID->pR->equilibre == -1){
            p_RouteID = DoubleRotationGauche_RouteID(p_RouteID);
        }

        //Rotation Simple Gauche
        else if (p_RouteID->pR->equilibre >= 0){
            p_RouteID = RotationGauche_RouteID(p_RouteID);
        }

        else {
            printf("Cas non pris en charge 1702.\n");
            exit(1702);
        }
    }
    else if (p_RouteID->equilibre == -2) {
        if (p_RouteID->pL == NULL){
            printf("Erreur 1703 : pointeur NULL.\n");
            exit(1703);
        }

        //Double rotation Droite (RG puis RD)
        if (p_RouteID->pL->equilibre == 1){
            p_RouteID = DoubleRotationDroite_RouteID(p_RouteID);
        }

        //Rotation Simple Droite
        else if (p_RouteID->pL->equilibre <= 0){
            p_RouteID = RotationDroite_RouteID(p_RouteID);
        }

        else {
            printf("Cas non pris en charge 1704.\n");
            exit(1704);
        }
    }
    else if (p_RouteID->equilibre>-2 && p_RouteID->equilibre<2){        //p_RouteID->equilibre == -1 ou 0 ou 1. Donc équilibré.
        //do nothing
        //printf("Pas besoin de rééquilibrage : equilibre = %d.\n", p_RouteID->equilibre);
    }
    else{
        printf("Cas non pris en charge 1705 : racine de l'arbre >2 ou <-2 .\n");
        exit(1705);
    }
    if (p_RouteID == NULL){
        printf("Erreur 1706 : pointeur NULL.\n");
        exit(1706);
    }

    return p_RouteID;
}


// 2) Equilibrage général
Tree* equilibrerAVL(Tree* p){
    if (p == NULL){
        printf("Erreur 40 : pointeur NULL.\n");
        exit(40);
    }

    if (p->equilibre == 2){
        if (p->pR == NULL){
            printf("Erreur 41 : pointeur NULL.\n");
            exit(41);
        }

        //Double rotation Gauche (RD puis RG)
        if (p->pR->equilibre == -1){
            p = DoubleRotationGauche(p);
        }

        //Rotation Simple Gauche
        else if (p->pR->equilibre >= 0){
            p = RotationGauche(p);
        }

        else {
            printf("Cas non pris en charge 42.\n");
            exit(42);
        }
    }
    else if (p->equilibre == -2) {
        if (p->pL == NULL){
            printf("Erreur 43 : pointeur NULL.\n");
            exit(43);
        }

        //Double rotation Droite (RG puis RD)
        if (p->pL->equilibre == 1){
            p = DoubleRotationDroite(p);
        }

        //Rotation Simple Droite
        else if (p->pL->equilibre <= 0){
            p = RotationDroite(p);
        }

        else {
            printf("Cas non pris en charge 44.\n");
            exit(44);
        }
    }
    else if (p->equilibre>-2 && p->equilibre<2){        //p->equilibre == -1 ou 0 ou 1. Donc équilibré.
        //do nothing
        //printf("Pas besoin de rééquilibrage : equilibre = %d.\n", p->equilibre);
    }
    else{
        printf("Cas non pris en charge 45 : racine de l'arbre >2 ou <-2 .\n");
        exit(45);
    }
    if (p == NULL){
        printf("Erreur 46 : pointeur NULL.\n");
        exit(46);
    }

    return p;
}



//INSERTION AVL 
// 1) Insertion AVL spécifique pour Route_ID

Node_RouteID* insertAVL_RouteID (Tree* p, Node_RouteID* p_RouteID, int v, int* h){  //p_RouteID = p->AVL_RouteID
    //TEST
    if (p == NULL || h == NULL){
        printf("Erreur 1600 : pointeur NULL.\n");
        exit(1600);
    }

    //PROCESS
    if (p_RouteID == NULL){
        p_RouteID = create_Node_RouteID(v);
        *h = 1;
        if (p_RouteID == NULL){
            printf("Erreur 1602 : pointeur NULL.\n");
            exit(1602);
        }
        return p_RouteID;
    }
    else if (v > p_RouteID->Route_ID){
        p_RouteID->pR = insertAVL_RouteID(p, p_RouteID->pR, v, h);
    }
    else if (v < p_RouteID->Route_ID){
        p_RouteID->pL = insertAVL_RouteID(p, p_RouteID->pL, v, h);
        *h = -*h;
    }
    else {
        p->doublons_ville_par_trajet = p->doublons_ville_par_trajet + 1 ;
        p->nb_apparition_par_trajet = p->nb_apparition_par_trajet - 1 ;
        *h = 0;
        return p_RouteID;
    }

    if (*h != 0){
        p_RouteID->equilibre = p_RouteID->equilibre + *h;
        p_RouteID = equilibrerAVL_RouteID(p_RouteID);
        if(p_RouteID->equilibre == 0){
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    return p_RouteID;
}


// 2) Insertion AVL général

Tree* insertAVL_numerique (Tree* p, Tree* p_copie, int* h){ // AVL trié avec des entiers (par nb d'apparition de ville)
    //TEST
    if (h == NULL){
        printf("Erreur 50 : pointeur NULL.\n");
        exit(50);
    }

    //PROCESS
    if (p == NULL){
        p = p_copie;
        *h = 1;
        if (p == NULL){
            printf("Erreur 51 : pointeur NULL.\n");
            exit(51);
        }
        return p;
    }
    else if (p_copie->nb_apparition_par_trajet >= p->nb_apparition_par_trajet){
        p->pR = insertAVL_numerique(p->pR, p_copie, h);
    }
    else if (p_copie->nb_apparition_par_trajet < p->nb_apparition_par_trajet){
        p->pL = insertAVL_numerique(p->pL, p_copie, h);
        *h = -*h;
    }
    else {
        printf("Erreur 52 : Cas impossible.\n");
        exit(52);
    }

    if (*h != 0){
        p->equilibre = p->equilibre + *h;
        p = equilibrerAVL(p);
        if(p->equilibre == 0){
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    // return l'adresse 
    return p;
}

// AVL trié par chaînes de caractères (ordre alphabétique de nom de ville)
Tree* insertAVL_alphabetique (Tree* p, int Route_ID, char nom_ville[], int ville_Arrivee, int* h){
    /*  ville_Arrivee == 1 signifie : on insère la ville de départ du trajet (ville départ de l'étape 1)
        ville_Arrivee == 2 signifie : on insère la ville d'arrivée de toute étape >= 1
    */
    //TEST
    if (h == NULL){
        printf("Erreur 50 : pointeur NULL.\n");
        exit(50);
    }

    //PROCESS
    if (p == NULL){
        p = createTree(nom_ville, Route_ID);
        //Ville départ trajet
        if (ville_Arrivee == 1){
            p->nb_apparition_ville_depart_trajet = p->nb_apparition_ville_depart_trajet + 1 ;
            p->nb_apparition_par_trajet = p->nb_apparition_par_trajet + 1 ;
        }
        //Ville arrivée étape
        else if (ville_Arrivee == 2){
            p->nb_apparition_ville_arrivee_etape = p->nb_apparition_ville_arrivee_etape + 1 ;
            p->nb_apparition_par_trajet = p->nb_apparition_par_trajet + 1 ;
        }
        else{
            printf("Erreur 51 : on a ni une ville de départ, ni une ville d'arrivée.\n");
            exit(51);
        }
        *h = 1;
        if (p == NULL){
            printf("Erreur 52 : pointeur NULL.\n");
            exit(52);
        }
        return p;
    }
    else if (strcmp(nom_ville, p->ville) > 0){
        p->pR = insertAVL_alphabetique(p->pR, Route_ID, nom_ville, ville_Arrivee, h);
    }
    else if (strcmp(nom_ville, p->ville) < 0){
        p->pL = insertAVL_alphabetique(p->pL, Route_ID, nom_ville, ville_Arrivee, h);
        *h = -*h;
    }
    else {
        // Si on a trouvé le même nom de ville : on ne fait rien
        // strcmp(nom, p->ville) == 0
        if (ville_Arrivee == 1){
            p->nb_apparition_ville_depart_trajet = p->nb_apparition_ville_depart_trajet + 1 ;
            p->nb_apparition_par_trajet = p->nb_apparition_par_trajet + 1 ;
            int* h2 = malloc(sizeof(int));
            p->AVL_RouteID = insertAVL_RouteID(p, p->AVL_RouteID, Route_ID, h2); //insérer Route_ID : p->AVL_RouteID = insertAVL_RouteID(Route_ID);
            free(h2);
        }
        else if (ville_Arrivee == 2){
            p->nb_apparition_ville_arrivee_etape = p->nb_apparition_ville_arrivee_etape + 1 ;
            p->nb_apparition_par_trajet = p->nb_apparition_par_trajet + 1 ;
            int* h2 = malloc(sizeof(int));
            p->AVL_RouteID = insertAVL_RouteID(p, p->AVL_RouteID, Route_ID, h2); //insérer Route_ID : p->AVL_RouteID = insertAVL_RouteID(Route_ID);
            free(h2);
        }
        else{
            printf("Erreur 53 : on a ni une ville de départ, ni une ville d'arrivée.\n");
            exit(53);
        }
        *h = 0;
        return p;
    }

    if (*h != 0){
        p->equilibre = p->equilibre + *h;
        p = equilibrerAVL(p);
        if(p->equilibre == 0){
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    // return the address (may be new)
    return p;
}

Tree* selection_insertAVL_alphabetique (Tree* p, char villeA[], char villeB[], int Route_ID, int etape, int* h){  //insertion dans AVL toute les villes d'un trajet 
    /* La fonction insertAVL_alphabetique s'occupe d'ajouter :
        -> +1 à nb_apparition_ville_depart_trajet si : on veut insérer villeA et etape==1
        -> +1 à nb_apparition_ville_arrivee_etape si : on veut insérer villeB et etape>=1
    */
    if (h == NULL){
        printf("Erreur 1200 : pointeur NULL.\n");
        exit(1200);
    }
    if (Route_ID <= 0 || etape <= 0){
        printf("Erreur 1201 : Identifiant de trajet <= 0 ou numéro d'étape de trajet <= 0.\n");
        exit(1201);
    }
    /*  ville_Arrivee == 1 signifie : on insère la ville de départ du trajet (ville départ de l'étape 1)
        ville_Arrivee == 2 signifie : on insère la ville d'arrivée de toute étape >= 1
    */
    int ville_Arrivee = 0;
    if (etape == 1){
        ville_Arrivee = 1;
        p = insertAVL_alphabetique(p, Route_ID, villeA, ville_Arrivee, h);
    }
    ville_Arrivee = 2;
    p = insertAVL_alphabetique(p, Route_ID, villeB, ville_Arrivee, h);

    return p;
}


//Destruction AVL

// 1) destroyAVL spécifique pour Route_ID
void destroyAVL_RouteID(Node_RouteID* p_RouteID) {
    if (p_RouteID != NULL) {
        destroyAVL_RouteID(p_RouteID->pL);
        destroyAVL_RouteID(p_RouteID->pR);
        free(p_RouteID);
    }
}

// 2) destroyAVL général
void destroyAVL(Tree* p) {
    if (p != NULL) {
        destroyAVL(p->pL);
        destroyAVL(p->pR);
        destroyAVL_RouteID(p->AVL_RouteID);
        free(p);
    }
}


//----------------- AFFICHAGE : préfixe, infixe -----------------

// 1) Affichage spécifique pour Route_ID
void displayPrefixe_RouteID(Node_RouteID* p_RouteID){
    if(p_RouteID !=NULL){
        printf("                        RouteID = [%d],   equilibre = %d\n",p_RouteID->Route_ID, p_RouteID->equilibre);
        displayPrefixe_RouteID(p_RouteID->pL);
        displayPrefixe_RouteID(p_RouteID->pR);
    }
}

void displayInfixe_RouteID(Node_RouteID* p_RouteID){
    if(p_RouteID !=NULL){
        displayInfixe_RouteID(p_RouteID->pL);
        printf("                        RouteID = [%d],   equilibre = %d\n",p_RouteID->Route_ID, p_RouteID->equilibre);
        displayInfixe_RouteID(p_RouteID->pR);
    }
}


// 2) Affichage général
void displayPrefixe(Tree* p){
    if(p !=NULL){
        printf("[%s],   equilibre = %d,\n            nb_apparition_ville_depart_trajet = %d\n            nb_apparition_ville_arrivee_etape = %d\n            nb_apparition_par_trajet = %d\n            doublons_ville_par_trajet = %d\n",p->ville, p->equilibre, p->nb_apparition_ville_depart_trajet, p->nb_apparition_ville_arrivee_etape, p->nb_apparition_par_trajet, p->doublons_ville_par_trajet);
        printf("            Affichage Préfixe AVL_RouteID :\n");
        displayPrefixe_RouteID(p->AVL_RouteID);
        printf("\n");
        displayPrefixe(p->pL);
        displayPrefixe(p->pR);
    }
}

void displayInfixe(Tree* p){
    if(p != NULL){
        displayInfixe(p->pL);
        printf("[%s],   equilibre = %d,\n            nb_apparition_ville_depart_trajet = %d\n            nb_apparition_ville_arrivee_etape = %d\n            nb_apparition_par_trajet = %d\n            doublons_ville_par_trajet = %d\n",p->ville, p->equilibre, p->nb_apparition_ville_depart_trajet, p->nb_apparition_ville_arrivee_etape, p->nb_apparition_par_trajet, p->doublons_ville_par_trajet);
        printf("            Affichage Infixe AVL_RouteID :\n");
        displayInfixe_RouteID(p->AVL_RouteID);
        printf("\n");
        displayInfixe(p->pR);
    }
}

void displayInfixeInverse(Tree* p){
    if(p !=NULL){
        displayInfixeInverse(p->pR);
        printf("[%s],   equilibre = %d,\n            nb_apparition_ville_depart_trajet = %d\n            nb_apparition_ville_arrivee_etape = %d\n            nb_apparition_par_trajet = %d\n            doublons_ville_par_trajet = %d\n",p->ville, p->equilibre, p->nb_apparition_ville_depart_trajet, p->nb_apparition_ville_arrivee_etape, p->nb_apparition_par_trajet, p->doublons_ville_par_trajet);
        /*
        printf("            Affichage Infixe AVL_RouteID :\n");
        displayInfixe_RouteID(p->AVL_RouteID);
        */
        printf("\n");
        displayInfixeInverse(p->pL);
    }
}

NODE* n_villes_plus_parcourues(Tree* p, int* n, NODE* pNode){ // on veut retourner les n villes les plus traversées. Ici nest un compteur qui décroit. (si on veut les 10 villes les plus traversées, on rentrera *n=10)
    if(p != NULL && *n > 0){
        pNode = n_villes_plus_parcourues(p->pR, n, pNode);
        if(*n > 0){
            //printf("TEST InsertNode\n");
            pNode = insertNode(pNode, p->ville, p->nb_apparition_par_trajet, p->nb_apparition_ville_depart_trajet);
            *n = *n -1;
            //printf("    compteur = %d\n", *n);
            pNode = n_villes_plus_parcourues(p->pL, n, pNode);
        }
        //printf("%s;%d;%d\n",p->ville, p->nb_apparition_par_trajet, p->nb_apparition_ville_depart_trajet);
    }
    return pNode;
}

//---------------------------------- COPIE de AVL ---------------------------------- (ces fonctions ne sont plus exécutées dans le présent code)
//copie l'AVL en entier et retourne l'adresse de la racine
Node_RouteID * copie_AVL_RouteID (Node_RouteID* p_origine_RouteID){
    if(p_origine_RouteID == NULL){
        return p_origine_RouteID;
    }
    else{
        Node_RouteID * pNew = create_Node_RouteID(-1);
        if(pNew == NULL){
            printf("Erreur 2000 : pointeur NULL.\n");
            exit(2000);
        }
        pNew->Route_ID = p_origine_RouteID->Route_ID;
        if (pNew->Route_ID == -1){
            printf("Erreur 2001 : Echec copie de Route_ID.\n");
            exit(2001);
        }
        pNew->equilibre = p_origine_RouteID->equilibre;
        pNew->pL = copie_AVL_RouteID(p_origine_RouteID->pL);
        pNew->pR = copie_AVL_RouteID(p_origine_RouteID->pR);
        return pNew;
    }
}

//copie l'AVL en entier et retourne l'adresse de la racine
Tree * copie_AVL (Tree* p_origine){
    if(p_origine == NULL){
        return p_origine;
    }
    else{
        Tree * pNew = createTree("", 0); //0 est une valeau arbitraire ici, si l'on pouvait, on ne mettrait pas de Route_ID.
        destroyAVL_RouteID(pNew->AVL_RouteID); // on supprime l'AVL_RouteID créé inutilement
        if(pNew == NULL){
            printf("Erreur 1000 : pointeur NULL.\n");
            exit(1000);
        }
        strcpy(pNew->ville , p_origine->ville);    //copie du nom de ville : strcpy(destination de la copie, source);
        pNew->AVL_RouteID = copie_AVL_RouteID(p_origine->AVL_RouteID);
        if(pNew->AVL_RouteID == NULL){
            printf("Erreur 1001 : pointeur NULL.\n");
            exit(1001);
        }
        pNew->nb_apparition_ville_depart_trajet = p_origine->nb_apparition_ville_depart_trajet;
        pNew->nb_apparition_ville_arrivee_etape = p_origine->nb_apparition_ville_arrivee_etape;
        pNew->nb_apparition_par_trajet = p_origine->nb_apparition_par_trajet;
        pNew->doublons_ville_par_trajet = p_origine->doublons_ville_par_trajet;
        pNew->equilibre = p_origine->equilibre;
        pNew->pL = copie_AVL(p_origine->pL);
        pNew->pR = copie_AVL(p_origine->pR);
        return pNew;
    }
}


//10 VILLES LES + TRAVERSEES 
       
Tree * transforme_AVLalphab_en_AVLnum(Tree* p_alphabetique, Tree* p_numerique, int* h){
   
    if (h == NULL){
        printf("Erreur 1000 : pointeur NULL.\n");
        exit(1000);
    }
    //PROCESS
    if(p_alphabetique == NULL){
        //do nothing
    }
    else{
        //copie du noeud p_alphabetique sans ses enfants
        Tree * pNew = createTree("", 0); //0 est une valeau arbitraire ici, si je pouvais, je ne mettrais pas de Route_ID.
        destroyAVL_RouteID(pNew->AVL_RouteID); // on supprime l'AVL_RouteID créé inutilement
        if(pNew == NULL){
            printf("Erreur 1010 : pointeur NULL.\n");
            exit(1010);
        }
        strcpy(pNew->ville , p_alphabetique->ville);    //copie du nom de ville : strcpy(destination de la copie, source);

        pNew->nb_apparition_ville_depart_trajet = p_alphabetique->nb_apparition_ville_depart_trajet;
        pNew->nb_apparition_ville_arrivee_etape = p_alphabetique->nb_apparition_ville_arrivee_etape;
        pNew->nb_apparition_par_trajet = p_alphabetique->nb_apparition_par_trajet;
        pNew->doublons_ville_par_trajet = p_alphabetique->doublons_ville_par_trajet;
        pNew->AVL_RouteID = copie_AVL_RouteID(p_alphabetique->AVL_RouteID);
        pNew->equilibre = 0;
        pNew->pL = NULL;
        pNew->pR = NULL;

        //insertion du noeud copié dans p_numerique (l'AVL trié par nb d'apparition de ville)
        p_numerique = insertAVL_numerique(p_numerique, pNew, h);

        //Récursivité fonction
        p_numerique = transforme_AVLalphab_en_AVLnum(p_alphabetique->pL, p_numerique, h);
        p_numerique = transforme_AVLalphab_en_AVLnum(p_alphabetique->pR, p_numerique, h);
    }
    //Cas où p_numerique est NULL au départ
    return p_numerique;
}

Tree * DIX_Villes_plus_traversees(Tree* AVL_alphabetique){
    if(AVL_alphabetique == NULL){
        printf("Erreur 1050 : pointeur NULL.\n");
        exit(1050);
    }
    /*
    //copie de AVL_alphabetique
    printf("\n             Avant copie AVL alphabetique\n");
    sleep(1);
    Tree* copie_AVL_alphabetique = NULL;
    copie_AVL_alphabetique = copie_AVL(AVL_alphabetique);
    printf("             Après copie AVL alphabetique\n");
    sleep(1);
    if(copie_AVL_alphabetique == NULL){
        printf("Erreur 1051 : pointeur NULL.\n");
        exit(1051);
    }
    */

    //Création de AVL_numerique (à partir de copie_AVL_alphabetique)
    Tree* AVL_numerique = NULL;
    int* h = malloc(sizeof(int));
    /*AVL_numerique = transforme_AVLalphab_en_AVLnum(copie_AVL_alphabetique, AVL_numerique , h);*/
    AVL_numerique = transforme_AVLalphab_en_AVLnum(AVL_alphabetique, AVL_numerique , h);
    free(h);
    if(AVL_numerique == NULL){
        printf("Erreur 1060 : pointeur NULL.\n");
        exit(1060);
    }
    /*
    destroyAVL(copie_AVL_alphabetique);
    printf("             Mémoire de copie_AVL_alphabetique LIBERE\n\n");
    */
    return AVL_numerique;
}


//----------------- Main -----------------
int main() {
    //printf("PROCESS.\n");

    // AVL trié par ordre alphabétique de nom de ville
    Tree* AVL_alphabetique = NULL;
    int* h = malloc(sizeof(int));
    char villeA[SIZE];
    char villeB[SIZE];
    int Route_ID;
    int etape;
    // Récupération des données de data.csv
    /*printf("             CREATION AVL_alphabetique\n");*/
    while (scanf("%d;%d;%99[^;];%99[^\n]\n", &Route_ID, &etape, villeA, villeB) == 4){
        AVL_alphabetique = selection_insertAVL_alphabetique(AVL_alphabetique, villeA, villeB, Route_ID, etape, h);
    }
    free(h);
    /*
    printf("             FIN CREATION AVL_alphabetique\n");
    printf("             AFFICHAGE Infixe AVL_alphabetique\n");
    sleep(0.5);
    displayInfixe(AVL_alphabetique);
    printf("             FIN AFFICHAGE Infixe AVL_alphabetique\n");
    sleep(0.2);
    */

    Tree* AVL_numerique = DIX_Villes_plus_traversees(AVL_alphabetique);
    /*
    printf("             AFFICHAGE Infixe AVL_numerique\n");
    printf("             Tri par nombre d'apparitions décroissant de villes\n\n");
    displayInfixeInverse(AVL_numerique);
    printf("             FIN AFFICHAGE Infixe AVL_numerique\n");
    */


    //printf("AVANT n_villes_plus_parcourues\n");
    NODE* pNode = NULL;
    int n = 10;                                                 //on veut les 10 villes les plus parcourues
    pNode = n_villes_plus_parcourues(AVL_numerique, &n, pNode);
    if(pNode == NULL){
        printf("ERREUR 2500 : pointeur NULL.\n");
        exit(2500);
    }
    displayList(pNode);

    destroyAVL(AVL_alphabetique);
    // printf("Mémoire de AVL_alphabetique LIBEREE\n");
    destroyAVL(AVL_numerique);
    // printf("Mémoire de AVL_numerique LIBEREE\n");
    destroyNode(pNode);
    //printf("Mémoire de pNode LIBEREE\n");
    return 0;
}
