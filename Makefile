NATIVE_CXX=g++
CXX=em++
OPTIMIZATION_FLAGS=-O2
COMMON_CXXFLAGS=-std=c++20 -Wall -Wextra -Wextra-semi

DEBUG_CXXFLAGS=$(COMMON_CXXFLAGS) -sNO_DISABLE_EXCEPTION_CATCHING -fexceptions
RELEASE_CXXFLAGS=$(COMMON_CXXFLAGS) -O2
TEST_CXXFLAGS=$(COMMON_CXXFLAGS) -fsanitize=address,undefined
PERF_CXXFLAGS=$(COMMON_CXXFLAGS) -O2 -flto

LDFLAGS=$(COMMON_CXXFLAGS) --preload-file assets --use-preload-plugins

DEBUG_BUILD_DIR=build/debug
RELEASE_BUILD_DIR=build/release
TEST_BUILD_DIR=build/test
PERF_BUILD_DIR=build/perf

OUTPUT=a.out.js
TEST_OUTPUT=$(TEST_BUILD_DIR)/test.elf
PERF_OUTPUT=$(PERF_BUILD_DIR)/test.elf

COMMON_SOURCES=Game.cpp Board.cpp Sprite.cpp Object.cpp
SOURCES=main.cpp $(COMMON_SOURCES)
TEST_SOURCES=test/harness.cpp test/main.cpp test/game.cpp \
			 test/board.cpp test/object.cpp $(COMMON_SOURCES)

TEST_OBJECTS=$(TEST_SOURCES:%.cpp=$(TEST_BUILD_DIR)/%.o)
PERF_OBJECTS=$(TEST_SOURCES:%.cpp=$(PERF_BUILD_DIR)/%.o)
DEBUG_OBJECTS=$(SOURCES:%.cpp=$(DEBUG_BUILD_DIR)/%.o)
RELEASE_OBJECTS=$(SOURCES:%.cpp=$(RELEASE_BUILD_DIR)/%.o)

all: build $(OUTPUT)
build:
	mkdir -p $(DEBUG_BUILD_DIR)
	mkdir -p $(RELEASE_BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)/test
	mkdir -p $(PERF_BUILD_DIR)/test
test: build $(TEST_OUTPUT)
	./$(TEST_OUTPUT)
perf: build $(PERF_OUTPUT)
	./$(PERF_OUTPUT)
$(TEST_OUTPUT): $(TEST_OBJECTS) *.hpp
	$(NATIVE_CXX) $(TEST_CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_OUTPUT)
$(PERF_OUTPUT): $(PERF_OBJECTS) *.hpp
	$(NATIVE_CXX) $(PERF_CXXFLAGS) $(PERF_OBJECTS) -o $(PERF_OUTPUT)
$(OUTPUT): $(DEBUG_OBJECTS) *.hpp
	$(CXX) $(LDFLAGS) $(DEBUG_OBJECTS)
	echo "Built!"
$(DEBUG_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(CXX) $(DEBUG_CXXFLAGS) -c $(<) -o $@
$(RELEASE_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(CXX) $(RELEASE_CXXFLAGS) -c $(<) -o $@
$(TEST_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(NATIVE_CXX) $(TEST_CXXFLAGS) -c $(<) -o $@
$(PERF_BUILD_DIR)/%.o: %.cpp Makefile *.hpp
	$(NATIVE_CXX) $(PERF_CXXFLAGS) -c $(<) -o $@
.PHONY: clang-tidy
clang-tidy:
	clang-tidy $(TEST_SOURCES) *.hpp -extra-arg=-std=c++20
cppcheck:
	cppcheck $(TEST_SOURCES) *.hpp main.cpp test/*.hpp
format:
	clang-format -i $(TEST_SOURCES) *.hpp main.cpp test/*.hpp
clean:
	rm -f *.wasm
	rm -f $(OUTPUT) $(TEST_OUTPUT)
	rm -r build
