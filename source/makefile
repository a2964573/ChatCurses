CC			= gcc
CFLAGS		= -Wall -g
LIBRARY		= -lncurses

CURRENTTIME	= $(shell date +"%Y%m%d.%H%M%S%3N")

CHATCURSES	= ~/Projects/C/ChatCurses
APPDIR		= $(CHATCURSES)/app
BUILDDIR	= $(CHATCURSES)/build
OBJECTDIR	= $(BUILDDIR)/$(PROJECT)_d

PROJECT		= chatting
SOURCE		= \
	main.c \
	utiln.c \
	utilc.c \
	login.c \
	chat.c \
	client.c
OBJECTS		= $(patsubst %.c, $(OBJECTDIR)/%.o, $(SOURCE))

LASTFILE    = $(shell ls -lrt $(BUILDDIR)/$(PROJECT).* | tail -n 1 | awk '{print$$NF}')

all: $(PROJECT)

$(OBJECTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BUILDDIR)/$(PROJECT).$(CURRENTTIME) $(LIBRARY)

clean:
	rm -f $(OBJECTS)

upver:
	cp -p $(LASTFILE) $(APPDIR)/$(PROJECT)

