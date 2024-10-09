#include "DifficultFunc.hpp"

using namespace std;

#define NUMBERS_OF_DIFFICULTIES 4
// 10 secondi di dafault
#define MILLISECONDS_STARTING_VALUE 10000

#define ANALOG_MAX_VALUE 1023

int difficulty = 0;

int getDifficult() {
    return difficulty;
}

void setDifficult(int value) {
    if (value >= 0 && value <= ANALOG_MAX_VALUE) {
        difficulty = value % NUMBERS_OF_DIFFICULTIES;
    }
}