#pragma once

#include <sys/uio.h>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <string>

#include "logger/Logger.hpp"
#include "process/memory/WriteMemoryException.hpp"
#include "process/memory/PermissionDeniedException.hpp"
#include "process/memory/ProcessNotExistException.hpp"

namespace process::memory {

class LinuxMemoryWriter {
private:
    logger::Logger& _logger;

public:
    LinuxMemoryWriter(logger::Logger& logger);

public:
    template<class Type>
    void write(int pid, std::uint64_t address, Type value);
    void write(int pid, std::uint64_t address, const std::string& value);
};

template<class Type>
void LinuxMemoryWriter::write(int pid, std::uint64_t address, Type value) {
    Type localBuffer = value;

    iovec local;
    local.iov_base = &localBuffer;
    local.iov_len = sizeof(Type);

    iovec remote;
    remote.iov_base = reinterpret_cast<void*>(address);
    remote.iov_len = sizeof(Type);

    ssize_t nread = process_vm_writev(pid, &local, 1, &remote, 1, 0);

    if (nread == -1) {
        std::string errorMsg;

        int err = errno;
        switch (err) {
            case EFAULT:
                errorMsg = "Invalid memory address (EFAULT)";
                break;
            case EINVAL:
                errorMsg = "Invalid arguments (EINVAL)";
                break;
            case ENOMEM:
                errorMsg = "Cannot allocate memory (ENOMEM)";
                break;
            case EPERM:
                errorMsg = "Permission denied (EPERM) - need CAP_SYS_PTRACE or ptrace rights";
                throw PermissionDeniedException(
                    "process_vm_writev failed: "
                    + errorMsg +
                    " (errno: " + std::to_string(err) + ")"
                );
                break;
            case ESRCH:
                errorMsg = "Process does not exist (ESRCH)";
                throw ProcessNotExistException(
                    "process_vm_writev failed: "
                    + errorMsg +
                    " (errno: " + std::to_string(err) + ")"
                );
                break;
            default:
                errorMsg = "Unknown error: " + std::string(std::strerror(err));
                break;
        }

        throw WriteMemoryException(
            "process_vm_writev failed: " + errorMsg +
            " (errno: " + std::to_string(err) + ")"
        );
    }
}

inline void LinuxMemoryWriter::write(int pid, std::uint64_t address, const std::string& value) {
    std::string localBuffer = value;

    iovec local;
    local.iov_base = localBuffer.data();
    local.iov_len = localBuffer.size();

    iovec remote;
    remote.iov_base = reinterpret_cast<void*>(address);
    remote.iov_len = localBuffer.size();

    ssize_t nread = process_vm_writev(pid, &local, 1, &remote, 1, 0);
    if (nread != localBuffer.size()) {
        throw WriteMemoryException(
            std::string("process_vm_writev failed: ") + "not write all bytes"
        );
    }

    if (nread == -1) {
        std::string errorMsg;

        int err = errno;
        switch (err) {
            case EFAULT:
                errorMsg = "Invalid memory address (EFAULT)";
                break;
            case EINVAL:
                errorMsg = "Invalid arguments (EINVAL)";
                break;
            case ENOMEM:
                errorMsg = "Cannot allocate memory (ENOMEM)";
                break;
            case EPERM:
                errorMsg = "Permission denied (EPERM) - need CAP_SYS_PTRACE or ptrace rights";
                throw PermissionDeniedException(
                    "process_vm_writev failed: "
                    + errorMsg +
                    " (errno: " + std::to_string(err) + ")"
                );
                break;
            case ESRCH:
                errorMsg = "Process does not exist (ESRCH)";
                throw ProcessNotExistException(
                    "process_vm_writev failed: "
                    + errorMsg +
                    " (errno: " + std::to_string(err) + ")"
                );
                break;
            default:
                errorMsg = "Unknown error: " + std::string(std::strerror(err));
                break;
        }

        throw WriteMemoryException(
            "process_vm_writev failed: " + errorMsg +
            " (errno: " + std::to_string(err) + ")"
        );
    }
}

}
