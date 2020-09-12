CC	=	cc
INCLUDE	=	-Iinclude
OPT_FLAGS	=	-O3
CFLAGS	=	-W -Wall -Wextra -pipe $(OPT_FLAGS) $(INCLUDE)
LDFLAGS	=	$(shell sdl2-config --libs) -lm

SRC	=	src/main.c				\
		src/emulator.c			\
		src/logger.c			\
		src/system.c			\
		src/audio.c				\
		src/display.c			\
		src/cpu.c				\
		src/opcodes/opcode_0.c	\
		src/opcodes/opcode_1.c	\
		src/opcodes/opcode_2.c	\
		src/opcodes/opcode_3.c	\
		src/opcodes/opcode_4.c	\
		src/opcodes/opcode_5.c	\
		src/opcodes/opcode_6.c	\
		src/opcodes/opcode_7.c	\
		src/opcodes/opcode_8.c	\
		src/opcodes/opcode_9.c	\
		src/opcodes/opcode_A.c	\
		src/opcodes/opcode_B.c	\
		src/opcodes/opcode_C.c	\
		src/opcodes/opcode_D.c	\
		src/opcodes/opcode_E.c	\
		src/opcodes/opcode_F.c

OBJ	=	$(SRC:.c=.o)
DEP	=	$(OBJ:.o=.d)

BINARY	=	chip8

.PHONY:	all	clean	fclean	re

all:	$(BINARY)

$(BINARY):	$(OBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

%.o:	%.c
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

clean:
	rm -f $(OBJ) $(DEP)

fclean:	clean
	rm -f $(BINARY)

re:	fclean	all

-include $(DEP)