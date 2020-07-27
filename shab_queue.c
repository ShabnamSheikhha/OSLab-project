#include "include/shab_queue.h"
#include <stdlib.h>

node_t* head = NULL;
node_t* tail = NULL;

void enqueue(int *fd_client_p) {
    node_t *new_node = malloc(sizeof(node_t));
    new_node->fd_client_p = fd_client_p;
    new_node->next = NULL;

    if (tail == NULL) {
        head = new_node;
        tail = new_node;
        return;
    }
    tail->next = new_node;
    tail = new_node;
}

int* dequeue() {
    if (head == NULL) {
        return NULL;
    } else {
        int *result = head->fd_client_p;

        node_t *temp = head;
        head = head->next;
        if (head == NULL) tail = NULL;
        free(temp);

        return result;
    }
}