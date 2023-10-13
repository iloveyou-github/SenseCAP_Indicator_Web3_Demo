# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Studio/Espressif/frameworks/esp-idf-v5.0/components/bootloader/subproject"
  "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader"
  "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix"
  "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix/tmp"
  "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix/src/bootloader-stamp"
  "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix/src"
  "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/WorkSpace/Seeed/examples/indicator_basis/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
