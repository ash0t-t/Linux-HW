# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -pthread
LDFLAGS := -pthread

LIB_TARGET := libparallel_scheduler.so
DEMO_TARGET := demo-application

LIB_SRC := parallel_scheduler.cpp
LIB_HDR := parallel_scheduler.hpp
DEMO_SRC := demo.cpp

all: $(LIB_TARGET) $(DEMO_TARGET)

$(LIB_TARGET): $(LIB_SRC) $(LIB_HDR)
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $(LIB_SRC)

$(DEMO_TARGET): $(DEMO_SRC) $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) -o $@ $(DEMO_SRC) -L. -lparallel_scheduler $(LDFLAGS)

clean:
	rm -f $(LIB_TARGET) $(DEMO_TARGET)