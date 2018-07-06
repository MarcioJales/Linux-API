/* sockets_seqnum.h

  Chapter 57, exercise 2:

  Reimplement the sequence-number server and client of
  "A Client-Server Application Using FIFOs" using UNIX domain stream sockets.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_SOCKET "/tmp/seqnum_sv" /* Well-known name for server's FIFO */

#define CLIENT_SOCKET_TEMPLATE "/tmp/seqnum_cl.%ld" /* Template for building client FIFO name */

/* Space required for client FIFO pathname (+20 as a generous allowance for the PID) */
#define CLIENT_SOCKET_NAME_LEN (sizeof(CLIENT_SOCKET_TEMPLATE) + 20)

#define BACKLOG 5

struct request {                /* Request (client --> server) */
    pid_t pid;                  /* PID of client */
    int seqLen;                 /* Length of desired sequence */
};

struct response {               /* Response (server --> client) */
    int seqNum;                 /* Start of sequence */
};
