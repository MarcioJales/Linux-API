/* reliable_unix_socket_cl.c

  Chapter 57, exercise 1:

  In Section 57.3, we noted that UNIX domain datagram sockets are reliable.
  Write programs to show that if a sender transmits datagrams to a UNIX domain datagram socket faster than the receiver reads them,
  then the sender is eventually blocked, and remains blocked until the receiver reads some of the pending datagrams.

  This is basically a copy from ud_ucase_cl.c
*/

#include "reliable_unix_socket.h"

int
main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: ./reliable_unix_sv message_1 [message_2] ...\n");
        exit(EXIT_FAILURE);
    }

    /* Create client socket; bind to unique pathname (based on PID) */
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        fprintf(stderr, "Error when creating socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/reliable_cl.%ld", (long) getpid());

    if (bind(sfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "Error when binding to path\n");
        exit(EXIT_FAILURE);
    }

    /* Construct address of server */
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    /* Send messages to server; echo responses on stdout */
    for (j = 1; j < argc; j++) {
        msgLen = strlen(argv[j]);       /* May be longer than BUF_SIZE */

        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) != msgLen) {
            fprintf(stderr, "sendto Error\n");
            exit(EXIT_FAILURE);
        } else {
            printf("Sent %lu bytes\n", msgLen);
        }
    }

    remove(claddr.sun_path);            /* Remove client socket pathname */
    exit(EXIT_SUCCESS);
}
