#include "Kinematics.h"
#include "math_utils.hpp"
#include <esp_log.h>

namespace cambot {

namespace {

// ---------------------------------------------------------------------------------------------------------------------

const char* tag = "cambot::Kinematics";

// ---------------------------------------------------------------------------------------------------------------------

/**
 * calculate the rotation of the roll joint
 *
 * @param phi_H   horizontal angle of the wrist relative to the wrist base
 * @param phi_V   vertical angle of the wrist relative to the wrist base
 */
float calculateRho(float phi_H, float phi_V) {
    auto sinPhiH = std::sin(phi_H);
    auto sinPhiV = std::sin(phi_V);
    if (sinPhiH == 0) {
        // for degenerate case where both angles are 0 we chose the result to be 0
        return 0;
    } else if (sinPhiV == 0) {
        return 0.5f * M_PI * mutil::signf(phi_H);
    } else {
        auto nom = 1.f / sinPhiV - 1;
        auto denom = 1.f / sinPhiH - 1;
        if (denom == 0 || nom == 0) {
            // this needs to be fixed
            // this just stabilizes the math, but does not solve the real issue
            return 0.f;
        } else {
            return std::atan(std::sqrt(nom / denom));
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * calculate the angle of the pitch joint
 *
 * @param phi_H   horizontal angle of the wrist relative to the wrist base
 * @param phi_V   vertical angle of the wrist relative to the wrist base
 */
float calculatePhi2(float phi_H, float phi_V) {
    return std::asin(std::cos(phi_H) * std::cos(phi_V));
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * calculate the angle between the wrist base pitch and a desired pitch
 */
float calculatePhiV(float targetPitch, float phi0, float phi1) {
    return targetPitch - phi0 - phi1 - 0.5f * M_PI;
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * calculate the angle between the wrist base yaw and a desired yaw
 */
float calculatePhiH(float targetYaw, float theta) {
    return targetYaw - theta;
}

// ---------------------------------------------------------------------------------------------------------------------

}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Kinematics(stepper::Actuator& yaw, stepper::Actuator& lowerVert, stepper::Actuator& upperVert,
                       stepper::Actuator& roll, stepper::Actuator& pitch)
        : b_(CONFIG_CAMBOT_LENGTH_BASE_TO_LOWER_VERT), a0_(CONFIG_CAMBOT_LENGTH_LOWER_VERT_TO_UPPER_VERT),
          a1_(CONFIG_CAMBOT_LENGTH_UPPER_VERT_TO_PITCH), a2_(CONFIG_CAMBOT_LENGTH_PITCH_TO_TOOL),
          theta_(yaw), phi0_(lowerVert), phi1_(upperVert), rho_(roll), phi2_(pitch) {}

// ---------------------------------------------------------------------------------------------------------------------

void Kinematics::moveTo(const vecs::float3& newPosition, const Orientation& orientation, float speed) {
    auto duration = linalg::distance(wristPosition(), newPosition) / speed;
    ESP_LOGI(tag, "duration: %f, speed %f", duration, speed);
    Angles angles = anglesForPosition(newPosition, orientation);

    auto yawSpeed = std::abs((angles.base.theta - theta_.positionRad()) / duration);
    auto lowerVertSpeed = std::abs((angles.base.phi0 - phi0_.positionRad()) / duration);
    auto upperVertSpeed = std::abs((angles.base.phi1 - phi1_.positionRad()) / duration);
    ESP_LOGI(tag, "yaw speed: %f, lv speed %f, uv speed %f", yawSpeed, lowerVertSpeed, upperVertSpeed);

    auto wristAngles = wristAnglesForPose(angles.base, orientation);
    auto rhoSpeed = std::abs((wristAngles.rho - rho_.positionRad()) / duration);
    auto phi2Speed = std::abs((wristAngles.phi2 - phi2_.positionRad()) / duration);

    theta_.moveToRad(angles.base.theta, yawSpeed);
    phi0_.moveToRad(angles.base.phi0, lowerVertSpeed);
    phi1_.moveToRad(angles.base.phi1, upperVertSpeed);
//    rho_.moveToRad(wristAngles.rho, rhoSpeed);
//    phi2_.moveToRad(wristAngles.phi2, phi2Speed);
}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Angles Kinematics::anglesForPosition(const linalg::aliases::float3& position,
                                                 const Kinematics::Orientation& orientation) const {
    auto orientationAdjustedPosition = position - wristPositionOffsetForOrientation(orientation);

    BaseAngles baseAngles{
            yawForPosition(position),
            lowerVertForPosition(position),
            upperVertForPosition(position)
    };
    auto wristAngles = wristAnglesForPose(baseAngles, orientation);
    return {
        baseAngles,
        wristAngles,
    };
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3 Kinematics::toolPosition() const {
    auto theta = theta_.positionRad();
    auto phi0 = phi0_.positionRad();
    auto phi1 = phi1_.positionRad();
    auto rho = rho_.positionRad();
    auto phi2 = phi2_.positionRad();

    auto sinTheta = std::sin(theta);
    auto cosTheta = std::cos(theta);
    auto sinEta = std::sin(phi0 + phi1);
    auto cosEta = std::cos(phi0 + phi1);
    auto sinRho = std::sin(rho);
    auto cosRho = std::cos(rho);
    auto sinPhi2 = std::sin(phi2);
    auto cosPhi2 = std::cos(phi2);

    auto mu = -(a0_ * std::sin(phi0) + a1_ * sinEta);

    return {
            a2_ * (
                    sinPhi2 * (sinRho * sinTheta - cosRho * cosEta * cosTheta)
                    - cosPhi2 * sinEta * cosTheta)
            + mu * cosTheta,
            -a2_ * (
                    sinPhi2 * (sinRho * cosTheta + cosRho * cosEta * sinTheta)
                    + cosPhi2 * sinEta * sinTheta)
            + mu * sinTheta,
            a0_ * std::cos(phi0) + a1_ * cosEta
            + a2_ * (cosPhi2 * cosEta - sinPhi2 * cosRho * sinEta)
            + b_
    };

}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3 Kinematics::wristPosition() const {
    auto theta = theta_.positionRad();
    auto phi0 = phi0_.positionRad();
    auto phi1 = phi1_.positionRad();
    auto mu = -(a0_ * std::sin(phi0) + a1_ * std::sin(phi0 + phi1));

    return {
        mu * std::cos(theta),
        mu * std::sin(theta),
        a0_ * std::cos(phi0) + a1_ * std::cos(phi0 + phi1) + b_
    };
}



// ---------------------------------------------------------------------------------------------------------------------

Kinematics::WristAngles Kinematics::wristAnglesForPose(const Kinematics::BaseAngles& baseAngles, const Kinematics::Orientation& orientation) const {
    auto phiH = calculatePhiH(orientation.yaw, baseAngles.theta);
    auto phiV = calculatePhiV(orientation.pitch, baseAngles.phi0, baseAngles.phi1);
    return {
        calculateRho(phiH, phiV),
        calculatePhi2(phiH, phiV)
    };
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::yawForPosition(const vecs::float3& position) const {
    return atan2f(position.y, position.x);
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::lowerVertForPosition(const vecs::float3& position) const {
    auto vec_d = calculateProjected_d(position);
    auto d = length(vec_d);
    float nom = a0_ * a0_ - a1_ * a1_ + d * d;
    float denom = 2 * a0_ * d;
    float gamma1 = atan2f(vec_d.y, vec_d.x);
    float gamma2 = mutil::stableAcos(nom, denom);
    return gamma1 + gamma2 - M_PI / 2;
}

// ---------------------------------------------------------------------------------------------------------------------

float Kinematics::upperVertForPosition(const vecs::float3& position) const {
    auto r = length(calculateProjected_d(position));
    float nom = a0_ * a0_ + a1_ * a1_ - r * r;
    float denom = 2 * a0_ * a1_;
    return mutil::stableAcos(nom,  denom) - M_PI;
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3 Kinematics::wristPositionOffsetForOrientation(const Orientation& orientation) const {
    return {
        a2_ * std::cos(orientation.yaw) * std::cos(orientation.pitch),
        a2_ * std::sin(orientation.yaw) * std::cos(orientation.pitch),
        a2_ * std::sin(orientation.pitch)
    };
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float2 Kinematics::calculateProjected_d(const vecs::float3 & position) const {
    auto norm_xy = sqrtf(position.x * position.x + position.y * position.y);
    return {norm_xy, position.z - b_};
}

// ---------------------------------------------------------------------------------------------------------------------

}
