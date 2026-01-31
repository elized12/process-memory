#pragma once

#include <variant>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace process::memory {

using TypeValue = std::variant<
    std::uint8_t,
    std::uint16_t,
    std::uint32_t,
    std::uint64_t,
    std::int8_t,
    std::int16_t,
    std::int32_t,
    std::int64_t,
    double,
    float,
    long double,
    std::string
>;

class MemoryRecord {
public:
    std::uint64_t address;
    TypeValue value;
};

TypeValue parseTargetValue(const QString& text, FindValueType type) {
    switch (type) {
        case FindValueType::uint8:
            return static_cast<std::uint8_t>(text.toUInt());

        case FindValueType::uint16:
            return static_cast<std::uint16_t>(text.toUInt());

        case FindValueType::uint32:
            return static_cast<std::uint32_t>(text.toUInt());

        case FindValueType::uint64:
            return static_cast<std::uint64_t>(text.toULongLong());

        case FindValueType::int8:
            return static_cast<std::int8_t>(text.toInt());

        case FindValueType::int16:
            return static_cast<std::int16_t>(text.toInt());

        case FindValueType::int32:
            return static_cast<std::int32_t>(text.toInt());

        case FindValueType::int64:
            return static_cast<std::int64_t>(text.toLongLong());

        case FindValueType::float8:
            return text.toFloat();

        case FindValueType::double8:
            return text.toDouble();

        case FindValueType::string:
            return text.toStdString();
    }

    throw std::runtime_error("Unknown FindValueType");
}

}
