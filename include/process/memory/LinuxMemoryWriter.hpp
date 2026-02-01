#pragma once

#include <sys/uio.h>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <string>

#include "process/memory/WriteMemoryException.hpp"

namespace process::memory {

class LinuxMemoryWriter {
public:
    template<class Type>
    static void write(int pid, std::uint64_t address, Type value);
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
        int err = errno;
        std::string errorMsg;

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
                break;
            case ESRCH:
                errorMsg = "Process does not exist (ESRCH)";
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
