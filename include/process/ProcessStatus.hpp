#pragma once

#include <string>
#include <cstdint>

namespace process {

enum class ProcessState : std::uint8_t {
    Run = 0,
    Sleeping = 1,
    DeepSleep = 2,
    Zombie = 3,
    Traced = 4,
    Dead = 5,
    Idle = 6
};

ProcessState parse(const std::string& state);

std::string convertProcessStateToString(const ProcessState& state);

}
