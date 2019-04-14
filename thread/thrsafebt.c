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
#include <thrsafebt.h>

enum childPos {left, right, none};

void initialize(tree *t)
{
    int ret;

    if(DEBUG)
        printf("Initializing tree..., Thread ID: %u\n", (unsigned int) pthread_self());

    t->left = NULL;
    t->right = NULL;
    (t->kv).key = NULL;
    (t->kv).value = NULL;

    /*  Book quoting:

        Among the cases where we must use pthread_mutex_init() rather than a static initializer are the following:

        - The mutex was dynamically allocated on the heap. For example, suppose that we create a dynamically allocated linked list of structures, and each structure in the list includes a pthread_mutex_t field that holds a mutex that is used to protect access to that structure.
    */
    ret = pthread_mutex_init(&(t->kv).mtx, NULL);
    if(ret) {
        fprintf(stderr, "(err = %d) Failed to initialize mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    if(DEBUG)
        printf("Sucessfully initialized the tree. Thread ID: %u\n", (unsigned int) pthread_self());
}

void add(tree *t, char *key, void *value)
{
    int ret;

    if(t == NULL)
        return;

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret) {
        fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting... (Thread ID: %u)\n", ret, (unsigned int) pthread_self());
        exit(EXIT_FAILURE);
    }

    if(DEBUG)
        printf("[add] key = %c, value = %f, Thread ID: %u\n", *key, *(float *) value, (unsigned int) pthread_self());

    if((t->kv).key == NULL) {
        if(DEBUG)
            printf("[add] key is NULL (key = %c, value = %f, Thread ID: %u)\n", *key, *(float *) value, (unsigned int) pthread_self());

        (t->kv).key = (char *) malloc(sizeof(char));
        (t->kv).value = malloc(sizeof(8));
        memcpy((t->kv).key, key, sizeof(char));
        memcpy((t->kv).value, value, sizeof(8));
    }
    else if(*key > *(t->kv).key) {
        if(DEBUG)
            printf("[add] key is greater than key in this node (key = %c, key here = %c, value = %f, Thread ID: %u)\n", *key, *(t->kv).key, *(float *) value, (unsigned int) pthread_self());

        if(t->right == NULL) {
            if(DEBUG)
                printf("[add] Right subtree is NULL (key = %c, value = %f, Thread ID: %u)\n", *key, *(float *) value, (unsigned int) pthread_self());

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
        if(DEBUG)
            printf("[add] key is less than key in this node (key = %c, key here = %c, value = %f, Thread ID: %u)\n", *key, *(t->kv).key, *(float *) value, (unsigned int) pthread_self());

        if(t->left == NULL) {
            if(DEBUG)
                printf("[add] Left subtree is NULL (key = %c, value = %f, Thread ID: %u)\n", *key, *(float *) value, (unsigned int) pthread_self());

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
        if(DEBUG)
            printf("[add] key is equal to key in this node (key = %c, key here = %c, value = %f, Thread ID: %u)\n", *key, *(t->kv).key, *(float *) value, (unsigned int) pthread_self());

        memcpy((t->kv).value, value, sizeof(8));
    }

    ret = pthread_mutex_unlock(&(t->kv).mtx);
    if(ret) {
        fprintf(stderr, "(err = %d) Failed to unlock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }
}

tree * findLeftMostNode(tree *t)
{
    tree *currentNode = t;

    while(currentNode->left != NULL)
        currentNode = currentNode->left;

    return currentNode;
}

void delete(tree *t, char *key)
{
    int ret, toDelete = 0;
    tree *currentNode = t;
    tree *previousNode = NULL;
    tree *leftMostNode = NULL;
    enum childPos pos = none;

    if(currentNode == NULL) {
        printf("[delete] key = %c cannot be deleted (tree = NULL), Thread ID: %u\n", *key, (unsigned int) pthread_self());
        return;
    }

    if(DEBUG)
        printf("[delete] key = %c, Thread ID: %u\n", *key, (unsigned int) pthread_self());

    while(1) {
        ret = pthread_mutex_lock(&(currentNode->kv).mtx);
        if(ret) {
            fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
            exit(EXIT_FAILURE);
        }

        if(*key == *(currentNode->kv).key) {
            if(DEBUG)
                printf("[delete] key is equal to key in this node (key = %c, key here = %c, Thread ID: %u)\n", *key, *(currentNode->kv).key, (unsigned int) pthread_self());

            if(currentNode->left == NULL) {
                if(DEBUG)
                    printf("[delete] Left subtree is NULL (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                if(previousNode != NULL) {
                    if(pos == right) {
                        if(DEBUG)
                            printf("[delete] This node is right subtree (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                        previousNode->right = currentNode->right;
                    }
                    if(pos == left) {
                        if(DEBUG)
                            printf("[delete] This node is left subtree (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                        previousNode->left = currentNode->right;
                    }
                }

                toDelete = 1;
                break;
            }
            else if(currentNode->right == NULL) {
                if(DEBUG)
                    printf("[delete] Right subtree is NULL (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                if(previousNode != NULL) {
                    if(pos == right) {
                        if(DEBUG)
                            printf("[delete] This node is right subtree (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                        previousNode->right = currentNode->left;
                    }
                    if(pos == left) {
                        if(DEBUG)
                            printf("[delete] This node is left subtree (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                        previousNode->left = currentNode->left;
                    }
                }

                toDelete = 1;
                break;
            }

            if(DEBUG)
                printf("[delete] Node has both children (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

            leftMostNode = findLeftMostNode(currentNode->right);

            ret = pthread_mutex_lock(&(leftMostNode->kv).mtx);
            if(ret) {
                fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
                exit(EXIT_FAILURE);
            }

            memcpy((currentNode->kv).key, (leftMostNode->kv).key, sizeof(char));
            memcpy((currentNode->kv).value, (leftMostNode->kv).value, sizeof(8));

            ret = pthread_mutex_unlock(&(leftMostNode->kv).mtx);
            if(ret) {
                fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
                exit(EXIT_FAILURE);
            }

            ret = pthread_mutex_unlock(&(currentNode->kv).mtx);
            if(ret) {
                fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
                exit(EXIT_FAILURE);
            }

            delete(currentNode, (leftMostNode->kv).key);

            break;
        }
        else if(*key < *(currentNode->kv).key) {
            if(DEBUG)
                printf("[delete] key is less than key in this node (key = %c, key here = %c, Thread ID: %u)\n", *key, *(currentNode->kv).key, (unsigned int) pthread_self());

            if(currentNode->left != NULL) {
                if(DEBUG)
                    printf("[delete] Left subtree is NOT NULL (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                previousNode = currentNode;
                pos = left;

                ret = pthread_mutex_unlock(&(currentNode->kv).mtx);
                if(ret){
                    fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
                    exit(EXIT_FAILURE);
                }

                currentNode = currentNode->left;
            }
            else {
                printf("[delete] key = %c not deleted (key not found), Thread ID: %u\n", *key, (unsigned int) pthread_self());
                break;
            }
        }
        else if(*key > *(currentNode->kv).key) {
            if(DEBUG)
                printf("[delete] key is greater than key in this node (key = %c, key here = %c, Thread ID: %u)\n", *key, *(currentNode->kv).key, (unsigned int) pthread_self());

            if(currentNode->right != NULL) {
                if(DEBUG)
                    printf("[delete] Right subtree is NOT NULL (key = %c, Thread ID: %u)\n", *key, (unsigned int) pthread_self());

                previousNode = currentNode;
                pos = right;

                ret = pthread_mutex_unlock(&(currentNode->kv).mtx);
                if(ret){
                    fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
                    exit(EXIT_FAILURE);
                }

                currentNode = currentNode->right;
            }
            else {
                printf("[delete] key = %c not deleted (key not found), Thread ID: %u\n", *key, (unsigned int) pthread_self());
                break;
            }
        }
    }

    if(toDelete) {
        ret = pthread_mutex_unlock(&(currentNode->kv).mtx);
        if(ret) {
            fprintf(stderr, "[delete] (err = %d) Failed to lock mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
            exit(EXIT_FAILURE);
        }

        ret = pthread_mutex_destroy(&(currentNode->kv).mtx);
        if(ret) {
            fprintf(stderr, "(err = %d) Failed to destroy mutex. Exiting... (Thread ID %u)\n", ret, (unsigned int) pthread_self());
            exit(EXIT_FAILURE);
        }

        free(currentNode);
    }
}

/* The lookup function described by the exercise doesn't take the "tree" argument, although it is needed. Therefore, I've changed the function prototype */
int lookup(tree *t, char *key, void **value)
{
    int ret;

    if(t == NULL)
        return NOTFOUND;

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret){
        fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    if(DEBUG) {
        printf("[lookup] key = %c, Thread ID: %u\n", *key, (unsigned int) pthread_self());
    }

    if((t->kv).key != NULL) {
        if(*key == *(t->kv).key) {
            if(DEBUG)
                printf("[lookup] key is equal to key in this node (key = %c, key here = %c, Thread ID: %u)\n", *key, *(t->kv).key, (unsigned int) pthread_self());

            *value = (t->kv).value;

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret){
                fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                exit(EXIT_FAILURE);
            }

            return FOUND;
        }
        else if(*key < *(t->kv).key) {
            if(t->left != NULL) {
                if(DEBUG)
                    printf("[lookup] key is less than key in this node (key = %c, key here = %c, Thread ID: %u)\n", *key, *(t->kv).key, (unsigned int) pthread_self());

                ret = pthread_mutex_unlock(&(t->kv).mtx);
                if(ret){
                    fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                    exit(EXIT_FAILURE);
                }

                return lookup(t->left, key, value);
            }
        }
        else if(*key > *(t->kv).key) {
            if(t->right != NULL) {
                if(DEBUG)
                    printf("[lookup] key is greater than key in this node (key = %c, key here = %c, Thread ID: %u)\n", *key, *(t->kv).key,(unsigned int) pthread_self());

                ret = pthread_mutex_unlock(&(t->kv).mtx);
                if(ret){
                    fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                    exit(EXIT_FAILURE);
                }

                return lookup(t->right, key, value);
            }
        }
    }

    ret = pthread_mutex_unlock(&(t->kv).mtx);
    if(ret) {
        fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    return NOTFOUND;
}
