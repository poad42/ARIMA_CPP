CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++14
LDFLAGS = -lboost_math_c99 -lboost_math_c99f

all: main

main: main.o data_loader.o adf_test.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.cpp data_loader.cpp adf_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

data_loader.o: data_loader.cpp data_loader.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

adf_test.o: adf_test.cpp adf_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o main