CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := signal-echo

all: $(TARGET)

$(TARGET): signal_echo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)