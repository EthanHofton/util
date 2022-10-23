CC = c++

#CXX flags
CXXFLAGS = -Wall -std=c++20
CXXFLAGS += -Isrc/

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
PROGRAM = out
BIN = bin

.PHONY: all clean

all: dirs libs $(PROGRAM)

run: all
	./$(BIN)/$(PROGRAM)

exe:
	./$(BIN)/$(PROGRAM)

dirs:
	mkdir -p ./$(BIN)

libs:

$(PROGRAM): $(OBJ)
	$(CC) -g -o $(BIN)/$(PROGRAM) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -g -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)