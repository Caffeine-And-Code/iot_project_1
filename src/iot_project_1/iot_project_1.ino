/*
Group members:
Senni Mattia, 0001079869
Fattori Fabio, 0001071463
Tonelli Francesco, 0001071531
*/

#include "controllers.hpp"
#include <LiquidCrystal_I2C.h>
#include "math.h"
#include <avr/sleep.h>

#define GO_PROMPT_TIME 1000
#define GAME_OVER_TIME 10000
#define CORRECT_ANSWER_TIME 2000
#define INTERUPT_DELAY 40

const int statusLed = 5;
const int analogInPot = A2;
const bool isTest = false;

const int buttons[] = {3, 4, 6, 7};
const int leds[] = {8, 9, 10, 11};
bool isButtonPressed[] = {false, false, false, false};

// Status led Fading variables
int brightness = 0;
int fadeAmount = 12;
int Fadedelay = 30;
unsigned long lastMillis = 0;
bool alrPrintedWelcome = false;
bool alrPrintedNum = false;

const bool verbose = true;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void log(char *message)
{
  if (verbose)
  {
    Serial.println(message);
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
  lcd.print("MAX MILLIS: ");
  lcd.setCursor(12, 3);
  auto time = getTimer();
  lcd.print(getTimer());
  const int decs[] = {4, 3, 2, 1};
  int curr = 12;
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
/// HANDLERS
long prevB1PressCheck = 0;
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

void goToSleepMyLittleBaby()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  startGame();
}

void gotoSleepHandler()
{
  setSleep();
}

void update_timer_1(int milli_sec)
{
  long frequency = 1000 / milli_sec; // Frequency in Hz
  OCR1A = (16000000 / (1024 * frequency)) - 1;
}

void setup_timer_1(int milli_sec)
{
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  update_timer_1(milli_sec);

  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void stop_timer_1()
{
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void restart_timer_1()
{
  TCCR1B |= (1 << CS12) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect)
{
  handleMoveTimerEnd();
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

  if (!isTest)
  {
    attachInterrupt(digitalPinToInterrupt(buttons[0]), handleB1Press, RISING);
    setup_timer_1(1000);
  }
}

unsigned long lastButtonClick = 0;

void loop()
{
  if (isTest)
  {
    random(1, 10);
    testComponentLoop();
  }
  else
  {
    if (isSleeping())
    {
      prepareSleepMode();
      goToSleepMyLittleBaby();
      prepareForWakeUp();
      lastButtonClick = millis();
    }
    else if (isWaitingForStart())
    {
      for (int button : buttons)
      {
        if (digitalRead(button) == HIGH)
        {
          lastButtonClick = millis();
        }
      }
      if (millis() - lastButtonClick > 10000)
      {
        setSleep();
      }
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
      update_timer_1(getTimer());
      restart_timer_1();
    }
    else if (isGameStarted())
    {
      gameRoutine();
    }
    else if (isWaitResult())
    {
      stop_timer_1();
      move();
    }
    else if (isCorrectAnswer())
    {
      printGoodScore();
      delay(CORRECT_ANSWER_TIME);
      resume();
      update_timer_1(getTimer());
      restart_timer_1();
    }
    else if (isGameOver())
    {
      printGameOver(getScore());
      triggerStatusLed();
      delay(5000);
      startGame();
      lastButtonClick = millis();
    }
  }
}