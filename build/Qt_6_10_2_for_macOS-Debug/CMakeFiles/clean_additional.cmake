# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "8085Simulator_autogen"
  "CMakeFiles/8085Simulator_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/8085Simulator_autogen.dir/ParseCache.txt"
  )
endif()
