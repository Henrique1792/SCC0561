CC = gcc
SRCS = deltaEncoding.c RLE.c bitmap.c pixel.c utils.c 
MAIN_SRC = main.c
TGT = T1
LD_FLAGS = -lm
OBJ_FLAGS = -Wall -Wextra -g -c 
OBJS = $(SRCS:.c=.o)



all: objs
	$(CC)  $(MAIN_SRC) -o $(TGT) $(LD_FLAGS) $(OBJS)

objs:
	$(CC)  $(OBJ_FLAGS) $(SRCS) $(LD_FLAGS)

clean:
	rm $(OBJS) $(TGT) 

