#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 5 //Sayaç eşik değeri
#define L 7 //Node sayısı limiti

typedef struct Node{
    char address[10];
    int count;
    struct Node *prev;
    struct Node *next;
}NODE;

int nodeCount = 0;

void insertFront(NODE **head, char address[10],int count);
void deleteLastNode(NODE **head);
void deleteAllNodes(NODE **head);
void displayList(NODE *head);
void addressControl(NODE **head,char address[10]);
void readFromFile(NODE **head,char fileName[10]);
void freeList(NODE *head);
void menu(NODE **head);

int main() {
    NODE *head = NULL;

    menu(&head);

    freeList(head);
    return 0;
}

void insertFront(NODE **head, char address[10],int count) {
    NODE *p = (NODE*)malloc(sizeof(NODE)); //Node oluştur
    if (p == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    strcpy(p->address, address);
    p->count = count;
    p->next = *head;
    p->prev = NULL;

    if (*head != NULL) {
        (*head)->prev = p;
    }

    *head = p;
}

void deleteLastNode(NODE **head) {
    if (*head == NULL) {
        return;
    }

    NODE *current = *head;
    NODE *prev = NULL;

    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }


    if(prev == NULL){
        *head = NULL;
    } else {
        prev->next = NULL;
    }

    free(current);
}

void deleteAllNodes(NODE **head) {
    while (*head != NULL) {
        deleteLastNode(head);
    }
    *head = NULL;
}

void displayList(NODE *head) {
    printf("Linked List Contents:\n");
    printf("---------------------\n");
    NODE *current = head;
    int i = 1;
    while (current != NULL) {
        printf("NODE: %d, Address: %s, Count: %d\n", i, current->address, current->count);
        current = current->next;
        i++;
    }
    printf("---------------------\n");
}

void addressControl(NODE **head,char address[10]){
    NODE *current = *head;
    int found = 0;

    while(current != NULL && found == 0){
        if(strcmp(current->address, address) == 0){
            current->count++;
            found = 1;
            if (current->count > T) {
                NODE *temp = current;
                if (current->prev != NULL) {
                    current->prev->next = current->next;
                }
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
                if (current != *head) {
                    temp->next = *head;
                    temp->prev = NULL;
                    (*head)->prev = temp;
                    *head = temp;
                }
            }
        }
        current = current->next;
    }

    if(!found){
        insertFront(head, address, 1);
        nodeCount++;
    }

    while(nodeCount > L){
        deleteLastNode(head);
        nodeCount--;
    }
}

void readFromFile(NODE **head,char fileName[10]) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char address[10];

    while (fgets(address, sizeof(address), file) != NULL) {
        address[strcspn(address, "\n")] = '\0';
        addressControl(head, address);
    }

    fclose(file);
}


void freeList(NODE *head) {
    NODE *current = head;
    while (current != NULL) {
        NODE *temp = current;
        current = current->next;
        free(temp);
    }
}

void menu(NODE **head) {
    int choice;
    do {
        char address[10];
        char fileName[10];

        printf("\nMenu:\n");
        printf("1. Read from file\n");
        printf("2. Enter an address manually\n");
        printf("3. Display list\n");
        printf("4. Delete all nodes\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the name of the file: ");
                scanf("%s",fileName);
                readFromFile(head,fileName);
                break;
            case 2:
                printf("Please enter the address: ");
                scanf("%s",address);
                addressControl(head,address);
                break;
            case 3:
                displayList(*head);
                break;
            case 4:
                deleteAllNodes(head);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 5);
}