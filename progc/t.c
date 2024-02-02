#include "t.h"

// La fonction ci-dessou permet de créer un nouveau noeud.
NODE * createNode (char word[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){ 
    NODE* pNew = malloc(sizeof(NODE));
    if (pNew == NULL){
        exit(1);
    }
    strcpy(pNew->ville , word);    //copie du nom de ville 
    pNew->nb_apparition_par_trajet = NB_apparition_par_trajet;
    pNew->nb_apparition_ville_depart_trajet = NB_apparition_ville_depart_trajet;
    pNew->pNext = NULL;
    return pNew;
}

// La fonction ci-dessou permet d'ajouter un noeud en début de liste. 
NODE* addStart (NODE * pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){  
    NODE * pNew = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    if (pNew == NULL){
        exit(2); //pour strcmp()
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
//La fonction ci-dessou permet d'ajouter un noeud en fin de liste. 
NODE * addEnd(NODE * pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){  
    NODE* pNew = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    NODE* pList = pHead;
    if (pNew == NULL){
        exit(3);
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
  // La procédure ci-dessou permet d'ajouter le noeud entre pList et pList->pNext
void addBetweenNodes(NODE* pList, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){      
    if (pList == NULL){
        exit(1);
    }
    NODE* pNew = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    if (pNew == NULL){
        exit(2);
    }
    pNew->pNext = pList->pNext;
    pList->pNext = pNew;
}
 // La fonction ci-dessou permet d'inserer un noeud dans la liste dans l'ordre alphabétique  
NODE* insertNode(NODE* pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet){ 
    
    if (pHead == NULL){                                               //si le noeud n'existe pas on le crée
        pHead = createNode(nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    }
   
    else if (strcmp(nom, pHead->ville) < 0) {                        //strcmp permet de comparer les noms des villes 
        pHead = addStart(pHead, nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
    }
    else{
        NODE* pList = pHead;
        NODE* pPrevious = NULL;
        while(strcmp(nom, pList->ville) > 0 && pList->pNext != NULL){
            pPrevious = pList;
            pList = pList->pNext;
        }
       
        if (pList->pNext == NULL && strcmp(nom, pList->ville) > 0){
            pList = addEnd(pList, nom, NB_apparition_par_trajet, NB_apparition_ville_depart_trajet);
        }
        
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

// Cette procédure permet d'afficher la liste 
void displayList(NODE* pHead){  //afficher la liste
    if (pHead == NULL){
        exit(3);
    }
    
    while(pHead != NULL){
        printf("%s;%d;%d\n", pHead->ville, pHead->nb_apparition_par_trajet, pHead->nb_apparition_ville_depart_trajet);
        pHead= pHead->pNext;
    }
}

// La procédure ci-dessou permet de supprimer un chainon
void destroyNode(NODE* pNode) {  
    if (pNode != NULL) {
        destroyNode(pNode->pNext);
        free(pNode);
       
    }
}

// La fonction ci-dessou crée un noeud pour les différentes étapes
Node_RouteID* create_Node_RouteID(int RouteID){
    Node_RouteID* pNew = malloc(sizeof(Node_RouteID));
    if(pNew == NULL){
        printf("Erreur : pointeur NULL.\n");
        exit(1);
    }
    pNew->Route_ID = RouteID;
    pNew->equilibre = 0;
    pNew->pL = NULL;
    pNew->pR = NULL;
    return pNew;
}

//On créer l'arbre qui contient les noeuds avec toute les informations 
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


// La fonction permet d'effectuer les rotations gauches en fonction de RouteId
Node_RouteID* RotationGauche_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pR == NULL){
        exit(3);
    }
    Node_RouteID* pivot = p_RouteID->pR;

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
            printf("Cas non pris en charge. \n");
            exit(1);
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
            printf("Cas non pris en charge. \n");
            exit(2);
        }
    }

    else{
        printf("Cas non pris en charge. \n");
        exit(3);
    }

    p_RouteID->pR = pivot->pL;
    pivot->pL = p_RouteID;
    if (pivot == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    return pivot;
}
// La fonction permet d'effectuer les rotations droites en fonction de RouteId
Node_RouteID* RotationDroite_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pL == NULL){
        exit(2);
    }
    Node_RouteID* pivot = p_RouteID->pL;

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
            printf("Cas non pris en charge. \n");
            exit(3);
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
            printf("Cas non pris en charge. \n");
            exit(1);
        }
    }

    else{
        printf("Cas non pris en charge. \n");
        exit(2);
    }

    p_RouteID->pL = pivot->pR;
    pivot->pR = p_RouteID;
    if (pivot == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(3);
    }

    return pivot;
}

// La fonction ci-dessou permet d'effectuer les rotations gauches de l'arbre qui contient les noeuds avec toute les informations.
Tree* RotationGauche(Tree* p){
    if (p==NULL || p->pR == NULL){
        exit(1);
    }
    Tree* pivot = p->pR;

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
            printf("Cas non pris en charge. \n");
            exit(2);
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
            printf("Cas non pris en charge. \n");
            exit(3);
        }
    }

    else{
        printf("Cas non pris en charge. \n");
        exit(1);
    }


    p->pR = pivot->pL;
    pivot->pL = p;
    if (pivot == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }

    return pivot;
}

// La fonction ci-dessou permet d'effectuer les rotations droites de l'arbre qui contient les noeuds avec toute les informations.
Tree* RotationDroite(Tree* p){
    if (p==NULL || p->pL == NULL){
        exit(3);
    }
    Tree* pivot = p->pL;

    if(p->equilibre == -2){
        //Rotation simple 1
        if(pivot->equilibre == -1){
            pivot->equilibre = 0;
            p->equilibre = 0;
        }
     
        else if(pivot->equilibre == 0){
            pivot->equilibre = 1;
            p->equilibre = -1;
        }
        else if(pivot->equilibre == -2){
            pivot->equilibre = 0;
            p->equilibre = 1;
        }
        else{
            printf("Cas non pris en charge. \n");
            exit(1);
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
            printf("Cas non pris en charge. \n");
            exit(3);
        }
    }

    else{
        printf("Cas non pris en charge. \n");
        exit(1);
    }

    p->pL = pivot->pR;
    pivot->pR = p;
    if (pivot == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }

    return pivot;
}


//DOUBLES ROTATIONS 

// La fonction permet d'effectuer les doubles rotations gauches spécifiques pour Route_ID
Node_RouteID* DoubleRotationGauche_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pR==NULL || p_RouteID->equilibre!=2 || p_RouteID->pR->equilibre!=-1){
        printf("Cas non pris en charge: pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(3);
    }

    p_RouteID->pR = RotationDroite_RouteID(p_RouteID->pR);
    if (p_RouteID->pR == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }
    p_RouteID = RotationGauche_RouteID(p_RouteID);
    if (p_RouteID == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }
    return p_RouteID;
}

// La fonction permet d'effectuer les doubles rotations droites spécifiques pour Route_ID
Node_RouteID* DoubleRotationDroite_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID==NULL || p_RouteID->pL==NULL || p_RouteID->equilibre!=-2 || p_RouteID->pL->equilibre!=1){
        printf("Cas non pris en charge: pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(3);
    }

    p_RouteID->pL = RotationGauche_RouteID(p_RouteID->pL);
    if (p_RouteID->pL == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    p_RouteID = RotationDroite_RouteID(p_RouteID);
    if (p_RouteID == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }
    return p_RouteID;
}


// La fonction ci-dessou permet d'effectuer les doubles rotations gauches de l'arbre qui contient les noeuds avec toute les informations.
Tree* DoubleRotationGauche(Tree* p){
    if (p==NULL || p->pR==NULL || p->equilibre!=2 || p->pR->equilibre!=-1){
        printf("Cas non pris en charge: pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(3);
    }

    p->pR = RotationDroite(p->pR);
    if (p->pR == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }
    p = RotationGauche(p);
    if (p == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }
    return p;
}
// La fonction ci-dessou permet d'effectuer les doubles rotations gauches de l'arbre qui contient les noeuds avec toute les informations.
Tree* DoubleRotationDroite(Tree* p){                                                                
    if (p==NULL || p->pL==NULL || p->equilibre!=-2 || p->pL->equilibre!=1){
        printf("Cas non pris en charge: pointeurs NULL ou facteurs d'équilibre incorrectes.\n");
        exit(3);
    }

    p->pL = RotationGauche(p->pL);
    if (p->pL == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    p = RotationDroite(p);
    if (p == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }
    return p;
}




//La fonction ci-dessou permet l'équilibrage de l'AVL spécifique pour Route_ID
Node_RouteID* equilibrerAVL_RouteID(Node_RouteID* p_RouteID){
    if (p_RouteID == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(3);
    }

    if (p_RouteID->equilibre == 2){
        if (p_RouteID->pR == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(1);
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
            printf("Cas non pris en charge.\n");
            exit(2);
        }
    }
    else if (p_RouteID->equilibre == -2) {
        if (p_RouteID->pL == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(3);
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
            printf("Cas non pris en charge.\n");
            exit(1);
        }
    }
   
    else{
        printf("Cas non pris en charge: racine de l'arbre >2 ou <-2 .\n");
        exit(2);
    }
    if (p_RouteID == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(3);
    }

    return p_RouteID;
}



//La fonction ci-dessou permet l'équilibrage de l'AVL qui contient les noeuds avec toute les informations.
Tree* equilibrerAVL(Tree* p){
    if (p == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    if (p->equilibre == 2){
        if (p->pR == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(2);
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
            printf("Cas non pris en charge.\n");
            exit(3);
        }
    }
    else if (p->equilibre == -2) {
        if (p->pL == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(1);
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
            printf("Cas non pris en charge.\n");
            exit(2);
        }
    }
    
    else{
        printf("Cas non pris en charge: racine de l'arbre >2 ou <-2 .\n");
        exit(3);
    }
    if (p == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    return p;
}



//INSERTION AVL

//Cette fonction permet l'insertion AVL spécifique pour Route_ID

Node_RouteID* insertAVL_RouteID (Tree* p, Node_RouteID* p_RouteID, int v, int* h){  //p_RouteID = p->AVL_RouteID

    if (p == NULL || h == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }

    if (p_RouteID == NULL){
        p_RouteID = create_Node_RouteID(v);
        *h = 1;
        if (p_RouteID == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(3);
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


///Cette fonction permet l'insertion les noeud dans l'AVL en fonction du nombre d'apparition des villes 
Tree* insertAVL_numerique (Tree* p, Tree* p_copie, int* h)
    if (h == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    //PROCESS
    if (p == NULL){
        p = p_copie;
        *h = 1;
        if (p == NULL){
            printf("Erreur 51 : pointeur NULL.\n");
            exit(2);
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
        exit(3);
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
   
    return p;
}

// La fonction ci-dessous permet de trier par ordre alphabétique le nom des villes dans l'AVL. 
Tree* insertAVL_alphabetique (Tree* p, int Route_ID, char nom_ville[], int ville_Arrivee, int* h){
   
    if (h == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }

    if (p == NULL){
        p = createTree(nom_ville, Route_ID);
      
        if (ville_Arrivee == 1){
            p->nb_apparition_ville_depart_trajet = p->nb_apparition_ville_depart_trajet + 1 ;
            p->nb_apparition_par_trajet = p->nb_apparition_par_trajet + 1 ;
        }
   
        else if (ville_Arrivee == 2){
            p->nb_apparition_ville_arrivee_etape = p->nb_apparition_ville_arrivee_etape + 1 ;
            p->nb_apparition_par_trajet = p->nb_apparition_par_trajet + 1 ;
        }
        else{
            printf("Erreur: on a ni une ville de départ, ni une ville d'arrivée.\n");
            exit(2);
        }
        *h = 1;
        if (p == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(3);
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
            printf("Erreur: on a ni une ville de départ, ni une ville d'arrivée.\n");
            exit(1);
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
    return p;
}
//Cette fonction permet l'insertion dans l'AVL de toute les villes d'un trajet 
Tree* selection_insertAVL_alphabetique (Tree* p, char villeA[], char villeB[], int Route_ID, int etape, int* h){ 
   
    if (h == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(1);
    }
    if (Route_ID <= 0 || etape <= 0){
        printf("Erreur: Identifiant de trajet <= 0 ou numéro d'étape de trajet <= 0.\n");
        exit(2);
    }
  
    int ville_Arrivee = 0;
    if (etape == 1){
        ville_Arrivee = 1;
        p = insertAVL_alphabetique(p, Route_ID, villeA, ville_Arrivee, h);
    }
    ville_Arrivee = 2;
    p = insertAVL_alphabetique(p, Route_ID, villeB, ville_Arrivee, h);

    return p;
}


//Suppression AVL

//Cette procédure permet de supprimer l'AVL Route_ID
void destroyAVL_RouteID(Node_RouteID* p_RouteID) {
    if (p_RouteID != NULL) {
        destroyAVL_RouteID(p_RouteID->pL);
        destroyAVL_RouteID(p_RouteID->pR);
        free(p_RouteID);
    }
}

//Cette procédure permet de supprimer l'AVL général 
void destroyAVL(Tree* p) {
    if (p != NULL) {
        destroyAVL(p->pL);
        destroyAVL(p->pR);
        destroyAVL_RouteID(p->AVL_RouteID);
        free(p);
    }
}


//AFFICHAGE : préfixe, infixe 

//Cette procédure permet l'affichage de l'AVL Route_ID en prefixe
void displayPrefixe_RouteID(Node_RouteID* p_RouteID){
    if(p_RouteID !=NULL){
        printf("                        RouteID = [%d],   equilibre = %d\n",p_RouteID->Route_ID, p_RouteID->equilibre);
        displayPrefixe_RouteID(p_RouteID->pL);
        displayPrefixe_RouteID(p_RouteID->pR);
    }
}
//Cette procédure permet l'affichage de l'AVL Route_ID en infixe
void displayInfixe_RouteID(Node_RouteID* p_RouteID){
    if(p_RouteID !=NULL){
        displayInfixe_RouteID(p_RouteID->pL);
        printf("                        RouteID = [%d],   equilibre = %d\n",p_RouteID->Route_ID, p_RouteID->equilibre);
        displayInfixe_RouteID(p_RouteID->pR);
    }
}


//Cette procédure permet l'affichage de l'AVL général en prefixe
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

//Cette procédure permet l'affichage de l'AVL général en infixe
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
//Cette procédure permet l'affichage de l'AVL général en infixe inverse
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
// Cette fonction retourne les n villes les plus traversées.
NODE* n_villes_plus_parcourues(Tree* p, int* n, NODE* pNode){ 
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


Node_RouteID * copie_AVL_RouteID (Node_RouteID* p_origine_RouteID){
    if(p_origine_RouteID == NULL){
        return p_origine_RouteID;
    }
    else{
        Node_RouteID * pNew = create_Node_RouteID(-1);
        if(pNew == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(2);
        }
        pNew->Route_ID = p_origine_RouteID->Route_ID;
        if (pNew->Route_ID == -1){
            printf("Erreur: Echec copie de Route_ID.\n");
            exit(3);
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
        Tree * pNew = createTree("", 0); 
        destroyAVL_RouteID(pNew->AVL_RouteID); 
        if(pNew == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(1);
        }
        strcpy(pNew->ville , p_origine->ville);    //copie du nom de ville : strcpy(destination de la copie, source);
        pNew->AVL_RouteID = copie_AVL_RouteID(p_origine->AVL_RouteID);
        if(pNew->AVL_RouteID == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(2);
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
        printf("Erreur: pointeur NULL.\n");
        exit(3);
    }
  
    if(p_alphabetique == NULL){
        //do nothing
    }
    else{
        //copie du noeud p_alphabetique sans ses enfants
        Tree * pNew = createTree("", 0); 
        destroyAVL_RouteID(pNew->AVL_RouteID); // on supprime l'AVL_RouteID créé inutilement
        if(pNew == NULL){
            printf("Erreur: pointeur NULL.\n");
            exit(1);
        }
        strcpy(pNew->ville , p_alphabetique->ville);    //copie du nom de ville 

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

        
        p_numerique = transforme_AVLalphab_en_AVLnum(p_alphabetique->pL, p_numerique, h);
        p_numerique = transforme_AVLalphab_en_AVLnum(p_alphabetique->pR, p_numerique, h);
    }
    
    return p_numerique;
}

Tree * DIX_Villes_plus_traversees(Tree* AVL_alphabetique){
    if(AVL_alphabetique == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(2);
    }
  

    //Création de AVL_numerique (à partir de copie_AVL_alphabetique)
    Tree* AVL_numerique = NULL;
    int* h = malloc(sizeof(int));
    /*AVL_numerique = transforme_AVLalphab_en_AVLnum(copie_AVL_alphabetique, AVL_numerique , h);*/
    AVL_numerique = transforme_AVLalphab_en_AVLnum(AVL_alphabetique, AVL_numerique , h);
    free(h);
    if(AVL_numerique == NULL){
        printf("Erreur: pointeur NULL.\n");
        exit(3);
    }
    
    return AVL_numerique;
}


// Main 
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
    while (scanf("%d;%d;%99[^;];%99[^\n]\n", &Route_ID, &etape, villeA, villeB) == 4){
        AVL_alphabetique = selection_insertAVL_alphabetique(AVL_alphabetique, villeA, villeB, Route_ID, etape, h);
    }
    free(h);
  

    //printf("AVANT n_villes_plus_parcourues\n");
    NODE* pNode = NULL;
    int n = 10;                                                 //on veut les 10 villes les plus parcourues
    pNode = n_villes_plus_parcourues(AVL_numerique, &n, pNode);
    if(pNode == NULL){
        printf("ERREUR : pointeur NULL.\n");
        exit(1);
    }
    displayList(pNode);

    destroyAVL(AVL_alphabetique);
  
    destroyAVL(AVL_numerique);
   
    destroyNode(pNode);
  
    return 0;
}
