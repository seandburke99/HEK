########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = avr-gcc
CXXFLAGS = -mmcu=atmega328p
LDFLAGS = -Iinclude
DEV = /dev/ttyUSB0
AVRCONF = -C/etc/avrdude.conf
AVRLOAD = $(AVRCONF) -v -patmega328p -carduino -P$(DEV) -b57600 -D -Uflash:w:bin/$(APPNAME).hex:i

# Makefile settings - Can be customized.
APPNAME = HEK
EXT = .c
SRCDIR = src
OBJDIR = obj
INCDIR = include
DEPDIR = dep

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: bin $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o bin/$@.elf $^ $(LDFLAGS)
	avr-objcopy -j .text -j .data -O ihex bin/$(APPNAME).elf bin/$(APPNAME).hex

bin:
	mkdir $@

# Creates the dependecy rules
$(DEPDIR)/%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:$(DEPDIR)/%.d=$(OBJDIR)/%.o) >$@ $(LDFLAGS)

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $< $(LDFLAGS)

load: bin $(APPNAME)
	avrdude $(AVRLOAD)

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) -r $(DELOBJ) $(DEP) bin

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)
