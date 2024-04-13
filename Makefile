NATIVE_CXX=g++
CXX=em++
LDFLAGS=$(CXXFLAGS) --preload-file assets --use-preload-plugins
CXXFLAGS=-std=c++20 -O0 -Wall -Wextra
NATIVE_CXXFLAGS=$(CXXFLAGS) -fsanitize=address,undefined

WASM_BUILD_DIR=build/wasm
NATIVE_BUILD_DIR=build/native

OUTPUT=a.out.js
TEST_OUTPUT=$(NATIVE_BUILD_DIR)/test.elf

COMMON_SOURCES=Game.cpp Board.cpp Sprite.cpp Object.cpp
TEST_SOURCES=test/harness.cpp test/main.cpp test/clamping.cpp test/game.cpp test/board.cpp $(COMMON_SOURCES)
TEST_OBJECTS=$(TEST_SOURCES:%.cpp=$(NATIVE_BUILD_DIR)/%.o)
SOURCES=main.cpp $(COMMON_SOURCES)
OBJECTS=$(SOURCES:%.cpp=$(WASM_BUILD_DIR)/%.o)

all: build $(OUTPUT)
build:
	mkdir -p $(WASM_BUILD_DIR)
	mkdir -p $(NATIVE_BUILD_DIR)/test
test: build $(TEST_OUTPUT)
	./$(TEST_OUTPUT)
$(TEST_OUTPUT): $(TEST_OBJECTS) *.hpp
	$(NATIVE_CXX) $(NATIVE_CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_OUTPUT)
$(OUTPUT): $(OBJECTS) *.hpp
	$(CXX) $(LDFLAGS) $(OBJECTS)
	echo "Built!"
$(WASM_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(CXX) $(CXXFLAGS) -c $(<) -o $@
$(NATIVE_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(NATIVE_CXX) $(NATIVE_CXXFLAGS) -c $(<) -o $@
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS)
	rm -f *.wasm
	rm -f $(OUTPUT) $(TEST_OUTPUT)
	rm -r build
