#include <pthread.h>

#ifdef VERBOSE
#define VERBOSE 1
#else
#define VERBOSE 0
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

tree *root;

void initialize(tree *t);
void add(tree *t, char *key, void *value);
void delete(tree *t, char *key);
int lookup(tree *t, char *key, void **value);
