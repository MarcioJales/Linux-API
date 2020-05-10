| Name | Assignment |
| ---- | ---------- |
| reliable_unix_socket_sv.c  | Exercise 1 from chapter 57 |
| reliable_unix_socket_cl.c  |                            |
| reliable_unix_socket.h     |                            |
| sockets_seqnum_client.c    | Exercise 3 from chapter 57 |
| sockets_seqnum_server.c    |                            |
| sockets_seqnum.h           |                            |

## Ch 57, Ex 1.

To check the suggested behavior, execute client as below:

```
$ cat args.txt | ./reliable_unix_socket_cl `xargs`
```
