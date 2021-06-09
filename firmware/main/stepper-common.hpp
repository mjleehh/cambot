#pragma once

#include <sdkconfig.h>

namespace cambot::stepper {

// ---------------------------------------------------------------------------------------------------------------------

inline constexpr uint millisPerCycle() {
    return CONFIG_CAMBOT_STEPPER_CLOCK_DELAY + CONFIG_CAMBOT_STEPPER_MOVE_DELAY;
}

}