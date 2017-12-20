CC=clang

EXEC_ISOC=test_isoc
EXEC_URANDOM=test_urandom

SRC=example.c

SRC_ISOC=$(SRC) diceroll/libdiceroll_isoc.a
SRC_URANDOM=$(SRC) diceroll/libdiceroll_posix.a

OPTS=-lm -g -Wall

all: $(EXEC_URANDOM) $(EXEC_ISOC)

$(EXEC_ISOC): $(SRC_ISOC)
	$(CC) $(SRC_ISOC) -o $(EXEC_ISOC) $(OPTS)

$(EXEC_URANDOM): $(SRC_URANDOM)
	$(CC) $(SRC_URANDOM) -o $(EXEC_URANDOM) $(OPTS)

.PHONY: clean

clean:
	rm -f $(EXEC_ISOC)
	rm -f $(EXEC_URANDOM)
