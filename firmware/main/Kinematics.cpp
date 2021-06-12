#pragma once

#include "Kinematics.h"
#include <cmath>
#include <esp_log.h>

namespace vecs = linalg::aliases;

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

}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Kinematics(stepper::Actuator &yaw, stepper::Actuator &lowerVert, stepper::Actuator &upperVert,
                       stepper::Actuator &roll, stepper::Actuator &pitch)
        : b_(CONFIG_CAMBOT_LENGTH_BASE_TO_LOWER_VERT), a0_(CONFIG_CAMBOT_LENGTH_LOWER_VERT_TO_UPPER_VERT), a1_(CONFIG_CAMBOT_LENGTH_UPPER_VERT_TO_PITCH),
            yaw_(yaw), lowerVert_(lowerVert), upperVert_(upperVert), roll_(roll), pitch_(pitch) {}

// ---------------------------------------------------------------------------------------------------------------------

void Kinematics::moveTo(const vecs::int3& position) {

}

// ---------------------------------------------------------------------------------------------------------------------

vecs::int3 Kinematics::position() const {
    vecs::float2 t_2 = {0, a1_};
    vecs::float2x2 rotPhi1 = rot2(-upperVert_.positionRad());
    auto t_1 = linalg::mul(rotPhi1, t_2) + vecs::float2{0, a0_};
    vecs::float2x2 rotPhi0 = rot2(lowerVert_.positionRad());
    auto t_0 = linalg::mul(rotPhi0, t_1) + vecs::float2{0, b_};
    auto r = linalg::mul(rot3_z(yaw_.positionRad()), vecs::float3{t_0.x, 0, t_0.y});

    ESP_LOGI(tag, "phi_1 %f", upperVert_.positionRad());
    ESP_LOGI(tag, "phi_0 %f", lowerVert_.positionRad());
    ESP_LOGI(tag, "t_2 {%f, %f}", t_2.x, t_2.y);
    ESP_LOGI(tag, "t_1 {%f, %f}", t_1.x, t_1.y);
    ESP_LOGI(tag, "t_0 {%f, %f}", t_0.x, t_0.y);
    return {r.x, r.y, r.z};
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::yawForPosition(const vecs::int3& position) const {
    return std::atan2(position.x, position.y);
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::lowerVertForPosition(const vecs::int3& position) const {
    auto vec_d = calculateProjected_d(position);
    auto d = length(vec_d);
    auto nom = a0_ * a0_ - a1_ * a1_ + d * d;
    auto denom = 2 * a0_ * d;
    auto gamma = atan2(vec_d.x, vec_d.y);
    return std::acos(nom / denom) - gamma;
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::upperVertForPosition(const vecs::int3& position) const {
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::int2 Kinematics::calculateProjected_d(const vecs::int3& position) const {
    auto norm_xy = std::sqrt(position.x * position.x + position.y * position.y);
    vecs::int2 projected_r{norm_xy, position.z};
    return projected_r - vecs::int2{0, b_};

}

// ---------------------------------------------------------------------------------------------------------------------

}
