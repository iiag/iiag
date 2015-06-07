#
# Makefile
#   This makefile only works right now with gmake :(
#

SHELL   = bash
TARGET  = iiag
CC      = gcc
DESTDIR = /opt/iiag

# Compile and link flags
CCFL := -g -Wall -Werror -Isrc
LDFL := -Wall -Werror -lm -lpthread

# List of source files
SRCS := $(subst src/,,$(shell find src -name \*.c -type f))

# List of all the source and headers, rooted at .
ALL_SRCS := $(shell find src \( -name \*.c -o -name \*.h \) -type f)

# List of all the "orig" files
ALL_ORIG := $(shell find src -name *.orig -type f)

# Construct file lists
OBJS := $(addprefix build/obj/,$(patsubst %.c,%.o,$(SRCS)))
DEPS := $(addprefix build/dep/,$(patsubst %.c,%.d,$(SRCS)))
SRCS := $(addprefix src/,$(SRCS))

# List of tests
TESTS := $(shell find tests -name test.c -type f)

# Object files to compile with tests, don't want conflicting mains
TEST_OBJS = $(filter-out build/obj/main.o,$(OBJS))

# The program that runs the tests, can be an empty string
TESTER = valgrind -q

# All the make rules
.PHONY: all clean install test style style_clean FORCE_RULE

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFL) -o $(TARGET)

build/obj/%.o: src/%.c
	@ mkdir -p $(@D)
	@ mkdir -p $(subst obj,dep,$(@D))
	@ $(CC) $(CCFL) -MM -MP -MT $@ -MF $(patsubst %.o,%.d,$(subst obj,dep,$@)) $<
	$(CC) -c $(CCFL) $< -o $@

test: all $(TESTS)
	@ echo All tests passed!

%/test.c: FORCE_RULE
	@ mkdir -p build/$(shell dirname $(@D))
	@ echo testing $(subst tests/,,$(@D)):
	@ echo " > building..."
	@ $(CC) $(CCFL) $(LDFL) -g -Isrc -I$(@D) $(@D)/*.c $(TEST_OBJS) -o build/$(@D)
	@ echo " > running..."
	@ $(TESTER) build/$(@D)

clean: style_clean
	rm -rf build
	rm -f $(TARGET)

install: all
	mkdir -p $(DESTDIR)
	cp $(TARGET) $(DESTDIR)

style: $(ALL_SRCS)
	astyle -tSjp -A2 -k3 -W3 $^

style_clean: $(ALL_ORIG)
	rm -f $^

# Include automagically generated dependencies
-include $(DEPS)

