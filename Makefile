# FlowDesk — Portfolio Project #2
# Default compiler: Clang. Use `make gcc` to build with GCC.

CXX      ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -DFLOWDESK_COLOR
LDFLAGS  ?=
SRC       = src/main.cpp src/app.cpp src/ui.cpp src/ledger.cpp \
            src/transaction.cpp src/projector.cpp src/storage.cpp
BIN       = flowdesk

.PHONY: all clang gcc plain run clean

all: clang

clang:
	clang++ $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

gcc:
	g++ $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

plain:
	$(CXX) -std=c++17 -Wall -Wextra -Wpedantic -Iinclude $(SRC) -o $(BIN)

run: $(BIN)
	./$(BIN)

$(BIN): $(SRC) include/config.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

clean:
	rm -f $(BIN)
