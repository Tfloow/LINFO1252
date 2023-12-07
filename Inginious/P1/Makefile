FLAGS=-Wall -Werror -g -Iinclude
LIBS=-lpthread
CC = gcc
CFLAGS=-Wall -Werror -g -O0
LIBS=-lpthread
INCLUDE_HEADERS_DIRECTORY=-Iinclude
FILE_SRC = philosopher prod-cons reader-writer test-and-set test-test-and-set sem philosopher_home prod-cons_home reader-writer_home
EXEC = $(FILE_SRC:%=%)

.PHONY: part1 clean

all: $(EXEC)


%.o:%.c
	@$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(MACROS) $(CFLAGS) -o $@ -c $< ${LIBS}

test:
#	Run and plot the various test
	@bash ./experiments.sh all plot

plot:
	@python3 perf/plot.py

philosopher: src/philosopher.c 
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

philosopher_home: src/philosopher_home.c include/sem.h
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

prod-cons: src/prod-cons.c
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

prod-cons_home: src/prod-cons_home.c include/sem.h
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

reader-writer: src/reader-writer.c include/sem.h
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

reader-writer_home: src/reader-writer_home.c include/sem.h
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

test-and-set: src/test-and-set.c
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

test-test-and-set: src/test-test-and-set.c include/test-test-and-set.h
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

sem: src/sem.c include/sem.h
	@gcc $(FLAGS)  $^ -o $@ $(LIBS)


clean:
	@rm -f *.out
	@rm -f *.o
	@rm -f *.s
	@rm -f $(EXEC)
	@rm -f *.png
	@rm -f src/rw src/pc
	@rm -f test
	@rm -f src/*.o
	@rm -f *.zip

deep-clean: clean
	@rm -f perf/plot/*.png
	@rm -f perf/plot/pdf/*.pdf

deep-deep-clean: clean 
	@rm -f perf/data/*.csv

archive: clean
	zip -r p1_matya_thomas.zip *

stdserv:
	python3 perf/stdserv.py
	make plot
