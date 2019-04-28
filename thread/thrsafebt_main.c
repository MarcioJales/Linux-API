/*  Chapter 30, Exercise 2
 *
    Program that uses library created from exercise.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <thrsafebt.h>

char keyToSearch[3];

void *operate1(void *arg)
{
    tree *root = (tree *) arg;
    char key = 'g';
    void *val;
    float value = 12;
    add(root, &key, &value);

    key = '8';
    value = 44.7;
    add(root, &key, &value);

    printf("Search for key %c...\n", keyToSearch[0]);
    if(lookup(root, &keyToSearch[0], &val))
        printf("Value for key %c: %f\n", keyToSearch[0], *((float *)val));
    else
        printf("Value for key %c not found\n", keyToSearch[0]);

    key = 'a';
    value = 0.77;
    add(root, &key, &value);

    key = 'w';
    value = 44.7;
    add(root, &key, &value);

    key = 'g';
    delete(root, &key);

    key = 'f';
    value = 41.55;
    add(root, &key, &value);

    return NULL;
};

void *operate2(void *arg)
{
    tree *root = (tree *) arg;
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
    tree *root = (tree *) arg;
    char key = '=';
    float value = 0.77;

    add(root, &key, &value);

    key = 'q';
    value = 5711.09;
    add(root, &key, &value);

    return NULL;
};

void *(*operate[])(void *arg) = { operate1, operate2, operate3 };

int main(int argc, char **argv)
{
    const int numThreads = 3;
    int idx, ret;
    char key;
    void *value;
    tree *root;
    pthread_t *thread;

    root = (tree *) malloc(sizeof(tree));
    thread = (pthread_t *) malloc(numThreads * sizeof(pthread_t));
    keyToSearch[0] = argv[1][0];
    keyToSearch[1] = argv[2][0];
    keyToSearch[2] = argv[3][0];

    initialize(root);

    printf("Threads to create: %d\n", numThreads);

    for (idx = 0; idx < numThreads; idx++) {
        ret = pthread_create(&thread[idx], NULL, operate[idx], (void *) root);
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

    printf("Search for key %c...\n", keyToSearch[1]);
    if(lookup(root, &keyToSearch[1], &value))
        printf("Value for key %c: %f\n", keyToSearch[1], *((float *)value));
    else
        printf("Value for key %c not found\n", keyToSearch[1]);

    printf("Search for key %c...\n", keyToSearch[2]);
    if(lookup(root, &keyToSearch[2], &value))
        printf("Value for key %c: %f\n", keyToSearch[2], *((float *)value));
    else
        printf("Value for key %c not found\n", keyToSearch[2]);


    return 0;
}
