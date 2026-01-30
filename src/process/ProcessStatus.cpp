#include "process/ProcessStatus.hpp"

namespace process {

ProcessState parse(const std::string& state) {
    if (state == "R") {
        return ProcessState::Run;
    }
    else if (state == "S") {
        return ProcessState::Sleeping;
    }
    else if (state == "D") {
        return ProcessState::DeepSleep;
    }
    else if (state == "Z") {
        return ProcessState::Zombie;
    }
    else if (state == "T") {
        return ProcessState::Traced;
    }

    return ProcessState::Idle;
}

std::string convertProcessStateToString(const ProcessState& state) {
    if (state == ProcessState::Run) {
        return "Run";
    }
    else if (state == ProcessState::Sleeping) {
        return "Sleeping";
    }
    else if (state == ProcessState::DeepSleep) {
        return "DeepSleep";
    }
    else if (state == ProcessState::Zombie) {
        return "Zombie";
    }
    else if (state ==ProcessState::Traced) {
        return "Traced";
    }

    return "Idle";
}

}
