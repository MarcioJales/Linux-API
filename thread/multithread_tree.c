/*  Chapter 30, Exercise 2
 *
    Implement a set of thread-safe functions that update and search an unbalanced binary tree. This library should include functions (with the obvious purposes) of the following form:

    initialize(tree);
    add(tree, char *key, void *value);
    delete(tree, char *key)
    Boolean lookup(char *key, void **value)

    In the above prototypes, tree is a structure that points to the root of the tree (you will need to define a suitable structure or this purpose). Each element of the tree holds a key-value pair. You will also need to define the structure for each element to include a mutex that protects that element so that only one thread at a time can access it. The initialize(), add(), and lookup() functions are relatively simple to implement. The delete() operation requires a little more effort.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
char keyToSearch[2];

void initialize(tree *t)
{
    int ret;

    if(VERBOSE)
        printf("Initializing tree...\n");

    t->left = NULL;
    t->right = NULL;
    (t->kv).key = NULL;
    (t->kv).value = NULL;

    /*  Book quoting:

        Among the cases where we must use pthread_mutex_init() rather than a static initializer are the following:

        - The mutex was dynamically allocated on the heap. For example, suppose that we create a dynamically allocated linked list of structures, and each structure in the list includes a pthread_mutex_t field that holds a mutex that is used to protect access to that structure.
    */
    if(ret = pthread_mutex_init(&(t->kv).mtx, NULL)) {
        fprintf(stderr, "(err = %d) Failed to initialize mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    if(VERBOSE)
        printf("Sucessfully initialized the tree.\n");
};

void add(tree *t, char *key, void *value)
{
    int ret;

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret){
        fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    if(VERBOSE) {
        printf("[add] key = %c, value = %f, Thread ID: %u\n", *key, *(float *) value, (unsigned int) pthread_self());
    }

    if((t->kv).key == NULL) {
        (t->kv).key = (char *) malloc(sizeof(char));
        (t->kv).value = malloc(sizeof(8));
        memcpy((t->kv).key, key, sizeof(char));
        memcpy((t->kv).value, value, sizeof(8));
    }
    else if(*key > *(t->kv).key) {
        if(t->right == NULL) {
            t->right = (tree *) malloc(sizeof(tree));
            initialize(t->right);
        }

        ret = pthread_mutex_unlock(&(t->kv).mtx);
        if(ret) {
            fprintf(stderr, "(err = %d) Failed to unlock mutex. Exiting...\n", ret);
            exit(EXIT_FAILURE);
        }

        add(t->right, key, value);
    }
    else if(*key < *(t->kv).key) {
        if(t->left == NULL) {
            t->left = (tree *) malloc(sizeof(tree));
            initialize(t->left);
        }

        ret = pthread_mutex_unlock(&(t->kv).mtx);
        if(ret){
            fprintf(stderr, "(err = %d) Failed to unlock mutex. Exiting...\n", ret);
            exit(EXIT_FAILURE);
        }

        add(t->left, key, value);
    }
    else if(*key == *(t->kv).key) {
        memcpy((t->kv).value, value, sizeof(8));
    }

    ret = pthread_mutex_unlock(&(t->kv).mtx);
    if(ret){
        fprintf(stderr, "(err = %d) Failed to unlock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }
};

void delete(tree *t, char *key)
{

};

int lookup(char *key, void **value)
{
    int wasFound = NOTFOUND;
    int ret;
    tree *currentTree = root;

    while(!wasFound) {
        ret = pthread_mutex_lock(&(currentTree->kv).mtx);
        if(ret){
            fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
            exit(EXIT_FAILURE);
        }

        if((currentTree->kv).key == NULL)
            break;
        else if(*key == *(currentTree->kv).key) {
            *value = (currentTree->kv).value;
            wasFound = FOUND;
        }
        else if(*key < *(currentTree->kv).key) {
            if(currentTree->left == NULL)
                break;
            else {
                ret = pthread_mutex_unlock(&(currentTree->kv).mtx);
                if(ret){
                    fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                    exit(EXIT_FAILURE);
                }

                currentTree = currentTree->left;
            }
        }
        else if(*key > *(currentTree->kv).key) {
            if(currentTree->right == NULL)
                break;
            else {
                ret = pthread_mutex_unlock(&(currentTree->kv).mtx);
                if(ret){
                    fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                    exit(EXIT_FAILURE);
                }

                currentTree = currentTree->right;
            }
        }
    }

    ret = pthread_mutex_unlock(&(currentTree->kv).mtx);
    if(ret){
        fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    return wasFound;
};

void *operate1(void *arg)
{
    char key = 'g';
    float value = 12;
    add(root, &key, &value);

    void *val;
    printf("[operate1] Search for key %c...\n", keyToSearch[1]);
    if(lookup(&keyToSearch[1], &val))
        printf("[operate1] Value for key %c: %f\n", keyToSearch[1], *((float *)val));
    else
        printf("[operate1] Value for key %c not found\n", keyToSearch[1]);

    return NULL;
};

void *operate2(void *arg)
{
    char key = 'b';
    float value = 66.6;
    add(root, &key, &value);

    key = 'v';
    value = 44.7;
    add(root, &key, &value);

    return NULL;
};

void *operate3(void *arg)
{
    char key = '=';
    float value = 0.77;
    add(root, &key, &value);

    key = 'h';
    value = 5711.09;
    add(root, &key, &value);

    return NULL;
};

void *(*operate[])(void *arg) = { operate1, operate2, operate3 };

int main(int argc, char **argv)
{
    const int numThreads = 3;
    int idx, ret;
    void *value;
    pthread_t *thread;

    root = (tree *) malloc(sizeof(tree));
    thread = (pthread_t *) malloc(numThreads * sizeof(pthread_t));
    keyToSearch[0] = argv[1][0];
    keyToSearch[1] = argv[2][0];

    initialize(root);

    if(VERBOSE)
    printf("Threads to create: %d\n", numThreads);

    for (idx = 0; idx < numThreads; idx++) {
        ret = pthread_create(&thread[idx], NULL, operate[idx], NULL);
        if (ret != 0) {
            fprintf(stderr, "(err = %d) Failed to create thread %d. Exiting...\n", ret, idx);
            exit(EXIT_FAILURE);
        }
    }

    for (idx = 0; idx < numThreads; idx++) {
        ret = pthread_join(thread[idx], NULL);
        if (ret != 0) {
          fprintf(stderr, "(err = %d) Failed to join thread %d. Exiting...\n", ret, idx);
          exit(EXIT_FAILURE);
        }
    }

    printf("Search for key %c...\n", keyToSearch[0]);
    if(lookup(&keyToSearch[0], &value))
        printf("Value for key %c: %f\n", keyToSearch[0], *((float *)value));
    else
        printf("Value for key %c not found\n", keyToSearch[0]);

    return 0;
}
