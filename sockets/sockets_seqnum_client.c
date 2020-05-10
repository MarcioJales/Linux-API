/*
   Chapter 57, exercise 3:

   Reimplement the sequence-number server and client of
   "A Client-Server Application Using FIFOs" using UNIX domain stream sockets.
*/

#include "sockets_seqnum.h"

int
main(int argc, char *argv[])
{
    int clientFd;
    struct sockaddr_un serverAddr;
    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
      fprintf(stderr, "Usage: %s [seq-len...]\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    /* Create our socket */
    clientFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clientFd == -1) {
      fprintf(stderr, "Error when creating socket\n");
      exit(EXIT_FAILURE);
    }

    /* Get server address */
    memset(&serverAddr, 0, sizeof(struct sockaddr_un));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SERVER_SOCKET, sizeof(serverAddr.sun_path) - 1);

    /* Connecting to it */
    if(connect(clientFd, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr_un)) == -1) {
      fprintf(stderr, "Error when trying connect to server\n");
      exit(EXIT_FAILURE);
    }

    /* Construct request message and send message */
    req.pid = getpid();
    req.seqLen = (argc > 1) ? strtol(argv[1], NULL, 10) : 1;
    if (write(clientFd, &req, sizeof(struct request)) != sizeof(struct request)) {
      fprintf(stderr, "Can't write to server %s\n", SERVER_SOCKET);
      exit(EXIT_FAILURE);
    }

    /* Read and display response */
    if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
      fprintf(stderr, "Can't read response from server %s\n", SERVER_SOCKET);
      exit(EXIT_FAILURE);
    }

    printf("%d\n", resp.seqNum);
    exit(EXIT_SUCCESS);
}
