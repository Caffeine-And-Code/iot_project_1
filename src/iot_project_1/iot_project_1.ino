#include "controllers.hpp"
#include <string>
#include LiquidCrystal_I2C.h

#define GO_PROMPT_TIME 500
#define GAME_OVER_TIME 1000
#define CORRECT_ANSWER_TIME 500

const int statusLed = 12;
const int analogInPot = A2;
const bool isTest = true;

const int buttons[] = {4, 5, 6, 7};
const int leds[] = {8, 9, 10, 11};
const bool isButtonPressed[] = {false, false, false, false};

// Status led Fading variables
int brightness = 0; // how bright the LED is
int fadeAmount = 5;
int delay = 30;
unsigned long lastMillis = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    for (int button : buttons)
    {
        pinMode(button, INPUT);
    }

    for (int led : leds)
    {
        pinMode(led, OUTPUT);
    }

    pinMode(statusLed, OUTPUT);

    Serial.begin(9600);

    lcd.init();
    lcd.backlight();
}

void loop()
{
    if (isTest)
    {
        testComponentLoop();
    }
    else
    {
        if (isSleeping())
        {
            // DO NOTHING, SLEEP
        }
        else if (isWaitingForStart())
        {
            printWelcomeMessage();
            pulseStatusLed();
            updatePotentiometerDifficult();
        }
        else if (isAboutToStart())
        {
            printGo();
            turnoOfAllLeds();
            delay(GO_PROMPT_TIME);
            startGame();
        }
        else if (isGameStarted())
        {
            gameRoutine();
        }
        else if (isCorrectAnswer())
        {
            delay(CORRECT_ANSWER_TIME);
            resume();
            // todo restart timer
        }
        else if (isGameOver())
        {
            triggerStatusLed();
            delay(GAME_OVER_TIME);
            startGame();
        }
    }
}

void turnoOfAllLeds()
{
    for (int led : leds)
    {
        digitalWrite(led, LOW);
    }

    digitalWrite(statusLed, LOW);
}

void pulseStatusLed()
{
    unsigned long currentMillis = millis();
    // adjust each delay seconds
    if (currentMillis - lastMillis >= delay)
    {
        analogWrite(statusLed, brightness);
        brightness = brightness + fadeAmount;
        if (brightness == 0 || brightness == 255)
        {
            fadeAmount = -fadeAmount;
        }
    }

    lastMillis = millis();
}

void triggerStatusLed()
{
    digitalWrite(statusLed, HIGH);
}

void cleanScreen()
{
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(1, 0);
    lcd.print("                ");
}

void printWelcomeMessage()
{
    cleanScreen();
    lcd.setCursor(0, 0);
    lcd.print("Welcome to GMB!");
    lcd.setCursor(1, 0);
    lcd.print("Press B1, Start");
}

void printGo()
{
    cleanScreen();
    lcd.setCursor(0, 0);
    lcd.print("GO!");
}

void printNumber(int number)
{
    cleanScreen();
    lcd.setCursor(0, 0);
    lcd.print(number);
}

void printGameOver(int score)
{
    cleanScreen();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(1, 0);
    lcd.print("Score: " + score);
}

void updatePotentiometerDifficult()
{
    int value = analogRead(analogInPot);
    updateDifficult(value);
}

/// HANDLERS

void handleB1Press()
{
    startGame();
}

void gameRoutine()
{
    printNumber(getExtractedNumber());

    for (int i = 0; i < getLedNumber(); i++)
    {
        // checkout pressed button
        if (digitalRead(buttons[i]) == HIGH)
        {
            if (isButtonPressed[i] == false)
            {
                // has started pressing now
                pressButton(i);
            }
            isButtonPressed[i] = true;
        }
        else
        {
            isButtonPressed[i] = false;
        }
    }

    for (int i = 0; i < getLedNumber(); i++)
    {
        if (isLedActive(i))
        {
            digitalWrite(i, HIGH);
        }
        else
        {
            digitalWrite(i, LOW);
        }
    }
}

void handleMoveTimerEnd()
{
    bool isCorrect = move();
    if (!isCorrect)
    {
        printGameOver(getScore());
    }
}

void hanndleSleepTimerEnd()
{
    setSleep();
}

void testComponentLoop()
{

    digitalWrite(statusLed, HIGH);
    int potentioMeterValue = analogRead(analogInPot);

    bool openButton[] = {false,
                         false,
                         false,
                         false};

    for (int i = 0; i < getLedNumber(); i++)
    {
        if (digitalRead(buttons[i]) == HIGH)
        {
            digitalWrite(leds[i], HIGH);
        }
        else
        {
            digitalWrite(leds[i], LOW);
        }
    }

    cleanScreen();
    lcd.setCursor(0, 0);
    lcd.print("TEST: P: " + potentioMeterValue);
    lcd.setCursor(1, 0);
}