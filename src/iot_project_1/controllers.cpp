#include <stdexcept>
#include "guessNumber.hpp"
#include "controllers.hpp"
#include "DifficultFunc.hpp"

#define WAITING_FOR_START 0
#define SLEEP_MODE 1
#define ABOUT_TO_START 2
#define IN_GAME 3
#define CORRECT_ANSWER 4
#define GAME_OVER 5

bool ledValues[] = {false, false, false, false};

short unsigned int gameState = 0;
// 0 -> Wait For Start
// 1 -> Sleep Mode
// 2 -> About To Start
// 3 -> In Game
// 4 -> CorrectAnswer
// 5 -> Game Over

int currentScore = 0;

bool isGameStarted()
{
    return gameState == IN_GAME;
}

bool isCorrectAnswer()
{
    return gameState == CORRECT_ANSWER;
}

bool isAboutToStart()
{
    return gameState == ABOUT_TO_START;
}

bool isSleeping()
{
    return gameState == SLEEP_MODE;
}

bool isGameOver()
{
    return gameState == GAME_OVER;
}

bool isWaitingForStart()
{
    return gameState == WAITING_FOR_START;
}

int getScore()
{
    return currentScore;
}

bool isLedActive(int index)
{
    if (isGameStarted())
    {
        if (index >= 0 && index < getLedNumber())
        {
            return ledValues[index];
        }
        else
        {
            throw std::invalid_argument("invalid index value");
        }
    }
    return false;
}

void startGame()
{
    if (isSleeping())
    {
        gameState = WAITING_FOR_START;
    }
    else if (isWaitingForStart())
    {
        gameState = ABOUT_TO_START;
    }
    else if (isAboutToStart())
    {
        gameState = IN_GAME;
        extractNumber();
    }
    else if (isGameOver())
    {
        gameState == ABOUT_TO_START;
    }
}

int getExtractedNumber()
{
    return getCurrentNumber();
}

void updateDifficult(int value)
{
    if (isWaitingForStart())
    {
        setDifficult(value);
    }
}

int getTimer()
{
    return getMillisecondsBasedByScore(currentScore);
}

int getCurrentDifficult()
{
    return getDifficult();
}

void pressButton(int index)
{
    if (isGameStarted())
    {
        if (index >= 0 && index < getLedNumber())
        {
            ledValues[index] = true;
        }
        else
        {
            throw std::invalid_argument("invalid index value");
        }
    }
}

bool move()
{
    bool correct = checkBinary(ledValues[0], ledValues[1], ledValues[2], ledValues[3]);
    if (correct)
    {
        resetLed();
        // TODO: NEXT NUMBER
        currentScore++;
        extractNumber();
        gameState = CORRECT_ANSWER;
    }
    else
    {
        gameState = GAME_OVER;
    }

    return correct;
}

void resume()
{
    gameState = IN_GAME;
}

void setSleep()
{
    if (!isGameStarted())
    {
        gameState = SLEEP_MODE;
    }
}

int getLedNumber()
{
    return sizeof(ledValues) / sizeof(ledValues[0]);
}

// private methods

int extractNumber()
{
    return generateNumber();
}

void resetLed()
{
    ledValues[0] = 0;
    ledValues[1] = 0;
    ledValues[2] = 0;
    ledValues[3] = 0;
}