NATIVE_CXX=g++
CXX=em++
COMMON_CXXFLAGS=-std=c++20 -O0 -Wall -Wextra -Wextra-semi
WASM_CXXFLAGS=$(COMMON_CXXFLAGS) -sNO_DISABLE_EXCEPTION_CATCHING -fexceptions
NATIVE_CXXFLAGS=$(COMMON_CXXFLAGS) -fsanitize=address,undefined
LDFLAGS=$(COMMON_CXXFLAGS) --preload-file assets --use-preload-plugins

WASM_BUILD_DIR=build/wasm
NATIVE_BUILD_DIR=build/native

OUTPUT=a.out.js
TEST_OUTPUT=$(NATIVE_BUILD_DIR)/test.elf

COMMON_SOURCES=Game.cpp Board.cpp Sprite.cpp Object.cpp
TEST_SOURCES=test/harness.cpp test/main.cpp test/clamping.cpp test/game.cpp \
			 test/board.cpp test/object.cpp $(COMMON_SOURCES)
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
	$(CXX) $(WASM_CXXFLAGS) -c $(<) -o $@
$(NATIVE_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(NATIVE_CXX) $(NATIVE_CXXFLAGS) -c $(<) -o $@
.PHONY: clang-tidy
clang-tidy:
	clang-tidy $(TEST_SOURCES) *.hpp -extra-arg=-std=c++20
cppcheck:
	cppcheck $(TEST_SOURCES) *.hpp main.cpp test/*.hpp
format:
	clang-format -i $(TEST_SOURCES) *.hpp main.cpp test/*.hpp
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS)
	rm -f *.wasm
	rm -f $(OUTPUT) $(TEST_OUTPUT)
	rm -r build
