cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME template)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(WARNINGS_AS_ERRORS OFF)
set(ENABLE_PVS_STUDIO OFF)
set(ENABLE_UBSan OFF)
set(ENABLE_ASAN OFF)
set(ENABLE_TSan OFF)
set(ENABLE_MSAN OFF)


if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()
include(cmake/CompilerWarnings.cmake)
include_directories(include)

add_executable(${PROJECT_NAME} main.cpp
				options_parser/options_parser.cpp options_parser/options_parser.h "include/wordCounter.h" "src/wordCounter.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE options_parser)
find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)

find_package(LibArchive REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${LibArchive_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}  ${LibArchive_LIBRARIES})

INSTALL(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

set(ALL_TARGETS ${PROJECT_NAME})

include(cmake/main-config.cmake)
