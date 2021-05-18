CC = gcc
SRCS = RLE.c bitmap.c pixel.c utils.c
MAIN_SRC = main.c
TGT = T1
COLORS = red.bmp green.bmp blue.bmp bw.bmp out.bin
LD_FLAGS = -lm
OBJ_FLAGS = -Wall -Wextra -g -c 
OBJS = $(SRCS:.c=.o)



all: objs
	$(CC)  $(MAIN_SRC) -o $(TGT) $(LD_FLAGS) $(OBJS)

objs:
	$(CC)  $(OBJ_FLAGS) $(SRCS) $(LD_FLAGS)

clean:
	rm $(OBJS) $(TGT) $(COLORS) $(MAIN_SRC:.c=.o)

run:
	./$(TGT)
