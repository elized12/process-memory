#pragma once

enum class FindValueType {
    uint8,
    uint16,
    uint32,
    uint64,
    int8,
    int16,
    int32,
    int64,
    double8,
    double10,
    float8,
    string
};

Q_DECLARE_METATYPE(FindValueType)

