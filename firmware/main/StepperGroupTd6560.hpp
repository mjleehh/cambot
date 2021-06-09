#pragma once

#include <driver/gpio.h>
#include <array>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <vector>

namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

struct Steppers;

// ---------------------------------------------------------------------------------------------------------------------

}

namespace cambot::stepper {

// ---------------------------------------------------------------------------------------------------------------------

struct InvalidSpeed : std::invalid_argument {
    explicit InvalidSpeed() : invalid_argument("speed too high for stepper") {};
};

// ---------------------------------------------------------------------------------------------------------------------

struct StepperTd6560 {
    gpio_num_t directionPin;
    gpio_num_t clockPin;
    bool inverted;
};

// ---------------------------------------------------------------------------------------------------------------------

/**
 * control stepper via TB6560 controller
 *
 * This class can only be used in combination with the Steppers class.
 * The public interface of this class is thread safe.
 */
struct StepperGroupTd6560 {
    enum class Direction : int {
        cw = -1,
        ccw = 1,
    };

    /**
     * @param pos     absolute rotation in steps (negative values are interpreted cw)
     * @param speed   speed in steps per second
     */
    void moveToStep(int pos, uint speed);

    /**
     *
     * @param angle          absolute rotation in degrees (negative values are interpreted cw)
     * @param angularSpeed   speed in degrees per second
     */
    void moveToAngle(float angle, float angularSpeed);

    /**
     *
     * @param angle          absolute rotation in degrees (negative values are interpreted cw)
     * @param angularSpeed   speed in rad per second
     */
    void moveToRad(float rad, float radSpeed);

    /**
     *
     * @param direction
     * @param steps   number of steps to rotate
     * @param speed   speed in steps per second
     */
    void rotateSteps(Direction direction, uint steps, uint speed);

    /**
     *
     * @param angle          angle in degree (positive as ccw)
     * @param angularSpeed   speed in degree per second
     */
    void rotateAngle(float angle, float angularSpeed);

    /**
     *
     * @param rad        angle in rad (positive as ccw)
     * @param radSpeed   speed in rad per second
     */
    void rotateRad(float rad, float radSpeed);

    uint numSteps() const;

    uint8_t positionSteps() const;

    float positionAngle() const;

    float positionRad() const;

    bool isMoving() const;

    float stepsToAngle(float steps) const;

    float angleToSteps(float angle) const;

    /**
     * this will also return negative steps for cw!
     */
    float stepsToRad(float steps) const;

    /**
     * this will also return negative steps for cw!
     */
    float radToSteps(float rad) const;

    StepperGroupTd6560(StepperGroupTd6560&& other) = delete;
    StepperGroupTd6560(const StepperGroupTd6560&) = delete;
    StepperGroupTd6560& operator=(const StepperGroupTd6560&) = delete;

private:
    friend Steppers;

    StepperGroupTd6560(uint numSteps, std::vector<StepperTd6560>&& steppers, SemaphoreHandle_t dataLock);

    /**
     * start a step
     *
     * @WARNING: not thread safe!
     */
    void clockUp();

    /**
     * stop a step
     *
     * @WARNING: not thread safe!
     */
    void clockDown();

    bool needToStep() const;

    const uint numSteps_;
    std::vector<StepperTd6560> steppers_;
    uint stepPos_;

    int delta_;
    uint cyclesPerStep_;
    uint cyclesSinceLastStep_;
    const SemaphoreHandle_t dataLock_;
};

// ---------------------------------------------------------------------------------------------------------------------

}
