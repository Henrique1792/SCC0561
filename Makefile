CC = gcc
SRCS = main.c deltaEncoding.c RLE.c bitmap.c pixel.c utils.c 
MAIN_SRC = main.c
TGT = T1
LD_FLAGS = -lm
OBJ_FLAGS = -Wall -Wextra -g -c 
OBJS = $(SRCS:.c=.o)



all: objs
	$(CC) $(OBJS)  $(LD_FLAGS) -o $(TGT)

objs:
	$(CC)  $(OBJ_FLAGS) $(SRCS) $(LD_FLAGS)

clean:
	rm $(OBJS) $(TGT) 

