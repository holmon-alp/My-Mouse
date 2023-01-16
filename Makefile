CC = gcc
CFLAGS = -Wall -Wextra -Werror
OUT = my_mouse

all:run success
run:
	  main.c -o  

success:
	echo my_mouse installed successfully

fclean:
	rm -f  *.o
