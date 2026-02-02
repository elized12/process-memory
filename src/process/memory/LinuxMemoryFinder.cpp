#include "process/memory/LinuxMemoryFinder.hpp"

namespace process::memory {

LinuxMemoryFinder::LinuxMemoryFinder(
        const LinuxMemoryRegionFilter& filter,
        logger::Logger& logger
)
    :
      _filter(filter),
      _logger(logger)
{

}

}
