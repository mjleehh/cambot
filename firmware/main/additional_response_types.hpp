#pragma once

#include <mfl/http/response_types/response-types.hpp>
#include "vecs.hpp"
#include <nlohmann/json.hpp>

namespace mfl::http::response_types {

// ---------------------------------------------------------------------------------------------------------------------

template<>
inline std::string getMimeType<nlohmann::json>() {
    return "application/json";
}

// ---------------------------------------------------------------------------------------------------------------------

template<>
std::string serialize(const nlohmann::json& resBody) {
    return resBody.dump();
}

// ---------------------------------------------------------------------------------------------------------------------

template<>
std::string serialize(const vecs::int3& resBody) {
    auto json = nlohmann::json{
            {"x", resBody.x},
            {"y", resBody.y},
            {"z", resBody.z},
    };
    return json.dump();
}

// ---------------------------------------------------------------------------------------------------------------------

}
