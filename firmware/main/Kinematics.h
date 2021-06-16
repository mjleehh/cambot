#pragma once

#include "vecs.hpp"
#include "Actuator.hpp"

namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

struct Kinematics {
    Kinematics(stepper::Actuator &yaw, stepper::Actuator &lowerVert, stepper::Actuator &upperVert,
               stepper::Actuator &roll, stepper::Actuator &pitch);

    /**
     * move to a specific location
     *
     * @param newPosition   new position in mm
     * @param speed      speed in millimeters per second
     */
    void moveTo(const vecs::int3& newPosition, float speed);

    /**
     * return the current tool position
     *
     * @return tool position in millimeters
     */
    vecs::int3 position() const;


    /**
     * determine the yaw rotation for a given position p
     *
     * since we omit backward bending, the yaw rotation is in the range (-pi, pi)
     */
    float yawForPosition(const vecs::int3& position) const;

    float lowerVertForPosition(const vecs::int3& position) const;
    float upperVertForPosition(const vecs::int3& position) const;

    /**
     * calculate the projection of a vector r into the plane P spanned by r and the z axis
     *
     * - the y coordinate represents the z dimension of r but is offset by b_
     * - the x coordinate represents the x-y length of r and through the choice of P is always positive
     */
    vecs::int2 calculateProjected_d(const vecs::int3& position) const;

private:
    int b_;
    int a0_;
    int a1_;
    int a2_;

    stepper::Actuator& yaw_;
    stepper::Actuator& lowerVert_;
    stepper::Actuator& upperVert_;
    stepper::Actuator& roll_;
    stepper::Actuator& pitch_;
};

// ---------------------------------------------------------------------------------------------------------------------

}
