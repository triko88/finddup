# FindDup - A Duplicate File Finder for John Crickett's Coding Challenge

This is my C++ implementation for duplicate file finder for John Crickett's Coding Challenge.
Challenge Link: https://codingchallenges.fyi/challenges/challenge-duplicate-files

The challenge was a good warmup. Thanks to C++ 17's `std::filesystem`, it
reduced the hassle of recursively reading directory items, deleting them, and
creating symbolic links. This was done by mapping file paths with their SHA-256
content hash.

# Testing the program
To test the program, run the following commands

```
$ ./create-test-files.sh
$ ./finddup <test-folder>
```

The program ignores directories and symbolic links. To recursively search for
duplicates, use the `-r` flag.

```
$ ./finddup -r <test-folder>
```

To filter files based on minimum size, use `--minsize=<size>` flag. The value
of `<size>` must be in bytes.

```
$ ./finddup --minsize=10 <test-folder>
```
This will ignore all files under 10 bytes.

You will get a prompt, type 'y' or 'Y' if you want to delete the duplicates.
Choose the index number, this will indicate the original file. The duplicates
will convert into symbolic links:

```
Duplicate files found:
1: file1
2: file21
Do you want to delete them (y/n)? y
Choose the original file number (1 - 2): 1
```

