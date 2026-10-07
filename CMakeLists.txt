cmake_minimum_required (VERSION 3.24)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("talle")
add_subdirectory("talle")

option(TALLE_INSTALL "Should TALL-E be installed" ON)
option(TALLE_BUILD_EXAMPLES "Should TALL-E usage examples be built" ON)
if(TALLE_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()
