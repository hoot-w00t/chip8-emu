CC	=	cc
INCLUDE	=	-Iinclude
CFLAGS	=	-W -Wall -Wextra -O2 -pipe $(INCLUDE)
LDFLAGS	=	$(shell sdl2-config --libs)

SRC	=	main.c				\
		emulator.c			\
		logger.c			\
		system.c			\
		audio.c				\
		display.c			\
		cpu.c				\
		opcodes/opcode_0.c	\
		opcodes/opcode_1.c	\
		opcodes/opcode_2.c	\
		opcodes/opcode_3.c	\
		opcodes/opcode_4.c	\
		opcodes/opcode_5.c	\
		opcodes/opcode_6.c	\
		opcodes/opcode_7.c	\
		opcodes/opcode_8.c	\
		opcodes/opcode_9.c	\
		opcodes/opcode_A.c	\
		opcodes/opcode_B.c	\
		opcodes/opcode_C.c	\
		opcodes/opcode_D.c	\
		opcodes/opcode_E.c	\
		opcodes/opcode_F.c

OBJ	=	$(SRC:%.c=obj/%.o)
DEP	=	$(OBJ:.o=.d)

BINARY	=	chip8

ifdef DEBUG
	CFLAGS	+=	-g
endif

.PHONY:	all	clean	fclean	re

all:	$(BINARY)

$(BINARY):	$(OBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

obj/%.o:	src/%.c
	@mkdir -p "$(shell dirname -- $@)"
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

clean:
	rm -rf obj

fclean:	clean
	rm -f $(BINARY)

re:	fclean	all

-include $(DEP)