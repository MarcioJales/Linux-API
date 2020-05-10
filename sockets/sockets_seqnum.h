/* sockets_seqnum.h

  Chapter 57, exercise 3:

  Reimplement the sequence-number server and client of
  "A Client-Server Application Using FIFOs" using UNIX domain stream sockets.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_SOCKET "/tmp/seqnum_sv" /* Well-known name for server's socket */

#define BACKLOG 5

struct request {                /* Request (client --> server) */
    pid_t pid;                  /* PID of client */
    int seqLen;                 /* Length of desired sequence */
};

struct response {               /* Response (server --> client) */
    int seqNum;                 /* Start of sequence */
};
