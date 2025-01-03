SUB_DIRS        = $(dir $(wildcard */makefile))
#SUB_DIRS       = $(wildcard */.)
#SUB_DIRS 	= day01 day02
SUB_DIRS_ALL    = $(SUB_DIRS:%=all-%)
SUB_DIRS_CLEAN  = $(SUB_DIRS:%=clean-%)

STD      = -std=gnu11
WARNINGS = -Wall -Wextra -Wpedantic -Wvla -Wconversion
CFLAGS   = $(STD) $(WARNINGS) -g
export CFLAGS

#
# Standard task
#
all: $(SUB_DIRS_ALL)

clean: $(SUB_DIRS_CLEAN)

.PHONY: lib
lib:
	@$(MAKE) $(MAKE_FLAGS) -C lib

# make lib before anything else (hence prereq on lib)
$(SUB_DIRS_ALL): lib
	@$(MAKE) $(MAKE_FLAGS) -C $(@:all-%=%)

$(SUB_DIRS_CLEAN):
	@$(MAKE) $(MAKE_FLAGS) -C $(@:clean-%=%) clean

# how declare that day01/, day02/ depend on lib/ and must not be built
# before it: ... nope, doesn't work.
# day01 day02: lib/
