| Name | Assignment |
| ---- | ---------- |
| log_dir.c | Exercise 1 from chapter 19 |

## Ex 1. - Monitoring a directory and its subdirectories

Fully functional.

To add a new watch to a new directory, `nftw()` is used again when the mask return both `IN_ISDIR` and `IN_CREATE`. This is not optimal, since we go through the whole directory tree every time we create a folder.
