#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define SIZE 100

typedef struct _node_RouteID{    //BUT : faire un AVL de tous trajets (Route_ID) dans lesquels la ville est présente
    int Route_ID;
    int equilibre;
    struct _node_RouteID* pL;
    struct _node_RouteID* pR;
} Node_RouteID;

typedef struct _tree {
    char ville[SIZE];
    Node_RouteID* AVL_RouteID;
    /*  AVL_RouteID : 
        On liste l'ensemble des trajets dans lesquelles la ville apparait.
        ATTENTION : si la ville apparait 2 fois dans le même trajet, il y a doublon, on fait alors :
                doublons_ville_par_trajet += 1;
                nb_apparition_par_trajet -=1;  (car on a un nb_apparition_par_trajet +=1 fait auparavant, il faut l'annuler)                
    */
    int nb_apparition_ville_depart_trajet;        //on compte les villes de départ de chaque trajet (chaque trajet a plusieurs étapes)
    int nb_apparition_ville_arrivee_etape;        //on compte les villes d'arrivée de chaque étape de trajet
    int nb_apparition_par_trajet;
    int doublons_ville_par_trajet;
    // nb_apparition_par_trajet = nb_apparition_ville_depart_trajet + nb_apparition_ville_arrivee_etape - les doublons de ville par trajet
    struct _tree* pL;
    struct _tree* pR;
    int equilibre;                                  //equilibre = hauteur sous-arbre droit - hauteur sous-arbre gauche
} Tree;

typedef struct _node{
    char ville[SIZE];
    int nb_apparition_par_trajet;
    int nb_apparition_ville_depart_trajet;
    struct _node* pNext;    //pointeur vers prochain chainon
} NODE;

NODE * createNode (char word[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet);
NODE* addStart (NODE * pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet);
NODE * addEnd(NODE * pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet);
void addBetweenNodes(NODE* pList, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet);
NODE* insertNode(NODE* pHead, char nom[], int NB_apparition_par_trajet, int NB_apparition_ville_depart_trajet);
void displayList(NODE* pHead);
void destroyNode(NODE* pNode);
Node_RouteID* create_Node_RouteID(int RouteID);
Tree* createTree(char nom[], int Route_ID);
Node_RouteID* RotationGauche_RouteID(Node_RouteID* p_RouteID);
Node_RouteID* RotationDroite_RouteID(Node_RouteID* p_RouteID);
Tree* RotationGauche(Tree* p);
Tree* RotationDroite(Tree* p);
Node_RouteID* DoubleRotationGauche_RouteID(Node_RouteID* p_RouteID);
Node_RouteID* DoubleRotationDroite_RouteID(Node_RouteID* p_RouteID);
Tree* DoubleRotationGauche(Tree* p);
Tree* DoubleRotationDroite(Tree* p);
Node_RouteID* equilibrerAVL_RouteID(Node_RouteID* p_RouteID);
Tree* equilibrerAVL(Tree* p);
Node_RouteID* insertAVL_RouteID (Tree* p, Node_RouteID* p_RouteID, int v, int* h);
Tree* insertAVL_numerique (Tree* p, Tree* p_copie, int* h);
Tree* insertAVL_alphabetique (Tree* p, int Route_ID, char nom_ville[], int ville_Arrivee, int* h);
Tree* selection_insertAVL_alphabetique (Tree* p, char villeA[], char villeB[], int Route_ID, int etape, int* h);
void destroyAVL_RouteID(Node_RouteID* p_RouteID);
void destroyAVL(Tree* p);
void displayPrefixe_RouteID(Node_RouteID* p_RouteID);
void displayInfixe_RouteID(Node_RouteID* p_RouteID);
void displayPrefixe(Tree* p);
void displayInfixe(Tree* p);
void displayInfixeInverse(Tree* p);
NODE* n_villes_plus_parcourues(Tree* p, int* n, NODE* pNode);
Node_RouteID * copie_AVL_RouteID (Node_RouteID* p_origine_RouteID);
Tree * copie_AVL (Tree* p_origine);
Tree * transforme_AVLalphab_en_AVLnum(Tree* p_alphabetique, Tree* p_numerique, int* h);
Tree * DIX_Villes_plus_traversees(Tree* AVL_alphabetique);
