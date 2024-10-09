#include <iostream>
#include "./../iot_project_1/DifficultFunc.hpp"

using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
    cout << "Test 1: getDifficult() should return 0" << endl;
    if (getDifficult() == 0)
    {
        cout << "Test 1 passed" << endl;
    }
    else
    {
        cout << "Test 1 failed" << endl;
        return 1;
    }
    return 0;
}
