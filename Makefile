CC = g++
BUILD_DIR = build
CFLAGS = -std=c++11 

OBJECTS = \
	$(BUILD_DIR)/Main.o 

MainSensitivityList = \
	main.cpp 

all: $(BUILD_DIR) topology.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

topology.out: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o topology.out

$(BUILD_DIR)/Main.o: $(MainSensitivityList)
	$(CC) -c main.cpp $(CFLAGS) -o $(BUILD_DIR)/Main.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out