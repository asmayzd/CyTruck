#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVLNode {
    char ville[50];
    int trajetsTotal;
    int trajetsDepart;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

AVLNode *createNode(char ville[]) {
    AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
    strcpy(newNode->ville, ville);
    newNode->trajetsTotal = 1;
    newNode->trajetsDepart = 1;
    newNode->left = newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int height(AVLNode *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode *rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode *leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLNode *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

AVLNode *insert(AVLNode *root, char ville[], int isDepart) {
    if (root == NULL)
        return createNode(ville);

int compareResult = strcmp(ville, root->ville);


    if (compareResult == 0) {
        // La ville est déjà présente, mise à jour des totaux
        root->trajetsTotal++;
        if (isDepart)
            root->trajetsDepart++;
    } else if (compareResult < 0) {
        root->left = insert(root->left, ville, isDepart);
    } else {
        root->right = insert(root->right, ville, isDepart);
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // Rééquilibrage de l'arbre
    if (balance > 1 && strcmp(ville, root->left->ville) < 0)
        return rightRotate(root);

    if (balance < -1 && strcmp(ville, root->right->ville) > 0)
        return leftRotate(root);

    if (balance > 1 && strcmp(ville, root->left->ville) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && strcmp(ville, root->right->ville) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}



void inOrderTraversal(FILE *outputFile, AVLNode *root) {
    if (root != NULL) {
        inOrderTraversal(outputFile, root->right); // Inverser l'ordre ici
        fprintf(outputFile, "%s - Trajets Total: %d, Trajets Départ: %d\n", root->ville, root->trajetsTotal, root->trajetsDepart);
        inOrderTraversal(outputFile, root->left); // Inverser l'ordre ici
    }
}


void readTrajetsFromFile(AVLNode **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[1000]; // Assurez-vous que la taille est suffisante pour contenir une ligne complète
    char villeA[50];
    char villeB[50];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Utilisez sscanf pour extraire les villes du format "villeA;villeB"
        if (sscanf(line, "%49[^;];%49[^;\n]", villeA, villeB) == 2) {
            *root = insert(*root, villeB, 1); // Ville B en tant que départ (modification ici)
            *root = insert(*root, villeA, 0); // Ville A en tant qu'arrivée (modification ici)
        } else {
            fprintf(stderr, "Erreur de format dans la ligne : %s\n", line);
        }
    }

    // Fermer le fichier après la lecture
    if (fclose(file) != 0) {
        perror("Erreur lors de la fermeture du fichier");
        exit(EXIT_FAILURE);
    }
}



typedef struct {
    char ville[50];
    int trajetsTotal;
    int trajetsDepart;
} Top10Info;

// Fonction pour parcourir l'arbre en infixe et mettre à jour les informations du top 10
void updateTop10(AVLNode *root, Top10Info top10[], int *count) {
    if (root != NULL) {
        updateTop10(root->left, top10, count);

        // Mettre à jour le top 10 avec les informations actuelles
        int index = *count;
        if (index < 10) {
            strcpy(top10[index].ville, root->ville);
            top10[index].trajetsTotal = root->trajetsTotal;
            top10[index].trajetsDepart = root->trajetsDepart;
            (*count)++;
        } else {
            // Trouver la position où insérer si le tableau est plein
            for (int i = 0; i < 10; i++) {
                if (root->trajetsTotal > top10[i].trajetsTotal) {
                    // Décaler les éléments pour faire de la place pour le nouveau
                    for (int j = 9; j > i; j--) {
                        strcpy(top10[j].ville, top10[j - 1].ville);
                        top10[j].trajetsTotal = top10[j - 1].trajetsTotal;
                        top10[j].trajetsDepart = top10[j - 1].trajetsDepart;
                    }
                    // Insérer le nouveau
                    strcpy(top10[i].ville, root->ville);
                    top10[i].trajetsTotal = root->trajetsTotal;
                    top10[i].trajetsDepart = root->trajetsDepart;
                    break;
                }
            }
        }

        updateTop10(root->right, top10, count);
    }
}

// Fonction pour afficher le top 10 dans le fichier
void writeTop10ToFile(FILE *outputFile, Top10Info top10[]) {
    for (int i = 0; i < 10; i++) {
        fprintf(outputFile, "%s;%d;%d\n", top10[i].ville, top10[i].trajetsTotal, top10[i].trajetsDepart);
    }
}




// Fonction pour libérer la mémoire allouée pour les nœuds de l'AVL
void freeAVL(AVLNode *root) {
    if (root != NULL) {
        freeAVL(root->left);
        freeAVL(root->right);
        free(root);
    }
}

int main() {
    AVLNode *root = NULL;

    // Spécifiez le chemin complet du fichier contenant les trajets
    const char *filename = "./temp/temp_t.csv";

    // Lecture des trajets à partir du fichier
    readTrajetsFromFile(&root, filename);

    // Vérification si l'arbre est vide
    if (root == NULL) {
        fprintf(stderr, "Erreur : Aucune donnée n'a été lue depuis le fichier ou le fichier est vide.\n");
        return EXIT_FAILURE;
    }

    // Ouvrir le fichier de sortie en écriture
    FILE *outputFile = fopen("./temp/histogram_datat.csv", "w");
    if (outputFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return EXIT_FAILURE;
    }

    // Initialiser le tableau pour le top 10
    Top10Info top10[10];
    for (int i = 0; i < 10; i++) {
        top10[i].trajetsTotal = 0;
    }

    // Mettre à jour les informations du top 10
    int count = 0;
    updateTop10(root, top10, &count);

    // Écrire le top 10 dans le fichier
    writeTop10ToFile(outputFile, top10);

    // Fermer le fichier de sortie
    if (fclose(outputFile) != 0) {
        perror("Erreur lors de la fermeture du fichier de sortie");
        return EXIT_FAILURE;
    }

    // Libération de la mémoire allouée pour les nœuds de l'AVL
    freeAVL(root);

    return EXIT_SUCCESS;
}
