COMPILE = gcc -c
LINK = gcc
CLEANUP = rm -f
MKDIR = mkdir -p
INCDIR = include/
OBJDIR = obj/
SRCDIR = src/
SRC = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))
DEPS = $(wildcard $(INCDIR)*.h)
OUT = filestat
CCFLAGS = -I ./$(INCDIR) -Wall

.PHONY: all
.PHONY: clean
.PHONY: test

#######################################
#	Building executable
all: $(OBJDIR) $(OUT)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)%.c $(DEPS)
	$(COMPILE) -g -o $@ $< $(CCFLAGS)

$(OUT): $(OBJS)
	$(LINK) -o $@ $^ $(CCFLAGS)
#######################################

UNITYDIR = unity/src/
TESTDIR = test/
BUILD = build/
BUILD_DEP = build/depends/
BUILD_OBJ = build/objs/
BUILD_RES =	build/results/
DEPEND= gcc -MM -MG -MF
TESTFLASGS = -I . -I ./$(UNITYDIR) -I ./$(SRCDIR) -I ./$(INCDIR)

SRCT = $(wildcard $(TESTDIR)*.c)

RESULTS = $(patsubst $(TESTDIR)test_%.c, $(BUILD_RES)test_%.txt, $(SRCT))

PASSED = `grep -s PASS $(BUILD_RES)*.txt`
FAIL = `grep -s FAIL $(BUILD_RES)*.txt`
IGNORE = `grep -s IGNORE $(BUILD_RES)*.txt`

BUILD_PATHS = $(BUILD) $(BUILD_DEP) $(BUILD_OBJ) $(BUILD_RES)

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

# 2>&1 tells to make to combine stderr and stdout in stdout
$(BUILD_RES)%.txt: $(BUILD)%.out
	./$< > $@ 2>&1

$(BUILD)test_%.out: $(BUILD_OBJ)test_%.o $(BUILD_OBJ)%.o $(BUILD_OBJ)unity.o | $(BUILD_DEP)test_%.d
	$(LINK) -o $@ $^

$(BUILD_OBJ)%.o:: $(TESTDIR)%.c
	$(COMPILE) $(TESTFLASGS) $< -o $@

$(BUILD_OBJ)%.o:: $(SRCDIR)%.c
	$(COMPILE) $(TESTFLASGS) $< -o $@

$(BUILD_OBJ)%.o:: $(UNITYDIR)%.c $(UNITYDIR)%.h
	$(COMPILE) $(TESTFLASGS) $< -o $@

$(BUILD_DEP)%.d:: $(TESTDIR)%.c
	$(DEPEND) $@ $(TESTFLASGS) $<

$(BUILD):
	$(MKDIR) $(BUILD)

$(BUILD_DEP):
	$(MKDIR) $(BUILD_DEP)

$(BUILD_OBJ):
	$(MKDIR) $(BUILD_OBJ)

$(BUILD_RES):
	$(MKDIR) $(BUILD_RES)

clean:
	$(CLEANUP) $(BUILD_DEP)*.d
	$(CLEANUP) $(BUILD_OBJ)*.o
	$(CLEANUP) $(BUILD)*.out
	$(CLEANUP) $(BUILD_RES)*.txt
	$(CLEANUP) $(OBJDIR)*.o
	$(CLEANUP) $(OUT)

.PRECIOUS: $(BUILD)test_%.out
.PRECIOUS: $(BUILD_DEP)%.d
.PRECIOUS: $(BUILD_OBJ)%.o
.PRECIOUS: $(BUILD_RES)%.txt
