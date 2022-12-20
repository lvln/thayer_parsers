CFLAGS= -Wall -pedantic -std=c11\
        -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include\
        -I/usr/local/include -L/usr/local/lib
LIBS= -lhammer -lglib-2.0

OBJS= bug1 bug2 bug3 bug4 bug5 bug6 bug7 bug8

all:	$(OBJS)

#bug1:	bug1.c 			gcc $(CFLAGS) -o bug1 bug1.c $(LIBS)			gcc $(CFLAGS) -DBKEND=PB_MIN -o bug1.pr bug1.c $(LIBS)

#bug2:	bug2.c 			gcc $(CFLAGS) -o bug2 bug2.c $(LIBS)			gcc $(CFLAGS) -DBKEND=PB_MIN -o bug2.pr bug2.c $(LIBS)

#bug3:	bug3.c 			gcc $(CFLAGS) -o bug3 bug3.c $(LIBS)			gcc $(CFLAGS) -DBKEND=PB_MIN -o bug3.pr bug3.c $(LIBS)
			
$(OBJS): %: %.c 
			gcc $(CFLAGS) -o $@ $< $(LIBS)
			gcc $(CFLAGS) -DBKEND=PB_MIN -o $@.pr $< $(LIBS)

run:
			./run.sh

clean:
#			rm -f bug1 bug2 bug3 bug1.pr bug2.pr bug3.pr
			rm -f $(OBJS) $(OBJS:%=%.pr)
