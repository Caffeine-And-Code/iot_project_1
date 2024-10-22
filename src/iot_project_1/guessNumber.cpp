/**
 * @author Senni Mattia, Fattori Fabio, Tonelli Francesco
 */
#include <Arduino.h>
#include "guessNumber.hpp"

static const int MAX = 16;
static const int MIN = 0;

static int currentNumber;

int generateNumber()
{
    randomSeed(micros());
    currentNumber = random(MIN, MAX);

    return currentNumber;
}

int getCurrentNumber()
{
    return currentNumber;
}

bool checkBinary(bool b3, bool b2, bool b1, bool b0)
{
    // Ensure the input integer is between 0 and 15 (4-bit numbers)
    if (currentNumber < 0 || currentNumber > 15)
    {
        return false;
    }

    int actNumber = 8 * b3 + 4 * b2 + 2 * b1 + b0;
    return currentNumber == actNumber;
}
