CC = gcc
INCDIR = include
OBJDIR = obj
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DEPS = $(wildcard $(INCDIR)/*.h)
CLEAN = rm -f
CLEANDIR = rm -fd
MKDIR = mkdir -p
OUT = filestat
CCFLAGS = -I ./$(INCDIR) -Wall -D_FILE_OFFSET_BITS=64


all: $(OBJDIR) $(OUT)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CCFLAGS)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(CCFLAGS)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

clean:
	$(CLEAN) $(OBJDIR)/*.o
	$(CLEAN) $(OUT)
	$(CLEANDIR) $(OBJDIR)
