CFLAGS=-g -Wall -Werror

all: tests lib_tar.o

lib_tar.o: lib_tar.c lib_tar.h

tests: tests.c lib_tar.o

clean:
	rm -f lib_tar.o tests soumission.tar

submit: all
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c *.h *.c Makefile > soumission.tar

easy: all 
	tar -cf test.tar tarInfo.txt
	./tests test.tar
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c tarInfo.txt >   archive.tar
	./tests archive.tar
