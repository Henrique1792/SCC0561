CC = gcc
SRCS = bitmap.c pixel.c utils.c
MAIN_SRC = main.c
TGT = T1
COLORS = red.bmp green.bmp blue.bmp bw.bmp
OBJ_FLAGS = -Wall -Wextra -g -c
OBJS = $(SRCS:.c=.o)



all: objs
	$(CC) $(OBJS) $(MAIN_SRC) -o $(TGT)

objs:
	$(CC)  $(OBJ_FLAGS) $(SRCS)

clean:
	rm $(OBJS) $(TGT) $(COLORS)

run:
	./$(TGT)
