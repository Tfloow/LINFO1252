CFLAGS=-g -Wall -Werror

all: tests lib_tar.o

lib_tar.o: lib_tar.c lib_tar.h

tests: tests.c lib_tar.o

clean:
	@rm -f lib_tar.o tests soumission.tar
	@rm -f *.tar
	@rm -f lib_tar tests 
	@rmdir --ignore-fail-on-non-empty hard

submit: all
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c *.h *.c Makefile > soumission.tar

easy: all 
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c aled.txt >   test.tar
	./tests test.tar
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c tarInfo.txt aled.txt >   archive.tar
	./tests archive.tar

hard: all
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c hard > hard.tar
	./tests hard.tar

damage: all
	./tests damagedTar/ustar.tar
	@echo SHOULD BE -1
	./tests damagedTar/version.tar
	@echo SHOULD BE -2
	./tests damagedTar/chk.tar
	@echo SHOULD BE -3


