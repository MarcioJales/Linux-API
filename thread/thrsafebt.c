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

enum nodePosition {right, left, none};

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
    ret = pthread_mutex_init(&(t->kv).mtx, NULL);
    if(ret) {
        fprintf(stderr, "(err = %d) Failed to initialize mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    if(VERBOSE)
        printf("Sucessfully initialized the tree.\n");
}

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
  static tree *leftMostNode;
  static tree *previousNode;
  enum nodePosition pos = none;

  int ret;

  ret = pthread_mutex_lock(&(t->kv).mtx);
  if(ret){
      fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
      exit(EXIT_FAILURE);
  }

  if((t->kv).key != NULL) {
      if(*key == *(t->kv).key) {
          if(t->left == NULL) {
              if(pos == left)
                  previousNode->left = t->right;
              if(pos == right)
                  previousNode->right = t->right;
          }
          else if(t->right == NULL) {
              if(pos == left)
                  previousNode->left = t->left;
              if(pos == right)
                  previousNode->right = t->left;
          }
          else {
              leftMostNode = findLeftMostNode(t->right);
              memcpy((t->kv).key, (leftMostNode->kv).key, sizeof(char));
              memcpy((t->kv).value, (leftMostNode->kv).value, sizeof(8));

              ret = pthread_mutex_unlock(&(t->kv).mtx);
              if(ret){
                  fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                  exit(EXIT_FAILURE);
              }

              delete(t->right, leftMostNode->kv).key);
          }
      }
      else if(*key < *(t->kv).key) {
          if(t->left != NULL) {
              previousNode = t;
              pos = left;

              ret = pthread_mutex_unlock(&(t->kv).mtx);
              if(ret){
                  fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                  exit(EXIT_FAILURE);
              }

              delete(t->left, key);
          }
          else
            printf("Key %c could not be deleted, since it wasn't found.\n");
      }
      else if(*key > *(t->kv).key) {
          if(t->right != NULL) {
              previousNode = t;
              pos = right;

              ret = pthread_mutex_unlock(&(t->kv).mtx);
              if(ret){
                  fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
                  exit(EXIT_FAILURE);
              }

              delete(t->right, key);
          }
          else
            printf("Key %c could not be deleted, since it wasn't found.\n");
      }
  }
  else
    printf("Key %c could not be deleted, since it wasn't found.\n");

  ret = pthread_mutex_unlock(&(t->kv).mtx);
  if(ret) {
      fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
      exit(EXIT_FAILURE);
  }
}

/* The lookup function described by the exercise doesn't take the "tree" argument, although it is needed. Therefore, I've changed the function prototype */
int lookup(tree *t, char *key, void **value)
{
    int ret;

    ret = pthread_mutex_lock(&(t->kv).mtx);
    if(ret){
        fprintf(stderr, "(err = %d) Failed to lock mutex. Exiting...\n", ret);
        exit(EXIT_FAILURE);
    }

    if((t->kv).key != NULL) {
        if(*key == *(t->kv).key) {
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
