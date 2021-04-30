CFLAGS= -Wall -pedantic -std=c11\
        -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include\
        -I/usr/local/include -L/usr/local/lib
LIBS= -lhammer -lglib-2.0


all:	bug1 bug2 bug3

bug1:	bug1.c 
			gcc $(CFLAGS) -o bug1 bug1.c $(LIBS)

bug2:	bug2.c 
			gcc $(CFLAGS) -o bug2 bug2.c $(LIBS)

bug3:	bug3.c 
			gcc $(CFLAGS) -o bug3 bug3.c $(LIBS)

run:
			@echo "[Bug1]"
			@./bug1 -k
			@echo "[Bug2]"
			@./bug2 -k
			@echo "[Bug3]"
			@./bug3 -k

clean:
			rm -f bug1 bug2 bug3

