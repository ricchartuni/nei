CLIBS      = -lm

OBJDIR    = obj
BINDIR    = bin
CSRCMAIN  = $(wildcard M*.c)
CSRC      = $(wildcard [!M]*.c)
COBJMAIN  = $(addprefix $(OBJDIR)/,$(notdir $(CSRCMAIN:.c=.o)))
COBJ      = $(addprefix $(OBJDIR)/,$(notdir $(CSRC:.c=.o)))
COBJLOCAL = $(COBJMAIN) $(COBJ)
CBIN      = $(addprefix $(BINDIR)/,$(patsubst M%.c,%,$(CSRCMAIN)))

DBG       = -D_DEBUG___

OBJLOCAL  = $(notdir $(COBJLOCAL))
BINLOCAL  = $(notdir $(CBIN))

.PHONY: $(OBJLOCAL)
.PHONY: $(BINLOCAL)
.PHONY: all
.PHONY: MKDIR
.PHONY: clean

all: MKDIR $(OBJLOCAL) $(BINLOCAL)
clean:
	rm -f $(COBJLOCAL) $(CBIN)
MKDIR:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

#===================================================================
#-------------------------------------------------------------------
$(OBJLOCAL): %: $(OBJDIR)/%
#-------------------------------------------------------------------
$(OBJDIR)/M%.o: M%.c
	$(CC) -c $< $(CFLAGS) -o $@ $(CINCS) $(DBG)
#-------------------------------------------------------------------
$(OBJDIR)/%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@ $(CINCS) $(DBG)
#-------------------------------------------------------------------
#===================================================================
$(BINLOCAL):         %: $(BINDIR)/%
#-------------------------------------------------------------------
$(BINDIR)/%: $(OBJDIR)/M%.o $(COBJ)
	$(CC) -o $@ $^ $(CLIBPATH) $(CLIBS)
#-------------------------------------------------------------------
#===================================================================
