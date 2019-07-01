CC = gcc
INCDIR = include
OBJDIR = obj
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
DEPS = $(wildcard $(INCDIR)/*.h)
CLEAN = rm -f
OUT = filestat
CCFLAGS = -I ./$(INCDIR) -Wall


all: $(OUT)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CCFLAGS)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(CCFLAGS)

clean:
	$(CLEAN) $(OBJDIR)/*.o
	$(CLEAN) $(OUT)
