NATIVE_CXX=g++
CXX=em++

COMMON_CXXFLAGS=-std=c++20 -Wall -Wextra -Wextra-semi

DEBUG_CXXFLAGS=$(COMMON_CXXFLAGS) -sNO_DISABLE_EXCEPTION_CATCHING -fexceptions
RELEASE_CXXFLAGS=$(COMMON_CXXFLAGS) -O3 -flto
TEST_CXXFLAGS=$(COMMON_CXXFLAGS) -fsanitize=address,undefined -Og
PERF_CXXFLAGS=$(COMMON_CXXFLAGS) -O2 -flto

DEBUG_LDFLAGS=$(DEBUG_CXXFLAGS) --preload-file assets --use-preload-plugins
RELEASE_LDFLAGS=$(RELEASE_CXXFLAGS) --preload-file assets --use-preload-plugins

DEBUG_BUILD_DIR=build/debug
RELEASE_BUILD_DIR=build/release
TEST_BUILD_DIR=build/test
PERF_BUILD_DIR=build/perf

DEBUG_OUTPUT=a.out.js
RELEASE_OUTPUT=publish/$(DEBUG_OUTPUT)
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

all: build $(DEBUG_OUTPUT)
release: build $(RELEASE_OUTPUT)
test: build $(TEST_OUTPUT)
	./$(TEST_OUTPUT)
perf: build $(PERF_OUTPUT)
	./$(PERF_OUTPUT)
build:
	mkdir -p $(DEBUG_BUILD_DIR)
	mkdir -p $(RELEASE_BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)/test
	mkdir -p $(PERF_BUILD_DIR)/test
$(TEST_OUTPUT): $(TEST_OBJECTS) *.hpp
	$(NATIVE_CXX) $(TEST_CXXFLAGS) $(TEST_OBJECTS) -o $(TEST_OUTPUT)
$(PERF_OUTPUT): $(PERF_OBJECTS) *.hpp
	$(NATIVE_CXX) $(PERF_CXXFLAGS) $(PERF_OBJECTS) -o $(PERF_OUTPUT)
$(DEBUG_OUTPUT): $(DEBUG_OBJECTS) *.hpp
	$(CXX) $(DEBUG_LDFLAGS) $(DEBUG_OBJECTS)
	echo "Built!"
$(RELEASE_OUTPUT): $(RELEASE_OBJECTS) *.hpp
	mkdir -p publish/
	cp index.html publish/
	cp style.css publish/
	$(CXX) $(RELEASE_LDFLAGS) $(RELEASE_OBJECTS) -o $(RELEASE_OUTPUT);
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
	clang-format -Werror -i $(TEST_SOURCES) *.hpp main.cpp test/*.hpp
check-format:
	clang-format --dry-run -Werror -i $(TEST_SOURCES) *.hpp main.cpp test/*.hpp
clean:
	rm -f *.wasm
	rm -f $(DEBUG_OUTPUT) $(TEST_OUTPUT)
	rm -r build
	rm -r publish
