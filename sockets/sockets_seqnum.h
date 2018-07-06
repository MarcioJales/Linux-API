/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2017.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 44-6 */

/* fifo_seqnum.h

   Header file used by fifo_seqnum_server.c and fifo_seqnum_client.c

   These programs create FIFOS in /tmp. This makes it easy to compile and
   run the programs. However, for a security reasons, a real-world
   application should never create sensitive files in /tmp. (As a simple of
   example of the kind of security problems that can result, a malicious
   user could create a FIFO using the name defined in SERVER_FIFO, and
   thereby cause a denial of service attack against this application.
   See Section 38.7 of "The Linux Programming Interface" for more details
   on this subject.)
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
