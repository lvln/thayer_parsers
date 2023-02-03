HFILES= pp_xml.h
OFILES= main.o pp_xml.o
CFLAGS= -Wall -pedantic -std=c11 \
				-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include\
				-I/usr/local/include -I/usr/local/include -L/usr/local/lib

all:		$(OFILES) main_pr.o
				ar cr libppxml.a $(OFILES)
				ar cr libppxml_pr.a main_pr.o pp_xml.o

main.o: main.c
				gcc $(CFLAGS) -c $<
main_pr.o: main.c
				gcc $(CFLAGS) -DBKEND=PB_MIN -c $< -o $@

%.o:		%.c %.h
				gcc $(CFLAGS) -c $<

clean:
				rm -f *~ *.o libppxml.a libppxml_pr.a




