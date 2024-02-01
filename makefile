all:exec

avl.o: avl.c avl.h
gcc -c avl.c-o avl.o

main.o: main.c avl.h
gcc -c main.c -o main.o

exec: avl.o main.o
gcc avl.o main.o -o exec

clean:
rm -f *.o
rm -f exec
