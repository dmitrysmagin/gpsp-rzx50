# gpSP makefile
# Gilead Kutnick - Exophase

# Global definitions

CC        = gcc
STRIP     = strip
AS        = as

PREFIX    = 
OBJS      = main.o cpu.o memory.o video.o input.o sound.o \
	    cpu_threaded_sl.o gui.o x86_stub.o cheats.o zip.o
BIN       = gpsp.exe 

# Platform specific definitions 

VPATH      += ..
INCLUDES   = -I/include/SDL
CFLAGS     += -DDINGUX_ON_WIN32 ${INCLUDES}
LIBS       = -mconsole -lz -lSDL

# Compilation:

.SUFFIXES: .c .S

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o: %.S
	${AS} -o $@ $<

all:	${OBJS}
	${CC} ${OBJS} ${LIBS} -o ${BIN}  
	${STRIP} ${BIN}

clean:
	rm -f *.o ${BIN} 

