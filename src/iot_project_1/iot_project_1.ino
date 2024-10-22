/**
 * The main arduino file
 * @author Senni Mattia, Fattori Fabio, Tonelli Francesco
 * Group members:
 * Senni Mattia, 0001079869
 * Fattori Fabio, 0001071463
 * Tonelli Francesco, 0001071531
 */

#include "controllers.hpp"
#include <LiquidCrystal_I2C.h>
#include "math.h"
#include <avr/sleep.h>

#define GO_PROMPT_TIME 1000
#define GAME_OVER_TIME 10000
#define SLEEP_MODE_TIME 10000
#define CORRECT_ANSWER_TIME 2000
#define INTERUPT_DELAY 40

/**
 * When isTest is true, the application run's on test mode, that means
 * that it do not start with the real application but it start a demo
 * application that let the user check if components setup is corrects
 *
 * When isTest is false, just start the normal application
 */
const bool isTest = false;

const int statusLed = 5;
const int analogInPot = A2;

const int buttons[] = {3, 4, 6, 7};
const int leds[] = {8, 9, 10, 11};
bool isButtonPressed[] = {false, false, false, false};

// Status led Fading variables
int brightness = 0;
int fadeAmount = 12;

long prevB1PressCheck = 0;

unsigned long lastButtonClick = 0;
unsigned long lastMancheStartTime = 0;
bool mancheTimerEndEventTriggered = false;

/**
 * When app is in verbose mode it enable logging using log function, else it do not display log
 */
const bool verbose = true;

LiquidCrystal_I2C lcd(0x27, 20, 4);

/**
 * The setup and loop functions are in the bottom of the file
 */

void log(char *message)
{
  if (verbose)
  {
    Serial.println(message);
  }
}

void initSleepModeTimer()
{
  lastButtonClick = millis();
}

void checkSleepMode()
{
  for (int button : buttons)
  {
    if (digitalRead(button) == HIGH)
    {
      lastButtonClick = millis();
    }
  }
  if (millis() - lastButtonClick > SLEEP_MODE_TIME)
  {
    setSleep();
  }
}

void startMancheTimer()
{
  lastMancheStartTime = millis();
  mancheTimerEndEventTriggered = false;
}

bool checkMancheTimer()
{
  const int actualTime = millis() - lastMancheStartTime;
  if (!mancheTimerEndEventTriggered && actualTime >= getTimer())
  {
    // match finished
    mancheTimerEndEventTriggered = true;
    handleMoveTimerEnd();
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
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255)
  {
    if (brightness < 0)
    {
      brightness = 0;
    }
    if (brightness > 255)
    {
      brightness = 255;
    }
    fadeAmount = -fadeAmount;
  }
  analogWrite(statusLed, brightness);
}

void triggerStatusLed()
{
  digitalWrite(statusLed, HIGH);
}

void cleanScreen()
{
  int rows[] = {0, 1, 2, 3};
  for (int row : rows)
  {
    lcd.setCursor(0, row);
    lcd.print("                   ");
  }
}

void printWelcomeMessage()
{
  lcd.setCursor(0, 0);
  lcd.print("Welcome to          ");
  lcd.setCursor(0, 1);
  lcd.print("GMB!                ");
  lcd.setCursor(0, 2);
  lcd.print("Press B1 to Start   ");
  lcd.setCursor(0, 3);
  lcd.print("Diff: ");
  lcd.setCursor(6, 3);
  lcd.print(getCurrentDifficult());
  lcd.setCursor(8, 3);
  lcd.print("            ");
}

void printGo()
{
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("        Go!         ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}

void printNumber(int number)
{
  lcd.setCursor(0, 0);
  lcd.print("Guess ->  ");
  lcd.setCursor(10, 0);

  if (number < 10)
  {
    lcd.print("0");
  }
  lcd.print(number);
  lcd.setCursor(12, 0);
  lcd.print("              ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("SEC: ");
  lcd.setCursor(5, 3);
  auto time = getTimer();
  auto rt = (int)ceil((time - (millis() - lastMancheStartTime))/1000);
  lcd.print(rt);
  const int decs[] = {4, 3, 2, 1};
  int curr = 5;
  for (int dec : decs)
  {
    if (time < pow(10, dec))
    {
      curr++;
      lcd.print(" ");
    }
  }
}

void printGoodScore()
{
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("GOOD! Score: ");
  lcd.setCursor(13, 1);
  auto score = getScore();
  lcd.print(score);
  if (score < 10)
  {
    lcd.setCursor(14, 1);
    lcd.print(" ");
  }
  lcd.setCursor(15, 1);
  lcd.print("     ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}

void printGameOver(int score)
{
  cleanScreen();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Final Score: ");
  lcd.setCursor(14, 1);
  lcd.print(score);
}

void updatePotentiometerDifficult()
{
  int value = analogRead(analogInPot);
  updateDifficult(value);
}

void prepareSleepMode()
{
  for (int led : leds)
  {
    digitalWrite(led, LOW);
  }
  digitalWrite(statusLed, LOW);
  lcd.noBacklight();
  lcd.setCursor(0, 0);
  lcd.print("                     ");
  lcd.setCursor(0, 1);
  lcd.print("                     ");
  lcd.setCursor(0, 2);
  lcd.print("                     ");
  lcd.setCursor(0, 3);
  lcd.print("                     ");
}

void prepareForWakeUp()
{
  lcd.backlight();
}

void handleB1Press()
{
  long ts = millis();
  if (ts - prevB1PressCheck > INTERUPT_DELAY)
  {
    if (isWaitingForStart())
    {
      startGame();
    }
    prevB1PressCheck = ts;
  }
}

void gameRoutine()
{
  printNumber(getExtractedNumber());

  for (int i = 0; i < getLedNumber(); i++)
  {
    if (digitalRead(buttons[i]) == HIGH)
    {
      if (isButtonPressed[i] == false)
      {
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
  if (isGameStarted())
  {
    setIsWaitResult();
  }
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

  lcd.setCursor(0, 1);
  lcd.print("TEST: P: ");

  if (testPotentioMeterPV != potentioMeterValue)
  {
    lcd.setCursor(0, 2);
    lcd.print("    ");
    lcd.setCursor(0, 2);
    lcd.print(potentioMeterValue);
    testPotentioMeterPV = potentioMeterValue;
  }
}

void goToSleep()
{
  prepareSleepMode();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  prepareForWakeUp();
  startGame();
}

void setup()
{
  // Init all buttons as input
  for (int button : buttons)
  {
    pinMode(button, INPUT);
  }

  // Init all buttons as output
  for (int led : leds)
  {
    pinMode(led, OUTPUT);
  }

  pinMode(statusLed, OUTPUT);

  Serial.begin(9600);

  // Init Display and turn on the backlight
  lcd.init();
  lcd.backlight();

  // If app is not running in test mode, attach all app Interrupt
  // For checkout what is test mode, please refer to the relative comment on the top of the file
  if (!isTest)
  {
    attachInterrupt(digitalPinToInterrupt(buttons[0]), handleB1Press, RISING);
  }
}

void loop()
{
  if (isTest)
  {
    // If app start in test mode run test routine loop
    random(1, 10);
    testComponentLoop();
  }
  else
  {
    // The game work as a state machine where for each different state it do something different
    if (isSleeping())
    {
      // When app is in sleep enable sleep mode
      goToSleep();
      // Once sleep mode end, the game will go in wait for start state so init the sleep mode timer
      initSleepModeTimer();
    }
    else if (isWaitingForStart())
    {
      // If app is in wait for start he is waiting for a B1 press, then display Welcome message,
      // pulse status led and listen for potentiometer variations
      checkSleepMode();
      printWelcomeMessage();
      pulseStatusLed();
      updatePotentiometerDifficult();
    }
    else if (isAboutToStart())
    {
      // In about to start app print GO message and once ready start manche timer
      printGo();
      turnoOfAllLeds();
      delay(GO_PROMPT_TIME);
      startGame();
      startMancheTimer();
    }
    else if (isGameStarted())
    {
      // The game routing listen for button press and turn on relativa led's.
      // Then checkout manche timer for see if time is end
      gameRoutine();
      checkMancheTimer();
    }
    else if (isWaitResult())
    {
      // When wait to result, use move() method for submit and check if user answer correct
      move();
    }
    else if (isCorrectAnswer())
    {
      // If the previous check return success, congrats to user and display score number, then it restart manche timer for next round
      // It automatically decrease the timer of an amount
      printGoodScore();
      delay(CORRECT_ANSWER_TIME);
      resume();
      startMancheTimer();
    }
    else if (isGameOver())
    {
      // When previous check return failure, print game over message with relative current score, then, initialize the game
      printGameOver(getScore());
      triggerStatusLed();
      delay(GAME_OVER_TIME);
      startGame();
      // Restart wait for start sleep mode timer
      initSleepModeTimer();
    }
  }
}