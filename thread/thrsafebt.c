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

void initialize(tree *t)
{
    int ret;

    if(DEBUG)
        printf("[Thread ID: %u] Initializing tree...\n", (unsigned int) pthread_self());

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
        fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to initialize mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
        exit(EXIT_FAILURE);
    }

    if(DEBUG)
        printf("[Thread ID: %u] Sucessfully initialized the tree.\n", (unsigned int) pthread_self());
}

void add(tree *t, char *key, void *value)
{
    int ret;

    if(t == NULL)
        return;

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret) {
        fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to lock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
        exit(EXIT_FAILURE);
    }

    if(DEBUG)
        printf("[Thread ID: %u] [add] key = %c\n", (unsigned int) pthread_self(), *key);

    if((t->kv).key == NULL) {
        if(DEBUG)
            printf("[Thread ID: %u] [add] key is NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

        (t->kv).key = (char *) malloc(BUF_SZ);
        (t->kv).value = malloc(BUF_SZ);
        memcpy((t->kv).key, key, BUF_SZ);
        memcpy((t->kv).value, value, BUF_SZ);

        if(DEBUG)
            printf("[Thread ID: %u] [add] key added (key = %c)\n", (unsigned int) pthread_self(), *key);

        ret = pthread_mutex_unlock(&(t->kv).mtx);
        if(ret) {
            fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
            exit(EXIT_FAILURE);
        }
    }
    else if(*key > *(t->kv).key) {
        if(DEBUG)
            printf("[Thread ID: %u] [add] key is greater than key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

        if(t->right == NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [add] Right subtree is NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

            t->right = (tree *) malloc(sizeof(tree));
            initialize(t->right);
        }

        ret = pthread_mutex_unlock(&(t->kv).mtx);
        if(ret) {
            fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
            exit(EXIT_FAILURE);
        }

        add(t->right, key, value);
    }
    else if(*key < *(t->kv).key) {
        if(DEBUG)
            printf("[Thread ID: %u] [add] key is less than key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

        if(t->left == NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [add] Left subtree is NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

            t->left = (tree *) malloc(sizeof(tree));
            initialize(t->left);
        }

        ret = pthread_mutex_unlock(&(t->kv).mtx);
        if(ret){
            fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
            exit(EXIT_FAILURE);
        }

        add(t->left, key, value);
    }
    else if(*key == *(t->kv).key) {
        if(DEBUG)
            printf("[Thread ID: %u] [add] key is equal to key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

        memcpy((t->kv).value, value, sizeof(8));

        ret = pthread_mutex_unlock(&(t->kv).mtx);
        if(ret) {
            fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
            exit(EXIT_FAILURE);
        }
    }
}

void freeNode(tree *node)
{
    int ret;

    free((node->kv).key);
    free((node->kv).value);

    ret = pthread_mutex_unlock(&(node->kv).mtx);
    if(ret) {
        fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
        exit(EXIT_FAILURE);
    }
    // ret = pthread_mutex_destroy(&(node->kv).mtx);
    // if(ret) {
    //     fprintf(stderr, "[delete] (err = %d) Failed to destroy mutex (Thread ID %u)\n", ret, (unsigned int) pthread_self());
    //     exit(EXIT_FAILURE);
    // }

    free(node);
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
    int ret;

    if(t == NULL) {
        printf("[Thread ID: %u] [delete] key = %c cannot be deleted (tree = NULL)\n", (unsigned int) pthread_self(), *key);
        return;
    }

    if(DEBUG)
        printf("[Thread ID: %u] [delete] key = %c\n", (unsigned int) pthread_self(), *key);

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret) {
        fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to lock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
        exit(EXIT_FAILURE);
    }

    if(*key == *(t->kv).key) {
        tree *leftMostNode, *left, *right;

        if(DEBUG)
            printf("[Thread ID: %u] [delete] key is equal to key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

        if(t->left == NULL && t->right == NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [delete] Both subtrees are NULL (key = %c)\n", (unsigned int) pthread_self(), *key);
            freeNode(t);

            if(DEBUG)
                printf("[Thread ID: %u] [delete] key deleted (key = %c)\n", (unsigned int) pthread_self(), *key);
        }
        else if(t->left == NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [delete] Left subtree is NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

            right = t->right;

            ret = pthread_mutex_lock(&(right->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to lock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            memcpy((t->kv).key, (right->kv).key, sizeof(char));
            memcpy((t->kv).value, (right->kv).value, sizeof(8));

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", ret, (unsigned int) pthread_self());
                exit(EXIT_FAILURE);
            }

            t->left = right->left;
            t->right = right->right;
            freeNode(right);

            if(DEBUG)
                printf("[Thread ID: %u] [delete] key deleted (key = %c)\n", (unsigned int) pthread_self(), *key);
        }
        else if(t->right == NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [delete] Right subtree is NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

            left = t->left;

            ret = pthread_mutex_lock(&(left->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to lock mutex. Exiting...)\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            memcpy((t->kv).key, (left->kv).key, sizeof(char));
            memcpy((t->kv).value, (left->kv).value, sizeof(8));

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            t->left = left->left;
            t->right = left->right;
            freeNode(left);

            if(DEBUG)
                printf("[Thread ID: %u] [delete] key deleted (key = %c)\n", (unsigned int) pthread_self(), *key);
        }
        else {
            if(DEBUG)
                printf("[Thread ID: %u] [delete] Node has both children (key = %c)\n", (unsigned int) pthread_self(), *key);

            leftMostNode = findLeftMostNode(t->right);

            if(DEBUG)
                printf("[Thread ID: %u] [delete] Leftmost node found (key = %c)\n", (unsigned int) pthread_self(), *(leftMostNode->kv).key);

            ret = pthread_mutex_lock(&(leftMostNode->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to lock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            memcpy((t->kv).key, (leftMostNode->kv).key, sizeof(char));
            memcpy((t->kv).value, (leftMostNode->kv).value, sizeof(8));

            ret = pthread_mutex_unlock(&(leftMostNode->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            delete(leftMostNode, (leftMostNode->kv).key);
        }
    }
    else if(*key < *(t->kv).key) {
        if(DEBUG)
            printf("[Thread ID: %u] [delete] key is less than key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

        if(t->left != NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [delete] Left subtree is NOT NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret){
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            delete(t->left, key);
        }
        else {
            printf("[Thread ID: %u] [delete] key = %c not deleted (key not found)\n", (unsigned int) pthread_self(), *key);

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }
        }
    }
    else if(*key > *(t->kv).key) {
        if(DEBUG)
            printf("[Thread ID: %u] [delete] key is greater than key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

        if(t->right != NULL) {
            if(DEBUG)
                printf("[Thread ID: %u] [delete] Right subtree is NOT NULL (key = %c)\n", (unsigned int) pthread_self(), *key);

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret){
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            delete(t->right, key);
        }
        else {
            printf("[Thread ID: %u] [delete] key = %c not deleted (key not found)\n", (unsigned int) pthread_self(),  *key);

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret) {
                fprintf(stderr, "[Thread ID: %u] [delete] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }
        }
    }
}

/* The lookup function described by the exercise doesn't take the "tree" argument, although it is needed. Therefore, I've changed the function prototype */
int lookup(tree *t, char *key, void **value)
{
    int ret;

    if(t == NULL)
        return NOTFOUND;

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret) {
        fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to lock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
        exit(EXIT_FAILURE);
    }

    if(DEBUG)
        printf("[Thread ID: %u] [lookup] key = %c\n", (unsigned int) pthread_self(), *key);

    if((t->kv).key != NULL) {
        if(*key == *(t->kv).key) {
            if(DEBUG)
                printf("[Thread ID: %u] [lookup] key is equal to key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

            *value = (t->kv).value;

            ret = pthread_mutex_unlock(&(t->kv).mtx);
            if(ret){
                fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                exit(EXIT_FAILURE);
            }

            return FOUND;
        }
        else if(*key < *(t->kv).key) {
            if(DEBUG)
                printf("[Thread ID: %u] [lookup] key is less than key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

            if(t->left != NULL) {
                if(DEBUG)
                    printf("[Thread ID: %u] [lookup] Left subtree is NOT NULL (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

                ret = pthread_mutex_unlock(&(t->kv).mtx);
                if(ret){
                    fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                    exit(EXIT_FAILURE);
                }

                return lookup(t->left, key, value);
            }
            else {
                if(DEBUG)
                    printf("[Thread ID: %u] [lookup] Left subtree is NULL (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

                ret = pthread_mutex_unlock(&(t->kv).mtx);
                if(ret) {
                    fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if(*key > *(t->kv).key) {
            if(DEBUG)
                printf("[Thread ID: %u] [lookup] key is greater than key in this node (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

            if(t->right != NULL) {
                if(DEBUG)
                    printf("[Thread ID: %u] [lookup] Right subtree is NOT NULL (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

                ret = pthread_mutex_unlock(&(t->kv).mtx);
                if(ret){
                    fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                    exit(EXIT_FAILURE);
                }

                return lookup(t->right, key, value);
            }
            else {
                if(DEBUG)
                    printf("[Thread ID: %u] [lookup] Right subtree is NULL (key = %c, key here = %c)\n", (unsigned int) pthread_self(), *key, *(t->kv).key);

                ret = pthread_mutex_unlock(&(t->kv).mtx);
                if(ret) {
                    fprintf(stderr, "[Thread ID: %u] (err = %d) Failed to unlock mutex. Exiting...\n", (unsigned int) pthread_self(), ret);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return NOTFOUND;
}
