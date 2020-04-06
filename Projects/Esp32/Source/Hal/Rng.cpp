
#include "Rng.h"

namespace Hal
{

Rng::Rng()
{
}

Rng::~Rng()
{
}

uint32_t Rng::GetNumber()
{
    return esp_random();
}

} // namespace Hal
