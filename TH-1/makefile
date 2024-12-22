CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -pthread

TARGET := array-summary
SRC := array-summary.cpp
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)