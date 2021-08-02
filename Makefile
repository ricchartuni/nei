CLIBS = -lm
SRC   = 01-modela.c
OBJ   = $(SRC:.c=.o)
BIN   = $(patsubst %.c,%,$(SRC))

.PHONY: all
.PHONY: obj
.PHONY: bin

all: obj bin
obj: $(OBJ)
$(OBJ): $(SRC)
	$(CC) -c $< $(CFLAGS) -o $@ $(CINCS)
bin: $(BIN)
$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CLIBPATH) $(CLIBS)

