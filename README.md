# How to run.
## individual files
```
gcc tokenize.c -o tokenize

./tokenize filename.txt
```
## all text files
find . -name \*.txt -type f -exec ./tokenize {} \;


