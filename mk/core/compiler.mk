$(info CC $(CC))
ifndef CC
COMPILER := gcc
CC := gcc
else

ifneq ($(CC),)

ifeq ($(wildcard $(CC)),)
$(error Compiler is not installed)
endif

ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang version"), 1)
COMPILER := clang
else
COMPILER := gcc
endif
endif

endif
export COMPILER
