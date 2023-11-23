FLAGS=-Wall -Werror -g
LIBS=-lpthread

FILE_SRC = philosopher prod-cons reader-writer
EXEC = $(FILE_SRC:%=%)

.PHONY: part1 clean

all: $(EXEC)

test:
#	Run and plot the various test
	@bash ./perf/eval.sh all plot

philosopher: src/philosopher.c
	@gcc $(FLAGS)  $< -o $@ $(LIBS)
#	@./$@

part2: src/part2.c
	gcc $(FLAGS)  $< -o $@ $(LIBS)
	./$@

prod-cons: src/prod-cons.c
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

reader-writer: src/reader-writer.c
	@gcc $(FLAGS)  $< -o $@ $(LIBS)

clean:
	@rm -f *.out
	@rm -f *.o
	@rm -f $(EXEC)
	@rm -f *.png
