#!/bin/bash
clang json.c 4ch.c 4curses.c -o program -lcurl -ljansson -lcurses -g
#gcc json.o 4ch.c 4curses.c -o program -lcurl -ljansson -lncurses -g
