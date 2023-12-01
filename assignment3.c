#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char firstName[50];
    char lastName[50];
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char* firstName, char* lastName) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->firstName, firstName);
    strcpy(newNode->lastName, lastName);
    newNode->left = newNode->right = NULL;
    return newNode;
}
Node* addNode(Node* root, char* firstName, char* lastName) {
    if (root == NULL) {
        return createNode(firstName, lastName);
    }

    int cmp = strcmp(firstName, root->firstName);
    if (cmp == 0) {
        cmp = strcmp(lastName, root->lastName);
        if (cmp == 0) {
            // Duplicate node, do not add it
            return root;
        }
    }

    if (cmp < 0) {
        root->left = addNode(root->left, firstName, lastName);
    } else {
        root->right = addNode(root->right, firstName, lastName);
    }

    return root;
}


Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}
Node* deleteNode(Node* root, char* firstName, char* lastName, FILE* output) {
    if (root == NULL) {
        fprintf(output, "Not Found\n");
        return root; // Return root without adding a node
    }

    int cmp = strcmp(firstName, root->firstName);
    if (cmp == 0) {
        cmp = strcmp(lastName, root->lastName);
    }

    if (cmp < 0) {
        root->left = deleteNode(root->left, firstName, lastName, output);
    } else if (cmp > 0) {
        root->right = deleteNode(root->right, firstName, lastName, output);
    } else {
        fprintf(output, "Found\n");
        // Node with only one child or no child
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor
        Node* temp = minValueNode(root->right);

        // Copy the inorder successor's content to this node
        strcpy(root->firstName, temp->firstName);
        strcpy(root->lastName, temp->lastName);

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->firstName, temp->lastName, output);
    }

    return root;
}


int searchNode(Node* root, char* firstName, char* lastName, FILE* output) {
    if (root == NULL) {
        fprintf(output, "Not Found\n");
        return 0;
    }

    int cmp = strcmp(firstName, root->firstName);
    if (cmp == 0) {
        cmp = strcmp(lastName, root->lastName);
    }

    if (cmp < 0) {
        return searchNode(root->left, firstName, lastName, output);
    } else if (cmp > 0) {
        return searchNode(root->right, firstName, lastName, output);
    } else {
        fprintf(output, "Found\n");
        return 1;
    }
}

void preOrderTraversal(Node* root, FILE* output) {
    if (root != NULL) {
        fprintf(output, "%s %s\n", root->firstName, root->lastName);
        preOrderTraversal(root->left, output);
        preOrderTraversal(root->right, output);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Unable to open input file\n");
        return 1;
    }

    FILE* outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        printf("Error: Unable to open output file\n");
        return 1;
    }

    Node* root = NULL;
    char firstName[50];
    char lastName[50];
    int operation;

    while (fscanf(inputFile, "%s %s %d", firstName, lastName, &operation) != EOF) {
        switch (operation) {
            case 1:
                root = addNode(root, firstName, lastName);
                break;
            case 2:
                root = deleteNode(root, firstName, lastName, outputFile);
                break;
            case 3:
                searchNode(root, firstName, lastName, outputFile);
                break;
            case 4:
                preOrderTraversal(root, outputFile);
                fprintf(outputFile, "\n");
                break;
            default:
                printf("Error: Invalid operation\n");
                return 1;
        }
    }


    fclose(inputFile);
    fclose(outputFile);

    return 0;
}