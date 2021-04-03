| Name | Assignment |
| ---- | ---------- |
| longrunner.c | Exercise 1 from chapter 18 |
| ETXTBSY.c    |                            |

## Ex. 1 - `ETXTBSY` error and file I-node

I wrote the two aforementioned files to demonstrate that we get the `ETXTBSY` when trying to open `longrunner` while it is being executed.

Then, we follow the steps described by the book:

```
[marciojales]: dirs_links $ ls -li
total 36
6090601 -rwxrwxr-x 1 marciojales marciojales 9080 Abr  3 19:51 ETXTBSY
6088998 -rw-rw-r-- 1 marciojales marciojales 1387 Abr  3 20:01 ETXTBSY.c
6091233 -rwxrwxr-x 1 marciojales marciojales 8680 Abr  3 20:01 longrunner
6088801 -rw-rw-r-- 1 marciojales marciojales  983 Abr  3 20:00 longrunner.c
6086096 -rw-rw-r-- 1 marciojales marciojales  292 Abr  2 20:40 README.md
[marciojales]: dirs_links $ ./longrunner &
[1] 11749
[marciojales]: dirs_links $ gcc -o longrunner longrunner.c
[marciojales]: dirs_links $ ls -li
total 36
6090601 -rwxrwxr-x 1 marciojales marciojales 9080 Abr  3 19:51 ETXTBSY
6088998 -rw-rw-r-- 1 marciojales marciojales 1387 Abr  3 20:01 ETXTBSY.c
6091308 -rwxrwxr-x 1 marciojales marciojales 8680 Abr  3 20:06 longrunner
6088801 -rw-rw-r-- 1 marciojales marciojales  979 Abr  3 20:05 longrunner.c
6086096 -rw-rw-r-- 1 marciojales marciojales  292 Abr  2 20:40 README.md
```

We can see the the I-node number for the `longrunner` executable changed from **6091233** to **6091308**, indicating that it is not the same file anymore, in the file system's perspective. Thus, the erro `ETXTBSY` doesn't apply for this case.
