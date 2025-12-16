CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Werror
SRC_DIR = src
TEST = test.cpp
NAME = array.exe

all:
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/$(TEST) -o $(NAME)

clean:
	@rm -f $(NAME)