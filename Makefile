CC=gcc
FILES=MIDI.c test.c
OPT=-Wall -std=c99
DBG=-g
OUT=test

build: $(FILES)
	$(CC) -o $(OUT) $(FILES) $(OPT)

debug: $(FILES)
	$(CC) -o $(OUT) $(FILES) $(OPT) $(DBG)

clean:
	rm -f *.o core

rebuild: clean build
