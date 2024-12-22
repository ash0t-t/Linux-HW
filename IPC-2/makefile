CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = prime-calculator

all: $(TARGET)

$(TARGET): prime_calculator.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) prime_calculator.o

prime_calculator.o: prime_calculator.cpp
	$(CXX) $(CXXFLAGS) -c prime_calculator.cpp

clean:
	rm -f *.o $(TARGET)