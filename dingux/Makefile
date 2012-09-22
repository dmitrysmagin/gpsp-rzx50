# gpSP makefile
# Gilead Kutnick - Exophase
# GP2X port(ion) - Z

# Global definitions

PREFIX  = 
CC        = mipsel-linux-gcc
STRIP     = mipsel-linux-strip

OBJS      = main.o cpu.o memory.o video.o input.o sound.o gui.o \
            cheats.o zip.o cpu_threaded.o mips_stub.o bios.o
BIN       = gpsp.dge 

# Platform specific definitions 

EXTRA_OPT = 

COMMON_OPT = -g
COMMON_OPT += -O2 \
	-falign-functions -falign-loops -falign-labels -falign-jumps \
	-fomit-frame-pointer -fno-builtin -fno-common


VPATH      += ..
CFLAGS     += -DZAURUS $(EXTRA_OPT)
# NOTE: -funroll-loops will slow down compiling considerably
CFLAGS     += -march=mips32 -G0 -mtune=r4600
CFLAGS     += $(COMMON_OPT)

ASFLAGS    += $(COMMON_OPT)
ASFLAGS    += -march=mips32

INCLUDES   = -I${PREFIX}/include -I${PREFIX}/include/SDL
LIBS       = -lSDL -lpthread -lz

# Compilation:

.SUFFIXES: .c

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(ASFLAGS) $(INCLUDES) -c -o $@ $<

all: $(BIN)

$(BIN) : $(OBJS)
	$(CC) $(COMMON_OPT) $(OBJS) $(LIBS) -o $(BIN)  

clean:
	rm -f *.o *.u *.z $(BIN) 

strip:
	$(STRIP) $(BIN)

release: $(BIN) strip	
