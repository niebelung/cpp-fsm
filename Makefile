all: test_fsm.cpp
	g++ -std=c++11 -Wall -Wextra test_fsm.cpp -o test

clean:
	$(RM) test