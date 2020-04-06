
#ifndef INCLUDE_HAL_RNG_H_
#define INCLUDE_HAL_RNG_H_

#include "HalCommon.h"
namespace Hal
{

class Rng
{
public:
	Rng();
	~Rng();

	uint32_t GetNumber();

};
} // namespace Hal

#endif /* INCLUDE_HAL_RNG_H_ */
