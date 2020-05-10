/* reliable_unix_socket_sv.c

  Chapter 57, exercise 1:

  In Section 57.3, we noted that UNIX domain datagram sockets are reliable.
  Write programs to show that if a sender transmits datagrams to a UNIX domain datagram socket faster than the receiver reads them,
  then the sender is eventually blocked, and remains blocked until the receiver reads some of the pending datagrams.

  This is basically a copy from ud_ucase_sv.c
*/

#include "reliable_unix_socket.h"

int
main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);       /* Create server socket */
    if (sfd == -1)
        errExit("socket");

    /* Construct well-known address and bind server socket to it */

    /* For an explanation of the following check, see the erratum note for
       page 1168 at http://www.man7.org/tlpi/errata/. */

    if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1) {
        fprintf(stderr, "Server socket path too long: %s", SV_SOCK_PATH);
        exit(EXIT_FAILURE);
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        fprintf(stderr, "Error removing socket file\n");
        exit(EXIT_FAILURE);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "Error when binding to path\n");
        exit(EXIT_FAILURE);
    }

    /* Receive messages, convert to uppercase, and return to client */
    for (;;) {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);

        if (numBytes == -1) {
            fprintf(stderr, "recvfrom Error\n");
            exit(EXIT_FAILURE);
        }

        printf("Server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

        for (j = 0; j < numBytes; j++)
            buf[j] = toupper((unsigned char) buf[j]);

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) {
            fprintf(stderr, "sendto Error\n");
            exit(EXIT_FAILURE);
        }
    }
}
