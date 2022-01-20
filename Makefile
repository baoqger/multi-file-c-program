ENCODER = meow
DECODER = unmeow

TAEGET = $(ENCODER) $(DECODER)

SRC = main.c mmencode.c mmdecode.c
OBJ = $(SRC:.c=.o)
HDR = $(SRC:.c=.h) table.h

MANIFEST = $(SRC) $(HDR) README.md Makefile
VERSION = 0.1.0
SDIST_ROOT = meowmeow
SDIST_TARFILE = $(SDIST_ROOT)-$(VERSION).tar.gz

CC = cc
CP = cp
DIFF = diff
LN = ln
MKDIR = mkdir
RM = rm
TAR = tar

CFLAGS = -g
LDFLAGS =

# A phony target is one that is not really the name of a file; 
# rather it is just a name for a recipe to be executed when you make an explicit request.
.PHONY: sdist # there is . at the front of PHONY

all: $(TAEGET)

# the -o output name is $@ the target file name
$(ENCODER): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS)

#old fashioned suffix rule
#new way is pattern rule is: %.o: %.c $(HDR)
# $< is the dependency file name
# -c option means only run compilation no linking

.c.o: $(HDR)
	$(CC) -c $< $(CFLAGS)

#use ln command to build decoder
$(DECODER): $(ENCODER)
	$(LN) -f $< $@

sdist: clobber
	$(MKDIR) -p $(SDIST_ROOT)
	$(CP) $(MANIFEST) $(SDIST_ROOT)
	$(TAR) zcf $(SDIST_TARFILE) $(SDIST_ROOT)


# generate a readme file in MEOWMEOW format
README.meow: $(ENCODER) README.md
	./$(ENCODER) < README.md > $@

clean:
	$(RM) -f $(OBJ) *~ README.meow


clobber: clean
	$(RM) -f $(TAEGET) $(SDIST_TARFILE)
	$(RM) -rf $(SDIST_ROOT)


