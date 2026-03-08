CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -O2 -g
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++17

# C程序目标
main: main.o
	$(CC) $(CFLAGS) $< -o $@

# C++程序目标（新加的）
hello: hello.o
	$(CXX) $(CXXFLAGS) $< -o $@

# 通用编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o main hello
	@echo "Clean all objects and executables"

.PHONY: print
print:
	@echo "SRC_C = $(wildcard *.c)"
	@echo "SRC_CPP = $(wildcard *.cpp)"
