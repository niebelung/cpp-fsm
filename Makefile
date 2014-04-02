CXXFLAGS = -std=c++11 -Wall -Wextra
ARM_LD_FLAGS = --specs=rdimon.specs -lg -lrdimon

SRC = test_fsm.cpp

TARGET = test
GCC_TGT = $(TARGET)_gcc
CLANG_TGT = $(TARGET)_clang
ARM_TGT = $(TARGET)_arm

gcc:
	g++ $(CXXFLAGS) $(SRC) -o $(GCC_TGT)

clang:
	clang++ $(CXXFLAGS) $(SRC) -o $(CLANG_TGT)

arm:
	arm-none-eabi-g++ $(CXXFLAGS) $(ARM_LD_FLAGS) $(SRC) -o $(ARM_TGT)

clean:
	$(RM) $(GCC_TGT) $(CLANG_TGT) $(ARM_TGT)
