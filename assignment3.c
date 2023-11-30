#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char firstName[50];
    char lastName[50];
    struct Node *left, *right;
} Node;

Node* createNode(char *firstName, char *lastName) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    strcpy(newNode->firstName, firstName);
    strcpy(newNode->lastName, lastName);
    newNode->left = newNode->right = NULL;
    return newNode;
}

int compareNames(char *firstName1, char *lastName1, char *firstName2, char *lastName2) {
    int firstCompare = strcmp(firstName1, firstName2);
    if (firstCompare == 0) {
        return strcmp(lastName1, lastName2);
    }
    return firstCompare;
}

Node* insertNode(Node *root, char *firstName, char *lastName) {
    if (root == NULL) return createNode(firstName, lastName);

    if (compareNames(firstName, lastName, root->firstName, root->lastName) < 0) {
        root->left = insertNode(root->left, firstName, lastName);
    } else {
        root->right = insertNode(root->right, firstName, lastName);
    }

    return root;
}

Node* findMinNode(Node *node) {
    Node *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* deleteNode(Node *root, char *firstName, char *lastName, int *found) {
    if (root == NULL) return root;

    int comp = compareNames(firstName, lastName, root->firstName, root->lastName);
    if (comp < 0) {
        root->left = deleteNode(root->left, firstName, lastName, found);
    } else if (comp > 0) {
        root->right = deleteNode(root->right, firstName, lastName, found);
    } else {
        *found = 1;

        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = findMinNode(root->right);
        strcpy(root->firstName, temp->firstName);
        strcpy(root->lastName, temp->lastName);
        root->right = deleteNode(root->right, temp->firstName, temp->lastName, found);
    }

    return root;
}

int searchNode(Node *root, char *firstName, char *lastName) {
    if (root == NULL) return 0;

    int comp = compareNames(firstName, lastName, root->firstName, root->lastName);
    if (comp == 0) {
        return 1;
    } else if (comp < 0) {
        return searchNode(root->left, firstName, lastName);
    } else {
        return searchNode(root->right, firstName, lastName);
    }
}

void preOrderTraversal(Node *root, FILE *output) {
    if (root == NULL) return;

    fprintf(output, "%s %s\n", root->firstName, root->lastName);
    preOrderTraversal(root->left, output);
    preOrderTraversal(root->right, output);
}

void freeTree(Node *root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <inputfile> <outputfile>\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];

    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (!input || !output) {
        perror("Error opening file");
        return 1;
    }

    Node *root = NULL;
    char firstName[50], lastName[50];
    int operation;

    while (fscanf(input, "%s %s %d", firstName, lastName, &operation) != EOF) {
        int found = 0;
        switch (operation) {
            case 1: // Add operation
                root = insertNode(root, firstName, lastName);
                break;
            case 2: // Delete operation
                root = deleteNode(root, firstName, lastName, &found);
                if (!found) {
                    root = insertNode(root, firstName, lastName);
                }
                break;
            case 3: // Search operation
                if (searchNode(root, firstName, lastName)) {
                    fprintf(output, "Found\n");
                } else {
                    fprintf(output, "Not Found\n");
                }
                break;
            case 4: // Pre-order traversal
                preOrderTraversal(root, output);
                break;
            default:
                fprintf(output, "Error\n");
                fclose(input);
                fclose(output);
                freeTree(root);
                return 1;
        }
    }

    fclose(input);
    fclose(output);
    freeTree(root);

    return 0;
}
