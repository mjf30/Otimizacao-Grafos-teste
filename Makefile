CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

BINARIES = grasp ils tabu simulated_annealing leitura_exec

all: $(BINARIES)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(BINARIES)
