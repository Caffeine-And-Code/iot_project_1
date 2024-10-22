/**
 * @author Senni Mattia, Fattori Fabio, Tonelli Francesco
 */
#include "DifficultFunc.hpp"

#define NUMBERS_OF_DIFFICULTIES 6
// 10 seconds by default
#define MILLISECONDS_STARTING_VALUE 10000
#define ANALOG_MAX_VALUE 1023
#define ANALOG_MIN_VALUE 0
#define PER

int difficulty = 0;

int getDifficult()
{
    return difficulty;
}

void setDifficult(int value)
{
    if (value >= ANALOG_MIN_VALUE && value <= ANALOG_MAX_VALUE)
    {
        for (int i = 0; i < NUMBERS_OF_DIFFICULTIES; i++)
        {
            int calculatedValue = (ANALOG_MAX_VALUE / NUMBERS_OF_DIFFICULTIES) * (i + 1);
            if (i == NUMBERS_OF_DIFFICULTIES - 1)
            {
                calculatedValue = ANALOG_MAX_VALUE;
            }
            if (value <= calculatedValue)
            {
                difficulty = i + 1;
                return;
            }
        }
    }
}

int getMillisecondsBasedByScore(int score)
{
    int time = MILLISECONDS_STARTING_VALUE - (MILLISECONDS_STARTING_VALUE * (((double)score) / 100.0 * difficulty * 2));
    return time < 0 ? 0 : time;
}