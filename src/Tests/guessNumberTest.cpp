#include "./../iot_project_1/guessNumber.hpp"
#include <iostream>

void testCheckNumer()
{
    int number = generateNumber();
    if (checkBinary(number & 0b1000, number & 0b0100, number & 0b0010, number & 0b0001))
    {
        std::cout << "Test [testCheckNumer] passed.";
    }
    else
    {
        std::cout << "Test [testCheckNumer] not passed.";
    }
}

void main()
{
    testCheckNumer();
}