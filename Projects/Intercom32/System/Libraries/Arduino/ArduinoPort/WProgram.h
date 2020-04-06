// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef _WPROGRAM_H_
#define _WPROGRAM_H_

#include "ArduinoPort.h"
#include "string.h"
#include "esp_types.h"
#include "freertos/FreeRTOS.h"

#define abs(x) ((x) < 0 ? -(x) : (x))
#define swap(x, y)                \
    do                            \
    {                             \
        typeof(x) temp##x##y = x; \
        x = y;                    \
        y = temp##x##y;           \
    } while (0)

typedef bool boolean;
// typedef char __FlashStringHelper;

// an abstract class used as a means to proide a unique pointer type
// but really has no body

class __FlashStringHelper;
// #define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))

class Print
{
public:
    void print(char *s);
    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str)
    {
        if(str == NULL) {
            return 0;
        }
        return write((const uint8_t *) str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size)
    {
        return write((const uint8_t *) buffer, size);
    }
};

class String
{
public:
    String() : _data(0), _isStatic(true) {}
    String(const char *data, bool isStaticData = true)
        : _data(data), _isStatic(isStaticData) {}

    const char *c_str() const
    {
        return _data;
    }

    bool isNull() const
    {
        return !_data;
    }

    bool isStatic() const
    {
        return _isStatic;
    }

    int length() const
    {
        return strlen(_data);
    }

    friend bool operator==(String lhs, String rhs)
    {
        if (lhs._data == rhs._data)
            return true;
        if (!lhs._data)
            return false;
        if (!rhs._data)
            return false;
        return strcmp(lhs._data, rhs._data) == 0;
    }

private:
    const char *_data;
    bool _isStatic;
};

#endif
