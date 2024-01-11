#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVLNode {
    int step;
    int maxi;
    int moyenne;
    int mini;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
    int count;  // Add count member to AVLNode
} AVLNode;

// Function to calculate the height of a node
int height(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// Function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

void freeAVLTree(AVLNode *node) {
    if (node != NULL) {
        freeAVLTree(node->left);
        freeAVLTree(node->right);
        free(node);
    }
}


// Function to create a new AVL node
AVLNode* newNode(int step, int maxi, int moyenne, int mini) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    node->step = step;
    node->maxi = maxi;
    node->moyenne = moyenne;
    node->mini = mini;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // New node is initially at height 1
    return node;
}

// Function to right rotate a subtree rooted with y
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// Function to left rotate a subtree rooted with x
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// Get the balance factor of a node
int getBalance(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

AVLNode* insertOrUpdate(AVLNode* node, int step, int maxi, int moyenne, int mini) {
    // Perform standard BST insertion
    if (node == NULL) {
        AVLNode* new_node = newNode(step, maxi, moyenne, mini);
        new_node->count = 1;
        return new_node;
    }

    if (step < node->step) {
        node->left = insertOrUpdate(node->left, step, maxi, moyenne, mini);
    } else if (step > node->step) {
        node->right = insertOrUpdate(node->right, step, maxi, moyenne, mini);
    } else {
        // Step already exists, update values
        if (maxi > node->maxi) {
            node->maxi = maxi;
        }

        if (mini < node->mini) {
            node->mini = mini;
        }

        // Update moyenne based on the new value and the number of occurrences
        node->moyenne = (node->moyenne * node->count + moyenne) / (node->count + 1);
        node->count++;

        return node;
    }

    // Update height of current node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor to check if this node became unbalanced
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && step < node->left->step) {
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && step > node->right->step) {
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && step > node->left->step) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && step < node->right->step) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the balanced node
    return node;
}

// Function to write AVL tree nodes to an array in reverse order
int writeInReverseOrderToArray(AVLNode *node, AVLNode **array, int count){
    if (node && count < 50) {
        count = writeInReverseOrderToArray(node->right, array, count);

        // Add the current node to the array
        array[count] = node;
        count++;

        return writeInReverseOrderToArray(node->left, array, count);
    }

    return count;
}



// Function to write the AVL tree in reverse order based on "distance_maxi - distance_mini"
int writeInReverseOrder(AVLNode *node, FILE *file, int count) {
    if (node && count < 50) {
        count = writeInReverseOrder(node->right, file, count);

        fprintf(file, "%d;%d;%d;%d\n", node->step, node->mini, node->moyenne, node->maxi);
        count++;

        return writeInReverseOrder(node->left, file, count);
    }

    return count;
}

/// Function to compare nodes based on "distance_maxi - distance_mini" for qsort
int compareNodes(const void *a, const void *b) {
    const AVLNode *nodeA = (const AVLNode *)a;
    const AVLNode *nodeB = (const AVLNode *)b;

    // Sort in descending order based on "distance_maxi - distance_mini"
    int diffA = nodeA->maxi - nodeA->mini;
    int diffB = nodeB->maxi - nodeB->mini;

    return diffB - diffA;
}
// Function to write the results to the output file
void writeOutputFile(const char *filename, AVLNode *root) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Step;Min;Avg;Max\n");  // Header

    // Create an array of AVLNode
    AVLNode *nodesArray[50];
    int nodeCount = writeInReverseOrderToArray(root, nodesArray, 0);

    // Sort the array of nodes based on "distance_maxi - distance_mini"
    qsort(nodesArray, nodeCount, sizeof(AVLNode), compareNodes);

    // Write the top 50 nodes to the file
    for (int i = 0; i < 50 && i < nodeCount; i++) {
        if (fprintf(file, "%d;%d;%d;%d\n", nodesArray[i]->step, nodesArray[i]->mini, nodesArray[i]->moyenne, nodesArray[i]->maxi) < 0) {
            perror("Error writing to output file");
            fclose(file);
            freeAVLTree(root);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

// Function to read data from the input file and insert/update nodes in the AVL tree
AVLNode* processInputFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    AVLNode *root = NULL;
    int step, maxi, moyenne, mini;

    while (fscanf(file, "%d;%d", &step, &mini) == 2) {
        // Assuming initial values for maxi and moyenne are the same as mini
        maxi = mini;
        moyenne = mini;

        // Insert or update node in the AVL tree
        root = insertOrUpdate(root, step, maxi, moyenne, mini);
    }

    if (ferror(file)) {
        perror("Error reading from input file");
        fclose(file);
        freeAVLTree(root);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return root;
}


int main() {
    const char *inputFile = "temp/temp_s.csv";
    const char *outputFile = "temp/histogram_datas.csv";

    // Process input file and build AVL tree
    AVLNode *root = processInputFile(inputFile);

    // Write results to output file
    writeOutputFile(outputFile, root);

    // Free memory if necessary
    freeAVLTree(root);

    return 0;
}
