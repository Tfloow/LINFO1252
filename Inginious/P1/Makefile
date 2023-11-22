FLAGS=-Wall -Werror -g
LIBS=-lpthread

FILE_SRC = part1
EXEC = $(FILE_SRC:%=%)

.PHONY: part1 clean

all: $(EXEC)

part1: src/part1.c
	@gcc $(FLAGS)  $< -o $@ $(LIBS)
	@./$@

part2: src/part2.c
	gcc $(FLAGS)  $< -o $@ $(LIBS)
	./$@


clean:
	@rm -f *.out
	@rm -f part1
	@rm -f part1_cmpl


