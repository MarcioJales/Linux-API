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

## Ch 57, Ex 2.

> Rewrite the programs in Listing 57-3 (us_xfr_sv.c) and Listing 57-4 (us_xfr_cl.c) to use the Linux-specific abstract socket namespace (Section 57.6).

---

**Solution**:

In `us_xfr_sv.c`, we change as below:

```diff
memset(&addr, 0, sizeof(struct sockaddr_un));
addr.sun_family = AF_UNIX;
- strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);
+ strncpy(&addr.sun_path[1], SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

- if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
+ if (bind(sfd, (struct sockaddr *) &addr, sizeof(sa_family_t) + strlen(SV_SOCK_PATH) + 1) == -1)
    errExit("bind");
```

In `us_xfr_cl.c`:

```diff
memset(&addr, 0, sizeof(struct sockaddr_un));
addr.sun_family = AF_UNIX;
- strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);
+ strncpy(&addr.sun_path[1], SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

- if (connect(sfd, (struct sockaddr *) &addr,
-             sizeof(struct sockaddr_un)) == -1)
+ if (connect(sfd, (struct sockaddr *) &addr,
            sizeof(sa_family_t) + strlen(SV_SOCK_PATH) + 1) == -1)
+ errExit("connect");
```
