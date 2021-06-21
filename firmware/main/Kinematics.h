#pragma once

#include "vecs.hpp"
#include "Actuator.hpp"

namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

struct Kinematics {
    struct Orientation {
        float yaw;
        float pitch;
    };

    struct BaseAngles {
        float theta;
        float phi0;
        float phi1;
    };

    struct WristAngles {
        float rho;
        float phi2;
    };

    struct Angles {
        BaseAngles base;
        WristAngles wrist;
    };

    Kinematics(stepper::Actuator &yaw, stepper::Actuator &lowerVert, stepper::Actuator &upperVert,
               stepper::Actuator &roll, stepper::Actuator &pitch);

    /**
     * move to a specific location
     *
     * @param newPosition   new position in mm
     * @param speed      speed in millimeters per second
     */
    void moveTo(const vecs::float3& newPosition, const Orientation& orientation, float speed);

    Angles anglesForPosition(const vecs::float3& position, const Orientation& orientation) const;

    /**
     * return the current tool position
     *
     * @return tool position in millimeters
     */
    vecs::float3 toolPosition() const;

    /**
     * return the current wrist position
     *
     * @return tool position in millimeters
     */
    vecs::float3 wristPosition() const;




private:
    WristAngles wristAnglesForPose(const BaseAngles& baseAngles, const Orientation& orientation) const;

    /**
     * calculates the position offset of the tool coordinate system center to the pitch base at a certain orientation
     */
    vecs::float3 wristPositionOffsetForOrientation(const Orientation& orientation) const;

    /**
     * determine the yaw rotation for a given position p
     *
     * since we omit backward bending, the yaw rotation is in the range (-pi, pi)
     */
    float yawForPosition(const vecs::float3& position) const;

    float lowerVertForPosition(const vecs::float3& position) const;
    float upperVertForPosition(const vecs::float3& position) const;

    /**
     * calculate the projection of a vector r into the plane P spanned by r and the z axis
     *
     * - the y coordinate represents the z dimension of r but is offset by b_
     * - the x coordinate represents the x-y length of r and through the choice of P is always positive
     */
    vecs::float2 calculateProjected_d(const vecs::float3& position) const;

    float b_;
    float a0_;
    float a1_;
    float a2_;

    stepper::Actuator& theta_;
    stepper::Actuator& phi0_;
    stepper::Actuator& phi1_;
    stepper::Actuator& rho_;
    stepper::Actuator& phi2_;
};

// ---------------------------------------------------------------------------------------------------------------------

}
