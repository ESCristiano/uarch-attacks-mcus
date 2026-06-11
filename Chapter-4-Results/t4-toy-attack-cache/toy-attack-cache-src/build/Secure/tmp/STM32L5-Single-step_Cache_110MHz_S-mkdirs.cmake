# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/src/Secure")
  file(MAKE_DIRECTORY "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/src/Secure")
endif()
file(MAKE_DIRECTORY
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/src/Secure/build"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure/tmp"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure/src/STM32L5-Single-step_Cache_110MHz_S-stamp"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure/src"
  "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure/src/STM32L5-Single-step_Cache_110MHz_S-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure/src/STM32L5-Single-step_Cache_110MHz_S-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/cris/Documents/0_Projects/uarch-attacks-mcus/evaluation/t4-toy-attack-cache/toy-attack-cache-src/build/Secure/src/STM32L5-Single-step_Cache_110MHz_S-stamp${cfgdir}") # cfgdir has leading slash
endif()
