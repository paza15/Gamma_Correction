CFLAGS = -g -Wall -Wextra -std=c17 -O3 
LDFLAGS = -lm -msse4.1

.PHONY: all
all: main

main: main.c read.c parse.c gamma_V0.c write.c gamma_V1.c gamma_V2.c gamma_V3.c  gamma_V4.c benchmarking.c
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f main
