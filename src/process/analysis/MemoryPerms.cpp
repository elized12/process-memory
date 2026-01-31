#include "process/analysis/MemoryPerms.hpp"

namespace process::analysis {

MemoryPerms::MemoryPerms(std::uint8_t perms) : _perms(perms) {

}

MemoryPerms& MemoryPerms::set(Perms perm) {
    this->_perms |= static_cast<std::uint8_t>(perm);
    return *this;
}

bool MemoryPerms::has(Perms perm) const {
    return (this->_perms & static_cast<std::uint8_t>(perm));
}

MemoryPerms MemoryPerms::parseFromString(const std::string& perm) {
    MemoryPerms perms(0b0);

    for (size_t i = 0 ; i < std::min<std::size_t>(perm.size(), 4); i++) {
        switch (perm[i]) {
        case 'r':
            perms.set(MemoryPerms::Perms::Read);
            break;

        case 'w':
            perms.set(MemoryPerms::Perms::Write);
            break;

        case 'x':
            perms.set(MemoryPerms::Perms::Executable);
            break;

        case 'p':
            perms.set(MemoryPerms::Perms::Private);
            break;

        case 's':
            perms.set(MemoryPerms::Perms::Shared);
            break;

        case '-':
            break;

        default:
            throw ParseException(std::format("Symbol {} not exists in perms memory", perm[i]));
        }
    }

    return perms;
}

std::string MemoryPerms::toString() const {
    std::string result;

    if (this->has(MemoryPerms::Perms::Read)) {
        result += 'r';
    } else {
        result += '-';
    }

    if (this->has(MemoryPerms::Perms::Write)) {
        result += 'w';
    } else {
        result += '-';
    }

    if (this->has(MemoryPerms::Perms::Executable)) {
        result += 'x';
    } else {
        result += '-';
    }

    if (this->has(MemoryPerms::Perms::Private)) {
        result += 'p';
    } else if (this->has(MemoryPerms::Perms::Shared)) {
        result += 's';
    } else {
        result += '-';
    }

    return result;
}

}
