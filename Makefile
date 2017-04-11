EXE = fp
CXX = clang++
CXXFLAGS = -std=c++11 -O3 -Wall
CPPS := $(wildcard src/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(CPPS:.cpp=.o)))

.PHONY: all clean

all: obj $(EXE)

obj:
	mkdir $@

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

clean:
	rm $(EXE) $(OBJS)
