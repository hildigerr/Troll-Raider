
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -lm -lncurses
SRCS =  rmhv_stdlib.c calc.c cwin.c map.c player.c item.c game.c trollraider.c
DATA =  item.dat human.dat 

HEADERS = ${SRCS:.c=.h}
OBJS = ${SRCS:.c=.o}

#Primary Targets
test: ${OBJS}
	${CC} -g ${OBJS} ${LFLAGS}

trollraider: ${OBJS}
	${CC} -o ${@} ${OBJS}

clean: 
	rm a.out ${OBJS}

