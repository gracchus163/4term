#!/bin/bash
clang json.c 4ch.c 4term.c -o program -lcurl -ljansson -ltermbox -lc -g
#gcc json.o 4ch.c 4curses.c -o program -lcurl -ljansson -lncurses -g
