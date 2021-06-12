#pragma once

#include "vecs.hpp"
#include "Actuator.hpp"

namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

struct Kinematics {
    Kinematics(stepper::Actuator &yaw, stepper::Actuator &lowerVert, stepper::Actuator &upperVert,
               stepper::Actuator &roll, stepper::Actuator &pitch);

    void moveTo(const vecs::int3& position);
    vecs::int3 position() const;

    float yawForPosition(const vecs::int3& position) const;
    float lowerVertForPosition(const vecs::int3& position) const;
    float upperVertForPosition(const vecs::int3& position) const;

    vecs::int2 calculateProjected_d(const vecs::int3& position) const;

private:
    int b_;
    int a0_;
    int a1_;

    stepper::Actuator& yaw_;
    stepper::Actuator& lowerVert_;
    stepper::Actuator& upperVert_;
    stepper::Actuator& roll_;
    stepper::Actuator& pitch_;
};

// ---------------------------------------------------------------------------------------------------------------------

}
