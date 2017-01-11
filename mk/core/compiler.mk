ifndef COMPILER
COMPILER := gcc
else

#ifneq ($(COMPILER),)
#CC_TEMP := $(shell command -v $(COMPILER) 2> /dev/null)
#endif
#ifeq ($(wildcard $(CC_TEMP)),)
#$(error Compiler is not installed)
#endif


#ifeq ($(shell $(CC_TEMP) -v 2>&1 | grep -c "clang version"), 1)
#COMPILER := clang
#else
#COMPILER := gcc
#endif

endif
export COMPILER
