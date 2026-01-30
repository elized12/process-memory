#pragma once

#include <string>
#include <chrono>

#include "process/ProcessStatus.hpp"

namespace process {

class ProcessInfo {
public:
    int pid;
    int parentPid;
    std::string name;
    long long startTimeTick;
    process::ProcessState state;
    time_t startDate;

public:
    ProcessInfo(const int& _pid = -1,
                const int& _parrentPid = -1,
                const std::string& _name = "none",
                const long long& _startTimeTick = -1,
                const process::ProcessState& state = process::ProcessState::Idle);
};

}
