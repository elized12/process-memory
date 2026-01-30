#pragma once

#include <sys/uio.h>
#include <QString>
#include <QDebug>

#include "process/memory/LinuxMemoryRegionFilter.hpp"
#include "process/memory/ReadMemoryException.hpp"
#include "process/memory/MemoryRecord.hpp"

namespace process::memory {

class LinuxMemoryReader {
private:
    process::memory::LinuxMemoryRegionFilter _filter;

public:
    LinuxMemoryReader(const LinuxMemoryRegionFilter& filter);

public:
    template<class Type>
    std::vector<MemoryRecord<Type>> readMemories(int pid, const std::vector<process::analysis::MemoryBlock>& memoryBlocks) const;

    template<class Type>
    std::vector<MemoryRecord<Type>> readMemory(int pid, const process::analysis::MemoryBlock& memoryBlocks) const;
};

template<class Type>
std::vector<MemoryRecord<Type>> LinuxMemoryReader::readMemories(int pid, const std::vector<process::analysis::MemoryBlock>& memoryBlocks) const {
    std::vector<MemoryRecord<Type>> memories;

    for (const process::analysis::MemoryBlock& block : memoryBlocks) {
        qDebug() << "Block" << QString::fromStdString(block.name) << QString::fromStdString(std::format("{:#0x}",block.rangeAdresses.first)) << " - " << QString::fromStdString(std::format("{:#0x}",block.rangeAdresses.second));
        if (this->_filter.shouldSkip(block)) {
            qDebug() << "skip";
            continue;
        }

        std::vector<uint8_t> buffer(block.rangeAdresses.second - block.rangeAdresses.first);

        iovec local;
        local.iov_base = buffer.data();
        local.iov_len = buffer.size();

        iovec remote;
        remote.iov_base = reinterpret_cast<void*>(block.rangeAdresses.first);
        remote.iov_len = buffer.size();

        ssize_t nread = process_vm_readv(pid, &local, 1, &remote, 1, 0);
        if (nread == -1) {
            throw ReadMemoryException("Error read memory");
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

            MemoryRecord<Type> record;
            record.address = block.rangeAdresses.first + offset;
            record.value = value;

            memories.push_back(std::move(record));
        }
    }

    return memories;
}

template<class Type>
std::vector<MemoryRecord<Type>> LinuxMemoryReader::readMemory(int pid, const process::analysis::MemoryBlock& block) const {
        if (this->_filter.shouldSkip(block)) {
            return {};
        }

        std::vector<MemoryRecord<Type>> memories;

        std::vector<uint8_t> buffer(block.rangeAdresses.second - block.rangeAdresses.first);

        iovec local;
        local.iov_base = buffer.data();
        local.iov_len = buffer.size();

        iovec remote;
        remote.iov_base = reinterpret_cast<void*>(block.rangeAdresses.first);
        remote.iov_len = buffer.size();

        ssize_t nread = process_vm_readv(pid, &local, 1, &remote, 1, 0);
        if (nread == -1) {
            throw ReadMemoryException("Error read memory");
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

            MemoryRecord<Type> record;
            record.address = block.rangeAdresses.first + offset;
            record.value = value;

            memories.push_back(std::move(record));
        }

        return memories;
}

}


