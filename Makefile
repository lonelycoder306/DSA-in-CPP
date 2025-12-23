# General Makefile to compile all of the data structures in this project.

CXX := g++
CXXFLAGS := -g -O2 -Wall -Wextra \
			-Wno-unused-parameter -Wno-sign-compare -Wno-maybe-uninitialized \
			-Werror
AR = ar rcs

INCLUDE_DIR := include
SRC_DIR := src

ARRAY_NAME	:= array
ARRAY_DIR	:= Dynamic-Array

CHAIN_NAME	:= chainTable
CHAIN_DIR	:= Hash-Table/Separate-Chaining

LINEAR_NAME	:= linearTable
LINEAR_DIR	:= Hash-Table/Linear-Probing

LIST_NAME	:= linkedList
LIST_DIR	:= Linked-List/Singly-Linked-List/Regular

INCLUDE := -I$(ARRAY_DIR)/$(INCLUDE_DIR) -I$(CHAIN_DIR)/$(INCLUDE_DIR) \
			-I$(LINEAR_DIR)/$(INCLUDE_DIR) -I$(LIST_DIR)/$(INCLUDE_DIR)
LIB_FILE = lib.cpp
LIB_NAME = libdsa.a

EXEC_FILE := test.cpp
NAMES = $(ARRAY_NAME) $(CHAIN_NAME) $(LINEAR_NAME) $(LIST_NAME)
EXECS = $(NAMES)
LIBS = $(addprefix lib, $(addsuffix .a, $(NAMES)))

test: test-array test-chain test-linear test-list

lib: $(LIB_FILE)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(LIB_FILE) -o tmp.o
	@$(AR) $(LIB_NAME) tmp.o
	@rm -f tmp.o

test-array: $(ARRAY_DIR)/$(SRC_DIR)/$(EXEC_FILE)
	@$(CXX) $(CXXFLAGS) $< -o $(ARRAY_NAME)

lib-array: $(ARRAY_DIR)/$(INCLUDE_DIR)/$(ARRAY_NAME).cpp
	@$(CXX) $(CXXFLAGS) -c $< -o tmp.o
	@$(AR) lib$(ARRAY_NAME).a tmp.o
	@rm -f tmp.o

test-chain: $(CHAIN_DIR)/$(SRC_DIR)/$(EXEC_FILE)
	@$(CXX) $(CXXFLAGS) $< -o $(CHAIN_NAME)

lib-chain: $(CHAIN_DIR)/$(INCLUDE_DIR)/$(CHAIN_NAME).cpp
	@$(CXX) $(CXXFLAGS) -c $< -o tmp.o
	@$(AR) lib$(CHAIN_NAME).a tmp.o
	@rm -f tmp.o

test-linear: $(LINEAR_DIR)/$(SRC_DIR)/$(EXEC_FILE)
	@$(CXX) $(CXXFLAGS) $< -o $(LINEAR_NAME)

lib-linear: $(LINEAR_DIR)/$(INCLUDE_DIR)/$(LINEAR_NAME).cpp
	@$(CXX) $(CXXFLAGS) -c $< -o tmp.o
	@$(AR) lib$(LINEAR_NAME).a tmp.o
	@rm -f tmp.o

test-list: $(LIST_DIR)/$(SRC_DIR)/$(EXEC_FILE)
	@$(CXX) $(CXXFLAGS) $< -o $(LIST_NAME)

lib-list: $(LIST_DIR)/$(INCLUDE_DIR)/$(LIST_NAME).cpp
	@$(CXX) $(CXXFLAGS) -c $< -o tmp.o
	@$(AR) lib$(LIST_NAME).a tmp.o
	@rm -f tmp.o

clean-test:
	@rm -f $(EXECS)

clean-lib:
	@rm -f $(LIBS)

clean-gen-lib:
	@rm -f $(LIB_NAME)

re: clean all

.PHONY = all clean re