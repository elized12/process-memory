#pragma once

#include <stdexcept>

namespace process::analysis {

class ParseException : public std::runtime_error {
public:
    ParseException(const std::string& message) : std::runtime_error(message) {}
};

}
