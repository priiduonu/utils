TOP = .
SRCS = $(shell ls *.cpp)
ifndef SSL
SRCS := $(filter-out SSL%,$(SRCS))
endif
OBJS = $(SRCS:%.cpp=%.o)
LIB = lib$(notdir $(CURDIR)).a
CPPFLAGS+=-g -Wall -W
CXXFLAGS=-std=c++14

lib: $(LIB)

$(LIB): $(OBJS)
	ar -rcs $@ $?

clean:
	rm -f *.a *.o
