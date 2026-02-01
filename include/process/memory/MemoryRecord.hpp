#pragma once

#include <variant>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <QString>

#include "FindValueType.hpp"

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

inline TypeValue parseTargetValue(const QString& text, FindValueType type) {
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

inline QString variantToString(const TypeValue& value, FindValueType type) {
    switch (type) {
        case FindValueType::uint8:
            return QString::number(std::get<std::uint8_t>(value));
        case FindValueType::uint16:
            return QString::number(std::get<std::uint16_t>(value));
        case FindValueType::uint32:
            return QString::number(std::get<std::uint32_t>(value));
        case FindValueType::uint64:
            return QString::number(std::get<std::uint64_t>(value));
        case FindValueType::int8:
            return QString::number(std::get<std::int8_t>(value));
        case FindValueType::int16:
            return QString::number(std::get<std::int16_t>(value));
        case FindValueType::int32:
            return QString::number(std::get<std::int32_t>(value));
        case FindValueType::int64:
            return QString::number(std::get<std::int64_t>(value));
        case FindValueType::float8:
            return QString::number(std::get<float>(value));
        case FindValueType::double8:
            return QString::number(std::get<double>(value));
        case FindValueType::string:
            return QString::fromStdString(std::get<std::string>(value));
        default:
            return "Unknown type";
    }
}

}
