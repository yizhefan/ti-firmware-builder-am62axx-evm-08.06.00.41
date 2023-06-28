# Name of the output executable
OUTPUT = dccgen
# Adding include directory inc/
CFLAGS += -I"inc"

# Compiler?
CC = gcc
# Debugging?
CFLAGS += -g
# Building under linux?
CFLAGS += -D__LINUX__

CFLAGS += -O0 -Wall

# The following library can also be replaced by an already installed one.
# To install a supported library from the ubuntu repository use:

#sudo apt-get install libexpat1

# The current version included is from:
# libexpat1-2.0.1-7ubuntu1.deb

# Choose whether to use the installed library or the one 
# included in the project:

#LDFLAGS += -lexpat
LDFLAGS += lib/libexpat.so


CFILES  =   src/dcc.c		    \
	    src/tools.c             \
            src/dcc_header.c        \
	    src/typedef.c	    \
            src/general_data.c      \
            src/usecase.c           \
            src/write_header.c      \
	    src/write_dtp.c	    \
            src/write_parser.c

OBJECTS := $(CFILES:.c=.o)

all: $(OBJECTS)
	@echo Linking console executable: $(OUTPUT)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(OUTPUT)
	@rm -Rf src/*.o
.c.o:
	@echo Compiling: $<
	@$(CC) $(CFLAGS) -c $< -o $*.o
clean:
	@rm -Rfv $(OUTPUT) 2>/dev/null

