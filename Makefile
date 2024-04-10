CXX=em++
CXXFLAGS=-std=c++20 -O0 -Wall -Wextra

OUTPUT=a.out.js
TEST_OUTPUT=test/test.elf

TEST_SOURCES=test/harness.cpp test/main.cpp test/clamping.cpp
TEST_OBJECTS=$(TEST_SOURCES:%.cpp=build/native/%.o)
SOURCES=main.cpp GameData.cpp KeyPress.cpp
OBJECTS=$(SOURCES:%.cpp=build/wasm/%.o)

all: $(OUTPUT)
test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)
$(TEST_OUTPUT): $(TEST_OBJECTS) *.hpp
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_OUTPUT)
$(OUTPUT): $(OBJECTS) *.hpp
	$(CXX) $(CXXFLAGS) $(OBJECTS)
	echo "Built!"
build/wasm/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(^) -o $@
clean:
	rm -f $(OBJECTS)
	rm -f *.wasm
	rm -f $(OUTPUT)
