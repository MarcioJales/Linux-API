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
    
    key = 'v';
    value = 44.7;
    add(root, &key, &value);
    //
    // printf("Search for key %c...\n", keyToSearch[0]);
    // if(lookup(root, &keyToSearch[0], &val))
    //     printf("Value for key %c: %f\n", keyToSearch[0], *((float *)val));
    // else
    //     printf("Value for key %c not found\n", keyToSearch[0]);
    //
    // key = 'a';
    // value = 0.77;
    // add(root, &key, &value);
    //
    // key = 'b';
    // value = 44.7;
    // add(root, &key, &value);

    key = 'g';
    delete(root, &key);
    //
    // key = 'f';
    // value = 41.55;
    // add(root, &key, &value);

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

    key = 'h';
    value = 5711.09;
    add(root, &key, &value);

    return NULL;
};

void *(*operate[])(void *arg) = { operate1, operate2, operate3 };

int main(int argc, char **argv)
{
    const int numThreads = 1;
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


/* Failed test

Initializing tree..., Thread ID: 2543912704
Sucessfully initialized the tree.
Threads to create: 3
[add] key = =, value = 0.770000, Thread ID: 2516727552
[add] key is NULL (key = =, value = 0.770000, Thread ID: 2516727552)
[add] key = h, value = 5711.089844, Thread ID: 2516727552
[add] key is greater than key in this node (key = h, value = 5711.089844, Thread ID: 2516727552)
[add] Right subtree is NULL (key = h, value = 5711.089844, Thread ID: 2516727552)
Initializing tree..., Thread ID: 2516727552
Sucessfully initialized the tree.
[add] key = h, value = 5711.089844, Thread ID: 2516727552
[add] key is NULL (key = h, value = 5711.089844, Thread ID: 2516727552)
[add] key = b, value = 66.599998, Thread ID: 2525120256
[add] key is greater than key in this node (key = b, value = 66.599998, Thread ID: 2525120256)
[add] key = b, value = 66.599998, Thread ID: 2525120256
[add] key is less than key in this node (key = b, value = 66.599998, Thread ID: 2525120256)
[add] Left subtree is NULL (key = b, value = 66.599998, Thread ID: 2525120256)
Initializing tree..., Thread ID: 2525120256
[add] key = h, value = 12.000000, Thread ID: 2533512960
[add] key is greater than key in this node (key = h, value = 12.000000, Thread ID: 2533512960)
Sucessfully initialized the tree.
[add] key = b, value = 66.599998, Thread ID: 2525120256
[add] key is NULL (key = b, value = 66.599998, Thread ID: 2525120256)
[add] key = v, value = 44.700001, Thread ID: 2525120256
[add] key is greater than key in this node (key = v, value = 44.700001, Thread ID: 2525120256)
[add] key = v, value = 44.700001, Thread ID: 2525120256
[add] key is greater than key in this node (key = v, value = 44.700001, Thread ID: 2525120256)
[add] Right subtree is NULL (key = v, value = 44.700001, Thread ID: 2525120256)
Initializing tree..., Thread ID: 2525120256
Sucessfully initialized the tree.
[add] key = v, value = 44.700001, Thread ID: 2525120256
[add] key is NULL (key = v, value = 44.700001, Thread ID: 2525120256)
[add] key = h, value = 12.000000, Thread ID: 2533512960
[add] key is equal to key in this node (key = h, value = 12.000000, Thread ID: 2533512960)
[add] key = a, value = 44.700001, Thread ID: 2533512960
[add] key is greater than key in this node (key = a, value = 44.700001, Thread ID: 2533512960)
[add] key = a, value = 44.700001, Thread ID: 2533512960
[add] key is less than key in this node (key = a, value = 44.700001, Thread ID: 2533512960)
[add] key = a, value = 44.700001, Thread ID: 2533512960
[add] key is less than key in this node (key = a, value = 44.700001, Thread ID: 2533512960)
[add] Left subtree is NULL (key = a, value = 44.700001, Thread ID: 2533512960)
Initializing tree..., Thread ID: 2533512960
Sucessfully initialized the tree.
[add] key = a, value = 44.700001, Thread ID: 2533512960
[add] key is NULL (key = a, value = 44.700001, Thread ID: 2533512960)
Search for key h...
[lookup] key = h, Thread ID: 2533512960
[lookup] key is greater than key in this node (key = h, Thread ID: 2533512960)
[lookup] key = h, Thread ID: 2533512960
[lookup] key is equal to key in this node (key = h, Thread ID: 2533512960)
Value for key h: 12.000000
[add] key = z, value = 0.770000, Thread ID: 2533512960
[add] key is greater than key in this node (key = z, value = 0.770000, Thread ID: 2533512960)
[add] key = z, value = 0.770000, Thread ID: 2533512960
[add] key is greater than key in this node (key = z, value = 0.770000, Thread ID: 2533512960)
[add] key = z, value = 0.770000, Thread ID: 2533512960
[add] key is greater than key in this node (key = z, value = 0.770000, Thread ID: 2533512960)
[add] Right subtree is NULL (key = z, value = 0.770000, Thread ID: 2533512960)
Initializing tree..., Thread ID: 2533512960
Sucessfully initialized the tree.
[add] key = z, value = 0.770000, Thread ID: 2533512960
[add] key is NULL (key = z, value = 0.770000, Thread ID: 2533512960)
Search for key b...
[lookup] key = b, Thread ID: 2543912704
[lookup] key is greater than key in this node (key = b, Thread ID: 2543912704)
[lookup] key = b, Thread ID: 2543912704
[lookup] key is less than key in this node (key = b, Thread ID: 2543912704)
[lookup] key = b, Thread ID: 2543912704
[lookup] key is equal to key in this node (key = b, Thread ID: 2543912704)
Value for key b: 66.599998
[delete] key = =, Thread ID: 2543912704
[delete] key is equal to key in this node (key = =, Thread ID: 2543912704)
[delete] Left subtree is NULL (key = =, Thread ID: 2543912704)
(err = 16) Failed to destroy mutex. Exiting... (Thread ID 2543912704)

*/
