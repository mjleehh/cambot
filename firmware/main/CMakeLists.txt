idf_component_register(SRCS
        cambot_main.cpp
        App.hpp
        App.cpp
        Actuators.hpp
        Actuators.cpp
        Actuator.hpp
        Actuator.cpp
        Kinematics.h
        Kinematics.cpp
        vecs.hpp
        additional_response_types.hpp
        INCLUDE_DIRS "")

set(COMPONENT_REQUIRES
        mfl_display
        mfl_helpers
        mfl_thread
        lohmann_json
        linalg)