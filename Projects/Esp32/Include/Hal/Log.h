#pragma once

/*
#define DEBUG_PRINTLN(a) Serial.println(a)
#define DEBUG_PRINT(a) Serial.print(a)
#define DEBUG_PRINTLNF(a, f) Serial.println(a, f)
#define DEBUG_PRINTF(a, f) Serial.print(a, f)
*/
#define DEBUG_PRINTLN(a) printf(a)
#define DEBUG_PRINT(a) printf(a)
#define DEBUG_PRINTLNF(a, f) printf(a, f)
#define DEBUG_PRINTF(a, f) printf(a, f)
