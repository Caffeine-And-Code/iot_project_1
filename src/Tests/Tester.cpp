#include <iostream>
#include "./../iot_project_1/DifficultFunc.hpp"

using std::cout;
using std::endl;

void separetTests()
{
    cout << "\n"
         << endl;
    cout << "--------------------------------" << endl;
    cout << "\n"
         << endl;
}

int TestGetAndSetDifficulty(int testNumber)
{
    separetTests();
    testNumber++;
    cout << "Test " + std::to_string(testNumber) + ": getDifficult() should return 0" << endl;
    if (getDifficult() == 0)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 0" << endl;
        cout << "Actual: " << getDifficult() << endl;
        return 1;
    }

    separetTests();
    testNumber++;
    cout << "Test " + std::to_string(testNumber) + ": setDifficult(0) should set difficulty to 1" << endl;
    setDifficult(0);
    if (getDifficult() == 1)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 1" << endl;
        cout << "Actual: " << getDifficult() << endl;
        return 1;
    }

    separetTests();
    testNumber++;
    cout << "Test " + std::to_string(testNumber) + ": setDifficult(1023) should set difficulty to 4" << endl;
    setDifficult(1023);
    if (getDifficult() == 4)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 4" << endl;
        cout << "Actual: " << getDifficult() << endl;
        return 1;
    }

    separetTests();
    testNumber++;
    cout << "Test " + std::to_string(testNumber) + ": setDifficult(256) should set difficulty to 2" << endl;
    setDifficult(256);
    if (getDifficult() == 2)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 2" << endl;
        cout << "Actual: " << getDifficult() << endl;
        return 1;
    }
    return 0;
}

int TestMillisecondsBasedToTheScore(int testNumber)
{
    separetTests();
    testNumber++;
    cout << "Test " + std::to_string(testNumber) + ": getMillisecondsBasedByScore(0) should return 10000" << endl;
    setDifficult(0);
    if (getMillisecondsBasedByScore(0) == 10000)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 10000" << endl;
        cout << "Actual: " << getMillisecondsBasedByScore(0) << endl;
        return 1;
    }

    separetTests();
    testNumber++;
    cout << "Test " + std::to_string(testNumber) + ": getMillisecondsBasedByScore(0) and setDifficult(1023) should return 10000 " << endl;
    setDifficult(1023);
    if (getMillisecondsBasedByScore(0) == 10000)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 10000" << endl;
        cout << "Actual: " << getMillisecondsBasedByScore(0) << endl;
        return 1;
    }

    separetTests();
    testNumber++;

    cout << "Test " + std::to_string(testNumber) + ": getMillisecondsBasedByScore(1) and setDifficult(1023) should return 9200 " << endl;
    setDifficult(1023);
    if (getMillisecondsBasedByScore(1) == 9200)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 9200" << endl;
        cout << "Actual: " << getMillisecondsBasedByScore(1) << endl;
        return 1;
    }

    separetTests();
    testNumber++;

    cout << "Test " + std::to_string(testNumber) + ": getMillisecondsBasedByScore(50) and setDifficult(1023) should return 0 " << endl;
    setDifficult(1023);
    if (getMillisecondsBasedByScore(50) == 0)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 0" << endl;
        cout << "Actual: " << getMillisecondsBasedByScore(50) << endl;
        return 1;
    }

    separetTests();
    testNumber++;

    cout << "Test " + std::to_string(testNumber) + ": getMillisecondsBasedByScore(2) and setDifficult(1023) should return 8400 " << endl;
    setDifficult(1023);
    if (getMillisecondsBasedByScore(2) == 8400)
    {
        cout << "Test " + std::to_string(testNumber) + " passed" << endl;
    }
    else
    {
        cout << "Test " + std::to_string(testNumber) + " failed" << endl;
        cout << "Expected: 8400" << endl;
        cout << "Actual: " << getMillisecondsBasedByScore(2) << endl;
        return 1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    int testNumber = 0;
    if (TestGetAndSetDifficulty(testNumber))
    {
        return 1;
    }
    testNumber = 4;

    if (TestMillisecondsBasedToTheScore(testNumber))
    {
        return 1;
    }
    testNumber = 9;

    return 0;
}
