#include "controllers.hpp"
#include <LiquidCrystal_I2C.h>

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
    lcd.print("Guess My Binary!    ");
    lcd.setCursor(0, 2);
    lcd.print("Press B1, Start     ");
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
  lcd.print("GO!                 ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}

void printNumber(int number)
{
  lcd.setCursor(0, 0);
  lcd.print(number); 
  if(number >= 10){
    lcd.setCursor(2, 0);
  }
  else {
    lcd.setCursor(1, 0);
  }
  lcd.print("              "); 
  lcd.setCursor(0, 1);
  lcd.print("                    ");
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
  lcd.print("Score: ");
  lcd.setCursor(7, 1);
  lcd.print(score);
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
  if (ts - prevB1PressCheck > INTERUPT_DELAY)
  {
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
  log("Trigger #1");
  if (isGameStarted())
  {
    log("Trigger #2");
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

  // cleanScreen();
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

int freq = 1;
int count = 0;

void update_timer_1(int milli_sec)
{
  long frequency = 1000 / milli_sec;           // Frequency in Hz
  OCR1A = (16000000 / (1024 * frequency)) - 1; // Calculate the correct OCR1A value
}

void setup_timer_1(int milli_sec)
{
  // disabling interrupt
  cli();
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0;  // initialize counter value to 0
  /*
   * set compare match register
   * OCR1A = (16*2^20) / (100*PRESCALER) - 1
   * (must be < 65536)
   * by assuming a prescaler = 1024, then
   * OCR1A = (16*2^10)/freq
   */
  update_timer_1(milli_sec);

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  // enabling interrupt
  sei();
}

void stop_timer_1()
{
  // Clear the clock bits to stop the timer (CS12, CS11, CS10 in TCCR1B)
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void restart_timer_1()
{
  // Resume the timer by setting the prescaler to 1024
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
    stop_timer_1();
  }
}

void loop()
{
  log("Loop");
  Serial.println(getGameState());
  if (isTest)
  {
    random(1, 10);
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
      log("New Timer");
      log(getTimer());
      update_timer_1(getTimer());
      restart_timer_1();
    }
    else if (isGameStarted())
    {
      gameRoutine();
    }
    else if (isWaitResult()){
      stop_timer_1();
      move();
    }
    else if (isCorrectAnswer())
    {
      delay(CORRECT_ANSWER_TIME);
      alrPrintedNum = false;
      resume();
      update_timer_1(getTimer());
      restart_timer_1();
    }
    else if (isGameOver())
    {
      log("Game Over");
      printGameOver(getScore());
      triggerStatusLed();
      delay(5000);
      alrPrintedNum = false;
      startGame();
    }
  }
}