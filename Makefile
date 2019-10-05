GCC=gcc
FLAGS=-Wall -Werror -pedantic

default:build
build:attack.exe

# build executable
attack.exe:main.o hmac.o
	$(GCC) $(FLAGS) main.o hmac.o -o attack.exe
main.o:main.c 
	$(GCC) $(FLAGS) main.c -c
hmac.o:hmac.c hmac.h
	$(GCC) $(FLAGS) hmac.c -c

test:test.o hmac.o
	$(GCC) $(FLAGS) test.o hmac.o -o test.exe
test.o:test.c
	$(GCC) $(FLAGS) test.c -c

.phony:clean
clean:
	rm *.o
	rm *.exe
