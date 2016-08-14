EXE = fp
CXX = clang++
CXXFLAGS = -std=c++14 -O2 -Wall -g
CPPS := $(wildcard src/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(CPPS:.cpp=.o)))

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean

clean:
	rm $(EXE) $(OBJS)
