#include "process/memory/LinuxSmartMemoryFinder.hpp"

namespace process::memory {

LinuxSmartMemoryFinder::LinuxSmartMemoryFinder(const LinuxMemoryFinder& finder) :
    _finder(finder)
{

}

std::vector<MemoryRecord> LinuxSmartMemoryFinder::findValues(
    int pid,
    const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
    TypeValue targetValue,
    const FindValueType& type
) const
{
    switch (type) {
        case FindValueType::uint8: {
            return this->_finder.readMemory<std::uint8_t>(pid, memoryBlocks, std::get<std::uint8_t>(targetValue));
        }
        case FindValueType::uint16: {
            return this->_finder.readMemory<std::uint16_t>(pid, memoryBlocks, std::get<std::uint16_t>(targetValue));
        }
        case FindValueType::uint32: {
            return this->_finder.readMemory<std::uint32_t>(pid, memoryBlocks, std::get<std::uint32_t>(targetValue));
        }
        case FindValueType::uint64: {
            return this->_finder.readMemory<std::uint64_t>(pid, memoryBlocks, std::get<std::uint64_t>(targetValue));
        }
        case FindValueType::int8: {
            return this->_finder.readMemory<std::int8_t>(pid, memoryBlocks, std::get<std::int8_t>(targetValue));
        }
        case FindValueType::int16: {
            return this->_finder.readMemory<std::int16_t>(pid, memoryBlocks, std::get<std::int16_t>(targetValue));
        }
        case FindValueType::int32: {
            return this->_finder.readMemory<std::int32_t>(pid, memoryBlocks, std::get<std::int32_t>(targetValue));
        }
        case FindValueType::int64: {
            return this->_finder.readMemory<std::int64_t>(pid, memoryBlocks, std::get<std::int64_t>(targetValue));
        }
        case FindValueType::double8: {
            return this->_finder.readMemory<double>(pid, memoryBlocks, std::get<double>(targetValue));
        }
        case FindValueType::double10: {
            return this->_finder.readMemory<long double>(pid, memoryBlocks, std::get<long double>(targetValue));
        }
        case FindValueType::float8: {
            return this->_finder.readMemory<float>(pid, memoryBlocks, std::get<float>(targetValue));
        }
        case FindValueType::string: {
            return this->_finder.readMemory(pid, memoryBlocks, std::get<std::string>(targetValue));
        }
    default:
        throw std::runtime_error("Неизвестная ошибка с типом");
    }
}

}
