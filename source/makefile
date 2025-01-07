CC = gcc
CFLAGS = -Wall -g
LIBRARY = -lncurses

BUILDDIR = /home/tester/Projects/C/ChatCurses/build
APPDIR = /home/tester/Projects/C/ChatCurses/app

SOURCE = main.c utiln.c utilc.c login.c chat.c client.c
OBJECTS = $(patsubst %.c, $(BUILDDIR)/%.o, $(SOURCE))
PROJECT = chatting

all: $(BUILDDIR)/$(PROJECT)

# 각 소스 파일을 개별 오브젝트 파일로 컴파일
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 오브젝트 파일을 링크하여 실행 파일 생성
$(BUILDDIR)/$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBRARY)

# 클린 타겟
clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/$(PROJECT)

# 실행 파일 복사 타겟
upver: all
	cp $(BUILDDIR)/$(PROJECT) $(APPDIR)/$(PROJECT)

