#include "DifficultFunc.hpp"

#define NUMBERS_OF_DIFFICULTIES 4

int difficulty = 0;

int getDifficult() {
    return difficulty;
}

void setDifficult(int value) {
    if (value >= 0){
        difficulty = value / NUMBERS_OF_DIFFICULTIES;
    }
}