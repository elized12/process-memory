#pragma once

#include <QString>
#include <QDebug>

#include <sys/uio.h>
#include <cerrno>

#include "logger/Logger.hpp"
#include "process/memory/LinuxMemoryRegionFilter.hpp"
#include "process/memory/ReadMemoryException.hpp"
#include "process/memory/PermissionDeniedException.hpp"
#include "process/memory/ProcessNotExistException.hpp"
#include "process/memory/MemoryRecord.hpp"

namespace process::memory {

class LinuxMemoryFinder {
private:
    const static int BATCH_SIZE = 4096;
    const static int MAX_FINDED_VALUES_COUNT = 1024 * 100;
    process::memory::LinuxMemoryRegionFilter _filter;
    logger::Logger& _logger;

public:
    LinuxMemoryFinder(const LinuxMemoryRegionFilter& filter, logger::Logger& loger);

public:
    template<class Type>
    std::vector<MemoryRecord> readMemory(
        int pid,
        const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
        Type searchValue
    ) const;

    std::vector<MemoryRecord> readMemory(
        int pid,
        const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
        const std::string& searchValue
    ) const;
};

template<class Type>
std::vector<MemoryRecord> LinuxMemoryFinder::readMemory(
    int pid,
    const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
    Type searchValue
) const
{
    constexpr size_t overlap = sizeof(Type) - 1;

    std::vector<MemoryRecord> memories;

    for (const process::analysis::MemoryBlock& block : memoryBlocks) {
        qDebug() << "Block" << QString::fromStdString(block.name)
                 << QString::fromStdString(std::format("{:#0x}",block.rangeAddresses.first))
                 << " - "
                 << QString::fromStdString(std::format("{:#0x}",block.rangeAddresses.second))
                 << QString::fromStdString(block.perms.toString());
        if (this->_filter.shouldSkip(block)) {
            qDebug() << "skip";
            continue;
        }


        process::analysis::address start = block.rangeAddresses.first;
        process::analysis::address end  = block.rangeAddresses.second;

        for (process::analysis::address addr = start; addr < end; addr += this->BATCH_SIZE + overlap - 1) {
            if (this->MAX_FINDED_VALUES_COUNT < memories.size()) {
                return memories;
            }

            size_t toRead = std::min(
                static_cast<size_t>(BATCH_SIZE),
                static_cast<size_t>(end - addr)
            );

            std::vector<std::uint8_t> buffer(toRead);

            iovec local;
            local.iov_base = buffer.data();
            local.iov_len = buffer.size();

            iovec remote;
            remote.iov_base = reinterpret_cast<void*>(addr);
            remote.iov_len = buffer.size();

            ssize_t nread = process_vm_readv(pid, &local, 1, &remote, 1, 0);
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
                        errorMsg = "Permission denied (EPERM) - need CAP_SYS_PTRACE capability";
                        throw PermissionDeniedException(
                            "process_vm_readv failed at address " +
                            std::format("{:#016x}", addr) + ": " + errorMsg +
                            " (errno: " + std::to_string(err) + ")"
                        );
                    case ESRCH:
                        errorMsg = "Process does not exist (ESRCH)";
                        throw ProcessNotExistException(
                            "process_vm_readv failed: " + errorMsg +
                            " (errno: " + std::to_string(err) + ")"
                        );
                    default:
                        errorMsg = "Unknown error: " + std::string(std::strerror(err));
                        break;
                }

                this->_logger.log(errorMsg, logger::LogLevel::Error);

                throw ReadMemoryException(
                    "process_vm_readv failed: " + errorMsg +
                    " (errno: " + std::to_string(err) + ")"
                );
            }

            size_t bytesRead = static_cast<size_t>(nread);
            size_t typeSize = sizeof(Type);

            for (size_t offset = 0; offset + typeSize <= bytesRead; offset++) {
                Type value;

                std::memcpy(
                    &value,
                    buffer.data() + offset,
                    typeSize
                );

                if (searchValue == value) {
                    MemoryRecord record;
                    record.address = addr + offset;
                    record.value = value;

                    memories.push_back(std::move(record));
                }
            }
        }
    }

    return memories;
}

inline std::vector<MemoryRecord> LinuxMemoryFinder::readMemory(
    int pid,
    const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
    const std::string& searchValue
) const
{
    size_t overlap = searchValue.size() - 1;

    std::vector<MemoryRecord> memories;

    for (const process::analysis::MemoryBlock& block : memoryBlocks) {
        qDebug() << "Block" << QString::fromStdString(block.name)
                 << QString::fromStdString(std::format("{:#0x}",block.rangeAddresses.first))
                 << " - "
                 << QString::fromStdString(std::format("{:#0x}",block.rangeAddresses.second))
                 << QString::fromStdString(block.perms.toString());
        if (this->_filter.shouldSkip(block)) {
            qDebug() << "skip";
            continue;
        }


        process::analysis::address start = block.rangeAddresses.first;
        process::analysis::address end  = block.rangeAddresses.second;

        for (process::analysis::address addr = start; addr < end; addr += this->BATCH_SIZE + overlap - 1) {
            if (this->MAX_FINDED_VALUES_COUNT < memories.size()) {
                return memories;
            }

            size_t toRead = std::min(
                static_cast<size_t>(BATCH_SIZE),
                static_cast<size_t>(end - addr)
            );

            std::vector<char> buffer(toRead);

            iovec local;
            local.iov_base = buffer.data();
            local.iov_len = buffer.size();

            iovec remote;
            remote.iov_base = reinterpret_cast<void*>(addr);
            remote.iov_len = buffer.size();

            ssize_t nread = process_vm_readv(pid, &local, 1, &remote, 1, 0);
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
                        errorMsg = "Permission denied (EPERM) - need CAP_SYS_PTRACE capability";
                        throw PermissionDeniedException(
                            "process_vm_readv failed at address " +
                            std::format("{:#016x}", addr) + ": " + errorMsg +
                            " (errno: " + std::to_string(err) + ")"
                        );
                    case ESRCH:
                        errorMsg = "Process does not exist (ESRCH)";
                        throw ProcessNotExistException(
                            "process_vm_readv failed: " + errorMsg +
                            " (errno: " + std::to_string(err) + ")"
                        );
                    default:
                        errorMsg = "Unknown error: " + std::string(std::strerror(err));
                        break;
                }

                this->_logger.log(errorMsg, logger::LogLevel::Error);

                throw ReadMemoryException(
                    "process_vm_readv failed: " + errorMsg +
                    " (errno: " + std::to_string(err) + ")"
                );
            }

            size_t bytesRead = static_cast<size_t>(nread);
            size_t typeSize = searchValue.size();

            for (size_t offset = 0; offset + typeSize <= bytesRead; offset++) {
                std::string value;

                for (size_t i = 0 ; i < searchValue.size(); i++) {
                    value += buffer[i + offset];
                }

                if (searchValue == value) {
                    MemoryRecord record;
                    record.address = addr + offset;
                    record.value = value;

                    memories.push_back(std::move(record));
                }
            }
        }
    }

    return memories;
}

}


