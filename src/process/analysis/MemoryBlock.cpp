#include "process/analysis/MemoryBlock.hpp"

namespace process::analysis {

MemoryBlock::MemoryBlock(const std::pair<address, address>& _range,
            const std::string& _name,
            const MemoryPerms _perms,
            address _offset,
            const std::string& _inode,
            const std::string& _device) :
    rangeAddresses(_range), name(_name), perms(_perms),
    offset(_offset), inode(_inode), device(_device) {

}

}
