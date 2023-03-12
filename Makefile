CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++14
LDLIBS = -lboost_math_c99 -lboost_math_c99f -larmadillo

SRCS = main.cpp data_loader.cpp adf_test.cpp arima_modeling.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = main

.PHONY: depend clean

all: $(MAIN)
	@echo "Build complete."

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(OBJS) $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it