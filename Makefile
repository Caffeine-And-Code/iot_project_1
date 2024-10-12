# Variabili
CXX = g++
CXXFLAGS = -Wall -g
SRC_DIR = ./src
PROJECT_DIR = $(SRC_DIR)/iot_project_1
TEST_DIR = $(SRC_DIR)/Tests

# File sorgenti
SRCS = $(TEST_DIR)/Tester.cpp $(PROJECT_DIR)/DifficultFunc.cpp $(PROJECT_DIR)/guessNumber.cpp
OBJS = $(SRCS:.cpp=.o)

# Nome dell'eseguibile
TARGET = progetto

# Obiettivo di default: costruire l'eseguibile
all: $(TARGET)

# Regola per creare l'eseguibile
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regola per compilare i file sorgente in file oggetto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Pulizia dei file generati
clean:
	rm -f $(OBJS) $(TARGET)
