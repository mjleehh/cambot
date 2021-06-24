#include "Kinematics.h"
#include "math_utils.hpp"
#include <esp_log.h>

namespace cambot {

namespace {

// ---------------------------------------------------------------------------------------------------------------------

const char* tag = "cambot::Kinematics";

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
    auto duration = linalg::distance(toolPosition(), newPosition) / speed;
    Angles angles = anglesForPosition(newPosition, orientation);

    auto yawSpeed = std::abs((angles.base.theta - theta_.positionRad()) / duration);
    auto lowerVertSpeed = std::abs((angles.base.phi0 - phi0_.positionRad()) / duration);
    auto upperVertSpeed = std::abs((angles.base.phi1 - phi1_.positionRad()) / duration);
    auto rhoSpeed = std::abs((angles.wrist.rho - rho_.positionRad()) / duration);
    auto phi2Speed = std::abs((angles.wrist.phi2 - phi2_.positionRad()) / duration);

    theta_.moveToRad(angles.base.theta, yawSpeed);
    phi0_.moveToRad(angles.base.phi0, lowerVertSpeed);
    phi1_.moveToRad(angles.base.phi1, upperVertSpeed);
    rho_.moveToRad(angles.wrist.rho, rhoSpeed);
    phi2_.moveToRad(angles.wrist.phi2, phi2Speed);
}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Angles Kinematics::anglesForPosition(const linalg::aliases::float3& position,
                                                 const Kinematics::Orientation& orientation) const {
    auto orientationAdjustedPosition = position - wristPositionOffsetForOrientation(orientation);;

    BaseAngles baseAngles{
        yawForPosition(orientationAdjustedPosition),
        lowerVertForPosition(orientationAdjustedPosition),
        upperVertForPosition(orientationAdjustedPosition)
    };
    auto wristAngles = wristAnglesForPose(baseAngles, orientation);
    return {
        baseAngles,
        wristAngles,
    };
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3 Kinematics::toolPosition() const {
    return toolPositionForAngles({
                                     {theta_.positionRad(), phi0_.positionRad(), phi1_.positionRad()},
                                     {rho_.positionRad(), phi2_.positionRad()}
                                 });
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3 Kinematics::wristPosition() const {
    return wristPositionForAngles({theta_.positionRad(), phi0_.positionRad(), phi1_.positionRad()});
}

// ---------------------------------------------------------------------------------------------------------------------

vecs::float3 Kinematics::toolPositionForAngles(const Angles& angles) const {
    auto& theta = angles.base.theta;
    auto& phi0 = angles.base.phi0;
    auto& phi1 = angles.base.phi1;
    auto& rho = angles.wrist.rho;
    auto& phi2 = angles.wrist.phi2;

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

vecs::float3 Kinematics::wristPositionForAngles(const BaseAngles& angles) const {
    auto& theta = angles.theta;
    auto& phi0 = angles.phi0;
    auto& phi1 = angles.phi1;
    auto mu = -(a0_ * std::sin(phi0) + a1_ * std::sin(phi0 + phi1));

    return {
        mu * std::cos(theta),
        mu * std::sin(theta),
        a0_ * std::cos(phi0) + a1_ * std::cos(phi0 + phi1) + b_
    };
}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Orientation Kinematics::orientation() const {
    return orientationForAngles({
        {theta_.positionRad(), phi0_.positionRad(), phi1_.positionRad()},
        {rho_.positionRad(), phi2_.positionRad()}
    });
}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::Orientation Kinematics::orientationForAngles(const Kinematics::Angles& angles) const {
    auto& theta = angles.base.theta;
    auto& phi0 = angles.base.phi0;
    auto& phi1 = angles.base.phi1;
    auto& rho = angles.wrist.rho;
    auto& phi2 = angles.wrist.phi2;

    auto sinTheta = std::sin(theta);
    auto cosTheta = std::cos(theta);
    auto sinEta = std::sin(phi0 + phi1);
    auto cosEta = std::cos(phi0 + phi1);
    auto sinRho = std::sin(rho);
    auto cosRho = std::cos(rho);
    auto sinPhi2 = std::sin(phi2);
    auto cosPhi2 = std::cos(phi2);

    vecs::float3 r = {
        sinPhi2 * (sinRho * sinTheta - cosRho * cosEta * cosTheta)
        - cosPhi2 * sinEta * cosTheta,
        sinPhi2 * (sinRho * cosTheta + cosRho * cosEta * sinTheta)
        + cosPhi2 * sinEta * sinTheta,
        cosPhi2 * cosEta - sinPhi2 * cosRho * sinEta
    };

    float yaw   = std::atan2(r.y, r.x);
    float pitch = std::asin(r.z);

    return {yaw, pitch};
}

// ---------------------------------------------------------------------------------------------------------------------

Kinematics::WristAngles Kinematics::wristAnglesForPose(const Kinematics::BaseAngles& baseAngles, const Kinematics::Orientation& orientation) const {
    float gamma_v = M_PI_2 + baseAngles.phi0 + baseAngles.phi1;
    vecs::float3 p_0 = {
        std::cos(gamma_v) * std::cos(baseAngles.theta),
        std::cos(gamma_v) * std::sin(baseAngles.theta),
        std::sin(gamma_v)
    };
    vecs::float3 p_y = {
        -std::sin(baseAngles.theta),
        std::cos(baseAngles.theta),
        0
    };
    vecs::float3 p = {
        std::cos(orientation.pitch) * std::cos(orientation.yaw),
        std::cos(orientation.pitch) * std::sin(orientation.yaw),
        std::sin(orientation.pitch)
    };
    auto p_z = linalg::cross(p_0, p_y);

    // this is the actual phi_2 joint axis vector in world coords
    auto p_perp = linalg::cross(p, p_0);
    // now convert this to the wrist frame
    auto proj_y = linalg::dot(p_perp, p_y);
    auto proj_z = linalg::dot(p_perp, p_z);

    // if proj_y < 0 then phi_2 must be negative
    auto phi_2 = linalg::angle(p, p_0) * mutil::signf(proj_y);
    auto rho = std::atan(proj_z / proj_y);
    return {rho, phi_2};
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
    return gamma1 + gamma2 - M_PI_2;
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
