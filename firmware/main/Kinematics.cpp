#include "Kinematics.h"
#include <esp_log.h>

namespace cambot {

namespace {

// ---------------------------------------------------------------------------------------------------------------------

const char* tag = "cambot::Kinematics";

// ---------------------------------------------------------------------------------------------------------------------

vecs::float2x2 rot2(float rad) {
    auto cosPhi = std::cos(rad);
    auto sinPhi = std::sin(rad);
    return vecs::float2x2{{cosPhi, sinPhi}, {-sinPhi, cosPhi}};
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3x3 rot3_z(float rad) {
    auto cosPhi = std::cos(rad);
    auto sinPhi = std::sin(rad);
    return vecs::float3x3{{cosPhi, sinPhi, 0}, {-sinPhi, cosPhi, 0}, {0, 0, 1}};
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * numerically stabilize against full joint stretches where the fraction used in acos approaches 1
 */
float stableAcos(float nom, float denom) {
    float frac =  nom / denom;
    return acosf(frac * (frac < 1) + (frac > 1));
}

}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Kinematics(stepper::Actuator &yaw, stepper::Actuator &lowerVert, stepper::Actuator &upperVert,
                       stepper::Actuator &roll, stepper::Actuator &pitch)
        : b_(CONFIG_CAMBOT_LENGTH_BASE_TO_LOWER_VERT), a0_(CONFIG_CAMBOT_LENGTH_LOWER_VERT_TO_UPPER_VERT), a1_(CONFIG_CAMBOT_LENGTH_UPPER_VERT_TO_PITCH),
            yaw_(yaw), lowerVert_(lowerVert), upperVert_(upperVert), roll_(roll), pitch_(pitch) {}

// ---------------------------------------------------------------------------------------------------------------------

void Kinematics::moveTo(const vecs::int3& position, float speed) {
    yaw_.moveToRad(yawForPosition(position), speed);
    lowerVert_.moveToRad(lowerVertForPosition(position), speed);
    upperVert_.moveToRad(upperVertForPosition(position), speed);
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::int3 Kinematics::position() const {
    vecs::float2 t_2 = {0, a1_};
    vecs::float2x2 rotPhi1 = rot2(upperVert_.positionRad());
    auto t_1 = linalg::mul(rotPhi1, t_2) + vecs::float2{0, a0_};
    vecs::float2x2 rotPhi0 = rot2(lowerVert_.positionRad());
    auto t_0 = linalg::mul(rotPhi0, t_1) + vecs::float2{0, b_};
    auto r = linalg::mul(rot3_z(yaw_.positionRad()), vecs::float3{t_0.x, 0, t_0.y});
    return {r.x, r.y, r.z};
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::yawForPosition(const vecs::int3& position) const {
    return atan2f(position.y, position.x);
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::lowerVertForPosition(const vecs::int3& position) const {
    auto vec_d = calculateProjected_d(position);
    auto d = length(vec_d);
    float nom = a0_ * a0_ - a1_ * a1_ + d * d;
    float denom = 2 * a0_ * d;
    float gamma = atan2f(vec_d.x, vec_d.y);
    ESP_LOGI(tag, "lv %d, %d, %d, %f", a0_, a1_, d, gamma);
    ESP_LOGI(tag, "%f, %f, %f", nom, denom, stableAcos(nom, denom));
    return stableAcos(nom, denom) - gamma;
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::upperVertForPosition(const vecs::int3& position) const {
    auto d = length(calculateProjected_d(position));
    float nom = a0_ * a0_ + a1_ * a1_ - d * d;
    float denom = 2 * a0_ * a1_;
    ESP_LOGI(tag, "uv %d, %d, %d", a0_, a1_, d);
    ESP_LOGI(tag, "%f, %f, %f", nom, denom, stableAcos(nom, denom));
    return stableAcos(nom, denom) - M_PI;
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::int2 Kinematics::calculateProjected_d(const vecs::int3& position) const {
    auto norm_xy = sqrtf(position.x * position.x + position.y * position.y);
    return {norm_xy, position.z - b_};
}

// ---------------------------------------------------------------------------------------------------------------------

}
