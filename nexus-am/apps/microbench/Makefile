NAME = microbench
SRCS = $(shell find -L ./src/ -name "*.c" -o -name "*.cpp")

INPUT ?= REF
CFLAGS += -DSETTING_$(INPUT)
CXXFLAGS += -DSETTING_$(INPUT)

include $(AM_HOME)/Makefile.app
