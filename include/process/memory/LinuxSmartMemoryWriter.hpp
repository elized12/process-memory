#pragma once

#include "process/memory/LinuxMemoryWriter.hpp"
#include "process/memory/MemoryRecord.hpp"

namespace process::memory {

class LinuxSmartMemoryWriter {
private:
    process::memory::LinuxMemoryWriter _writer;

public:
    LinuxSmartMemoryWriter(const LinuxMemoryWriter& writer) :
        _writer(writer)
    {

    }

public:
    void write(
        int pid,
        std::uint64_t,
        TypeValue value,
        const FindValueType& type
    );
};

inline void LinuxSmartMemoryWriter::write(
    int pid,
    std::uint64_t address,
    TypeValue value,
    const FindValueType& type
) {
    switch (type) {
        case FindValueType::uint8: {
            this->_writer.write<std::uint8_t>(pid, address, std::get<std::uint8_t>(value));
            break;
        }
        case FindValueType::uint16: {
            this->_writer.write<std::uint16_t>(pid, address, std::get<std::uint16_t>(value));
            break;
        }
        case FindValueType::uint32: {
            this->_writer.write<std::uint32_t>(pid, address, std::get<std::uint32_t>(value));
            break;
        }
        case FindValueType::uint64: {
            this->_writer.write<std::uint64_t>(pid, address, std::get<std::uint64_t>(value));
            break;
        }
        case FindValueType::int8: {
            this->_writer.write<std::int8_t>(pid, address, std::get<std::int8_t>(value));
            break;
        }
        case FindValueType::int16: {
            this->_writer.write<std::int16_t>(pid, address, std::get<std::int16_t>(value));
            break;
        }
        case FindValueType::int32: {
            qDebug() << static_cast<int>(type) << std::get<std::int32_t>(value) << address << pid;
            this->_writer.write<std::int32_t>(pid, address, std::get<std::int32_t>(value));
            break;
        }
        case FindValueType::int64: {
            this->_writer.write<std::int64_t>(pid, address, std::get<std::int64_t>(value));
            break;
        }
        case FindValueType::double8: {
            this->_writer.write<double>(pid, address, std::get<double>(value));
            break;
        }
        case FindValueType::double10: {
            this->_writer.write<long double>(pid, address, std::get<long double>(value));
            break;
        }
        case FindValueType::float8: {
            this->_writer.write<float>(pid, address, std::get<float>(value));
            break;
        }
        case FindValueType::string: {
            this->_writer.write<std::string>(pid, address, std::get<std::string>(value));
            break;
        }
    default:
        throw std::runtime_error("Неизвестная ошибка с типом");
    }
}

}
