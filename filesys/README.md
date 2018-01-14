| Name | Assignment |
| ---- | ---------- |
| overhead.c | Exercise 1 from chapter 14 |

## Ex 1. - Creating and removing files from distinct filesystems

*Incomplete.*

Currently creating files with random 6-digit numbers and deleting them in increasing sequencial order

Tests done in a VPS from Digital Ocean, running Ubuntu server 16.04.3, 512 MB memory/20 GB disk + 10 GB, 1 CPU.

First, the 20 GB disk has EXT4 on top, while the 10 GB volume has VFAT:

- EXT4
  - Created 10,000 files in 0.321773 seconds;
- VFAT
  - Created 10,000 files in 38.026945 seconds.
