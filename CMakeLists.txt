cmake_minimum_required(VERSION 3.19)
project(8085Simulator LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(8085Simulator
    main.cpp
    cpu/cpu.h cpu/cpu.cpp
    memory/memory.h memory/memory.cpp
    assembler/assembler.cpp
)

target_link_libraries(8085Simulator
    PRIVATE
        Qt::Core
)

target_include_directories(8085Simulator PRIVATE
    cpu
    memory
)

include(GNUInstallDirs)

install(TARGETS 8085Simulator
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
