
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

void SoftwareResetTest();
void PutCpuToSleep();
void TestSpiffs();
void TestSdCard();
void WifiMenu();
void LedMenu();
char ReadKey();
void ReadString(char * string, uint8_t size);
void CameraMenu();

const char *GetTestPhrase();
#endif /* TESTS_H_ */
