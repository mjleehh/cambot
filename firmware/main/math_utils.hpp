#pragma once

#include <linalg.h>

namespace vecs = linalg::aliases;

namespace cambot::mutil {

// ---------------------------------------------------------------------------------------------------------------------

inline constexpr int signi(int value) {
    return (0 < value) - (value < 0);
}

// ---------------------------------------------------------------------------------------------------------------------

inline constexpr float signf(float value) {
    return (0 < value) - (value < 0);
}

// ---------------------------------------------------------------------------------------------------------------------

inline constexpr uint absi(int value) {
    return value * (value > 0) - value * (value < 0);
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * 2D rotation
 */
inline constexpr vecs::float2x2 rot2(float rad) {
    auto cosPhi = std::cos(rad);
    auto sinPhi = std::sin(rad);
    return vecs::float2x2{{cosPhi,  sinPhi},
                          {-sinPhi, cosPhi}};
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * 3D rotation round the z-axis
 */
inline constexpr vecs::float3x3 rot3_z(float rad) {
    auto cosPhi = std::cos(rad);
    auto sinPhi = std::sin(rad);
    return vecs::float3x3{{cosPhi,  sinPhi, 0},
                          {-sinPhi, cosPhi, 0},
                          {0,       0,      1}};
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * numerically stabilize against full joint stretches where the fraction used in acos approaches 1
 */
inline constexpr float stableAcos(float nom, float denom) {
    float frac = nom / denom;
    return acosf(frac * (frac < 1) * (frac > -1) + (frac >= 1) - (frac <= -1));
}

// ---------------------------------------------------------------------------------------------------------------------

}