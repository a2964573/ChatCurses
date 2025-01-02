CC = gcc
CFLAGS = -Wall -g
LIBRARY = -lncurses

BUILDDIR = /home/tester/Projects/C/Chat/build
APPDIR = /home/tester/Projects/C/Chat/app

SOURCE = main.c util.c login.c
OBJECTS = $(patsubst %.c, $(BUILDDIR)/%.o, $(SOURCE))
PROJECT = chatting

all: $(BUILDDIR)/$(PROJECT)

# �� �ҽ� ������ ���� ������Ʈ ���Ϸ� ������
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ������Ʈ ������ ��ũ�Ͽ� ���� ���� ����
$(BUILDDIR)/$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBRARY)

# Ŭ�� Ÿ��
clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/$(PROJECT)

# ���� ���� ���� Ÿ��
upver: all
	cp $(BUILDDIR)/$(PROJECT) $(APPDIR)/$(PROJECT)

