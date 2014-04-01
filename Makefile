CXXFLAGS = -std=c++11 -Wall -Wextra
SRC = test_fsm.cpp
TARGET = test

gcc:
	g++ $(CXXFLAGS) $(SRC) -o $(TARGET)

clang:
	clang++ $(CXXFLAGS) $(SRC) -o $(TARGET)

all: gcc

clean:
	$(RM) $(TARGET)
