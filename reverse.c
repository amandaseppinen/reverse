#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Node structure for linked list
typedef struct Node {
    char *line;
    struct Node *next;
} Node;

// Function to create a new node
Node* createNode(char *line) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    newNode->line = line;
    newNode->next = NULL;
    return newNode;
}

// Function to read lines from file and store in linked list
Node* readFile(FILE *file) {
    Node *head = NULL;
    Node *tail = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char *copy = strdup(line);
        if (copy == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        Node *newNode = createNode(copy);
        if (head == NULL) {
            head = newNode;
        } else {
            tail->next = newNode;
        }
        tail = newNode;
    }
    free(line);
    return head;
}

// Function to reverse and print the linked list
void reversePrint(Node *head, FILE *output) {
    if (head == NULL) {
        return;
    }
    reversePrint(head->next, output);
    fprintf(output, "%s", head->line);
    // Ensure there is a newline at the end of each line
    if (head->line[strlen(head->line) - 1] != '\n') {
        fprintf(output, "\n");
    }
}

// Function to free the linked list
void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            if (input != stdin) fclose(input);
            exit(1);
        }
        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            if (input != stdin) fclose(input);
            exit(1);
        }
    }

    Node *head = readFile(input);
    reversePrint(head, output);

    freeList(head);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
