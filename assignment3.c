#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char firstName[50];
    char lastName[50];
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char* firstName, char* lastName);
Node* addNode(Node* root, char* firstName, char* lastName);
Node* deleteNode(Node* root, char* firstName, char* lastName);
Node* searchNode(Node* root, char* firstName, char* lastName);
void preOrder(Node* root, FILE* outputFile);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "w");

    if (!inputFile || !outputFile) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    Node* root = NULL;
    char firstName[50], lastName[50];
    int operation;

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
    if (sscanf(line, "%s %s %d", firstName, lastName, &operation) == 3) {
        switch (operation) {
            case 1:
                root = addNode(root, firstName, lastName);
                break;
            case 2:
                if (searchNode(root, firstName, lastName)) {
                    root = deleteNode(root, firstName, lastName);
                }
                break;
            case 3:
                if (searchNode(root, firstName, lastName)) {
                    fprintf(outputFile, "Found\n");
                } else {
                    fprintf(outputFile, "Not Found\n");
                }
                break;
            default:
                printf("Error: Invalid operation\n");
                return 1;
        }
    } else if (sscanf(line, "%d", &operation) == 1 && operation == 4) {
        preOrder(root, outputFile);
    } else {
        printf("Error: Invalid input\n");
        return 1;
    }
}

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

// Implement the createNode, addNode, deleteNode, searchNode, and preOrder functions here

Node* createNode(char* firstName, char* lastName) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->firstName, firstName);
    strcpy(newNode->lastName, lastName);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* addNode(Node* root, char* firstName, char* lastName) {
    if (root == NULL) {
        return createNode(firstName, lastName);
    }

    if (strcmp(lastName, root->lastName) < 0) {
        root->left = addNode(root->left, firstName, lastName);
    } else if (strcmp(lastName, root->lastName) > 0) {
        root->right = addNode(root->right, firstName, lastName);
    } else {
        if (strcmp(firstName, root->firstName) < 0) {
            root->left = addNode(root->left, firstName, lastName);
        } else {
            root->right = addNode(root->right, firstName, lastName);
        }
    }

    return root;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, char* firstName, char* lastName) {
    if (root == NULL) return root;
    if (strcmp(lastName, root->lastName) < 0)
        root->left = deleteNode(root->left, firstName, lastName);
    else if (strcmp(lastName, root->lastName) > 0)
        root->right = deleteNode(root->right, firstName, lastName);
    else {
        if (strcmp(firstName, root->firstName) < 0)
            root->left = deleteNode(root->left, firstName, lastName);
        else if (strcmp(firstName, root->firstName) > 0)
            root->right = deleteNode(root->right, firstName, lastName);
        else {
            if (root->left == NULL) {
                Node* temp = root->right;
                free(root);
                return temp;
            } else if (root->right == NULL) {
                Node* temp = root->left;
                free(root);
                return temp;
            }
            Node* temp = minValueNode(root->right);
            strcpy(root->firstName, temp->firstName);
            strcpy(root->lastName, temp->lastName);
            root->right = deleteNode(root->right, temp->firstName, temp->lastName);
        }
    }
    return root;
}

Node* searchNode(Node* root, char* firstName, char* lastName) {
    if (root == NULL || (strcmp(firstName, root->firstName) == 0 && strcmp(lastName, root->lastName) == 0))
        return root;
    if (strcmp(lastName, root->lastName) < 0)
        return searchNode(root->left, firstName, lastName);
    if (strcmp(lastName, root->lastName) > 0)
        return searchNode(root->right, firstName, lastName);
    if (strcmp(firstName, root->firstName) < 0)
        return searchNode(root->left, firstName, lastName);
    return searchNode(root->right, firstName, lastName);
}

void preOrder(Node* root, FILE* outputFile) {
    if (root != NULL) {
        preOrder(root->left, outputFile);
        fprintf(outputFile, "%s %s\n", root->firstName, root->lastName);
        preOrder(root->right, outputFile);
    }
}