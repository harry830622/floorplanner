EXE = fp
CXX = clang++
CXXFLAGS = -std=c++11 -O2 -Wall
CPPS := $(wildcard src/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(CPPS:.cpp=.o)))

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean

clean:
	rm $(EXE) $(OBJS)
