CC = gcc
CFLAGS = -Iinclude
SRC = src/main.c src/cli.c
OUT = build/pacman

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	del /Q build\*.exe 2>nul || true
