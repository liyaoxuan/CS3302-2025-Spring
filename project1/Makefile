obj-m += hello.o calc.o
MODULES := hello calc
all: calc ps hello
ps: ps.c
	gcc ps.c -o ps -Wall -Werror -std=c17 -O2
$(MODULES):
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm ps
