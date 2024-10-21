#include "controllers.hpp"
#include <LiquidCrystal_I2C.h>
#include<TimerOne.h>


#define GO_PROMPT_TIME 500
#define GAME_OVER_TIME 1000
#define CORRECT_ANSWER_TIME 500
#define INTERUPT_DELAY 40

const int statusLed = 5;
const int analogInPot = A2;
const bool isTest = false;

const int buttons[] = {3, 4, 6, 7};
const int leds[] = {8, 9, 10, 11};
bool isButtonPressed[] = {false, false, false, false};

// Status led Fading variables
int brightness = 0; // how bright the LED is
int fadeAmount = 5;
int Fadedelay = 30;
unsigned long lastMillis = 0;
bool alrPrintedWelcome = false;

const bool verbose = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void log(char* message){
  if(verbose){
    Serial.println(message);
  }
}

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
    
    log("Initialization finished");

    if(!isTest){
      log("Real app started");
      log("Attempt: setting random seed");
      //randomSeed(analogRead(0));      
      log("Finsh: setting random seed");
      log("Attempt: Setting interrup");
      //Timer1.attachInterrupt(handleMoveTimerEnd);
      attachInterrupt(digitalPinToInterrupt(buttons[0]), handleB1Press, RISING);
      log("Finsh: Setting interrup");
      Serial.print("cammon");

    }
    else {
      log("Test app started");
    }
}

void loop()
{
  log("Game super loop");
    if (isTest)
    {
      log("App in component test mode");
      random(1,10);
      testComponentLoop();
    }
    else
    {
        if (isSleeping())
        {
          log("App in sleep mode");
            // DO NOTHING, SLEEP
        }
        else if (isWaitingForStart())
        {
          log("App is waiting for start");
          log("Attemp: Printing welcome messages");
            printWelcomeMessage();
          log("Finish: Printing welcome messages");
          log("Attemp: pulse status led");
            pulseStatusLed();
            log("Finish: pulse status led");
            log("Attemp: check for potentiometer updates");
            updatePotentiometerDifficult();
            log("Finish: check for potentiometer updates");
        }
        else if (isAboutToStart())
        {
          log("App is about to start");
          log("Attemp: printing go");
            printGo();
            log("Finish: printing go");
            log("Attemp: turn off all leds");
            turnoOfAllLeds();
            log("Finsh: turn off all leds");
            log("Attemp: wait the necessary time for start");
            delay(GO_PROMPT_TIME);
            log("Finsh: wait the necessary time for start");
            log("Attemp: start game function call");
            startGame();
            log("Finsh: start game function call");
            log("Attemp: initializing game timer to new timer");
            Timer1.initialize(getTimer());
            Timer1.attachInterrupt(handleMoveTimerEnd);
            Timer1.start();
            log("Finish: initializing game timer to new timer");
        }
        else if (isGameStarted())
        {
          log("App in game");
          log("Attempt: game routine");
            gameRoutine();
          log("Finish: game routine");
        }
        else if (isCorrectAnswer())
        {
          log("App in correct answer");
          log("Attempt: wait currect answer timer");
            delay(CORRECT_ANSWER_TIME);
            log("Finsh: wait currect answer timer");
            log("Attempt: call to resume function");
            resume();
            log("Finish: wait currect answer timer");
            log("Attempt: initializing timer to new time");
            Timer1.initialize(getTimer());
            Timer1.attachInterrupt(handleMoveTimerEnd);
            Timer1.start();
            log("Finish: initializing timer to new time");
        }
        else if (isGameOver())
        {
          log("App in game over");
          log("Attempt: trigger status led");
            triggerStatusLed();
            log("Finish: trigger status led");
            log("Attempt: waiting game over timer");
            delay(GAME_OVER_TIME);
            log("Finsh: trigger status led");
            log("Attempt: restarting the game (call to startgame function)");
            startGame();
            log("Finish: restarting the game (call to startgame function)");
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
    if (currentMillis - lastMillis >= Fadedelay)
    {
        analogWrite(statusLed, brightness);
        brightness = brightness + fadeAmount;
        if (brightness == 0 || brightness == 255)
        {
            fadeAmount = -fadeAmount;
        }
      lastMillis = millis();
    }

    
}

void triggerStatusLed()
{
    digitalWrite(statusLed, HIGH);
}

void cleanScreen()
{
  int rows[] = {0,1,2,3};
  for (int row: rows){
    lcd.setCursor(0, row);
    lcd.print("                   ");
  }
}

void printWelcomeMessage()
{
    if (!alrPrintedWelcome)
    {
      alrPrintedWelcome = true;
      cleanScreen();
      lcd.setCursor(0, 1);
      lcd.print("Welcome to GMB!");
      lcd.setCursor(0, 2);
      lcd.print("Press B1, Start");
    }
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
long prevB1PressCheck = 0;
void handleB1Press()
{
  long ts = millis();
  if(ts - prevB1PressCheck > INTERUPT_DELAY){
    startGame();
    prevB1PressCheck = ts;
  }
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
            digitalWrite(leds[i], HIGH);
        }
        else
        {
            digitalWrite(leds[i], LOW);
        }
    }
}

void handleMoveTimerEnd()
{
  Serial.println("HOT GOD DOG");
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

int testPotentioMeterPV = 0;
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

    //cleanScreen();
    lcd.setCursor(0, 1);
    lcd.print("TEST: P: ");
    
    if(testPotentioMeterPV != potentioMeterValue){
      lcd.setCursor(0, 2);
      lcd.print("    ");
      lcd.setCursor(0, 2);
      lcd.print(potentioMeterValue);
      testPotentioMeterPV = potentioMeterValue;
    }

    
    
}