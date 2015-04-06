#
# Makefile
#   This makefile only works right now with gmake :(
#

SHELL   = bash
TARGET  = iiag
CC      = gcc
DESTDIR = /opt/iiag

# Compile and link flags
CCFL := -c -g -Wall -Werror
LDFL := -Wall -Werror -lm

# List of source files
SRCS := $(shell find src -name \*.c -type f -exec basename {} \;)

# List of tests
TESTS := $(shell find tests -name test -type f)

# Construct file lists
OBJS := $(addprefix build/obj/,$(patsubst %.c,%.o,$(SRCS)))
DEPS := $(addprefix build/dep/,$(patsubst %.c,%.d,$(SRCS)))
SRCS := $(addprefix src/,$(SRCS))

# All the make rules
.PHONY: all clean install test FORCE_RULE

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFL) -o $(TARGET)

build/obj/%.o: src/%.c
	@ mkdir -p $(@D)
	@ mkdir -p $(subst obj,dep,$(@D))
	@ $(CC) -MM -MP -MT $@ -MF $(patsubst %.o,%.d,$(subst obj,dep,$@)) $<
	$(CC) $(CCFL) $< -o $@

test: $(TESTS)
	@ echo All tests passed!

%/test: FORCE_RULE
	@ echo Testing $(subst tests/,,$(@D))...
	@ cd $(@D) && ./test

clean:
	rm -rf build
	rm -f $(TARGET)

install: all
	mkdir -p $(DESTDIR)
	cp $(TARGET) $(DESTDIR)

# Include automagically generated dependencies
-include $(DEPS)

