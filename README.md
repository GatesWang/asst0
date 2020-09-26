# How to run.
## compilation 
```
gcc tokenize.c -o tokenize
```
## individual files
```
./tokenize filename.txt
```
## all text files
```
find . -name \*.txt -type f -exec ./tokenize {} \;
```

