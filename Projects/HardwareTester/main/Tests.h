
#ifndef TESTS_H_
#define TESTS_H_

#include <cstring>
#include <string>
#include "stdio.h"
#include <cstdint>
#include <cstdarg>
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"

using Hal::Timer;
using Hal::Hardware;

void SoftwareResetTest();
void PutCpuToSleep();
void TestSpiffs();
void WifiMenu();
void LedMenu();
char ReadKey();
void ReadString(char * string, uint8_t size);
void CameraMenu();
void TestTimer();

const char *GetTestPhrase();

class TestClass : Timer::Callback
{
public:
    TestClass()
    {
        Hal::Hardware::Instance()->GetTimer0().AddCallback(this);
    }
    void TimerCallback() override
    {
        Hardware::Instance()->GetLeds().Toggle(Hal::Leds::LedIndex::Blue);
    }
};

#endif /* TESTS_H_ */
