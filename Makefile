output: main.o ls_base.o ls_utils.o ls_linked_list_operations.o
	gcc main.o ls_base.o ls_utils.o ls_linked_list_operations.o -g3 -fsanitize=address -fno-omit-frame-pointer -o my_ls

main.o: main.c
	gcc -c main.c

ls_base.o: ls_base.c ls_base.h
	gcc -c ls_base.c

ls_linked_list_operations.o: ls_linked_list_operations.c ls_linked_list_operations.h
	gcc -c ls_linked_list_operations.c

ls_utils.o: ls_utils.c ls_utils.h
	gcc -c ls_utils.c

clean:
	rm *.o my_ls