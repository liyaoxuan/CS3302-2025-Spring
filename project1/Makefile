all: hello calc ps

ps: ps.c
	gcc ps.c -o ps -Wall -Werror -std=c17 -O2

calc:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) obj-m=calc.o modules

hello:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) obj-m=hello.o modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ps
