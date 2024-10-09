#include "DifficultFunc.hpp"

void setup()
{
    setDifficult(1023);
    Serial.begin(9600);
}

void loop()
{
    
    Serial.println(getDifficult());
}