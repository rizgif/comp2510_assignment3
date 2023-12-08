#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bst_node_t
{
    char *key;
    char *value;
    struct bst_node_t *left, *right;
    int duplicate_count;
} bst_node_t;

bst_node_t *bst_create_node(const char *key, const char *value)
{
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->left = new_node->right = NULL;
    new_node->duplicate_count = 1;

    if (new_node->key == NULL || new_node->value == NULL)
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return NULL;
    }

    return new_node;
}

bst_node_t *bst_insert(bst_node_t *node, const char *key, const char *value)
{
    if (node == NULL)
    {
        return bst_create_node(key, value);
    }

    int cmp = strcmp(key, node->key);
    if (cmp < 0)
    {
        node->left = bst_insert(node->left, key, value);
    }
    else if (cmp > 0)
    {
        node->right = bst_insert(node->right, key, value);
    }
    else
    {
        node->duplicate_count++;
    }

    return node;
}

char *bst_search(bst_node_t *node, const char *key)
{
    if (node == NULL)
    {
        return NULL;
    }

    int cmp = strcmp(key, node->key);
    if (cmp < 0)
    {
        return bst_search(node->left, key);
    }
    else if (cmp > 0)
    {
        return bst_search(node->right, key);
    }
    else
    {
        return node->value;
    }
}

bst_node_t *bst_find_min(bst_node_t *node)
{
    while (node && node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

bst_node_t *bst_delete(bst_node_t *root, const char *key)
{
    if (root == NULL)
    {
        return NULL;
    }

    int cmp = strcmp(key, root->key);
    if (cmp < 0)
    {
        root->left = bst_delete(root->left, key);
    }
    else if (cmp > 0)
    {
        root->right = bst_delete(root->right, key);
    }
    else
    {
        if (root->duplicate_count > 1)
        {
            root->duplicate_count--;
            return root;
        }

        if (root->left == NULL || root->right == NULL)
        {
            bst_node_t *temp = (root->left != NULL) ? root->left : root->right;
            free(root->key);
            free(root->value);
            free(root);
            return temp;
        }

        bst_node_t *temp = bst_find_min(root->right);
        free(root->key);
        free(root->value);
        root->key = strdup(temp->key);
        root->value = strdup(temp->value);
        root->duplicate_count = temp->duplicate_count;
        temp->duplicate_count = 1;
        root->right = bst_delete(root->right, temp->key);
    }

    return root;
}

void bst_inorder(bst_node_t *node, void (*func)(bst_node_t *, void *), void *arg)
{
    if (node != NULL)
    {
        bst_inorder(node->left, func, arg);
        func(node, arg);
        bst_inorder(node->right, func, arg);
    }
}

void bst_print_node(bst_node_t *node, void *arg)
{
    FILE *outputFile = (FILE *)arg;
    for (int i = 0; i < node->duplicate_count; i++)
    {
        fprintf(outputFile, "%s\n", node->value);
    }
}

void bst_free(bst_node_t *node)
{
    if (node != NULL)
    {
        bst_free(node->left);
        bst_free(node->right);
        free(node->key);
        free(node->value);
        free(node);
    }
}

void processFile(FILE *input, FILE *output, bst_node_t **bst_root)
{
    char buffer[1000];
    char delimiter[] = " \n";
    int option;
    char *fullName = NULL;

    while (fgets(buffer, sizeof(buffer), input))
    {
        int tokenCount = 0;
        char *tokens[3] = {NULL, NULL, NULL};

        char *token = strtok(buffer, delimiter);
        while (token != NULL)
        {
            if (tokenCount >= 3)
            {
                fprintf(stderr, "Too many tokens.\n");
                break;
            }
            tokens[tokenCount++] = strdup(token);
            token = strtok(NULL, delimiter);
        }

        if (tokenCount == 1)
        {
            option = atoi(tokens[0]);
            if (option == 4)
            {
                bst_inorder(*bst_root, bst_print_node, output);
            }
            for (int i = 0; i < tokenCount; ++i)
            {
                free(tokens[i]);
            }
        }
        else if (tokenCount == 3)
        {
            free(fullName);
            fullName = malloc(strlen(tokens[0]) + strlen(tokens[1]) + 2);
            if (!fullName)
            {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < tokenCount; ++i)
                {
                    free(tokens[i]);
                }
                continue;
            }
            sprintf(fullName, "%s %s", tokens[0], tokens[1]);
            option = atoi(tokens[2]);

            switch (option)
            {
            case 1: // Add
                *bst_root = bst_insert(*bst_root, fullName, fullName);
                break;
            case 2: // Delete
                *bst_root = bst_delete(*bst_root, fullName);
                break;
            case 3: // Search
                if (bst_search(*bst_root, fullName) != NULL)
                {
                    fprintf(output, "Found\n");
                }
                else
                {
                    fprintf(output, "Not Found\n");
                }
                break;
            }
            for (int i = 0; i < tokenCount; ++i)
            {
                free(tokens[i]);
            }
        }
        else
        {
            fprintf(stderr, "Invalid input format.\n");
            for (int i = 0; i < tokenCount; ++i)
            {
                free(tokens[i]);
            }
        }
    }
    free(fullName);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL)
    {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (outputFile == NULL)
    {
        perror("Error opening output file");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    bst_node_t *bst_root = NULL;

    processFile(inputFile, outputFile, &bst_root);

    fclose(inputFile);
    fclose(outputFile);
    bst_free(bst_root);

    return EXIT_SUCCESS;
}
