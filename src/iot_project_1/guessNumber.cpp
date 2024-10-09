#include <cstdlib>
#include <ctime>

static const int MAX = 15;
static const int MIN = 0;

static int currentNumber;

int generateNumber()
{
    std::srand(static_cast<unsigned>(std::time(0)));
    currentNumber = (std::rand() % MAX) + MIN;

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

    // Convert the integer to binary and compare with the booleans
    bool bit3 = currentNumber & 0b1000; // 4th bit (most significant)
    bool bit2 = currentNumber & 0b0100; // 3rd bit
    bool bit1 = currentNumber & 0b0010; // 2nd bit
    bool bit0 = currentNumber & 0b0001; // 1st bit (least significant)

    return (bit3 == b3) && (bit2 == b2) && (bit1 == b1) && (bit0 == b0);
}
