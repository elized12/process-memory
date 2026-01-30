#include "process/ProcessInfo.hpp"

namespace process {

ProcessInfo::ProcessInfo(const int& _pid,
            const int& _parentPid,
            const std::string& _name,
            const long long& _startTimeTick,
            const process::ProcessState& _state) :
    pid(_pid), name(_name), startTimeTick(_startTimeTick), parentPid(_parentPid), state(_state) {}

}
