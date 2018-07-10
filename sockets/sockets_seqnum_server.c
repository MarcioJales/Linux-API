/*
   Chapter 57, exercise 2:

   Reimplement the sequence-number server and client of
   "A Client-Server Application Using FIFOs" using UNIX domain stream sockets.
  */

#include "sockets_seqnum.h"

int
main(int argc, char *argv[])
{
  int serverFd, clientFd;
  struct sockaddr_un serverAddr, clientAddr;
  socklen_t serverAddrSize = sizeof(struct sockaddr_un);
  struct request req;
  struct response resp;
  int seqNum = 0; /* This is our "service" */


  umask(0); /* So we get the permissions we want */
  serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (serverFd == -1) {
    fprintf(stderr, "Error when creating socket\n");
    exit(EXIT_FAILURE);
  }

  /* Using remove because the socket remains after the server is shut down */
  if(remove(SERVER_SOCKET) == -1 && errno != ENOENT) {
    fprintf(stderr, "Error removing socket file\n");
    exit(EXIT_FAILURE);
  }

  memset(&serverAddr, 0, sizeof(struct sockaddr_un));
  serverAddr.sun_family = AF_UNIX;
  strncpy(serverAddr.sun_path, SERVER_SOCKET, sizeof(serverAddr.sun_path) - 1);

  if(bind(serverFd, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr_un)) == -1) {
    fprintf(stderr, "Error when binding to path\n");
    exit(EXIT_FAILURE);
  }

  if(listen(serverFd, BACKLOG) == -1) {
    fprintf(stderr, "Error when binding to path\n");
    exit(EXIT_FAILURE);
  }

  /* Read requests and send responses */
  for (;;)
  {
    clientFd = accept(serverFd, (struct sockaddr *) &clientAddr, &serverAddrSize);
    if(clientFd == -1) {
      fprintf(stderr, "Error accepting connection from %s\n", clientAddr.sun_path);
      continue;
    }
    /* Either partial read or error */
    if (read(clientFd, &req, sizeof(struct request)) != sizeof(struct request)) {
      fprintf(stderr, "Error reading request; discarding\n");
      continue;
    }

    /* Send response and close socket */
    resp.seqNum = seqNum;
    if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
      fprintf(stderr, "Error writing to socket %s\n", clientAddr.sun_path);
    if (close(clientFd) == -1)
      fprintf(stderr, "Error closing socket %s\n", clientAddr.sun_path);

    seqNum += req.seqLen;           /* Update our sequence number */
  }
}
