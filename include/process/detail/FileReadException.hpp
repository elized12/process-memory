#pragma once

#include <stdexcept>

namespace process::detail {

class FileReadException : public std::runtime_error {
public:
    FileReadException(const std::string& message) : std::runtime_error(message) {}
};

}
