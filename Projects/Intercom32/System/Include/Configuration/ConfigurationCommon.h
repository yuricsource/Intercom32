#pragma once

#include <stdio.h>
#include <cstdint>
#include <cstring>
#include <cctype>
#include <array>

namespace Configuration
{

    static constexpr uint8_t MaxCameraFrameBuffer = 10;
    using DeviceName = std::array<char, 21>;
} // namespace Common