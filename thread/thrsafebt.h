#include <pthread.h>

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define NOTFOUND  0
#define FOUND     1

typedef struct keyPair {
    char *key;
    void *value;
    pthread_mutex_t mtx;
} element;

typedef struct binaryTree {
    struct binaryTree *left;
    struct binaryTree *right;
    element kv;
} tree;

void initialize(tree *t);
void add(tree *t, char *key, void *value);
void delete(tree *t, char *key);
int lookup(tree *t, char *key, void **value);
