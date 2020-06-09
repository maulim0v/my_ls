# my_ls

## Name
./my_ls - list directory contents

## Synopsis
./my_ls [Option (-Rat)] [File ...]

## Description
For each operand that names a file of a type other than directory, my_ls displays its name as well as any requested, associated information. For each operand that names a file of type directory, my_ls displays the names of files contained within that directory, as well as any requested, associated information.

If no operands are given, the contents of the current directory are displayed. If more than one operand is given, non-directory operands are displayed first; directory and non-directory operands are sorted separately and in lexicographical order.

The following options are available:

-a Include directory entries whose names begin with a dot (.)
-R Recursively list subdirectories encountered
-t Sort by time modified (most recently modified first) before sorting the operands by lexicographical order

## Additional information
Functions used in this project:
```
malloc
free
open
close
lseek
read
write
stat
opendir
closedir
readdir
```
