idf_component_register(SRCS
        cambot_main.cpp
        stepper-common.hpp
        Steppers.hpp
        Steppers.cpp
        StepperGroupTd6560.hpp
        StepperGroupTd6560.cpp
        INCLUDE_DIRS "")

set(COMPONENT_REQUIRES
        mfl_display
        mfl_helpers
        mfl_thread)