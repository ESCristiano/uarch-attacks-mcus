# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/src/NonSecure")
  file(MAKE_DIRECTORY "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/src/NonSecure")
endif()
file(MAKE_DIRECTORY
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/src/NonSecure/build"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure/tmp"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure/src/STM32L5-Single-step_Cache_110MHz_NS-stamp"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure/src"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure/src/STM32L5-Single-step_Cache_110MHz_NS-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure/src/STM32L5-Single-step_Cache_110MHz_NS-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/NonSecure/src/STM32L5-Single-step_Cache_110MHz_NS-stamp${cfgdir}") # cfgdir has leading slash
endif()
