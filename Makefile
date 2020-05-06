output: main.o ls_base.o
	gcc main.o ls_base.o -o my_ls

main.o: main.c
	gcc -c main.c

ls_base.o: ls_base.c ls_base.h
	gcc -c ls_base.c

clean:
	rm *.o my_ls