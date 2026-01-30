#pragma once

#include <stdexcept>

namespace process::analysis {

class NotExistsException : public std::runtime_error {
public:
    NotExistsException(const std::string& message) : std::runtime_error(message) {}
};

}
