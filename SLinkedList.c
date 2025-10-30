#include "stdio.h"
#include <stdlib.h>
// singly-linked list


// todo: if defined with typedef, can I use Node name on its own?
/* No. The member Node* is referring to a struct type that has not been 
// defined yet. 
error: invalid application of 'sizeof' to incomplete type 'struct Node'    
   18 |     struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));

   typedef struct {

    // Can be of any type or count 
    int data;

    struct Node* next;
} Node;


BUT this works: 
typedef struct Node{

    // Can be of any type or count // todo: create generic ll
    int data;

    struct Node* next;
}  Node;
*/


struct Node {

    // Can be of any type or count // todo: create generic ll
    int data;

    struct Node* next;
} ;

// todo: Why return a pointer and not just a node?
struct Node* createNode(int data) {
    // Allocate memory
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Insertion 
// todo: why take in a pointer to a pointer of a ll?
// You need to update the address of the head to point to the new node.
// In other words, you need to update the pointer to the head. 
// Function create a copy of the variables passed as arguments so changing 
// the value of an argument inside a function is actually just changing the copy.
// To actually change a variable's value inside a function, you need to pass
// a pointer to that variable.
// In this case, to actually change the value of the pointer, you need to pass 
// a pointer to that pointer (Node**).
// Otherwise, you would have to return the new pointer and update the pointer
// to the head in the caller function.
void prepend(struct Node** linkedListHead, int data) {
    struct Node* newNode = createNode(data);
    newNode->next = *linkedListHead;
    *linkedListHead = newNode;
}

void prepend2(struct Node* linkedListHead, int data) {
    struct Node* newNode = createNode(data);
    newNode->next = linkedListHead;
    return newNode;
}

void append(struct Node** linkedListHead, int data) {
    struct Node* newNode = createNode(data);

    if (*linkedListHead == NULL) {
        *linkedListHead = newNode;
    

    // find the last node
    struct Node* curr = *linkedListHead;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newNode;
}
}

void insert(struct Node** linkedListHead, int data, int position) {
    if (position == 0) {
        prepend(linkedListHead, data);
        return;
    }

    struct Node* newNode = createNode(data);

    struct Node* curr = *linkedListHead;
    for (int i = 0; i < position - 1; i++) {
        curr = curr->next;

        if (curr == NULL)
        {
            printf("position out of range.\n");
            free(newNode);
            return;
        }
    }

    newNode->next = curr->next;
    curr->next = newNode;
}

// Deletion
void deleteFirst(struct Node** linkedListHead) {
    if (*linkedListHead == NULL) {
        print("position out of range.");
        return;
    }
    struct Node* temp = *linkedListHead;
    *linkedListHead = temp->next;
    free(temp);
}

void delete(struct Node** linkedListHead, int position) {
    if (position == 0) {
       deleteFirst(linkedListHead);
       return;
    }

    struct Node* curr = *linkedListHead;
    for (int i = 0; i < position - 1; i++) {
        curr = curr->next;

        if (curr->next == NULL)
        {
            printf("position out of range.\n");
            return;
        }
    }

    // node to delete is at the end.
    if (curr->next->next == NULL) {
        free(curr->next);
        curr->next = NULL;
        return;
    }

    struct Node* temp = curr->next;
    curr->next = temp->next;
    free(temp);
}

void main(){
    printf("hello world\n");
}