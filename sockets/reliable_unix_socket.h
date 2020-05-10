/* reliable_unix_socket.h

  Chapter 57, exercise 1:

  In Section 57.3, we noted that UNIX domain datagram sockets are reliable.
  Write programs to show that if a sender transmits datagrams to a UNIX domain datagram socket faster than the receiver reads them,
  then the sender is eventually blocked, and remains blocked until the receiver reads some of the pending datagrams.

  This is basically a copy from ud_ucase.h
*/
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 10             /* Maximum size of messages exchanged
                                   between client and server */

#define SLEEP_TIME 5

#define SV_SOCK_PATH "/tmp/reliable_sv.sock"
