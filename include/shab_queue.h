#ifndef CODE_SHAB_QUEUE_H
#define CODE_SHAB_QUEUE_H

struct node {
    struct node* next;
    int *fd_client_p;
};

typedef struct node node_t;

void enqueue(int *client_socket);
int* dequeue();

#endif //CODE_SHAB_QUEUE_H
