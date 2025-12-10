# General Makefile to compile all of the data structures in this project.

CXX := g++
CXXFLAGS := -O2 -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Werror

ARRAY_DIR	:= (Dynamic)\ Array
ARRAY_EXEC	:= array.exe
CHAIN_DIR	:= Hash\ Table/Separate\ Chaining
CHAIN_EXEC	:= chain.exe
LINEAR_DIR	:= Hash\ Table/Linear\ Probing
LINEAR_EXEC	:= linear.exe
LIST_DIR	:= Linked\ List/Singly\ Linked\ List/Regular
LIST_EXEC	:= list.exe

SRC_DIR := src
FILE := test.cpp
EXECS = $(ARRAY_EXEC) $(CHAIN_EXEC) $(LINEAR_EXEC) $(LIST_EXEC)

all: $(EXECS)

$(ARRAY_EXEC) : $(ARRAY_DIR)/$(SRC_DIR)/$(FILE)
	@$(CXX) $(CXXFLAGS) "$<" -o $@

$(CHAIN_EXEC) : $(CHAIN_DIR)/$(SRC_DIR)/$(FILE)
	@$(CXX) $(CXXFLAGS) "$<" -o $@

$(LINEAR_EXEC) : $(LINEAR_DIR)/$(SRC_DIR)/$(FILE)
	@$(CXX) $(CXXFLAGS) "$<" -o $@

$(LIST_EXEC) : $(LIST_DIR)/$(SRC_DIR)/$(FILE)
	@$(CXX) $(CXXFLAGS) "$<" -o $@

clean:
	@rm -f $(EXECS)

re: clean all

.PHONY = all clean re