#include "process/detail/LinuxProcessFinder.hpp"

namespace process::detail {

LinuxProcessFinder::LinuxProcessFinder(logger::Logger& logger) : _logger(logger) {

}

std::vector<process::ProcessInfo> LinuxProcessFinder::getProcesses() const {
    std::vector<process::ProcessInfo> processes;
    
    const std::filesystem::directory_iterator end;
    std::filesystem::directory_iterator it(this->ROOT_PROC, std::filesystem::directory_options::skip_permission_denied);
    for (; it != end; ++it) {
        if (!it->is_directory()) {
            continue;
        }

        const std::string directoryName = it->path().filename().string();
        if (!std::all_of(directoryName.cbegin(), directoryName.cend(), ::isdigit)) {
            continue;
        }

        try {
            process::ProcessInfo process = this->getProccessInfo(it->path());
            processes.push_back(std::move(process));
        }
        catch (const std::exception& ex) {
            this->_logger.log(ex.what(), logger::LogLevel::Error);
        }
    }

    return processes;
}

process::ProcessInfo LinuxProcessFinder::getProccessInfo(const std::filesystem::path& path) const {
    std::ifstream reader(path / "stat");
    if (!reader.is_open()) {
        throw process::detail::FileReadException("The file could not be opened");
    }

    std::string stateLine;
    std::getline(reader, stateLine);

    std::vector<std::string> processParams;

    int posStartProcessName = stateLine.find('(');
    int posEndProcessName = stateLine.rfind(')');

    std::stringstream ssPid(stateLine.substr(0, posStartProcessName));

    std::string pid;
    ssPid >> pid;

    processParams.push_back(std::move(pid));
    processParams.push_back(stateLine.substr(posStartProcessName + 1, (posEndProcessName - posStartProcessName) - 1));

    std::stringstream ssTime(stateLine.substr(posEndProcessName + 2));

    std::string param;
    while (ssTime >> param) {
        processParams.push_back(std::move(param));
    }

    process::ProcessInfo process;
    process.pid = std::stoi(processParams[0].c_str());
    process.name = processParams[1];
    process.state = process::parse(processParams[2].c_str());
    process.parentPid = std::stoi(processParams[3].c_str());
    process.startTimeTick = std::stoll(processParams[21].c_str());

    return process;
}

const std::filesystem::path LinuxProcessFinder::ROOT_PROC = std::filesystem::path("/proc");

}
