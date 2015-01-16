CC = gcc
CFLAGS = -std=gnu99 -I$(IDIR) -g
LDFLAGS = -lncursesw -lmenuw -lpthread -lrt

EPS_LDFLAGS = -lpthread -lrt
EPS_LIBS = /lib/i386-linux-gnu/libncursesw.so.5.9 /usr/lib/i386-linux-gnu/libmenuw.so.5.9

SDIR = src
IDIR = include
ODIR = src/obj
BDIR = .

_DEPS = boss.h color.h combat.h combat_interface.h enemy.h item.h layout.h messages.h physics.h platform.h player.h skill.h types.h utils.h weapon.h world.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = boss.o color.o combat.o combat_interface.o enemy.o item.o layout.o main.o physics.o platform.o player.o skill.o utils.o weapon.o world.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_BIN = TheEigenQuest
BIN = $(patsubst %,$(BDIR)/%,$(_BIN))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@if [ ! -d "$(ODIR)" ]; then mkdir -p $(ODIR); fi	
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN):%: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

eps: $(OBJ)
	$(CC) -o $(BIN) $^ $(EPS_LIBS) $(EPS_LDFLAGS)

play_tuto:
	$(BIN) TUTO

play_lvl1:
	$(BIN) LVL1

.PHONY: clean
clean:
	@rm -fv $(BIN) $(OBJ) *~ $(IDIR)/*~
	@if [ -d "$(ODIR)" ]; then rm -rv $(ODIR); fi
