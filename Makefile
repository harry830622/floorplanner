EXE = fp
CXX = clang++
CXXFLAGS = -std=c++11 -O3 -Wall
CPPS := $(wildcard src/*.cpp)
OBJS := $(addprefix obj/,$(notdir $(CPPS:.cpp=.o)))
LDLIBS = -lboost_program_options -lboost_system -lboost_filesystem

.PHONY: all clean

all: obj $(EXE)

obj:
	mkdir $@

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

clean:
	rm $(EXE) $(OBJS)
